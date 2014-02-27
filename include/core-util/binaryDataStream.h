#pragma once

#ifndef _BINARY_DATA_STREAM_H_
#define _BINARY_DATA_STREAM_H_



////////////////////////////////////////////////
// BinaryDataStream (uses BINARY_DATA_BUFFER) //
////////////////////////////////////////////////

template<class BinaryDataBuffer, bool useCompression>
class BinaryDataStream {
public:
	BinaryDataStream() {
	}
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

	//start compression after that byte value (only if compression is enabled)
#define COMPRESSION_THRESHOLD_ 1024
	void writeData(const BYTE* t, size_t size) {
/*
		//TODO FIX COMPRESSION
		if (useCompression && size > COMPRESSION_THRESHOLD_) {
			std::vector<BYTE> compressedT;
			ZLibWrapper::CompressStreamToMemory(t, size, compressedT, false);
			UINT64 compressedSize = compressedT.size();
			m_DataBuffer.writeData((const BYTE*)&compressedSize, sizeof(compressedSize));
			m_DataBuffer.writeData(&compressedT[0], compressedSize);
		} else 
*/
		{
			m_DataBuffer.writeData(t, size);
		}

	}

	template <class T>
	void readData(T* result) {
		readData((BYTE*)result, sizeof(T));
	}

