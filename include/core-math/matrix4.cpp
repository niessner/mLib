  
template<class D>
Matrix4<D> Matrix4<D>::inverse() const
{
    //
    // Inversion by Cramer's rule.  Code taken from an Intel publication
    //
    double result[4][4];
    double tmp[12]; /* temp array for pairs */
    double src[16]; /* array of transpose source matrix */
    double det; /* determinant */
    /* transpose matrix */
    for (UINT i = 0; i < 4; i++)
    {
        src[i + 0 ] = (*this)[i][0];
        src[i + 4 ] = (*this)[i][1];
        src[i + 8 ] = (*this)[i][2];
        src[i + 12] = (*this)[i][3];
    }
    /* calculate pairs for first 8 elements (cofactors) */
    tmp[0] = src[10] * src[15];
    tmp[1] = src[11] * src[14];
    tmp[2] = src[9] * src[15];
    tmp[3] = src[11] * src[13];
    tmp[4] = src[9] * src[14];
    tmp[5] = src[10] * src[13];
    tmp[6] = src[8] * src[15];
    tmp[7] = src[11] * src[12];
    tmp[8] = src[8] * src[14];
    tmp[9] = src[10] * src[12];
    tmp[10] = src[8] * src[13];
    tmp[11] = src[9] * src[12];
    /* calculate first 8 elements (cofactors) */
    result[0][0] = tmp[0]*src[5] + tmp[3]*src[6] + tmp[4]*src[7];
    result[0][0] -= tmp[1]*src[5] + tmp[2]*src[6] + tmp[5]*src[7];
    result[0][1] = tmp[1]*src[4] + tmp[6]*src[6] + tmp[9]*src[7];
    result[0][1] -= tmp[0]*src[4] + tmp[7]*src[6] + tmp[8]*src[7];
    result[0][2] = tmp[2]*src[4] + tmp[7]*src[5] + tmp[10]*src[7];
    result[0][2] -= tmp[3]*src[4] + tmp[6]*src[5] + tmp[11]*src[7];
    result[0][3] = tmp[5]*src[4] + tmp[8]*src[5] + tmp[11]*src[6];
    result[0][3] -= tmp[4]*src[4] + tmp[9]*src[5] + tmp[10]*src[6];
    result[1][0] = tmp[1]*src[1] + tmp[2]*src[2] + tmp[5]*src[3];
    result[1][0] -= tmp[0]*src[1] + tmp[3]*src[2] + tmp[4]*src[3];
    result[1][1] = tmp[0]*src[0] + tmp[7]*src[2] + tmp[8]*src[3];
    result[1][1] -= tmp[1]*src[0] + tmp[6]*src[2] + tmp[9]*src[3];
    result[1][2] = tmp[3]*src[0] + tmp[6]*src[1] + tmp[11]*src[3];
    result[1][2] -= tmp[2]*src[0] + tmp[7]*src[1] + tmp[10]*src[3];
    result[1][3] = tmp[4]*src[0] + tmp[9]*src[1] + tmp[10]*src[2];
    result[1][3] -= tmp[5]*src[0] + tmp[8]*src[1] + tmp[11]*src[2];
    /* calculate pairs for second 8 elements (cofactors) */
    tmp[0] = src[2]*src[7];
    tmp[1] = src[3]*src[6];
    tmp[2] = src[1]*src[7];
    tmp[3] = src[3]*src[5];
    tmp[4] = src[1]*src[6];
    tmp[5] = src[2]*src[5];

    tmp[6] = src[0]*src[7];
    tmp[7] = src[3]*src[4];
    tmp[8] = src[0]*src[6];
    tmp[9] = src[2]*src[4];
    tmp[10] = src[0]*src[5];
    tmp[11] = src[1]*src[4];
    /* calculate second 8 elements (cofactors) */
    result[2][0] = tmp[0]*src[13] + tmp[3]*src[14] + tmp[4]*src[15];
    result[2][0] -= tmp[1]*src[13] + tmp[2]*src[14] + tmp[5]*src[15];
    result[2][1] = tmp[1]*src[12] + tmp[6]*src[14] + tmp[9]*src[15];
    result[2][1] -= tmp[0]*src[12] + tmp[7]*src[14] + tmp[8]*src[15];
    result[2][2] = tmp[2]*src[12] + tmp[7]*src[13] + tmp[10]*src[15];
    result[2][2] -= tmp[3]*src[12] + tmp[6]*src[13] + tmp[11]*src[15];
    result[2][3] = tmp[5]*src[12] + tmp[8]*src[13] + tmp[11]*src[14];
    result[2][3] -= tmp[4]*src[12] + tmp[9]*src[13] + tmp[10]*src[14];
    result[3][0] = tmp[2]*src[10] + tmp[5]*src[11] + tmp[1]*src[9];
    result[3][0] -= tmp[4]*src[11] + tmp[0]*src[9] + tmp[3]*src[10];
    result[3][1] = tmp[8]*src[11] + tmp[0]*src[8] + tmp[7]*src[10];
    result[3][1] -= tmp[6]*src[10] + tmp[9]*src[11] + tmp[1]*src[8];
    result[3][2] = tmp[6]*src[9] + tmp[11]*src[11] + tmp[3]*src[8];
    result[3][2] -= tmp[10]*src[11] + tmp[2]*src[8] + tmp[7]*src[9];
    result[3][3] = tmp[10]*src[10] + tmp[4]*src[8] + tmp[9]*src[9];
    result[3][3] -= tmp[8]*src[9] + tmp[11]*src[10] + tmp[5]*src[8];
    /* calculate determinant */
    det=src[0]*result[0][0]+src[1]*result[0][1]+src[2]*result[0][2]+src[3]*result[0][3];
    /* calculate matrix inverse */
    det = 1.0 / det;

    Matrix4<D> inv;
    for (UINT i = 0; i < 4; i++)
        for (UINT j = 0; j < 4; j++)
            inv[i][j] = D(result[i][j] * det);
    return inv;
}

