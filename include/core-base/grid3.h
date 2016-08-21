
#ifndef CORE_BASE_GRID3D_H_
#define CORE_BASE_GRID3D_H_

namespace ml
{

	template <class T> class Grid3
	{
	public:
		Grid3();
		Grid3(size_t dimX, size_t dimY, size_t dimZ);
		Grid3(size_t dimX, size_t dimY, size_t dimZ, const T &value);
		Grid3(const vec3ul& dim) : Grid3(dim.x, dim.y, dim.z) {}
		Grid3(const vec3ul& dim, const T& value) : Grid3(dim.x, dim.y, dim.z, value) {}

		Grid3(const Grid3<T> &G);
		Grid3(Grid3<T> &&G);
		Grid3(size_t dimX, size_t dimY, size_t dimZ, const std::function< T(size_t x, size_t y, size_t z) > &fillFunction);

		~Grid3();

        struct iteratorEntry
        {
            iteratorEntry(size_t _x, size_t _y, size_t _z, T &_value)
                : x(_x), y(_y), z(_z), value(_value)
            {

            }
            size_t x;
            size_t y;
            size_t z;
            T &value;
        };

        struct constIteratorEntry
        {
            constIteratorEntry(size_t _x, size_t _y, size_t _z, const T &_value)
                : x(_x), y(_y), z(_z), value(_value)
            {

            }
            size_t x;
            size_t y;
            size_t z;
            const T &value;
        };


        struct iterator
        {
            iterator(Grid3<T> *_grid)
            {
                x = 0;
                y = 0;
                z = 0;
                grid = _grid;
            }
            iterator(const iterator &i)
            {
                x = i.x;
                y = i.y;
                z = i.z;
                grid = i.grid;
            }
            ~iterator() {}
            iterator& operator=(const iterator &i)
            {
                x = i.x;
                y = i.y;
                z = i.z;
                grid = i.grid;
                return *this;
            }
            iterator& operator++()
            {
                x++;
                if (x == grid->getDimX())
                {
                    x = 0;
                    y++;
                    if (y == grid->getDimY())
                    {
                        y = 0;
                        z++;
                        if (z == grid->getDimZ())
                        {
                            grid = NULL;
                        }
                    }
                }
                return *this;
            }
            iteratorEntry operator* () const
            {
                return iteratorEntry(x, y, z, (*grid)(x, y, z));
            }

            bool operator != (const iterator &i) const
            {
                return i.grid != grid;
            }

            template<class U>
            friend void swap(iterator &a, iterator &b);

            size_t x, y, z;

        private:
            Grid3<T> *grid;
        };

        struct constIterator
        {
            constIterator(const Grid3<T> *_grid)
            {
                x = 0;
                y = 0;
                z = 0;
                grid = _grid;
            }
            constIterator(const constIterator &i)
            {
                x = i.x;
                y = i.y;
                z = i.z;
                grid = i.grid;
            }
            ~constIterator() {}
            constIterator& operator=(const constIterator &i)
            {
                x = i.x;
                y = i.y;
                z = i.z;
                grid = i.grid;
                return *this;
            }
            constIterator& operator++()
            {
                x++;
                if (x == grid->getDimX())
                {
                    x = 0;
                    y++;
                    if (y == grid->getDimY())
                    {
                        y = 0;
                        z++;
                        if (z == grid->getDimZ())
                        {
                            grid = NULL;
                        }
                    }
                }
                return *this;
            }
            constIteratorEntry operator* () const
            {
                return constIteratorEntry(x, y, z, (*grid)(x, y, z));
            }

            bool operator != (const constIterator &i) const
            {
                return i.grid != grid;
            }

            template<class U>
            friend void swap(const constIterator &a, const constIterator &b);

            size_t x, y, z;

        private:
            const Grid3<T> *grid;
        };

		//
		// Memory
		//
		void free();
		Grid3<T>& operator = (const Grid3<T> &G);
		Grid3<T>& operator = (Grid3<T> &&G);

		void allocate(size_t dimX, size_t dimY, size_t dimZ);
		void allocate(size_t dimX, size_t dimY, size_t dimZ, const T &value);
		void allocate(const vec3ul& dim) {						allocate(dim.x, dim.y, dim.z);				}
		void allocate(const vec3ul& dim, const T& value) {		allocate(dim.x, dim.y, dim.z, value);		}

		inline Grid3<T>& operator += (const Grid3<T> &right)
		{
			MLIB_ASSERT_STR(m_dimX == right.m_dimX && m_dimY == right.m_dimY && m_dimZ == right.m_dimZ, "grid dimensions must be equal");
			for (size_t i = 0; i < getNumElements(); i++) {
				m_data[i] += right.m_data[i];
			}
			return *this;
		}
		inline Grid3<T>& operator += (T right)
		{
			for (size_t i = 0; i < getNumElements(); i++) {
				m_data[i] += right;
			}
			return *this;
		}
		inline Grid3<T>& operator *= (T right)
		{
			for (size_t i = 0; i < getNumElements(); i++) {
				m_data[i] *= right;
			}
			return *this;
		}

		inline Grid3<T> operator * (T x)
		{
			Grid3<T> result(m_dimX, m_dimY, m_dimZ);
			for (size_t i = 0; i < getNumElements(); i++) {
				result.m_data = m_data * x;
			}
			return result;
		}

		//
		// Accessors
		//
		inline T& operator() (size_t x, size_t y, size_t z)
		{
#if defined(MLIB_BOUNDS_CHECK) || defined(_DEBUG)
			MLIB_ASSERT_STR((x < m_dimX) && (y < m_dimY) && (z < m_dimZ), "Out-of-bounds grid access");
#endif
			return m_data[z*m_dimY*m_dimX + x*m_dimY + y];
		}

		inline const T& operator() (size_t dimX, size_t dimY, size_t slice) const
		{
#if defined(MLIB_BOUNDS_CHECK) || defined(_DEBUG)
			MLIB_ASSERT_STR((dimX < m_dimX) && (dimY < m_dimY) && (slice < m_dimZ), "Out-of-bounds grid access");
#endif
			return m_data[slice*m_dimY*m_dimX + dimX*m_dimY + dimY];
		}

        inline T& operator() (const vec3i &coord)
        {
#if defined(MLIB_BOUNDS_CHECK) || defined(_DEBUG)
            MLIB_ASSERT_STR((coord.x < m_dimX) && (coord.y < m_dimY) && (coord.z < m_dimZ), "Out-of-bounds grid access");
#endif
            return m_data[coord.z*m_dimY*m_dimX + coord.x*m_dimY + coord.y];
        }

        inline const T& operator() (const vec3i &coord) const
        {
#if defined(MLIB_BOUNDS_CHECK) || defined(_DEBUG)
            MLIB_ASSERT_STR((coord.x < m_dimX) && (coord.y < m_dimY) && (coord.z < m_dimZ), "Out-of-bounds grid access");
#endif
            return m_data[coord.z*m_dimY*m_dimX + coord.x*m_dimY + coord.y];
        }

		inline size_t getDimX() const
		{
			return m_dimX;
		}
		inline size_t getDimY() const
		{
			return m_dimY;
		}
		inline size_t getDimZ() const
		{
			return m_dimZ;
		}

		inline vec3ul getDimensions() const {
			return vec3ul(getDimX(), getDimY(), getDimZ());
		}

		size_t getNumElements() const {
			return m_dimX * m_dimY * m_dimZ;
		}

		inline bool isSquare() const
		{
			return (m_dimX == m_dimY && m_dimY == m_dimZ);
		}
		inline T* getData()
		{
			return m_data;
		}
		inline const T* getData() const
		{
			return m_data;
		}

		//
		// Query
		//
		inline bool isValidCoordinate(size_t x, size_t y, size_t z) const
		{
			return (x < m_dimX && y < m_dimY && z < m_dimZ);
		}
        inline bool isValidCoordinate(const vec3i &coord) const
        {
            return (coord.x < m_dimX && coord.y < m_dimY && coord.z < m_dimZ);
        }

		vec3ul getMaxIndex() const;
		const T& getMaxValue() const;
		vec3ul getMinIndex() const;
		const T& getMinValue() const;

		//
		// Modifiers
		//
		void setValues(const T &clearValue);

		void fill(const std::function<T(size_t x, size_t y, size_t z)> &fillFunction)
		{
			for (UINT xIndex = 0; xIndex < m_dimX; xIndex++)
				for (UINT yIndex = 0; yIndex < m_dimY; yIndex++)
					for (UINT zIndex = 0; zIndex < m_dimZ; zIndex++)
					{
						(*this)(xIndex, yIndex, zIndex) = fillFunction(xIndex, yIndex, zIndex);
					}
		}

        iterator begin()
        {
            return iterator(this);
        }

        iterator end()
        {
            return iterator(NULL);
        }

        constIterator begin() const
        {
            return constIterator(this);
        }

        constIterator end() const
        {
            return constIterator(NULL);
        }

	protected:
		T *m_data;
		size_t m_dimX, m_dimY, m_dimZ;
	};

