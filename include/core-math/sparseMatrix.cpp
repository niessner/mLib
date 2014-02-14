/*
SparseMatrix.cpp
Written by Matthew Fisher
*/

#pragma once

template<class D>
D SparseMatrix<D>::maxMagnitude() const
{
	double result = 0.0;
	for(UINT row = 0; row < m_rows; row++)
		for(const SparseRowEntry<D> &e : m_data[row].entries)
			result = std::max(result, fabs(e.val));
	return result;
}

template<class D>
SparseMatrix<D> SparseMatrix<D>::transpose() const
{
    SparseMatrix<D> result(m_cols, m_rows);
    for(UINT row = 0; row < m_rows; row++)
        for(const SparseRowEntry<D> &e : m_data[row].entries)
            result.insert(e.col, row, e.val);
    return result;
}

template<class D>
SparseMatrix<D> SparseMatrix<D>::multiply(const SparseMatrix<D> &A, D val)
{
	SparseMatrix<D> result = A;
	for(UINT row = 0; row < A.m_rows; row++)
		for(SparseRowEntry<D> &e : result.m_data[row].entries)
			e.val *= val;
	return result;
}

template<class D>
SparseMatrix<D> SparseMatrix<D>::add(const SparseMatrix<D> &A, const SparseMatrix<D> &B)
{
	MLIB_ASSERT(A.rows() == B.rows() && A.cols() == B.cols(), "invalid matrix dimensions");
	SparseMatrix<D> result = A;
	for(UINT row = 0; row < B.m_rows; row++)
		for(const SparseRowEntry<D> &e : B.m_data[row].entries)
			result(row, e.col) += e.val;
	return result;
}

template<class D>
SparseMatrix<D> SparseMatrix<D>::subtract(const SparseMatrix<D> &A, const SparseMatrix<D> &B)
{
	MLIB_ASSERT(A.rows() == B.rows() && A.cols() == B.cols(), "invalid matrix dimensions");
	SparseMatrix<D> result = A;
	for(UINT row = 0; row < B.m_rows; row++)
		for(const SparseRowEntry<D> &e : B.m_data[row].entries)
			result(row, e.col) -= e.val;
	return result;
}

template<class D>
Vector<D> SparseMatrix<D>::multiply(const SparseMatrix<D> &A, const Vector<D> &B)
{
	MLIB_ASSERT(A.cols() == B.size(), "invalid dimensions");
	const UINT rows = A.m_rows;
	Vector<D> result(rows);
	for(UINT row = 0; row < rows; row++)
	{
		D val = 0.0;
		for(const SparseRowEntry<D> &e : A.m_data[row].entries)
			val += e.val * B[e.col];
		result[row] = val;
	}
	return result;
}

template<class D>
SparseMatrix<D> SparseMatrix<D>::multiply(const SparseMatrix<D> &A, const SparseMatrix<D> &B)
{
	MLIB_ASSERT(A.cols() == B.rows(), "invalid dimensions");

	const UINT rows = A.rows();
	SparseMatrix<D> result(rows, B.cols());
	
	for(UINT row = 0; row < rows; row++)
	{
		for(const SparseRowEntry<D> &eA : A.m_data[row].entries)
		{
			for(const SparseRowEntry<D> &eB : B.m_data[eA.col].entries)
			{
				result(row, eB.col) += eA.val * eB.val;
			}
		}
	}
	return result;
}
