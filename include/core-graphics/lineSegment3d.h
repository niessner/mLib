
#ifndef CORE_GRAPHICS_LINESEGMENT_H_
#define CORE_GRAPHICS_LINESEGMENT_H_

namespace ml {

template<class FloatType>
class LineSegment
{
public:

    LineSegment(const point3d<FloatType> &p0, const point3d<FloatType> &p1)
    {
        m_p0 = p0;
        m_p1 = p1;
        m_delta = m_p1 - m_p0;
	}

	const point3d<FloatType>& p0() const
    {
        return m_p0;
	}

	const point3d<FloatType>& p1() const
    {
        return m_p1;
	}

	const point3d<FloatType>& delta() const
    {
        return m_delta;
	}

private:
	point3d<FloatType> m_p0;
	point3d<FloatType> m_p1;
	point3d<FloatType> m_delta;  //p1 - p0
};

typedef LineSegment<float> LineSegmentf;
typedef LineSegment<double> LineSegmentd;

}  // namespace ml

#endif  // CORE_GRAPHICS_RAY_H_
