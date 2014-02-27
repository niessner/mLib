#pragma once

#ifndef _BINARY_DATA_BUFFER_H_
#define _BINARY_DATA_BUFFER_H_


#include <vector>
#include <list>
#include <fstream>
#include <string>

/////////////////////////////////////////////////////////////
// BinaryDataBuffers (class used by BINARY_DATA_STREAM)    //
/////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////
// BinaryDataBuffers (one for file, one for system memory) //
/////////////////////////////////////////////////////////////

class BinaryDataBufferFile {
public:
	BinaryDataBufferFile() {
		m_ReadOffset = 0;
		m_FileSize = 0;
	}

	void openBufferStream(const std::string& filename, bool clearBuffer = false) {
		m_FileName = filename;
		if (clearBuffer)	remove(m_FileName.c_str());
		closeFileStream();
		openFileStream();
	}
	void closeBufferStream() {
		closeFileStream();
	}

	~BinaryDataBufferFile() {
		closeFileStream();
		if (m_ReadOffset == 0 && m_FileSize == 0) {
			remove(m_FileName.c_str());
		}
	}

	void writeData(const BYTE* t, size_t size) {
		//std::cout << "tellp() " << m_FileStream.tellp() << std::endl;
		m_FileStream.seekp(m_FileSize);	//always append at the end
		m_FileStream.write((char*)t, size);
		m_FileSize += size;
	}

	void readData(BYTE* result, size_t size) {
		//std::cout << "tellg() " << m_FileStream.tellg() << std::endl;
		//assert(m_ReadOffset + size <= m_FileSize);
		if (m_ReadOffset + size > m_FileSize) throw MLIB_EXCEPTION("invalid read; probably wrong file name (" + m_FileName + ")?");
		m_FileStream.seekg(m_ReadOffset);
		m_FileStream.read((char*)result, size);
		m_ReadOffset += size;
	}


	//! destroys all the data in the stream
	void clearBuffer() {
		closeFileStream();
		remove(m_FileName.c_str());
		openFileStream();
		MLIB_ASSERT(m_FileSize == 0);
		m_ReadOffset = 0;
	}

	void clearReadOffset() {
		size_t len = m_FileSize - m_ReadOffset;
		if (len == 0) {  //if there is no data left, clear the buffer
			clearBuffer();
		} else {
			std::vector<BYTE> oldData;
			copyDataToMemory(oldData);

			closeFileStream();
			remove(m_FileName.c_str());
			openFileStream();
			m_FileStream.write((char*)&oldData[0], oldData.size());
			m_ReadOffset = 0;
			MLIB_ASSERT(m_FileSize == oldData.size());
		}
	}

	void reserve(size_t size) {
		return;	//doesn't make much sense for files, does it?
	}

	void saveToFile(const std::string &filename) {
		std::vector<BYTE> oldData;
		copyDataToMemory(oldData);

		std::ofstream output(filename, std::ios::binary);
		output.write((char*)&oldData[0], sizeof(BYTE)*oldData.size());
		if (!output.is_open())	throw MLIB_EXCEPTION(filename);
		output.close();
		return;
	}

	//! loads a binary stream from file; destorys all previous data in the stream
	void loadFromFile(const std::string &filename) {
		m_FileStream.close();

		size_t inputFileSize = getFileSizeInBytes(filename);
		BYTE* data = new BYTE[inputFileSize];
		std::ifstream input(filename, std::ios::binary);
		if (!input.is_open())	throw MLIB_EXCEPTION(filename);
		input.read((char*)data, sizeof(BYTE)*inputFileSize);
		input.close();

		clearBuffer();	//clear the old values
		m_FileStream.write((char*)data, sizeof(BYTE)*inputFileSize);
		MLIB_ASSERT(m_FileSize == inputFileSize);
		m_ReadOffset = 0;
	}

	//! flushes the stream
	void flushBufferStream() {
		m_FileStream.flush();
	}
private:


	//! reads all the 'active' file data to system memory
	void copyDataToMemory(std::vector<BYTE>& data) {
		size_t len = m_FileSize - m_ReadOffset;
		data.resize(len);
		m_FileStream.seekg(m_ReadOffset);
		m_FileStream.read((char*)&data[0], sizeof(BYTE)*len);
	}