template<class D>
Matrix4<D> Matrix4<D>::transpose() const
{
    Matrix4<D> result;
    for(UINT i = 0; i < 4; i++)
    {
        for(UINT i2 = 0; i2 < 4; i2++)
        {
            result[i2][i] = m[i][i2];
        }
    }
    return result;
}

template<class D>
Matrix4<D> Matrix4<D>::identity()
{
    Matrix4<D> result;
    for(UINT i = 0; i < 4; i++)
        for(UINT i2 = 0; i2 < 4; i2++)
        {
            if(i == i2)
				result[i][i2] = 1.0f;
            else
                result[i][i2] = 0.0f;
        }
    return result;
}

template<class D>
Matrix4<D> Matrix4<D>::camera(const point3d<D> &eye, const point3d<D> &look, const point3d<D> &up, const point3d<D> &right)
{
    point3d<D> lookNormalized = look.normalize();
    point3d<D> upNormalized = up.normalize();
    point3d<D> rightNormalized = right.normalize();

    Matrix4<D> result;
    result[0][0] = rightNormalized.x;
    result[1][0] = rightNormalized.y;
    result[2][0] = rightNormalized.z;
    result[3][0] = -point3d<D>::dot(rightNormalized, eye);

    result[0][1] = upNormalized.x;
    result[1][1] = upNormalized.y;
    result[2][1] = upNormalized.z;
    result[3][1] = -point3d<D>::dot(upNormalized, eye);

    result[0][2] = lookNormalized.x;
    result[1][2] = lookNormalized.y;
    result[2][2] = lookNormalized.z;
    result[3][2] = -point3d<D>::dot(lookNormalized, eye);

    result[0][3] = 0.0f;
    result[1][3] = 0.0f;
    result[2][3] = 0.0f;
    result[3][3] = 1.0f;
    return result;
}

template<class D>
Matrix4<D> Matrix4<D>::perspectiveFov(D fov, D aspect, D zNear, D zFar)
{
    D width = 1.0f / tanf(fov/2.0f);
	D height = aspect / tanf(fov/2.0f);

    Matrix4<D> result;
    result[0][0] = width;
    result[1][0] = 0.0f;
    result[2][0] = 0.0f;
    result[3][0] = 0.0f;

    result[0][1] = 0.0f;
    result[1][1] = height;
    result[2][1] = 0.0f;
    result[3][1] = 0.0f;

    result[0][2] = 0.0f;
    result[1][2] = 0.0f;
    result[2][2] = zFar / (zNear - zFar);
    result[3][2] = zFar * zNear / (zNear - zFar);

    result[0][3] = 0.0f;
    result[1][3] = 0.0f;
    result[2][3] = -1.0f;
    result[3][3] = 0.0f;
    return result;
}

template<class D>
Matrix4<D> Matrix4<D>::rotation(const point3d<D> &axis, D angle)
{
    D c = cosf(angle);
    D s = sinf(angle);
    D t = 1.0f - c;

    point3d<D> normalizedAxis = axis.normalize();
    D x = normalizedAxis.x;
    D y = normalizedAxis.y;
    D z = normalizedAxis.z;

    Matrix4<D> result;
    result[0][0] = 1 + t*(x*x-1);
    result[0][1] = z*s+t*x*y;
    result[0][2] = -y*s+t*x*z;
    result[0][3] = 0.0f;

    result[1][0] = -z*s+t*x*y;
    result[1][1] = 1+t*(y*y-1);
    result[1][2] = x*s+t*y*z;
    result[1][3] = 0.0f;

    result[2][0] = y*s+t*x*z;
    result[2][1] = -x*s+t*y*z;
    result[2][2] = 1+t*(z*z-1);
    result[2][3] = 0.0f;

    result[3][0] = 0.0f;
    result[3][1] = 0.0f;
    result[3][2] = 0.0f;
    result[3][3] = 1.0f;
    return result;
}

