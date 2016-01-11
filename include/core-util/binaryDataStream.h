
#ifndef CORE_UTIL_BINARYDATASTREAM_H_
#define CORE_UTIL_BINARYDATASTREAM_H_

namespace ml
{

template<class T> class Grid2;
template<class T> class Grid3;

template<class BinaryDataBuffer, class BinaryDataCompressor>
class BinaryDataStream {
public:
	BinaryDataStream() {}
	BinaryDataStream(const std::string& filename, bool clearStream) {
		m_DataBuffer.openBufferStream(filename, clearStream);
	}
	//! only required for files
	void openStream(const std::string& filename, bool clearStream) {
		m_DataBuffer.openBufferStream(filename, clearStream);
	}

	void closeStream() {
		m_DataBuffer.closeBufferStream();
	}

	template <class T>
	void writeData(const T& t) {
		writeData((const BYTE*)&t, sizeof(T));
	}

    template<class T>
    void writePrimitiveVector(const std::vector<T> &v)
    {
        writeData(v.size());
        writeData((const BYTE *)v.data(), sizeof(T) * v.size());
    }

    template<class T>
    void writePrimitive(const Grid2<T> &g)
    {
        writeData(g.getDimX());
        writeData(g.getDimY());
        writeData((const BYTE *)g.getData(), sizeof(T) * g.getDimX() * g.getDimY());
    }

    template<class T>
    void writePrimitive(const Grid3<T> &g)
    {
        writeData(g.getDimX());
        writeData(g.getDimY());
        writeData(g.getDimZ());
        writeData((const BYTE *)g.getData(), sizeof(T) * g.getDimX() * g.getDimY() * g.getDimZ());
    }

    template<class T>
    void readPrimitiveVector(std::vector<T> &v)
    {
        size_t size;
        readData(&size);
        v.clear();
        v.resize(size);
        readData((BYTE *)v.data(), sizeof(T) * v.size());
    }

    template<class T>
    void readPrimitive(Grid2<T> &g)
    {
        size_t dimX, dimY;
        readData(&dimX);
        readData(&dimY);
        g.allocate(dimX, dimY);
        readData((BYTE *)g.getData(), sizeof(T) * g.getDimX() * g.getDimY());
    }

    template<class T>
    void readPrimitive(Grid3<T> &g)
    {
        size_t dimX, dimY, dimZ;
        readData(&dimX);
        readData(&dimY);
        readData(&dimZ);
        g.allocate(dimX, dimY, dimZ);
        readData((BYTE *)g.getData(), sizeof(T) * g.getDimX() * g.getDimY() * g.getDimZ());
    }

	//start compression after that byte size (only if compression is enabled)
#define COMPRESSION_THRESHOLD_ 1024 
	void writeData(const BYTE* t, size_t size) {
		const bool useCompression = !std::is_same<BinaryDataCompressorNone, BinaryDataCompressor>::value;
		if (useCompression && size > COMPRESSION_THRESHOLD_) {
			std::vector<BYTE> compressedT;
			//ZLibWrapper::CompressStreamToMemory(t, size, compressedT, false);
			m_DataCompressor.compressStreamToMemory(t, size, compressedT);
			UINT64 compressedSize = compressedT.size();
			m_DataBuffer.writeData((const BYTE*)&compressedSize, sizeof(compressedSize));
			m_DataBuffer.writeData(&compressedT[0], compressedSize);
		} else 	{
			m_DataBuffer.writeData(t, size);
		}

	}

	template <class T>
	void readData(T* result) {
		readData((BYTE*)result, sizeof(T));
	}

	void readData(BYTE* result, size_t size) {
		const bool useCompression = !std::is_same<BinaryDataCompressorNone, BinaryDataCompressor>::value;
		if (useCompression && size > COMPRESSION_THRESHOLD_) {
			UINT64 compressedSize;
			m_DataBuffer.readData((BYTE*)&compressedSize, sizeof(UINT64));
			std::vector<BYTE> compressedT;	compressedT.resize(compressedSize);
			m_DataBuffer.readData(&compressedT[0], compressedSize);
			//ZLibWrapper::DecompressStreamFromMemory(&compressedT[0], compressedSize, result, size);
			m_DataCompressor.decompressStreamFromMemory(&compressedT[0], compressedSize, result, size);
		} else 
		{
			m_DataBuffer.readData(result, size);
		}
	}

