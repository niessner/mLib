
#ifndef CORE_MATH_DENSEMATRIX_H_
#define CORE_MATH_DENSEMATRIX_H_

namespace ml {

template <class FloatType> class DenseMatrix
{
public:
	DenseMatrix()
	{
		m_rows = 0;
		m_cols = 0;
	}

	DenseMatrix(const DenseMatrix<FloatType>& s)
	{
		m_rows = s.m_rows;
		m_cols = s.m_cols;
		m_data = s.m_data;
		m_dataPtr = &m_data[0];
	}

    DenseMatrix(DenseMatrix<FloatType> &&s)
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
		m_data.resize(m_rows * m_cols);
		m_dataPtr = &m_data[0];
	}

	explicit DenseMatrix(const MathVector<FloatType> &diagonal)
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
					(*this)(row, col) = (FloatType)std::stod(s);
				}
			}
		}
		else
		{
			MLIB_ERROR("invalid matrix string format");
		}
	}

    DenseMatrix(const Matrix4x4<FloatType> &m)
    {
        m_rows = 4;
        m_cols = 4;
        m_data.resize(16);
        m_dataPtr = &m_data[0];
        for (unsigned int element = 0; element < m_data.size(); element++)
            m_data[element] = m[element];
    }

	DenseMatrix(const Matrix3x3<FloatType> &m)
	{
		m_rows = 3;
		m_cols = 3;
		m_data.resize(9);
		m_dataPtr = &m_data[0];
		for (unsigned int element = 0; element < m_data.size(); element++)
			m_data[element] = m[element];
	}

	DenseMatrix(const Matrix2x2<FloatType> &m)
	{
		m_rows = 2;
		m_cols = 2;
		m_data.resize(4);
		m_dataPtr = &m_data[0];
		for (unsigned int element = 0; element < m_data.size(); element++)
			m_data[element] = m[element];
	}


	void operator=(const DenseMatrix<FloatType>& s)
	{
		m_rows = s.m_rows;
		m_cols = s.m_cols;
		m_data = s.m_data;
        m_dataPtr = &m_data[0];
	}

	void operator=(DenseMatrix<FloatType>&& s)
	{
		m_rows = s.m_rows;
		m_cols = s.m_cols;
		s.m_rows = 0;
		s.m_cols = 0;
		m_data = std::move(s.m_data);
		m_dataPtr = &m_data[0];
	}

	//
	// Accessors
	//
	FloatType& operator()(UINT row, UINT col)
	{
		return m_dataPtr[row * m_cols + col];
	}
	FloatType operator()(UINT row, UINT col) const
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

	//! Access i-th element of the Matrix for constant access
	inline FloatType operator[] (unsigned int i) const {
		assert(i < m_cols*m_rows);
		return m_dataPtr[i];
	}
	//! Access i-th element of the Matrix
	inline  FloatType& operator[] (unsigned int i) {
		assert(i < m_cols*m_rows);
		return m_dataPtr[i];
	}

	std::vector<FloatType> diagonal() const
	{
		MLIB_ASSERT_STR(square(), "diagonal called on non-square matrix");
		std::vector<FloatType> result(m_rows);
		for(UINT row = 0; row < m_rows; row++)
			result[row] = m_data[row * m_cols + row];
		return result;
	}
    const FloatType* ptr() const
    {
        return m_dataPtr;
    }

	//
	// math functions
	//
	DenseMatrix<FloatType> transpose() const;
	FloatType maxMagnitude() const;
	DenseMatrix<FloatType> inverse();
	void invertInPlace();
	bool valid() const;

	//
	// overloaded operator helpers
	//
	static DenseMatrix<FloatType> add(const DenseMatrix<FloatType> &A, const DenseMatrix<FloatType> &B);
	static DenseMatrix<FloatType> subtract(const DenseMatrix<FloatType> &A, const DenseMatrix<FloatType> &B);
	static DenseMatrix<FloatType> multiply(const DenseMatrix<FloatType> &A, FloatType c);
	static std::vector<FloatType> multiply(const DenseMatrix<FloatType> &A, const std::vector<FloatType> &v);
	static DenseMatrix<FloatType> multiply(const DenseMatrix<FloatType> &A, const DenseMatrix<FloatType> &B);

	//
	// common matrices
	//
	static DenseMatrix<FloatType> identity(int n)
	{
		return DenseMatrix<FloatType>(MathVector<FloatType>(n, (FloatType)1.0));
	}

	unsigned int rank(FloatType eps = (FloatType)0.00001) const {
		if (!square())	throw MLIB_EXCEPTION("");
		return util::rank<DenseMatrix<FloatType>, FloatType>(*this, m_rows, eps);
	} 
private:
	UINT m_rows, m_cols;
	FloatType* m_dataPtr;
    std::vector< FloatType > m_data;
};

template<class FloatType>
DenseMatrix<FloatType> operator + (const DenseMatrix<FloatType> &A, const DenseMatrix<FloatType> &B)
{
	return DenseMatrix<FloatType>::add(A, B);
}

template<class FloatType>
DenseMatrix<FloatType> operator - (const DenseMatrix<FloatType> &A, const DenseMatrix<FloatType> &B)
{
	return DenseMatrix<FloatType>::subtract(A, B);
}

template<class FloatType>
DenseMatrix<FloatType> operator * (const DenseMatrix<FloatType> &A, const DenseMatrix<FloatType> &B)
{
	return DenseMatrix<FloatType>::multiply(A, B);
}

template<class FloatType>
std::vector<FloatType> operator * (const DenseMatrix<FloatType> &A, const MathVector<FloatType> &B)
{
	return DenseMatrix<FloatType>::multiply(A, B);
}

template<class FloatType>
DenseMatrix<FloatType> operator * (const DenseMatrix<FloatType> &A, FloatType val)
{
	return DenseMatrix<FloatType>::multiply(A, val);
}

//! writes to a stream
template <class FloatType> 
inline std::ostream& operator<<(std::ostream& s, const DenseMatrix<FloatType>& m)
{ 
	for (unsigned int i = 0; i < m.rows(); i++) {
		for (unsigned int j = 0; j < m.cols(); j++) {
			s << m(i,j) << " ";
		}
		std::cout << std::endl;
	}
	return s;
}

typedef DenseMatrix<float> DenseMatrixf;
typedef DenseMatrix<double> DenseMatrixd;

}  // namespace ml

#include "denseMatrix.cpp"

#endif  // CORE_MATH_DENSEMATRIX_H_