template<class D>
Matrix4<D> Matrix4<D>::rotation(D yaw, D pitch, D roll)
{
    return rotationY(yaw) * rotationX(pitch) * rotationZ(roll);
}

template<class D>
Matrix4<D> Matrix4<D>::rotation(const point3d<D> &axis, D angle, const point3d<D> &center)
{
    return translation(-center) * rotation(axis, angle) * translation(center);
}

template<class D>
Matrix4<D> Matrix4<D>::rotationX(D theta)
{
    D cosT = cosf(theta);
    D sinT = sinf(theta);

    Matrix4<D> result = identity();
    result[1][1] = cosT;
    result[1][2] = sinT;
    result[2][1] = -sinT;
    result[2][2] = cosT;
    return result;
}

template<class D>
Matrix4<D> Matrix4<D>::rotationY(D theta)
{
    D cosT = cosf(theta);
    D sinT = sinf(theta);

    Matrix4<D> result = identity();
    result[0][0] = cosT;
    result[0][2] = sinT;
    result[2][0] = -sinT;
    result[2][2] = cosT;
    return result;
}

template<class D>
Matrix4<D> Matrix4<D>::rotationZ(D theta)
{
    D cosT = cosf(theta);
    D sinT = sinf(theta);

    Matrix4<D> result = identity();
    result[0][0] = cosT;
    result[0][1] = sinT;
    result[1][0] = -sinT;
    result[1][1] = cosT;
    return result;
}

template<class D>
Matrix4<D> Matrix4<D>::scaling(const point3d<D> &ScaleFactors)
{
    Matrix4<D> result;
    result[0][0] = ScaleFactors.x;
    result[1][0] = 0.0f;
    result[2][0] = 0.0f;
    result[3][0] = 0.0f;

    result[0][1] = 0.0f;
    result[1][1] = ScaleFactors.y;
    result[2][1] = 0.0f;
    result[3][1] = 0.0f;

    result[0][2] = 0.0f;
    result[1][2] = 0.0f;
    result[2][2] = ScaleFactors.z;
    result[3][2] = 0.0f;

    result[0][3] = 0.0f;
    result[1][3] = 0.0f;
    result[2][3] = 0.0f;
    result[3][3] = 1.0f;
    return result;
}

template<class D>
Matrix4<D> Matrix4<D>::translation(const point3d<D> &v)
{
    Matrix4<D> result;
    result[0][0] = 1.0f;
    result[1][0] = 0.0f;
    result[2][0] = 0.0f;
    result[3][0] = v.x;

    result[0][1] = 0.0f;
    result[1][1] = 1.0f;
    result[2][1] = 0.0f;
    result[3][1] = v.y;

    result[0][2] = 0.0f;
    result[1][2] = 0.0f;
    result[2][2] = 1.0f;
    result[3][2] = v.z;

    result[0][3] = 0.0f;
    result[1][3] = 0.0f;
    result[2][3] = 0.0f;
    result[3][3] = 1.0f;
    return result;
}

template<class D>
Matrix4<D> operator * (const Matrix4<D> &l, const Matrix4<D> &r)
{
    Matrix4<D> result;
    for(UINT i = 0; i < 4; i++)
    {
        for(UINT i2 = 0; i2 < 4; i2++)
        {
            D Total = 0.0f;
            for(UINT i3 = 0; i3 < 4; i3++)
            {
                Total += l.m[i][i3] * r.m[i3][i2];
            }
            result.m[i][i2] = Total;
        }
    }
    return result;
}

template<class D>
Matrix4<D> operator * (const Matrix4<D> &l, D &r)
{
    Matrix4<D> result;
    for(UINT i = 0; i < 4; i++)
    {
        for(UINT i2 = 0; i2 < 4; i2++)
        {
            result[i][i2] = l[i][i2] * r;
        }
    }
    return result;
}

template<class D>
Matrix4<D> operator * (D &l, const Matrix4<D> &r)
{
    Matrix4<D> result;
    for(UINT i = 0; i < 4; i++)
    {
        for(UINT i2 = 0; i2 < 4; i2++)
        {
            result[i][i2] = r[i][i2] * l;
        }
    }
    return result;
}

template<class D>
Matrix4<D> operator + (const Matrix4<D> &l, const Matrix4<D> &r)
{
    Matrix4<D> result;
    for(UINT i = 0; i < 4; i++)
    {
        for(UINT i2 = 0; i2 < 4; i2++)
        {
            result[i][i2] = l[i][i2] + r[i][i2];
        }
    }
    return result;
}

template<class D>
Matrix4<D> operator - (const Matrix4<D> &l, const Matrix4<D> &r)
{
    Matrix4<D> result;
    for(UINT i = 0; i < 4; i++)
    {
        for(UINT i2 = 0; i2 < 4; i2++)
        {
            result[i][i2] = l[i][i2] - r[i][i2];
        }
    }
    return result;
}
