
#ifndef CORE_MATH_DENSEMATRIX_H_
#define CORE_MATH_DENSEMATRIX_H_

namespace ml {

template <class D> class DenseMatrix
{
public:
	DenseMatrix()
	{
		m_rows = 0;
		m_cols = 0;
	}

	DenseMatrix(const DenseMatrix<D>& s)
	{
		m_rows = s.m_rows;
		m_cols = s.m_cols;
		m_data = s.m_data;
		m_dataPtr = &m_data[0];
	}

	DenseMatrix(DenseMatrix &&s)
	{
		m_rows = s.m_rows;
		m_cols = s.m_cols;
		s.m_rows = 0;
		s.m_cols = 0;
		m_data = std::move(s.m_data);
		m_dataPtr = &m_data[0];
	}

	explicit DenseMatrix(UINT squareDimension)
	{
		m_rows = squareDimension;
		m_cols = squareDimension;
		m_data.allocate(m_rows * m_cols);
		m_dataPtr = m_data.ptr();
	}

	explicit DenseMatrix(const MathVector<D> &diagonal)
	{
		m_rows = (UINT)diagonal.size();
		m_cols = (UINT)diagonal.size();
		m_data.resize(m_rows * m_cols);
		m_dataPtr = &m_data[0];
		for(UINT row = 0; row < m_rows; row++)
		{
			for(UINT col = 0; col < m_cols; col++)
				(*this)(row, col) = 0.0;
			(*this)(row, row) = diagonal[row];
		}

	}

	DenseMatrix(UINT rows, UINT cols)
	{
		m_rows = rows;
		m_cols = cols;
		m_data.resize(m_rows * m_cols);
		m_dataPtr = &m_data[0];
	}

	DenseMatrix(const std::string &s, MatrixStringFormat format)
	{
		if(format == MatrixStringFormatMathematica)
		{
			//
			// this is really a dense format and should be loaded as such, then cast into a SparseMatrix
			//
			std::vector<std::string> data = ml::util::split(s,"},{");
			m_rows = (UINT)data.size();
			m_cols = (UINT)ml::util::split(data[0], ",").size();
			m_data.resize(m_rows * m_cols);
			m_dataPtr = &m_data[0];

			for(UINT row = 0; row < m_rows; row++)
			{
				std::vector<std::string> values = ml::util::split(data[row], ",");
				for(UINT col = 0; col < values.size(); col++)
				{
					const std::string s = ml::util::replace(ml::util::replace(values[col], "{",""), "}","");
					(*this)(row, col) = (D)std::stod(s);
				}
			}
		}
		else
		{
			MLIB_ERROR("invalid matrix string format");
		}
	}

	void operator=(const DenseMatrix<D>& s)
	{
		m_rows = s.m_rows;
		m_cols = s.m_cols;
		m_data = s.m_data;
		m_dataPtr = m_data.ptr();
	}

	void operator=(DenseMatrix<D>&& s)
	{
		m_rows = s.m_rows;
		m_cols = s.m_cols;
		s.m_rows = 0;
		s.m_cols = 0;
		m_data = std::move(s.m_data);
		m_dataPtr = m_data.ptr();
	}

	//
	// Accessors
	//
	D& operator()(UINT row, UINT col)
	{
		return m_dataPtr[row * m_cols + col];
	}
	D operator()(UINT row, UINT col) const
	{
		return m_dataPtr[row * m_cols + col];
	}
	UINT rows() const
	{
		return m_rows;
	}
	UINT cols() const
	{
		return m_cols;
	}
	bool square() const
	{
		return (m_rows == m_cols);
	}
	std::vector<D> diagonal() const
	{
		MLIB_ASSERT_STR(square(), "diagonal called on non-square matrix");
		std::vector<D> result(m_rows);
		for(UINT row = 0; row < m_rows; row++)
			result[row] = m_data[row * m_cols + row];
		return result;
	}

	//
	// math functions
	//
	DenseMatrix<D> transpose() const;
	D maxMagnitude() const;
	DenseMatrix<D> inverse();
	void invertInPlace();
	bool valid() const;

	//
	// overloaded operator helpers
	//
	static DenseMatrix<D> add(const DenseMatrix<D> &A, const DenseMatrix<D> &B);
	static DenseMatrix<D> subtract(const DenseMatrix<D> &A, const DenseMatrix<D> &B);
	static DenseMatrix<D> multiply(const DenseMatrix<D> &A, D c);
	static std::vector<D> multiply(const DenseMatrix<D> &A, const std::vector<D> &v);
	static DenseMatrix<D> multiply(const DenseMatrix<D> &A, const DenseMatrix<D> &B);

	//
	// common matrices
	//
	static DenseMatrix<D> identity(int n)
	{
		return DenseMatrix<D>(MathVector<D>(n, (D)1.0));
	}

private:
	UINT m_rows, m_cols;
	D* m_dataPtr;
    std::vector< D > m_data;
};

template<class D>
DenseMatrix<D> operator + (const DenseMatrix<D> &A, const DenseMatrix<D> &B)
{
	return DenseMatrix<D>::add(A, B);
}

template<class D>
DenseMatrix<D> operator - (const DenseMatrix<D> &A, const DenseMatrix<D> &B)
{
	return DenseMatrix<D>::subtract(A, B);
}

template<class D>
DenseMatrix<D> operator * (const DenseMatrix<D> &A, const DenseMatrix<D> &B)
{
	return DenseMatrix<D>::multiply(A, B);
}

template<class D>
std::vector<D> operator * (const DenseMatrix<D> &A, const MathVector<D> &B)
{
	return DenseMatrix<D>::multiply(A, B);
}

template<class D>
DenseMatrix<D> operator * (const DenseMatrix<D> &A, D val)
{
	return DenseMatrix<D>::multiply(A, val);
}

}  // namespace ml

#include "denseMatrix.cpp"

#endif  // CORE_MATH_DENSEMATRIX_H_