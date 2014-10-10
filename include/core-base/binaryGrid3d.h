
#ifndef CORE_BASE_BINARY_GRID3D_H_
#define CORE_BASE_BINARY_GRID3D_H_

namespace ml {

class BinaryGrid3d {
public:
	BinaryGrid3d(unsigned int rows, unsigned int cols, unsigned int slices) {
		m_rows = rows;
		m_cols = cols;
		m_slices = slices;

		unsigned int numEntries = getNumTotalEntries();
		m_data = new unsigned int[(numEntries + bitsPerUInt - 1) / bitsPerUInt];
	}

	~BinaryGrid3d() {
		SAFE_DELETE_ARRAY(m_data)
	}

	//! clears all voxels
	void clear() {
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
	static const unsigned int bitsPerUInt = sizeof(unsigned int);
	unsigned int*	m_data;
	unsigned int	m_rows, m_cols, m_slices;
};


}

#endif