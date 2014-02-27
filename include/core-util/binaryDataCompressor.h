
#pragma once
#ifndef _BINARY_DATA_COMPRESSOR_
#define _BINARY_DATA_COMPRESSOR_


//! interface to compress data
class BinaryDataCompressorInterface {
public:
	virtual void compressStreamToMemory(const BYTE *decompressedStream, UINT64 decompressedStreamLength, std::vector<BYTE> &compressedStream) const = 0;
	virtual void decompressStreamFromMemory(const BYTE *compressedStream, UINT64 compressedStreamLength, BYTE *decompressedStream, UINT64 decompressedStreamLength) const = 0;

};

//! interface to compress data
class BinaryDataCompressorNone : public BinaryDataCompressorInterface {
public:
	void compressStreamToMemory(const BYTE *decompressedStream, UINT64 decompressedStreamLength, std::vector<BYTE> &compressedStream) const {
		MLIB_ASSERT(false);	//just a dummy; should never come here
	}
	void decompressStreamFromMemory(const BYTE *compressedStream, UINT64 compressedStreamLength, BYTE *decompressedStream, UINT64 decompressedStreamLength) const {
		MLIB_ASSERT(false); //just a dummy; should never come here
	}

};

//! interface to compress data
class BinaryDataCompressorDefault : public BinaryDataCompressorInterface {
public:
	void compressStreamToMemory(const BYTE *decompressedStream, UINT64 decompressedStreamLength, std::vector<BYTE> &compressedStream) const {
		compressedStream.resize(decompressedStreamLength);
		memcpy(&compressedStream[0], decompressedStream, decompressedStreamLength);
	}

	void decompressStreamFromMemory(const BYTE *compressedStream, UINT64 compressedStreamLength, BYTE *decompressedStream, UINT64 decompressedStreamLength) const {
		memcpy(decompressedStream, compressedStream, compressedStreamLength);
	}
};

#endif //  _BINARY_DATA_COMPRESSOR_