	template <class T> inline bool operator == (const Grid3<T> &a, const Grid3<T> &b)
	{
		if (a.getDimX() != b.getDimX() || a.getDimY() != b.getDimY() || a.getDimZ() != b.getDimZ()) return false;
		const size_t totalEntries = a.getNumElements();
		for (size_t i = 0; i < totalEntries; i++) {
			if (a.ptr()[i] != b.ptr()[i])	return false;
		}
		return true;
	}

	template <class T> inline bool operator != (const Grid3<T> &a, const Grid3<T> &b)
	{
		return !(a == b);
	}

	template<class BinaryDataBuffer, class BinaryDataCompressor, class T>
	inline BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& operator<<(BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& s, const Grid3<T>& g) {
		s << (UINT64)g.getDimX() << (UINT64)g.getDimY() << (UINT64)g.getDimZ();
		s.reserve(sizeof(T) * g.getDimX() * g.getDimY() * g.getDimZ());

		for (UINT64 z = 0; z < g.getDimZ(); z++)
			for (UINT64 y = 0; y < g.getDimY(); y++)
				for (UINT64 x = 0; x < g.getDimX(); x++)
					s << g(x, y, z);
		return s;
	}

	template<class BinaryDataBuffer, class BinaryDataCompressor, class T>
	inline BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& operator>>(BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& s, Grid3<T>& g) {
		UINT64 dimX, dimY, dimZ;
		s >> dimX >> dimY >> dimZ;
		g.allocate(dimX, dimY, dimZ);
		for (UINT64 z = 0; z < g.getDimZ(); z++)
			for (UINT64 y = 0; y < g.getDimY(); y++)
				for (UINT64 x = 0; x < g.getDimX(); x++)
					s >> g(x, y, z);
		return s;
	}

	typedef Grid3<float> Grid3f;
	typedef Grid3<double> Grid3d;
	typedef Grid3<int> Grid3i;
	typedef Grid3<unsigned int> Grid3ui;
	typedef Grid3<unsigned char> Grid3uc;

}  // namespace ml

#include "grid3.inl"

#endif  // CORE_BASE_GRID3D_H_
