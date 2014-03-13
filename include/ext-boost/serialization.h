#ifndef EXT_BOOST_SERIALIZATION_H_
#define EXT_BOOST_SERIALIZATION_H_

#include <boost/serialization/list.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/array.hpp>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include <boost/asio/streambuf.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/traits.hpp>

namespace ml {

class InOutArchive {
  public:
	InOutArchive()
		: m_buf()
		, m_os(&m_buf, std::ios::binary)
		, m_is(&m_buf, std::ios::binary)
		, m_oa(m_os)
		, m_ia(m_is) { }
	~InOutArchive() { }

	template<typename T>
	inline InOutArchive& operator<<(T const& obj) {
		m_oa << obj;
		return *this;
	}

	template<typename T>
	inline InOutArchive& operator>>(T& obj) {
		m_ia >> obj;
		return *this;
	}

  private:
	boost::asio::streambuf m_buf;
	std::ostream m_os;
	std::istream m_is;
	boost::archive::binary_oarchive m_oa;
	boost::archive::binary_iarchive m_ia;
};

}  // namespace ml

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
