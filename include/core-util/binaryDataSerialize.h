#pragma once

#ifndef _BINARY_DATA_SERIALIZE_
#define _BINARY_DATA_SERIALIZE_

#include "BinaryDataStream.h"

template<class ChildClass>
class BinaryDataSerialize {
public:
	unsigned int getSizeInBytes() {
		return sizeof(ChildClass);
	}
};

template<class BinaryDataBuffer, class BinaryDataCompressor, class ChildClass>
inline BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& operator<< (BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& s, const BinaryDataSerialize<ChildClass>& o) {		
	s.writeData(*(const ChildClass*)&o);	//cast it to the child class to get the right size
	return s;
} 

template<class BinaryDataBuffer, class BinaryDataCompressor, class ChildClass>
inline BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& operator>> (BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& s, BinaryDataSerialize<ChildClass> &o) {
	s.readData((ChildClass*)&o);
	return s;
}

#endif

