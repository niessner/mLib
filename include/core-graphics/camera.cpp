
namespace ml {

template <class FloatType>
Camera<FloatType>::Camera(const point3d<FloatType>& eye, const point3d<FloatType>& worldUp, const point3d<FloatType>& right, FloatType fieldOfView, FloatType aspect, FloatType zNear, FloatType zFar) {
	m_eye = eye;
	m_worldUp = worldUp.getNormalized();
	m_right = right.getNormalized();
	m_look = (m_worldUp ^ m_right).getNormalized();
	m_up = (m_right ^ m_look).getNormalized();

	m_fieldOfView = fieldOfView;
	m_aspect = aspect;
	m_zNear = zNear;
	m_zFar = zFar;

	m_perspective = perspectiveFov(m_fieldOfView, m_aspect, m_zNear, m_zFar);

	update();
}

template <class FloatType>
Camera<FloatType>::Camera(const Matrix4x4<FloatType>& m, FloatType fieldOfView, FloatType aspect, FloatType zNear, FloatType zFar) {
	m_eye = point3d<FloatType>(m(0, 3), m(1, 3), m(2, 3));
	m_worldUp = point3d<FloatType>(m(0, 1), m(1, 1), m(2, 1));
	m_right = -point3d<FloatType>(m(0, 0), m(1, 0), m(2, 0));  // NOTE: Negation to compensate for sensor horizontal flip
	m_look = (m_worldUp ^ m_right).getNormalized();
	m_up = (m_right ^ m_look).getNormalized();

	m_fieldOfView = fieldOfView;
	m_aspect = aspect;
	m_zNear = zNear;
	m_zFar = zFar;

	m_perspective = perspectiveFov(m_fieldOfView, m_aspect, m_zNear, m_zFar);

	update();
}

template <class FloatType>
void Camera<FloatType>::updateAspectRatio(FloatType newAspect) {
	m_aspect = newAspect;
	m_perspective = perspectiveFov(m_fieldOfView, m_aspect, m_zNear, m_zFar);
	update();
}

template <class FloatType>
void Camera<FloatType>::update() {
	m_camera = viewMatrix(m_eye, m_look, m_up, m_right);
	m_cameraPerspective = m_perspective * m_camera;
	//m_cameraPerspective = m_perspective;
}

template <class FloatType>
void Camera<FloatType>::lookRight(FloatType theta) {
	applyTransform(Matrix4x4<FloatType>::rotation(m_worldUp, theta));
}

template <class FloatType>
void Camera<FloatType>::lookUp(FloatType theta) {
	applyTransform(Matrix4x4<FloatType>::rotation(m_right, -theta));
}

template <class FloatType>
void Camera<FloatType>::roll(FloatType theta) {
	applyTransform(Matrix4x4<FloatType>::rotation(m_look, theta));
}

template <class FloatType>
void Camera<FloatType>::applyTransform(const Matrix4x4<FloatType>& transform) {
	m_up = transform * m_up;
	m_right = transform * m_right;
	m_look = transform * m_look;
	update();
}

template <class FloatType>
void Camera<FloatType>::strafe(FloatType delta) {
	m_eye += m_right * delta;
	update();
}

template <class FloatType>
void Camera<FloatType>::jump(FloatType delta) {
	m_eye += m_up * delta;
	update();
}

template <class FloatType>
void Camera<FloatType>::move(FloatType delta) {
	m_eye += m_look * delta;
	update();
}

// field of view is in degrees
template <class FloatType>
Matrix4x4<FloatType> Camera<FloatType>::perspectiveFov(FloatType fieldOfView, FloatType aspectRatio, FloatType zNear, FloatType zFar) {
	FloatType width = 1.0f / tanf(math::degreesToRadians(fieldOfView) * 0.5f);
	FloatType height = aspectRatio / tanf(math::degreesToRadians(fieldOfView) * 0.5f);

	return Matrix4x4<FloatType>(width, 0.0f, 0.0f, 0.0f,
	             0.0f, height, 0.0f, 0.0f,
	             0.0f, 0.0f, zFar / (zNear - zFar), zFar * zNear / (zNear - zFar),
	             0.0f, 0.0f, -1.0f, 0.0f);
}

template <class FloatType>
Matrix4x4<FloatType> Camera<FloatType>::viewMatrix(const point3d<FloatType>& eye, const point3d<FloatType>& look, const point3d<FloatType>& up, const point3d<FloatType>& right) {
	point3d<FloatType> l = look.getNormalized();
	point3d<FloatType> r = right.getNormalized();
	point3d<FloatType> u = up.getNormalized();

	return Matrix4x4<FloatType>(r.x, r.y, r.z, -point3d<FloatType>::dot(r, eye),
	             u.x, u.y, u.z, -point3d<FloatType>::dot(u, eye),
	             //l.x, l.y, l.z, -point3d<FloatType>::dot(l, eye),
				 -l.x, -l.y, -l.z, point3d<FloatType>::dot(l, eye),  // Negation of look to create right-handed view matrix
	             0.0f, 0.0f, 0.0f, 1.0f);
}

}  // namespace ml