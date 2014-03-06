#pragma  once

template<class T>
class SparseGrid3D {
public:
	SparseGrid3D() {
		m_Data = 0;
		m_SizeX = 0;
		m_SizeY = 0;
		m_SizeZ = 0;
	}
	~SparseGrid3D() {
		SAFE_DELETE_ARRAY(m_Data);
		m_SizeX = 0;
		m_SizeY = 0;
		m_SizeZ = 0;
	}

	void insert(const T& v) {

	}

	T& operator()(const vec3i& i) {

	}
	const T& operator()(const vec3i& i) const {

	}

	//! inserts the value if it does not exist; returns the reference
	inline T& operator() (UINT row, UINT col, UINT slice) {
#if defined(MLIB_BOUNDS_CHECK) || defined(_DEBUG)
		MLIB_ASSERT_STR( (row < m_rows) && (col < m_cols) && (slice < m_slices), "Out-of-bounds grid access");
#endif
	}
private:
	const UINT m_BucketSize;
	const UINT m_NumBuckets;
	T* m_Data;
	UINT m_rows, m_cols, m_slices;
};