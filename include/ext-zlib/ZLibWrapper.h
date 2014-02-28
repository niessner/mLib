
#pragma once


#ifndef _ZLIB_WRAPPER_
#define _ZLIB_WRAPPER_


class ZLibWrapper
{
public:

    //static void CompressStreamToFile(const Vector<BYTE> &stream, const String &filename)
    //static void CompressStreamToFile(const BYTE *stream, UINT byteCount, const String &filename)
    //static void DecompressStreamFromFile(const String &filename, Vector<BYTE> &stream);

    static Vector<BYTE> CompressStreamToMemory(const Vector<BYTE> &decompressedStream, bool writeHeader)
    {
        Vector<BYTE> result;
        CompressStreamToMemory(decompressedStream, result, writeHeader);
        return result;
    }

    static Vector<BYTE> CompressStreamToMemory(const BYTE *decompressedStream, UINT64 decompressedStreamLength, bool writeHeader)
    {
        Vector<BYTE> result;
        CompressStreamToMemory(decompressedStream, decompressedStreamLength, result, writeHeader);
        return result;
    }

    static void CompressStreamToMemory(const Vector<BYTE> &decompressedStream, Vector<BYTE> &compressedStream, bool writeHeader)
    {
        CompressStreamToMemory(CArray(decompressedStream), decompressedStream.size(), compressedStream, writeHeader);
    }

    static void CompressStreamToMemory(const BYTE *decompressedStream, UINT64 decompressedStreamLength, Vector<BYTE> &compressedStream, bool writeHeader)
    {
        compressedStream.resize(decompressedStreamLength + 64);

        z_stream zstream;

        zstream.zalloc = Z_NULL;
        zstream.zfree = Z_NULL;
        zstream.opaque = Z_NULL;

        zstream.avail_in = (uInt)decompressedStreamLength;
        zstream.next_in = const_cast<BYTE*>(decompressedStream);

        zstream.data_type = Z_BINARY;

        zstream.avail_out = (uInt)decompressedStreamLength + 64;

		int headerOffset = sizeof(UINT64);
		if (!writeHeader)	headerOffset = 0;
		zstream.next_out = CArray(compressedStream) + headerOffset;

        const int Level = 7; // 1 (fastest speed) to 9 (best compression)
        int result = deflateInit2(&zstream, Level, Z_DEFLATED, 15, 8, Z_DEFAULT_STRATEGY);
        if(result != 0) throw MLIB_EXCEPTION("deflateInit2 failed");
        
        result = deflate(&zstream, Z_FINISH);
        if(result != Z_STREAM_END) throw MLIB_EXCEPTION("deflate failed");

        deflateEnd(&zstream);

		if (writeHeader) {
	        ((UINT64*)&compressedStream[0])[0] = decompressedStreamLength;
		}

        compressedStream.resize(zstream.total_out + sizeof(UINT64));
    }
    
    static Vector<BYTE> DecompressStreamFromMemory(const Vector<BYTE> &compressedStream)
    {
        Vector<BYTE> result;
        DecompressStreamFromMemory(compressedStream, result);
        return result;
    }

    static void DecompressStreamFromMemory(const Vector<BYTE> &compressedStream, Vector<BYTE> &decompressedStream)
    {
        UINT decompressedByteCount = ((UINT*)CArray(compressedStream))[0];
        decompressedStream.resize(decompressedByteCount);
        DecompressStreamFromMemory(CArray(compressedStream) + sizeof(UINT64), compressedStream.size() - sizeof(UINT64), CArray(decompressedStream), decompressedStream.size());
    }

    static void DecompressStreamFromMemory(const BYTE *compressedStream, UINT64 compressedStreamLength, BYTE *decompressedStream, UINT64 decompressedStreamLength)
    {
        if(decompressedStreamLength == 0) throw MLIB_EXCEPTION("Caller must provide the length of the decompressed stream");

        uLongf finalByteCount = (uLongf)decompressedStreamLength;
        int result = uncompress(decompressedStream, &finalByteCount, compressedStream, (uLong)compressedStreamLength);
        if(result != Z_OK) throw MLIB_EXCEPTION("uncompress failed");
        if(finalByteCount != decompressedStreamLength) throw MLIB_EXCEPTION("Decompression returned invalid length");
    }

private:
    static const BYTE* CArray(const Vector<BYTE> &v)
    {
        return &v[0];
    }
    static BYTE* CArray(Vector<BYTE> &v)
    {
        return &v[0];
    }
};


//! interface to compress data
class BinaryDataCompressorZLib : public BinaryDataCompressorInterface {
public:
	void compressStreamToMemory(const BYTE *decompressedStream, UINT64 decompressedStreamLength, Vector<BYTE> &compressedStream) const {
		ZLibWrapper::CompressStreamToMemory(decompressedStream, decompressedStreamLength, compressedStream, false);
	}

	void decompressStreamFromMemory(const BYTE *compressedStream, UINT64 compressedStreamLength, BYTE *decompressedStream, UINT64 decompressedStreamLength) const {
		ZLibWrapper::DecompressStreamFromMemory(compressedStream, compressedStreamLength, decompressedStream, decompressedStreamLength);
	}

	std::string getTypename() const {
		return "zlib compression";
	}
};




typedef BinaryDataStream<BinaryDataBufferMemory, BinaryDataCompressorZLib> BinaryDataStreamZLibVector;
typedef BinaryDataStream<BinaryDataBufferFile, BinaryDataCompressorZLib> BinaryDataStreamZLibFile;

#endif