	//! returns the file size; if the file cannot be opened returns -1 (e.g., the file does not exist)
	size_t getFileSizeInBytes(const std::string &filename) {
		std::ifstream file(filename, std::ios::binary | std::ios::ate);
		if (!file.is_open())	return -1;
		size_t size = file.tellg();
		file.close();
		return size;
	}

	//! opens the file stream
	void openFileStream() {
		if (m_FileStream.is_open())	m_FileStream.close();
		m_FileSize = getFileSizeInBytes(m_FileName);
		if (m_FileSize == -1)	m_FileSize = 0;	//in case there was no file before

		m_FileStream.open(m_FileName.c_str(), std::ios::binary | std::ios::out | std::ios::in);
		if (!m_FileStream.is_open()) {
			m_FileStream.open(m_FileName.c_str(), std::ios::binary | std::ios::out);
			m_FileStream.close();
			m_FileStream.open(m_FileName.c_str(), std::ios::binary | std::ios::out | std::ios::in);
			if (!m_FileStream.is_open()) throw MLIB_EXCEPTION(m_FileName);
		} 
	}

	//! closes the file stream; data is automatically saved...
	void closeFileStream() {
		if (m_FileStream.is_open())	m_FileStream.close();
	}

	std::string		m_FileName;
	std::fstream	m_FileStream;
	size_t			m_ReadOffset;
	size_t			m_FileSize;
};






class BinaryDataBufferMemory {
public:
	BinaryDataBufferMemory() {
		m_ReadOffset = 0;
	}
	void openBufferStream(const std::string& filename, bool clearBuffer = false) {
		MLIB_ASSERT(false);
		//dummy just needed for file stream
		return;
	}
	void closeBufferStream() {
		MLIB_ASSERT(false);
		//dummy just needed for file stream
		return;
	}

	void writeData(const BYTE* t, size_t size) {
		size_t basePtr = m_Data.size();
		m_Data.resize(basePtr + size);
		memcpy(&m_Data[0] + basePtr, t, size);
	}

	void readData(BYTE* result, size_t size) {
		MLIB_ASSERT(m_ReadOffset + size <= m_Data.size());

		memcpy(result, &m_Data[0] + m_ReadOffset, size);
		m_ReadOffset += size;

		//free memory if we reached the end of the stream
		if (m_ReadOffset == m_Data.size()) {
			m_Data.resize(0);
			m_ReadOffset = 0;
		}
	}


	//! destroys all the data in the stream
	void clearBuffer() {
		m_Data.clear();
		m_ReadOffset = 0;
	}

	void clearReadOffset() {
		size_t len = m_Data.size() - m_ReadOffset;
		for (unsigned int i = 0; i < len; i++) {
			m_Data[i] = m_Data[i + m_ReadOffset];
		}
		m_Data.resize(len);
		m_ReadOffset = 0;
	}

	void reserve(size_t size) {
		if (size > m_Data.size())
			m_Data.reserve(size);
	}

	void saveToFile(const std::string &filename) {
		std::ofstream output(filename, std::ios::binary);
		output.write((char*)&m_Data[0], sizeof(BYTE)*m_Data.size());
		if (!output.is_open())	throw MLIB_EXCEPTION(filename);
		output.close();
	}


	//! returns the file size; if the file cannot be opened returns -1 (e.g., the file does not exist)
	size_t getFileSizeInBytes(const std::string &filename) {
		std::ifstream file(filename, std::ios::binary | std::ios::ate);
		if (!file.is_open())	return -1;
		size_t size = file.tellg();
		file.close();
		return size;
	}

	//! loads a binary stream from file; destorys all previous data in the stream
	void loadFromFile(const std::string &filename) {
		size_t inputFileSize = getFileSizeInBytes(filename);
		m_Data.resize(inputFileSize);
		std::ifstream input(filename, std::ios::binary);
		if (!input.is_open())	throw MLIB_EXCEPTION(filename);
		input.read((char*)&m_Data[0], sizeof(BYTE)*inputFileSize);
		input.close();
		m_ReadOffset = 0;
	} 

	//! since all writes are immediate, there is nothing to do
	void flushBufferStream() {
		return;
	}
private:
	std::vector<BYTE>	m_Data;
	size_t				m_ReadOffset;
};



#endif