	void readData(BYTE* result, size_t size) {
		//TODO FIX COMPRESSION
		//if (useCompression && size > COMPRESSION_THRESHOLD_) {
		//	UINT64 compressedSize;
		//	m_DataBuffer.readData((BYTE*)&compressedSize, sizeof(UINT64));
		//	std::vector<BYTE> compressedT;	compressedT.resize(compressedSize);
		//	m_DataBuffer.readData(&compressedT[0], compressedSize);
		//	ZLibWrapper::DecompressStreamFromMemory(&compressedT[0], compressedSize, result, size);
		//} else 
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

	//! saves the  stream to file; does not affect current data
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
	BinaryDataBuffer m_DataBuffer;

};

typedef BinaryDataStream<BinaryDataBufferVector, false> BinaryDataStreamVector;
typedef BinaryDataStream<BinaryDataBufferFile, false> BinaryDataStreamFile;
typedef BinaryDataStream<BinaryDataBufferVector, true> BinaryDataStreamCompressedVector;
typedef BinaryDataStream<BinaryDataBufferFile, true> BinaryDataStreamCompressedFile;

//////////////////////////////////////////////////////
/////////write stream operators for base types ///////
//////////////////////////////////////////////////////

//cannot overload via template since it is not supposed to work for complex types

template<class BinaryDataBuffer, bool useCompression>
inline BinaryDataStream<BinaryDataBuffer, useCompression>& operator<<(BinaryDataStream<BinaryDataBuffer, useCompression>& s, UINT64 i) {
	s.writeData(i);
	return s;
}
template<class BinaryDataBuffer, bool useCompression>
inline BinaryDataStream<BinaryDataBuffer, useCompression>& operator<<(BinaryDataStream<BinaryDataBuffer, useCompression>& s, int i) {
	s.writeData(i);
	return s;
}
template<class BinaryDataBuffer, bool useCompression>
inline BinaryDataStream<BinaryDataBuffer, useCompression>& operator<<(BinaryDataStream<BinaryDataBuffer, useCompression>& s, unsigned int i) {
	s.writeData(i);
	return s;
}
template<class BinaryDataBuffer, bool useCompression>
inline BinaryDataStream<BinaryDataBuffer, useCompression>& operator<<(BinaryDataStream<BinaryDataBuffer, useCompression>& s, short i) {
	s.writeData(i);
	return s;
}
template<class BinaryDataBuffer, bool useCompression>
inline BinaryDataStream<BinaryDataBuffer, useCompression>& operator<<(BinaryDataStream<BinaryDataBuffer, useCompression>& s, unsigned short i) {
	s.writeData(i);
	return s;
}
template<class BinaryDataBuffer, bool useCompression>
inline BinaryDataStream<BinaryDataBuffer, useCompression>& operator<<(BinaryDataStream<BinaryDataBuffer, useCompression>& s, char i) {
	s.writeData(i);
	return s;
}
template<class BinaryDataBuffer, bool useCompression>
inline BinaryDataStream<BinaryDataBuffer, useCompression>& operator<<(BinaryDataStream<BinaryDataBuffer, useCompression>& s, unsigned char i) {
	s.writeData(i);
	return s;
}
template<class BinaryDataBuffer, bool useCompression>
inline BinaryDataStream<BinaryDataBuffer, useCompression>& operator<<(BinaryDataStream<BinaryDataBuffer, useCompression>& s, float i) {
	s.writeData(i);
	return s;
}
template<class BinaryDataBuffer, bool useCompression>
inline BinaryDataStream<BinaryDataBuffer, useCompression>& operator<<(BinaryDataStream<BinaryDataBuffer, useCompression>& s, double i) {
	s.writeData(i);
	return s;
}

template<class BinaryDataBuffer, bool useCompression, class T>
inline BinaryDataStream<BinaryDataBuffer, useCompression>& operator<<(BinaryDataStream<BinaryDataBuffer, useCompression>& s, const std::vector<T>& v) {
	s << (UINT64)v.size();
	s.reserve(sizeof(T)*v.size());
	for (size_t i = 0; i < v.size(); i++) {
		s << v[i];
	}
	return s;
}
template<class BinaryDataBuffer, bool useCompression, class T>
inline BinaryDataStream<BinaryDataBuffer, useCompression>& operator<<(BinaryDataStream<BinaryDataBuffer, useCompression>& s, const std::list<T>& l) {
	s << (UINT64)l.size();
	s.reserve(sizeof(T)*l.size());
	for (std::list<T>::const_iterator iter = l.begin(); iter != l.end(); iter++) {
		s << *l;
	}
	return s;
}
template<class BinaryDataBuffer, bool useCompression>
inline BinaryDataStream<BinaryDataBuffer, useCompression>& operator<<(BinaryDataStream<BinaryDataBuffer, useCompression>& s, const std::string& l) {
	s << (UINT64)l.size();
	s.reserve(sizeof(char)*l.size());
	for (size_t i = 0; i < l.size(); i++) {
		s << l[i];
	}
	return s;
}





//////////////////////////////////////////////////////
/////////read stream operators for base types ///////
//////////////////////////////////////////////////////

//cannot overload via template since it is not supposed to work for complex types
template<class BinaryDataBuffer, bool useCompression>
inline BinaryDataStream<BinaryDataBuffer, useCompression>& operator>>(BinaryDataStream<BinaryDataBuffer, useCompression>& s, UINT64& i) {
	s.readData(&i);
	return s;
}
template<class BinaryDataBuffer, bool useCompression>
inline BinaryDataStream<BinaryDataBuffer, useCompression>& operator>>(BinaryDataStream<BinaryDataBuffer, useCompression>& s, int& i) {
	s.readData(&i);
	return s;
}
template<class BinaryDataBuffer, bool useCompression>
inline BinaryDataStream<BinaryDataBuffer, useCompression>& operator>>(BinaryDataStream<BinaryDataBuffer, useCompression>& s, unsigned int& i) {
	s.readData(&i);
	return s;
}
template<class BinaryDataBuffer, bool useCompression>
inline BinaryDataStream<BinaryDataBuffer, useCompression>& operator>>(BinaryDataStream<BinaryDataBuffer, useCompression>& s, short& i) {
	s.readData(&i);
	return s;
}
template<class BinaryDataBuffer, bool useCompression>
inline BinaryDataStream<BinaryDataBuffer, useCompression>& operator>>(BinaryDataStream<BinaryDataBuffer, useCompression>& s, unsigned short& i) {
	s.readData(&i);
	return s;
}
template<class BinaryDataBuffer, bool useCompression>
inline BinaryDataStream<BinaryDataBuffer, useCompression>& operator>>(BinaryDataStream<BinaryDataBuffer, useCompression>& s, char& i) {
	s.readData(&i);
	return s;
}
template<class BinaryDataBuffer, bool useCompression>
inline BinaryDataStream<BinaryDataBuffer, useCompression>& operator>>(BinaryDataStream<BinaryDataBuffer, useCompression>& s, unsigned char& i) {
	s.readData(&i);
	return s;
}
template<class BinaryDataBuffer, bool useCompression>
inline BinaryDataStream<BinaryDataBuffer, useCompression>& operator>>(BinaryDataStream<BinaryDataBuffer, useCompression>& s, float& i) {
	s.readData(&i);
	return s;
}
template<class BinaryDataBuffer, bool useCompression>
inline BinaryDataStream<BinaryDataBuffer, useCompression>& operator>>(BinaryDataStream<BinaryDataBuffer, useCompression>& s, double& i) {
	s.readData(&i);
	return s;
}

template<class BinaryDataBuffer, bool useCompression, class T>
inline BinaryDataStream<BinaryDataBuffer, useCompression>& operator>>(BinaryDataStream<BinaryDataBuffer, useCompression>& s, std::vector<T>& v) {
	UINT64 size;
	s >> size;
	v.resize(size);
	for (size_t i = 0; i < v.size(); i++) {
		s >> v[i];
	}
	return s;
}
template<class BinaryDataBuffer, bool useCompression, class T>
inline BinaryDataStream<BinaryDataBuffer, useCompression>& operator>>(BinaryDataStream<BinaryDataBuffer, useCompression>& s, const std::list<T>& l) {
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
template<class BinaryDataBuffer, bool useCompression>
inline BinaryDataStream<BinaryDataBuffer, useCompression>& operator>>(BinaryDataStream<BinaryDataBuffer, useCompression>& s, std::string& v) {
	UINT64 size;
	s >> size;
	v.resize(size);
	for (size_t i = 0; i < v.size(); i++) {
		s >> v[i];
	}
	return s;
}


#endif