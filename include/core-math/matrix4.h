
template<class D>
class Matrix4
{
public:
    
	Matrix4()
	{

	}

    Matrix4(const Matrix4<D> &x)
	{
		for(UINT row = 0; row < 4; row++)
			for(UINT col = 0; col < 4; col++)
				m[row][col] = x.m[row][col];
	}
    
	//
    // math functions
    //
    D determinant() const;
    Matrix4<D> transpose() const;
    Matrix4<D> inverse() const;
	
    //
    // vector transforms
    //
    point3d<D> transformPoint(const point3d<D> &point) const
    {
        D w = point.x * m[0][3] + point.y * m[1][3] + point.z * m[2][3] + m[3][3];
        if(w)
        {
            const D invW = 1.0f / w;
            return point3d<D>( (point.x * m[0][0] + point.y * m[1][0] + point.z * m[2][0] + m[3][0]) * invW,
                               (point.x * m[0][1] + point.y * m[1][1] + point.z * m[2][1] + m[3][1]) * invW,
                               (point.x * m[0][2] + point.y * m[1][2] + point.z * m[2][2] + m[3][2]) * invW);
        }
        else
        {
            return point3d<D>::origin;
        }
    }

	point3d<D> transformPointNoProjection(const point3d<D> &point) const
    {
        return point3d<D>( (point.x * m[0][0] + point.y * m[1][0] + point.z * m[2][0] + m[3][0]),
                           (point.x * m[0][1] + point.y * m[1][1] + point.z * m[2][1] + m[3][1]),
                           (point.x * m[0][2] + point.y * m[1][2] + point.z * m[2][2] + m[3][2]));
    }
    
    point3d<D> transformNormal(const point3d<D> &normal) const
    {
        return point3d<D>(normal.x * m[0][0] + normal.y * m[1][0] + normal.z * m[2][0],
                          normal.x * m[0][1] + normal.y * m[1][1] + normal.z * m[2][1],
                          normal.x * m[0][2] + normal.y * m[1][2] + normal.z * m[2][2]);
    }

    //
    // accessors
    //
    D* operator [] (UINT row)
    {
        return m[row];
    }
    const D* operator [] (UINT row) const
    {
        return m[row];
    }
    void setColumn(UINT col, const point4d<D> &v)
    {
        m[0][col] = v.x;
        m[1][col] = v.y;
        m[2][col] = v.z;
        m[3][col] = v.w;
    }
    void setRow(UINT row, const point4d<D> &v)
    {
        m[row][0] = v.x;
        m[row][1] = v.y;
        m[row][2] = v.z;
        m[row][3] = v.w;
    }
    point4d<D> getColumn(UINT col)
    {
        point4d<D> result;
        result.x = m[0][col];
        result.y = m[1][col];
        result.z = m[2][col];
        result.w = m[3][col];
        return result;
    }
    point4d<D> getRow(UINT row)
    {
        point4d<D> result;
        result.x = m[row][0];
        result.y = m[row][1];
        result.z = m[row][2];
        result.w = m[row][3];
        return result;
    }

    //
    // Transformation matrices
    //
    static Matrix4 identity();
    static Matrix4 scaling(const point3d<D> &scaleFactors);
    static Matrix4 scaling(D scale)
    {
        return scaling(point3d<D>(scale, scale, scale));
    }
    static Matrix4 translation(const point3d<D> &v);
    static Matrix4 rotation(const point3d<D> &axis, D angle, const point3d<D> &center);
    static Matrix4 rotation(const point3d<D> &axis, D angle);
    static Matrix4 rotation(D yaw, D pitch, D roll);
    static Matrix4 rotationX(D theta);
    static Matrix4 rotationY(D theta);
    static Matrix4 rotationZ(D theta);
    static Matrix4 camera(const point3d<D> &eye, const point3d<D> &look, const point3d<D> &up, const point3d<D> &right);
    static Matrix4 perspectiveFov(D fov, D aspect, D zNear, D zFar);
    
    friend Matrix4 operator * (const Matrix4 &l, const Matrix4 &r);
    friend Matrix4 operator * (const Matrix4 &l, D r);
    friend Matrix4 operator * (D l, const Matrix4 &r);
    friend Matrix4 operator + (const Matrix4 &l, const Matrix4 &r);
    friend Matrix4 operator - (const Matrix4 &l, const Matrix4 &r);

    D m[4][4];
};

template<class D>
point4d<D> operator * (const point4d<D> &r, const Matrix4<D> &l)
{
    return point4d<D>(r.x * l[0][0] + r.y * l[1][0] + r.z * l[2][0] + r.w * l[3][0],
					  r.x * l[0][1] + r.y * l[1][1] + r.z * l[2][1] + r.w * l[3][1],
					  r.x * l[0][2] + r.y * l[1][2] + r.z * l[2][2] + r.w * l[3][2],
				      r.x * l[0][3] + r.y * l[1][3] + r.z * l[2][3] + r.w * l[3][3]);
}

typedef Matrix4<float> Matrix4f;
typedef Matrix4<double> Matrix4d;

#include "matrix4.cpp"