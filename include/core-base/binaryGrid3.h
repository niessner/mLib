
#ifndef CORE_BASE_BINARY_GRID3D_H_
#define CORE_BASE_BINARY_GRID3D_H_

namespace ml {


class BinaryGrid3 {
public:
	BinaryGrid3() {
		m_slices = m_cols = m_rows = 0;
		m_data = nullptr;
	}
	BinaryGrid3(size_t rows, size_t cols, size_t slices) {
		m_data = nullptr;
		allocate(rows,cols,slices);
		clear();
	}

	BinaryGrid3(const vec3ui& dim) {
		m_data = nullptr;
		allocate(dim);
		clear();
	}

	BinaryGrid3(const BinaryGrid3& other) {
		m_data = nullptr;
		allocate(other.m_rows, other.m_cols, other.m_slices);
		memcpy(m_data, other.m_data, getNumUInts());
	}

	BinaryGrid3(BinaryGrid3&& other) {
		m_cols = other.m_cols;
		m_rows = other.m_rows;
		m_slices = other.m_slices;

		m_data = other.m_data;

		other.m_rows = 0;
		other.m_cols = 0;
		other.m_slices = 0;

		other.m_data = nullptr;
	}

	~BinaryGrid3() {
		SAFE_DELETE_ARRAY(m_data)
	}

	inline void allocate(size_t rows, size_t cols, size_t slices) {
		SAFE_DELETE_ARRAY(m_data);
		m_rows = rows;
		m_cols = cols;
		m_slices = slices;

		m_data = new unsigned int[getNumUInts()];
	}

	inline void allocate(const vec3ul& dim) {
		allocate(dim.x, dim.y, dim.z);
	}

	inline void operator=(const BinaryGrid3& other) {
		allocate(other.m_rows, other.m_cols, other.m_slices);
		memcpy(m_data, other.m_data, getNumUInts());
	}

	inline void operator=(BinaryGrid3&& other) {
		std::swap(m_rows, other.m_rows);
		std::swap(m_cols, other.m_cols);
		std::swap(m_slices, other.m_slices);
		std::swap(m_data, other.m_data);
	}

	inline bool operator==(const BinaryGrid3& other) const {
		if (m_rows != other.m_rows ||
			m_cols != other.m_cols ||
			m_rows != other.m_rows)	return false;

		for (unsigned int i = 0; i < getNumUInts(); i++) {
			if (m_data[i] != other.m_data[i])	return false;
		}

		return true;
	}

	inline bool operator!=(const BinaryGrid3& other) const {
		return !(*this == other);
	}

	//! clears all voxels
	inline void clear() {
		for (size_t i = 0; i < getNumUInts(); i++) {
			m_data[i] = 0;
		}
	}

	inline bool isVoxelSet(size_t row, size_t col, size_t slice) const {
		size_t linIdx = slice*m_cols*m_rows + row*m_cols + col;
		size_t baseIdx = linIdx / bitsPerUInt;
		size_t localIdx = linIdx % bitsPerUInt;
		return (m_data[baseIdx] & (1 << localIdx)) != 0;
	}

	inline bool isVoxelSet(const vec3ui& v) {
		return isVoxelSet(v.x, v.y, v.z);
	}

	inline void setVoxel(size_t row,size_t col, size_t slice) {
		size_t linIdx = slice*m_cols*m_rows + row*m_cols + col;
		size_t baseIdx = linIdx / bitsPerUInt;
		size_t localIdx = linIdx % bitsPerUInt;
		m_data[baseIdx] |= (1 << localIdx);
	}

	inline void setVoxel(const vec3ul& v) {
		setVoxel(v.x, v.y, v.z);
	}

	inline void clearVoxel(size_t row, size_t col, size_t slice) {
		size_t linIdx = slice*m_cols*m_rows + row*m_cols + col;
		size_t baseIdx = linIdx / bitsPerUInt;
		size_t localIdx = linIdx % bitsPerUInt;
		m_data[baseIdx] &= ~(1 << localIdx);
	}

	inline void clearVoxel(const vec3ul& v) {
		clearVoxel(v.x, v.y, v.z);
	}

	inline void toggleVoxel(size_t row, size_t col, size_t slice) {
		size_t linIdx = slice*m_cols*m_rows + row*m_cols + col;
		size_t baseIdx = linIdx / bitsPerUInt;
		size_t localIdx = linIdx % bitsPerUInt;
		m_data[baseIdx] ^= (1 << localIdx);
	}

	inline void toggleVoxel(const vec3ul& v) {
		toggleVoxel(v.x, v.y, v.z);
	}

	inline void toggleVoxelAndBehindRow(size_t row, size_t col, size_t slice) {
		for (size_t i = row; i < m_rows; i++) {
			toggleVoxel(i, col, slice);
		}
	}
	inline void toggleVoxelAndBehindRow(const vec3ul& v) {
		toggleVoxelAndBehindRow(v.x, v.y, v.z);
	}

	inline void toggleVoxelAndBehindSlice(size_t row, size_t col, size_t slice) {
		for (size_t i = slice; i < m_slices; i++) {
			toggleVoxel(row, col, i);
		}
	}
	inline void toggleVoxelAndBehindSlice(const vec3ul& v) {
		toggleVoxelAndBehindSlice(v.x, v.y, v.z);
	}

	inline void print() const {
		for (size_t z = 0; z < m_slices; z++) {
			std::cout << "slice0" << std::endl;
			for (size_t y = 0; y < m_cols; y++) {
				for (size_t x = 0; x < m_rows; x++) {
					if (isVoxelSet(x,y,z)) {
						std::cout << "1";
					} else {
						std::cout << "0";
					}
					
				}
				std::cout << "\n";
			}
		}
	}

  inline size_t dimX() const {
    return m_rows;
  }
  inline size_t dimY() const {
    return m_cols;
  }
  inline size_t dimZ() const {
    return m_slices;
  }

  inline vec3ul getDimensions() const {
	  return vec3ul(dimX(), dimY(), dimZ());
  }

	inline size_t getNumTotalEntries() const {
		return m_rows*m_cols*m_slices;
	}


private:

	inline size_t getNumUInts() const {
		size_t numEntries = getNumTotalEntries();
		return (numEntries + bitsPerUInt - 1) / bitsPerUInt;
	}

	static const unsigned int bitsPerUInt = sizeof(unsigned int);
	unsigned int*	m_data;
	size_t			m_rows, m_cols, m_slices;
};


}

#endif