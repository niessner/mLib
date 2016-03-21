
#ifndef CORE_BASE_GRID2D_H_
#define CORE_BASE_GRID2D_H_

namespace ml
{
	template <class T> class Grid2
	{
	public:
		//
		// Grid2 iterators
		//

		struct iteratorEntry
		{
			iteratorEntry(size_t _x, size_t _y, T &_value)
				: x(_x), y(_y), value(_value)
			{

			}
			size_t x;
			size_t y;
			T &value;
		};

		struct constIteratorEntry
		{
			constIteratorEntry(size_t _x, size_t _y, const T &_value)
				: x(_x), y(_y), value(_value)
			{

			}
			size_t x;
			size_t y;
			const T &value;
		};


		struct iterator
		{
			iterator(Grid2<T> *_grid)
			{
				x = 0;
				y = 0;
				grid = _grid;
			}
			iterator(const iterator &i)
			{
				x = i.x;
				y = i.y;
				grid = i.grid;
			}
			~iterator() {}
			iterator& operator=(const iterator &i)
			{
				x = i.x;
				y = i.y;
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
						grid = NULL;
					}
				}
				return *this;
			}
			iteratorEntry operator* () const
			{
				return iteratorEntry(x, y, (*grid)(x, y));
			}

			bool operator != (const iterator &i) const
			{
				return i.grid != grid;
			}

			friend void swap(iterator &a, iterator &b);

			size_t x, y;

