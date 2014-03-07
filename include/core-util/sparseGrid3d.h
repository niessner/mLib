#pragma  once

template<>
struct std::hash<vec3i> : public std::unary_function<vec3i, size_t> {
	size_t operator()(const vec3i& v) const {
		//TODO larger prime number (64 bit) to match size_t
		const size_t p0 = 73856093;
		const size_t p1 = 19349669;
		const size_t p2 = 83492791;
		const size_t res = ((size_t)v.x * p0)^((size_t)v.y * p1)^((size_t)v.z * p2);
		return res;
	}
};

template<class T>
class SparseGrid3D {
public:
	
	SparseGrid3D(float maxLoadFactor = 0.6, size_t reserveBuckets = 64) {
		m_Data.reserve(reserveBuckets);
		m_Data.max_load_factor(maxLoadFactor);
	}
	~SparseGrid3D() {

	}

	bool exists(const vec3i& i) {
		return (m_Data.find(i) != m_Data.end());
	}

	const T& operator()(const vec3i& i) const {
		return m_Data.find(i)->second;
	}

	//! if the element does not exist, it will be created with its default constructor
	T& operator()(const vec3i& i) {
		return m_Data[i];
	}

	const T& operator()(int x, int y, int z) const {
		return (*this)(vec3i(x,y,z));
	}
	T& operator()(int x, int y, int z) {
		return (*this)(vec3i(x,y,z));
	}

	const T& operator[](const vec3i& i) const {
		return (*this)(i);
	}
	T& operator[](const vec3i& i) {
		return (*this)(i);
	}

	template<class U>
	friend std::ostream& operator<<(std::ostream& s, const SparseGrid3D<T>& g);
private:
	std::unordered_map<vec3i, T, std::hash<vec3i>> m_Data;
};

template<class T>
inline std::ostream& operator<<(std::ostream& s, const SparseGrid3D<T>& g) {
	for (auto iter = g.m_Data.begin(); iter != g.m_Data.end(); iter++) {
		s << "\t" << iter->first << "\t: " << iter->second << std::endl;
	}
	return s;
}