
class TestBinaryStreamTestData {
public:
	TestBinaryStreamTestData() { m_Data = NULL; m_Size = 0;}
	~TestBinaryStreamTestData() { deleteMemory(); }

	void deleteMemory() {
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
	data.deleteMemory();
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

		//for an output stream, 'clearBuffer' should be set to true to reset it
		BinaryDataStreamZLibFile streamOut("testBinaryStream.out", true);
		TestBinaryStreamTestData data;	data.init();
		streamOut << data;
		streamOut.closeStream();	//must call this here to make sure everything has been written to disk
		TestBinaryStreamTestData reRead;
		BinaryDataStreamZLibFile streamIn("testBinaryStream.out", false);
		streamIn >> reRead;

		MLIB_ASSERT(data == reRead);

	}

	std::string name()
	{
		return "Binary Stream";
	}
};