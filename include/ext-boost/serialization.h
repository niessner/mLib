#ifndef EXT_BOOST_SERIALIZATION_H_
#define EXT_BOOST_SERIALIZATION_H_

#include <boost/serialization/utility.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/array.hpp>

#include <boost/archive/tmpdir.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include <boost/asio/streambuf.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/filter/bzip2.hpp>

namespace boost {
namespace serialization {

template<class Archive, class T>
inline void serialize(Archive& ar, ml::point3d<T>& p, const unsigned int version) {
	ar & p.array;
}

template<class Archive, class T>
inline void serialize(Archive& ar, ml::Matrix4x4<T>& m, const unsigned int version) {
	ar & m.matrix;
}

}  // namespace serialization
}  // namespace boost

#endif  // EXT_BOOST_SERIALIZATION_H_