	//! clears the read offset: copies all data to the front of the data array and frees all unnecessary memory
	void clearReadOffset() {
		m_DataBuffer.clearReadOffset();
	}

	//! destroys all the data in the stream (DELETES ALL DATA!)
	void clearStream() {
		m_DataBuffer.clearBuffer();
	}

	//! saves the stream to file; does not affect current data
	void saveToFile(const std::string &filename) {
		m_DataBuffer.saveToFile(filename);
	}

	//! loads a binary stream from file; destorys all previous data in the stream
	void loadFromFile(const std::string &filename) {
		m_DataBuffer.loadFromFile(filename);
	} 

	void reserve(size_t size) {
		m_DataBuffer.reserve(size);
	}

	void flush() {
		m_DataBuffer.flushBufferStream();
	}
private:
	BinaryDataBuffer		m_DataBuffer;
	BinaryDataCompressor	m_DataCompressor;
};

typedef BinaryDataStream<BinaryDataBufferMemory, BinaryDataCompressorNone> BinaryDataStreamVector;
typedef BinaryDataStream<BinaryDataBufferFile, BinaryDataCompressorNone> BinaryDataStreamFile;
//typedef BinaryDataStream<BinaryDataBufferVector, BinaryDataCompressorDefault> BinaryDataStreamCompressedVector; (see zlib for instance)
//typedef BinaryDataStream<BinaryDataBufferFile, BinaryDataCompressorDefault> BinaryDataStreamCompressedFile; (see zlib for instance)

//////////////////////////////////////////////////////
/////////write stream operators for base types ///////
//////////////////////////////////////////////////////

//cannot overload via template since it is not supposed to work for complex types

template<class BinaryDataBuffer, class BinaryDataCompressor>
inline BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& operator<<(BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& s, UINT64 i) {
	s.writeData(i);
	return s;
}
template<class BinaryDataBuffer, class BinaryDataCompressor>
inline BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& operator<<(BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& s, bool i) {
    s.writeData(i);
    return s;
}
template<class BinaryDataBuffer, class BinaryDataCompressor>
inline BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& operator<<(BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& s, int i) {
	s.writeData(i);
	return s;
}
template<class BinaryDataBuffer, class BinaryDataCompressor>
inline BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& operator<<(BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& s, unsigned int i) {
	s.writeData(i);
	return s;
}
template<class BinaryDataBuffer, class BinaryDataCompressor>
inline BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& operator<<(BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& s, short i) {
	s.writeData(i);
	return s;
}
template<class BinaryDataBuffer, class BinaryDataCompressor>
inline BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& operator<<(BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& s, unsigned short i) {
	s.writeData(i);
	return s;
}
template<class BinaryDataBuffer, class BinaryDataCompressor>
inline BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& operator<<(BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& s, char i) {
	s.writeData(i);
	return s;
}
template<class BinaryDataBuffer, class BinaryDataCompressor>
inline BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& operator<<(BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& s, unsigned char i) {
	s.writeData(i);
	return s;
}
template<class BinaryDataBuffer, class BinaryDataCompressor>
inline BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& operator<<(BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& s, float i) {
	s.writeData(i);
	return s;
}
template<class BinaryDataBuffer, class BinaryDataCompressor>
inline BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& operator<<(BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& s, double i) {
	s.writeData(i);
	return s;
}

template<class BinaryDataBuffer, class BinaryDataCompressor, class T>
inline BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& operator<<(BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& s, const std::vector<T>& v) {
	s << (UINT64)v.size();
	s.reserve(sizeof(T)*v.size());
	for (size_t i = 0; i < v.size(); i++) {
		s << v[i];
	}
	return s;
}

template<class BinaryDataBuffer, class BinaryDataCompressor, class T>
inline BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& operator<<(BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& s, const std::list<T>& l) {
	s << (UINT64)l.size();
	s.reserve(sizeof(T)*l.size());
	for (typename std::list<T>::const_iterator iter = l.begin(); iter != l.end(); iter++) {
		s << *l;
	}
	return s;
}
template<class BinaryDataBuffer, class BinaryDataCompressor>
inline BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& operator<<(BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& s, const std::string& l) {
	s << (UINT64)l.size();
	s.reserve(sizeof(char)*l.size());
	for (size_t i = 0; i < l.size(); i++) {
		s << l[i];
	}
	return s;
}

template<class BinaryDataBuffer, class BinaryDataCompressor, class K, class T>
inline BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& operator<<(BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& s, const std::unordered_map<K,T>& m) {
	s << m.size();
	s << m.max_load_factor();
	for (auto iter = m.begin(); iter != m.end(); iter++) {
		s << iter->first << iter->second;
	}
	return s;
}





//////////////////////////////////////////////////////
/////////read stream operators for base types ///////
//////////////////////////////////////////////////////

//cannot overload via template since it is not supposed to work for complex types
template<class BinaryDataBuffer, class BinaryDataCompressor>
inline BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& operator>>(BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& s, UINT64& i) {
	s.readData(&i);
	return s;
}
template<class BinaryDataBuffer, class BinaryDataCompressor>
inline BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& operator>>(BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& s, int& i) {
	s.readData(&i);
	return s;
}
template<class BinaryDataBuffer, class BinaryDataCompressor>
inline BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& operator>>(BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& s, bool& i) {
    s.readData(&i);
    return s;
}
template<class BinaryDataBuffer, class BinaryDataCompressor>
inline BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& operator>>(BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& s, unsigned int& i) {
	s.readData(&i);
	return s;
}
template<class BinaryDataBuffer, class BinaryDataCompressor>
inline BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& operator>>(BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& s, short& i) {
	s.readData(&i);
	return s;
}
template<class BinaryDataBuffer, class BinaryDataCompressor>
inline BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& operator>>(BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& s, unsigned short& i) {
	s.readData(&i);
	return s;
}
template<class BinaryDataBuffer, class BinaryDataCompressor>
inline BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& operator>>(BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& s, char& i) {
	s.readData(&i);
	return s;
}
template<class BinaryDataBuffer, class BinaryDataCompressor>
inline BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& operator>>(BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& s, unsigned char& i) {
	s.readData(&i);
	return s;
}
template<class BinaryDataBuffer, class BinaryDataCompressor>
inline BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& operator>>(BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& s, float& i) {
	s.readData(&i);
	return s;
}
template<class BinaryDataBuffer, class BinaryDataCompressor>
inline BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& operator>>(BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& s, double& i) {
	s.readData(&i);
	return s;
}

template<class BinaryDataBuffer, class BinaryDataCompressor, class T>
inline BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& operator>>(BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& s, std::vector<T>& v) {
	UINT64 size;
	s >> size;
	v.resize(size);
	for (size_t i = 0; i < v.size(); i++) {
		s >> v[i];
	}
	return s;
}

template<class BinaryDataBuffer, class BinaryDataCompressor, class T>
inline BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& operator>>(BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& s, const std::list<T>& l) {
	UINT64 size;
	s >> size;
	l.clear();
	for (size_t i = 0; i < size; i++) {
		T curr;
		s >> curr;
		l.push_back(l);
	}
	return s;
}
template<class BinaryDataBuffer, class BinaryDataCompressor>
inline BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& operator>>(BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& s, std::string& v) {
	UINT64 size;
	s >> size;
	v.resize(size);
	for (size_t i = 0; i < v.size(); i++) {
		s >> v[i];
	}
	return s;
}

template<class BinaryDataBuffer, class BinaryDataCompressor, class K, class T>
inline BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& operator>>(BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& s, std::unordered_map<K,T>& m) {
	m.clear();
	size_t size;	float maxLoadFactor;
	s >> size >> maxLoadFactor;
	m.max_load_factor(maxLoadFactor);
	for (size_t i = 0; i < size; i++) {
		K first;	T second;
		s >> first >> second;
		m[first] = second;
	}
	return s;
}

namespace util
{
    template<class T>
    void serializeToFile(const std::string &filename, const T &o)
    {
        BinaryDataStreamFile out(filename, true);
        out << o;
        out.closeStream();
    }

    template<class T, class U>
    void serializeToFile(const std::string &filename, const T &o0, const U &o1)
    {
        BinaryDataStreamFile out(filename, true);
        out << o0 << o1;
        out.closeStream();
    }

    template<class T>
    void deserializeFromFile(const std::string &filename, T &o)
    {
        BinaryDataStreamFile in(filename, false);
        in >> o0 >> o1;
        in.closeStream();
    }

    template<class T, class U>
    void deserializeFromFile(const std::string &filename, T &o0, U &o1)
    {
        BinaryDataStreamFile in(filename, false);
        in >> o0 >> o1;
        in.closeStream();
    }
}

}  // namespace ml

#endif  // CORE_UTIL_BINARYDATASTREAM_H_
