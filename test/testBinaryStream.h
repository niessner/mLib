
class TestBinaryStreamTestData {
public:
	TestBinaryStreamTestData() { m_Data = NULL; m_Size = 0;}
	~TestBinaryStreamTestData() { clear(); }

	void clear() {
		SAFE_DELETE_ARRAY(m_Data);	
		m_Size = 0;
	}

	void init() {
		m_Size = 1000;
		m_Data = new int[m_Size];
		for (unsigned int i = 0; i < m_Size; i++) {
			m_Data[i] = i;
		}
	}

	bool operator==(const TestBinaryStreamTestData& other) {
		if (other.m_Size != m_Size)	return false;
		for (unsigned int i = 0; i < m_Size; i++) {
			if (m_Data[i] != other.m_Data[i])	return false;
		}
		return true;
	}

	unsigned int m_Size;
	int* m_Data;
};

//! read from binary stream overload
template<class BinaryDataBuffer, class BinaryDataCompressor>
inline BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& operator>>(BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& s, TestBinaryStreamTestData& data) {
	data.clear();
	s >> data.m_Size;
	data.m_Data = new int[data.m_Size];
	s.readData((BYTE*)data.m_Data, sizeof(int)*data.m_Size);
	return s;
}
//! write to binary stream overload
template<class BinaryDataBuffer, class BinaryDataCompressor>
inline BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& operator<<(BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& s, const TestBinaryStreamTestData& data) {
	s << data.m_Size;
	s.writeData((BYTE*)data.m_Data, sizeof(int)*data.m_Size);
	return s;
}

class TestBinaryStream : public Test {

	void test0()
	{
		const std::string filenameCompressed = "testStreamCompressed.out";
		const std::string filename = "testStream.out";

		std::vector<int> v(10000, 0);

		//for an output stream, 'clearBuffer' should be set to true to reset it
		BinaryDataStreamZLibFile streamOut(filenameCompressed, true);
		TestBinaryStreamTestData data;	data.init();
		streamOut << data << v;
		streamOut.closeStream();	//must call this here to make sure everything has been written to disk

		BinaryDataStreamFile streamOutComp(filename, true);
		TestBinaryStreamTestData data2;	data2.init();
		streamOutComp << data2 << v;
		streamOutComp.closeStream();	//must call this here to make sure everything has been written to disk
		

		UINT64 dataSize = Utility::getFileData(filename).size();
		UINT64 dataSizeCompressed = Utility::getFileData(filenameCompressed).size();

		TestBinaryStreamTestData reRead;
		BinaryDataStreamZLibFile streamIn(filenameCompressed, false);
		streamIn >> reRead;
		streamIn >> v;

		MLIB_ASSERT(data == reRead);

	}

	std::string name()
	{
		return "Binary Stream";
	}
};