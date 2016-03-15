
namespace ml
{

namespace mBase
{

struct RecordOffset : public BinaryDataSerialize< RecordOffset >
{
    UINT64 recordIndex;
    UINT64 fileIndex;
    UINT64 recordSize;
    UINT64 byteOffset;
};

template<class RecordType>
class Writer
{
public:
    Writer(const std::string &directory)
    {
        finalized = false;
        init(directory);
    }

    ~Writer()
    {
        finalize();
    }

    void init(const std::string &_directory)
    {
        util::makeDirectory(_directory);
        directory = _directory;
        activeFile = nullptr;
        activeFileIndex = 0;
        activeFileSize = 0;
    }

    void updateActiveFile()
    {
        const int maxFileSize = 1024 * 1024 * 256;
        if (activeFileSize >= maxFileSize && activeFile)
        {
            fclose(activeFile);
            activeFile = nullptr;
            activeFileSize = 0;
            activeFileIndex++;
        }

        if (activeFile == nullptr)
        {
            activeFile = util::checkedFOpen(directory + util::zeroPad(activeFileIndex, 3), "wb");
        }
    }

    void addRecord(const RecordType &record)
    {
        updateActiveFile();

        BinaryDataStreamVector out;
        out << record;
        const std::vector<BYTE> compressedData = ZLibWrapper::CompressStreamToMemory(out.getData(), true);

        RecordOffset newRecord;
        newRecord.recordIndex = records.size();
        newRecord.byteOffset = activeFileSize;
        newRecord.fileIndex = activeFileIndex;
        newRecord.recordSize = compressedData.size();
        records.push_back(newRecord);

        util::checkedFWrite(compressedData.data(), sizeof(BYTE), compressedData.size(), activeFile);
        activeFileSize += compressedData.size();
    }

    void finalize()
    {
        if (finalized) return;
        finalized = true;
        if(activeFile) fclose(activeFile);
        util::serializeToFile(directory + "records.dat", records);
        cout << "Saved " << records.size() << " records" << endl;
    }

private:
    std::string directory;
    UINT64 activeFileSize;
    int activeFileIndex;
    FILE *activeFile;
    std::vector<RecordOffset> records;
    bool finalized;
};

template<class RecordType>
class Reader
{
public:
    Reader(const std::string &_directory, size_t _cacheSize)
    {
        init(_directory, _cacheSize);
    }

    ~Reader()
    {
        terminateThread = true;
        if (decompThread.joinable()) {
            decompThread.join();
        }
    }

    void init(const std::string &_directory, size_t _cacheSize)
    {
        directory = _directory;
        cacheSize = _cacheSize;
        terminateThread = false;
        activeRecordIndex = 0;
        util::deserializeFromFile(directory + "records.dat", records);
        cout << "Loaded " << records.size() << " records" << endl;

        startDecompressBackgroundThread();
    }

    void readNextRecord(RecordType &result)
    {
        while (1) {
            if (cache.size() > 0) {
                cacheMutex.lock();
                result = move(cache.front());
                cache.pop_front();
                cacheMutex.unlock();
                return;
            }
            else {
                this_thread::sleep_for(std::chrono::microseconds(1));
            }
        }
    }

private:

    void readRecordInternal(UINT64 recordIndex, RecordType &result)
    {
        const RecordOffset &offset = records[recordIndex];
        const std::string filename = directory + util::zeroPad(offset.fileIndex, 3);

        FILE* file = fopen(filename.c_str(), "rb");
        if (file == nullptr || ferror(file))
        {
            cout << "Failed to open file: " << filename << endl;
            return;
        }

        fseek(file, offset.byteOffset, SEEK_SET);

        if (cacheStorage.size() < offset.recordSize)
            cacheStorage.resize(offset.recordSize);
        fread(cacheStorage.data(), offset.recordSize, 1, file);

        fclose(file);

        std::vector<BYTE> uncompressedData = ZLibWrapper::DecompressStreamFromMemory(cacheStorage);

        BinaryDataStreamVector out;
        out.setData(std::move(uncompressedData));
        out >> result;
    }

    void readNextRecordInternal(RecordType &result)
    {
        readRecordInternal(activeRecordIndex, result);
        activeRecordIndex++;
        if (activeRecordIndex == records.size())
        {
            cout << "All records read; restarting from beginning" << endl;
            activeRecordIndex = 0;
        }
    }

    void startDecompressBackgroundThread() {
        decompThread = std::thread(decompressStub, this);
    }

    static void decompressStub(Reader<RecordType>* reader) {
        reader->decompressLoop();
    }

    void decompressLoop() {
        while (1) {
            if (terminateThread) break;
            
            if (cache.size() < cacheSize) {	//need to fill the cache
                RecordType newRecord;
                readNextRecordInternal(newRecord);

                cacheMutex.lock();
                cache.push_back(move(newRecord));
                cout << "New cache size: " << cache.size() << endl;
                cacheMutex.unlock();
            }
            else {
                this_thread::sleep_for(std::chrono::microseconds(1));
            }
        }
    }

    std::string directory;
    std::vector<RecordOffset> records;
    UINT64 activeRecordIndex;

    size_t cacheSize;
    std::list<RecordType> cache;
    std::thread decompThread;
    std::mutex cacheMutex;
    bool terminateThread;
    std::vector<BYTE> cacheStorage;
};

}

}