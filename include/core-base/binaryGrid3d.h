
#ifndef CORE_BASE_BINARY_GRID3D_H_
#define CORE_BASE_BINARY_GRID3D_H_

namespace ml {

class BinaryGrid3d {
public:
	BinaryGrid3d() {
		m_slices = m_cols = m_rows = 0;
		m_data = NULL;
	}
	BinaryGrid3d(unsigned int rows, unsigned int cols, unsigned int slices) {
		m_data = NULL;
		allocate(rows,cols,slices);
	}

	BinaryGrid3d(const BinaryGrid3d& other) {
		m_data = NULL;
		allocate(other.rows(), other.cols(), other.slices());
		memcpy(m_data, other.m_data, getNumUInts());
	}
	BinaryGrid3d(BinaryGrid3d&& other) {
		m_cols = other.m_cols;
		m_rows = other.m_rows;
		m_slices = other.m_slices;

		m_data = other.m_data;

		other.m_rows = 0;
		other.m_cols = 0;
		other.m_slices = 0;

		other.m_data = nullptr;
	}

	~BinaryGrid3d() {
		SAFE_DELETE_ARRAY(m_data)
	}

	inline void allocate(unsigned int rows, unsigned int cols, unsigned int slices) {
		SAFE_DELETE_ARRAY(m_data);
		m_rows = rows;
		m_cols = cols;
		m_slices = slices;

		m_data = new unsigned int[getNumUInts()];
	}

	inline void operator=(const BinaryGrid3d& other) {
		allocate(other.rows(), other.cols(), other.slices());
		memcpy(m_data, other.m_data, getNumUInts());
	}

	inline void operator=(BinaryGrid3d&& other) {
		std::swap(m_rows, other.m_rows);
		std::swap(m_cols, other.m_cols);
		std::swap(m_slices, other.m_slices);
		std::swap(m_data, other.m_data);
	}

	inline bool operator==(const BinaryGrid3d& other) const {
		if (m_rows != other.m_rows ||
			m_cols != other.m_cols ||
			m_rows != other.m_rows)	return false;

		for (unsigned int i = 0; i < getNumUInts(); i++) {
			if (m_data[i] != other.m_data[i])	return false;
		}

		return true;
	}

	inline bool operator!=(const BinaryGrid3d& other) const {
		return !(*this == other);
	}

	//! clears all voxels
	inline void clear() {
		unsigned int numEntries = getNumTotalEntries();
		unsigned int numUInts = (numEntries + bitsPerUInt - 1) / bitsPerUInt;
		for (unsigned int i = 0; i < numUInts; i++) {
			m_data[i] = 0;
		}
	}

	inline bool isVoxelSet(unsigned int row, unsigned int col, unsigned int slice) const {
		unsigned int linIdx = slice*m_cols*m_rows + row*m_cols + col;
		unsigned int baseIdx = linIdx / bitsPerUInt;
		unsigned int localIdx = linIdx % bitsPerUInt;
		return (m_data[baseIdx] & (1 << localIdx)) != 0;
	}

	inline void setVoxel(unsigned int row, unsigned int col, unsigned int slice) {
		unsigned int linIdx = slice*m_cols*m_rows + row*m_cols + col;
		unsigned int baseIdx = linIdx / bitsPerUInt;
		unsigned int localIdx = linIdx % bitsPerUInt;
		m_data[baseIdx] |= (1 << localIdx);
	}

	inline void clearVoxel(unsigned int row, unsigned int col, unsigned int slice) {
		unsigned int linIdx = slice*m_cols*m_rows + row*m_cols + col;
		unsigned int baseIdx = linIdx / bitsPerUInt;
		unsigned int localIdx = linIdx % bitsPerUInt;
		m_data[baseIdx] &= ~(1 << localIdx);
	}

	inline void print() const {
		for (unsigned int z = 0; z < m_slices; z++) {
			std::cout << "slice0" << std::endl;
			for (unsigned int y = 0; y < m_cols; y++) {
				for (unsigned int x = 0; x < m_rows; x++) {
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

	inline unsigned int rows() const {
		return m_rows;
	}
	inline unsigned int cols() const {
		return m_cols;
	}
	inline unsigned int slices() const {
		return m_slices;
	}

	inline unsigned int getNumTotalEntries() const {
		return m_rows*m_cols*m_slices;
	}

private:

	inline unsigned getNumUInts() const {
		unsigned int numEntries = getNumTotalEntries();
		return (numEntries + bitsPerUInt - 1) / bitsPerUInt;
	}

	static const unsigned int bitsPerUInt = sizeof(unsigned int);
	unsigned int*	m_data;
	unsigned int	m_rows, m_cols, m_slices;
};


}

#endif