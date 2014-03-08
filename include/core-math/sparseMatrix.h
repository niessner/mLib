
#pragma once
#ifndef SPARSE_MATRIX_H
#define SPARSE_MATRIX_H

enum MatrixStringFormat
{
	MatrixStringFormatMathematica,
};

template <class D> struct SparseRowEntry
{
	SparseRowEntry() {}
	SparseRowEntry(UINT _col, D _val)
	{
		col = _col;
		val = _val;
	}
	UINT col;
	D val;
};

template <class D> struct SparseRow
{
	D& operator()(UINT col)
	{
		for(SparseRowEntry<D> &e : entries)
		{
			if(e.col == col) return e.val;
		}
		entries.pushBack(SparseRowEntry<D>(col, 0.0));
		return entries.back().val;
	}
	D operator()(UINT col) const
	{
		for(const SparseRowEntry<D> &e : entries)
		{
			if(e.col == col) return e.val;
		}
		return 0.0;
	}
	std::vector< SparseRowEntry<D> > entries;
};

template <class D> class SparseMatrix
{
public:
	SparseMatrix()
	{
		m_rows = 0;
		m_cols = 0;
	}

	SparseMatrix(const SparseMatrix<D>& s)
	{
		m_rows = s.m_rows;
		m_cols = s.m_cols;
		m_data = s.m_data;
	}

	SparseMatrix(SparseMatrix &&s)
	{
		m_rows = s.m_rows;
		m_cols = s.m_cols;
		s.m_rows = 0;
		s.m_cols = 0;
		m_data = std::move(s.m_data);
	}

	explicit SparseMatrix(UINT squareDimension)
	{
		m_rows = squareDimension;
		m_cols = squareDimension;
		m_data.allocate(m_rows);
	}

	SparseMatrix(UINT rows, UINT cols)
	{
		m_rows = rows;
		m_cols = cols;
		m_data.resize(m_rows);
	}

	SparseMatrix(const std::string &s, MatrixStringFormat format)
	{
		if(format == MatrixStringFormatMathematica)
		{
			//
			// this is really a dense format and should be loaded as such, then cast into a SparseMatrix
			//
			std::vector<std::string> data = StringUtil::split(s, "},{");
			m_rows = (UINT)data.size();
			//m_cols = (UINT)data[0].split(",").size();
			m_cols = (UINT)StringUtil::split(data[0], ",").size();
			m_data.resize(m_rows);

			for(UINT row = 0; row < m_rows; row++)
			{
				std::vector<std::string> values = StringUtil::split(data[row], ",");
				for(UINT col = 0; col < values.size(); col++)
				{
					//(*this)(row, col) = (D)values[col].findAndReplace("{","").findAndReplace("}","").toDOUBLE();
					const std::string s = StringUtil::replace(StringUtil::replace(values[col], "{",""), "}","");
					(*this)(row, col) = (D)std::stod(s);
				}
			}
		}
		else
		{
			MLIB_ERROR("invalid matrix string format");
		}
	}

	void operator=(const SparseMatrix<D>& s)
	{
		m_rows = s.m_rows;
		m_cols = s.m_cols;
		m_data = s.m_data;
	}

	void operator=(SparseMatrix<D>&& s)
	{
		m_rows = s.m_rows;
		m_cols = s.m_cols;
		s.m_rows = 0;
		s.m_cols = 0;
		m_data = std::move(s.m_data);
	}

	//
	// Accessors
	//
	D& operator()(UINT row, UINT col)
	{
		return m_data[row](col);
	}
	D operator()(UINT row, UINT col) const
	{
		return m_data[row](col);
	}
	UINT rows() const
	{
		return m_rows;
	}
	UINT cols() const
	{
		return m_cols;
	}
	const SparseRow<D>& sparseRow(UINT row) const
	{
		return m_data[row];
	}
	const std::vector<D> denseRow(UINT row) const
	{
		std::vector<D> result(m_cols);
		for(UINT col = 0; col < m_cols; col++)
			result[col] = (*this)(row, col);
		return result;
	}
	const std::vector<D> denseCol(UINT col) const
	{
		std::vector<D> result(m_rows);
		for(UINT row = 0; row < m_rows; row++)
			result[row] = (*this)(row, col);
		return result;
	}
	std::vector<D> diagonal() const
	{
		MLIB_ASSERT_STR(square(), "diagonal called on non-square matrix");
		std::vector<D> result(m_rows);
		for(UINT row = 0; row < m_rows; row++)
			result[row] = m_data[row](row);
		return result;
	}

	//
	// Math functions
	//
	SparseMatrix<D> transpose() const;
	D maxMagnitude() const;
	bool square() const
	{
		return (m_rows == m_cols);
	}
	void invertInPlace();

	//
	// overloaded operator helpers
	//
	static SparseMatrix<D> add(const SparseMatrix<D> &A, const SparseMatrix<D> &B);
	static SparseMatrix<D> subtract(const SparseMatrix<D> &A, const SparseMatrix<D> &B);
	static SparseMatrix<D> multiply(const SparseMatrix<D> &A, D c);
	static std::vector<D> multiply(const SparseMatrix<D> &A, const std::vector<D> &v);
	static SparseMatrix<D> multiply(const SparseMatrix<D> &A, const SparseMatrix<D> &B);
	
	// returns the scalar v^T A v
	static D quadratic(const SparseMatrix<D> &A, const std::vector<D> &v)
	{
		return std::vector<D>::dotProduct(v, multiply(A, v));
	}

private:
	UINT m_rows, m_cols;
    std::vector< SparseRow<D> > m_data;

	// set is a more efficient version of operator() that assumes the entry
	// does not exist.
	void insert(UINT row, UINT col, double val)
	{
		m_data[row].entries.pushBack(SparseRowEntry<D>(col, val));
	}
};

template<class D>
SparseMatrix<D> operator + (const SparseMatrix<D> &A, const SparseMatrix<D> &B)
{
	return SparseMatrix<D>::add(A, B);
}

template<class D>
SparseMatrix<D> operator - (const SparseMatrix<D> &A, const SparseMatrix<D> &B)
{
	return SparseMatrix<D>::subtract(A, B);
}

template<class D>
SparseMatrix<D> operator * (const SparseMatrix<D> &A, const SparseMatrix<D> &B)
{
	return SparseMatrix<D>::multiply(A, B);
}

template<class D>
std::vector<D> operator * (const SparseMatrix<D> &A, const std::vector<D> &B)
{
	return SparseMatrix<D>::multiply(A, B);
}

template<class D>
SparseMatrix<D> operator * (const SparseMatrix<D> &A, D val)
{
	return SparseMatrix<D>::multiply(A, val);
}

//typedef SparseMatrix<float> SparseMatrixf;
//typedef SparseMatrix<double> SparseMatrixd;

#include "sparseMatrix.cpp"

#endif // SPARSE_MATRIX_H