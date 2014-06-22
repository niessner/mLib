
#ifndef CORE_MATH_EIGENSOLVER_INL_H_
#define CORE_MATH_EIGENSOLVER_INL_H_

namespace ml {

//
// Jacobi iteration for the solution of eigenvectors/eigenvalues of a nxn
// real symmetric matrix. Square nxn matrix a; size of matrix in n;
// output eigenvalues in w; and output eigenvectors in eigenvectors. Resulting
// eigenvalues/vectors are sorted in decreasing order; eigenvectors are
// normalized.
//
// Code modified from VTK vtkJacobiN function
//
template<class T>
void EigenSolverVTK<T>::eigenSystem(const DenseMatrix<T> &M, T **eigenvectors, T *eigenvalues) const
{
    const unsigned int rows = M.rows();
    MLIB_ASSERT_STR(M.square() && M.rows() >= 2, "invalid matrix dimensions in EigenSolverVTK<T>::eigenSystem");
    int i, j, k, iq, ip, numPos, n = int(rows);
    T tresh, theta, tau, t, sm, s, h, g, c, tmp;
    T bspace[4], zspace[4];
    T *b = bspace;
    T *z = zspace;

    //
    // Jacobi iteration destroys the matrix so create a temporary copy
    //
    DenseMatrix<T> a = M;
    
    //
    // only allocate memory if the matrix is large
    //
    if (n > 4)
    {
        b = new T[n];
        z = new T[n];
    }

    //
    // initialize
    //
    for (ip = 0; ip<n; ip++)
    {
        for (iq = 0; iq<n; iq++)
        {
            eigenvectors[ip][iq] = 0.0;
        }
        eigenvectors[ip][ip] = 1.0;
    }
    for (ip = 0; ip<n; ip++)
    {
        b[ip] = a[ip][ip];
        eigenvalues[ip] = T(a[ip][ip]);
        z[ip] = 0.0;
    }

    // begin rotation sequence
    for (i = 0; i<VTK_MAX_ROTATIONS; i++)
    {
        sm = 0.0;
        for (ip = 0; ip<n - 1; ip++)
        {
            for (iq = ip + 1; iq<n; iq++)
            {
                sm += fabs(a[ip][iq]);
            }
        }
        if (sm == 0.0)
        {
            break;
        }

        if (i < 3)                                // first 3 sweeps
        {
            tresh = 0.2*sm / (n*n);
        }
        else
        {
            tresh = 0.0;
        }

        for (ip = 0; ip<n - 1; ip++)
        {
            for (iq = ip + 1; iq<n; iq++)
            {
                g = T(100.0*fabs(a[ip][iq]));

                // after 4 sweeps
                if (i > 3 && (fabs(eigenvalues[ip]) + g) == fabs(eigenvalues[ip])
                    && (fabs(eigenvalues[iq]) + g) == fabs(eigenvalues[iq]))
                {
                    a[ip][iq] = 0.0;
                }
                else if (fabs(a[ip][iq]) > tresh)
                {
                    h = eigenvalues[iq] - eigenvalues[ip];
                    if ((fabs(h) + g) == fabs(h))
                    {
                        t = (a[ip][iq]) / h;
                    }
                    else
                    {
                        theta = 0.5*h / (a[ip][iq]);
                        t = 1.0 / (fabs(theta) + sqrt(1.0 + theta*theta));
                        if (theta < 0.0)
                        {
                            t = -t;
                        }
                    }
                    c = 1.0 / sqrt(1 + t*t);
                    s = t*c;
                    tau = s / (1.0 + c);
                    h = t*a[ip][iq];
                    z[ip] -= h;
                    z[iq] += h;
                    eigenvalues[ip] -= T(h);
                    eigenvalues[iq] += T(h);
                    a[ip][iq] = 0.0;

                    // ip already shifted left by 1 unit
                    for (j = 0; j <= ip - 1; j++)
                    {
                        VTK_ROTATE(a, j, ip, j, iq);
                    }
                    // ip and iq already shifted left by 1 unit
                    for (j = ip + 1; j <= iq - 1; j++)
                    {
                        VTK_ROTATE(a, ip, j, j, iq);
                    }
                    // iq already shifted left by 1 unit
                    for (j = iq + 1; j<n; j++)
                    {
                        VTK_ROTATE(a, ip, j, iq, j);
                    }
                    for (j = 0; j<n; j++)
                    {
#pragma warning ( disable : 4244 )
                        VTK_ROTATE(eigenvectors, j, ip, j, iq);
#pragma warning ( default : 4244 )
                    }
                }
            }
        }

        for (ip = 0; ip<n; ip++)
        {
            b[ip] += z[ip];
            eigenvalues[ip] = T(b[ip]);
            z[ip] = 0.0;
        }
    }

    if (i >= VTK_MAX_ROTATIONS)
    {
        //return false;
    }

    // sort eigenfunctions                 these changes do not affect accuracy 
    for (j = 0; j<n - 1; j++)                  // boundary incorrect
    {
        k = j;
        tmp = eigenvalues[k];
        for (i = j + 1; i<n; i++)                // boundary incorrect, shifted already
        {
            if (eigenvalues[i] >= tmp)                   // why exchage if same?
            {
                k = i;
                tmp = eigenvalues[k];
            }
        }
        if (k != j)
        {
            eigenvalues[k] = eigenvalues[j];
            eigenvalues[j] = T(tmp);
            for (i = 0; i<n; i++)
            {
                tmp = eigenvectors[i][j];
                eigenvectors[i][j] = eigenvectors[i][k];
                eigenvectors[i][k] = T(tmp);
            }
        }
    }

    //
    // insure eigenvector consistency (i.e., Jacobi can compute vectors that
    // are negative of one another (.707,.707,0) and (-.707,-.707,0). This can
    // reek havoc in hyperstreamline/other stuff. We will select the most
    // positive eigenvector.
    //
    int ceil_half_n = (n >> 1) + (n & 1);
    for (j = 0; j<n; j++)
    {
        for (numPos = 0, i = 0; i<n; i++)
        {
            if (eigenvectors[i][j] >= 0.0)
            {
                numPos++;
            }
        }
        //    if ( numPos < ceil(double(n)/double(2.0)) )
        if (numPos < ceil_half_n)
        {
            for (i = 0; i<n; i++)
            {
                eigenvectors[i][j] *= -1.0;
            }
        }
    }

    if (n > 4)
    {
        delete[] b;
        delete[] z;
    }
    return true;
}

}  // namespace ml

#endif  // CORE_MATH_EIGENSOLVER_INL_H_