		private:
			Grid2<T> *grid;
		};

		struct constIterator
		{
			constIterator(const Grid2<T> *_grid)
			{
				x = 0;
				y = 0;
				grid = _grid;
			}
			constIterator(const constIterator &i)
			{
				x = i.x;
				y = i.y;
				grid = i.grid;
			}
			~constIterator() {}
			constIterator& operator=(const constIterator &i)
			{
				x = i.x;
				y = i.y;
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
						grid = NULL;
					}
				}
				return *this;
			}
			constIteratorEntry operator* () const
			{
				return constIteratorEntry(x, y, (*grid)(x, y));
			}

			bool operator != (const constIterator &i) const
			{
				return i.grid != grid;
			}

			friend void swap(const constIterator &a, const constIterator &b);

			size_t x, y;

		private:
			const Grid2<T> *grid;
		};

		//
		// Grid2 implementation
		//

		Grid2();
		Grid2(size_t dimX, size_t dimY);
		Grid2(size_t dimX, size_t dimY, const T &value);
		Grid2(const vec2ul& dim) : Grid2(dim.x, dim.y) {}
		Grid2(const vec2ul& dim, const T& value) : Grid2(dim.x, dim.y, value) {}

		Grid2(const Grid2<T> &G);
		Grid2(Grid2<T> &&G);
		Grid2(size_t dimX, size_t dimY, const std::function< T(size_t x, size_t y) > &fillFunction);

		~Grid2();

		//
		// Memory
		//
		void free();
		Grid2<T>& operator = (const Grid2<T> &G);
		Grid2<T>& operator = (Grid2<T> &&G);

		void allocate(size_t dimX, size_t dimY);
		void allocate(size_t dimX, size_t dimY, const T &value);
		void allocate(const vec2ul& dim) { allocate(dim.x, dim.y); }
		void allocate(const vec2ul& dim, const T& value) { allocate(dim.x, dim.y, value); }

		inline Grid2<T>& operator += (const Grid2<T> &value)
		{
            MLIB_ASSERT_STR(m_dimX == value.m_dimX && m_dimY == value.m_dimY, "grid dimensions must be equal");
			for (size_t y = 0; y < m_dimY; y++)
				for (size_t x = 0; x < m_dimX; x++)
					m_data[y * m_dimX + x] += value(x, y);
			return *this;
		}

		inline Grid2<T>& operator += (T value)
		{
			for (size_t y = 0; y < m_dimY; y++)
				for (size_t x = 0; x < m_dimX; x++)
					m_data[y * m_dimX + x] += value;
			return *this;
		}

		inline Grid2<T>& operator *= (T value)
		{
			for (size_t y = 0; y < m_dimY; y++)
				for (size_t x = 0; x < m_dimX; x++)
					m_data[y * m_dimX + x] *= value;
			return *this;
		}

		inline Grid2<T> operator * (T value)
		{
			Grid2<T> result(m_dimX, m_dimY);
			for (size_t y = 0; y < m_dimY; y++)
				for (size_t x = 0; x < m_dimX; x++)
					result(x, y) = m_data[y * m_dimX + x] * value;
			return result;
		}

		//
		// Accessors
		//
		inline T& operator() (size_t x, size_t y)
		{
#if defined(MLIB_BOUNDS_CHECK) || defined(_DEBUG)
			MLIB_ASSERT_STR((x < m_dimX) && (y < m_dimY), "Out-of-bounds grid access");
#endif
			return m_data[y * m_dimX + x];
		}
		inline const T& operator() (size_t x, size_t y) const
		{
#if defined(MLIB_BOUNDS_CHECK) || defined(_DEBUG)
			MLIB_ASSERT_STR((x < m_dimX) && (y < m_dimY), "Out-of-bounds grid access");
#endif
			return m_data[y * m_dimX + x];
		}

        inline T& operator() (const vec2i &coord)
        {
            return (*this)(coord.x, coord.y);
        }

        inline const T& operator() (const vec2i &coord) const
        {
            return (*this)(coord.x, coord.y);
        }

		inline size_t getDimX() const
		{
			return m_dimX;
		}

		inline size_t getDimY() const
		{
			return m_dimY;
		}

		inline vec2ul getDimensions() const {
			return vec2ul(m_dimX, m_dimY);
		}

		inline size_t getNumElements() const
		{
			return m_dimX * m_dimY;
		}

		inline bool isSquare() const
		{
			return (m_dimX == m_dimY);
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
		inline bool isValidCoordinate(int x, int y) const
		{
			return (x >= 0 && x < int(m_dimX) && y >= 0 && y < int(m_dimY));
		}
        inline bool isValidCoordinate(vec2i coord) const
        {
            return (coord.x >= 0 && coord.x < int(m_dimX) && coord.y >= 0 && coord.y < int(m_dimY));
        }

		//
		// TODO: rename
		//
		void setRow(size_t row, const std::vector<T> &values)
		{
			for (size_t col = 0; col < m_dimY; col++) m_data[row * m_dimX + col] = values[col];
		}

		void setCol(size_t col, const std::vector<T> &values)
		{
			for (size_t row = 0; row < m_dimX; row++) m_data[row * m_dimX + col] = values[row];
		}

		std::vector<T> getRow(size_t y) const
		{
			std::vector<T> result(m_dimX);
			const T *CPtr = m_data;
			for (size_t x = 0; x < m_dimX; x++)
			{
				result[x] = CPtr[y * m_dimX + x];
			}
			return result;
		}

		std::vector<T> getCol(size_t x) const
		{
			std::vector<T> result(m_dimY);
			const T *CPtr = m_data;
			for (size_t y = 0; y < m_dimY; y++)
			{
				result[y] = CPtr[y * m_dimX + x];
			}
			return result;
		}


		vec2ul getMaxIndex() const;
		const T& getMaxValue() const;
		vec2ul getMinIndex() const;
		const T& getMinValue() const;

		//
		// Modifiers
		//
		void setValues(const T &value);

		void fill(const std::function< T(size_t x, size_t y) > &fillFunction);

		std::vector<T> toStdVector() const
		{
			std::vector<T> result;
			for (size_t i = 0; i < m_dimX * m_dimY; i++)
				result.push_back(i);
			return result;
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
		size_t m_dimX, m_dimY;
	};

	template <class T> inline bool operator == (const Grid2<T> &a, const Grid2<T> &b)
	{
		if (a.getDimX() != b.getDimX() || a.getDimY() != b.getDimY()) return false;
		for (size_t y = 0; y < a.getDimY(); y++)
			for (size_t x = 0; x < a.getDimX(); x++)
				if (a(x, y) != b(x, y))
					return false;
		return true;
	}

	template <class T> inline bool operator != (const Grid2<T> &a, const Grid2<T> &b)
	{
		return !(a == b);
	}


	template<class BinaryDataBuffer, class BinaryDataCompressor, class T>
	inline BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& operator<<(BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& s, const Grid2<T>& g) {
		s << (UINT64)g.getDimX() << (UINT64)g.getDimY();
		s.reserve(sizeof(T) * g.getDimX() * g.getDimY());
		for (UINT64 y = 0; y < g.getDimY(); y++)
			for (UINT64 x = 0; x < g.getDimX(); x++)
				s << g(x, y);
		return s;
	}

	template<class BinaryDataBuffer, class BinaryDataCompressor, class T>
	inline BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& operator>>(BinaryDataStream<BinaryDataBuffer, BinaryDataCompressor>& s, Grid2<T>& g) {
		UINT64 dimX, dimY;
		s >> dimX >> dimY;
		g.allocate(dimX, dimY);
		for (UINT64 y = 0; y < g.getDimY(); y++)
			for (UINT64 x = 0; x < g.getDimX(); x++)
				s << g(x, y);
		return s;
	}

	typedef Grid2<float> Grid2f;
    typedef Grid2<int> Grid2i;
	typedef Grid2<double> Grid2d;
    typedef Grid2<unsigned char> Grid2uc;

}

#include "grid2.cpp"

#endif  // CORE_BASE_GRID2D_H_
