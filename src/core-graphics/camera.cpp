
namespace ml {

Camera::Camera(const Vec3f& eye, const Vec3f& worldUp, const Vec3f& right, float fieldOfView, float aspect, float zNear, float zFar) {
	m_eye = eye;
	m_worldUp = worldUp.normalize();
	m_right = right.normalize();
	m_look = (m_worldUp ^ m_right).normalize();
	m_up = (m_right ^ m_look).normalize();

	m_fieldOfView = fieldOfView;
	m_aspect = aspect;
	m_zNear = zNear;
	m_zFar = zFar;

	m_perspective = perspectiveFov(m_fieldOfView, m_aspect, m_zNear, m_zFar);

	update();
}

Camera::Camera(const Mat4f& m, float fieldOfView, float aspect, float zNear, float zFar) {
	m_eye = Vec3f(m(0, 3), m(1, 3), m(2, 3));
	m_worldUp = Vec3f(m(0, 1), m(1, 1), m(2, 1));
	m_right = Vec3f(m(0, 0), m(1, 0), m(2, 0));
	m_look = (m_worldUp ^ m_right).normalize();
	m_up = (m_right ^ m_look).normalize();

	m_fieldOfView = fieldOfView;
	m_aspect = aspect;
	m_zNear = zNear;
	m_zFar = zFar;

	m_perspective = perspectiveFov(m_fieldOfView, m_aspect, m_zNear, m_zFar);

	update();
}

void Camera::updateAspectRatio(float newAspect) {
	m_aspect = newAspect;
	m_perspective = perspectiveFov(m_fieldOfView, m_aspect, m_zNear, m_zFar);
	update();
}

void Camera::update() {
	m_camera = viewMatrix(m_eye, m_look, m_up, m_right);
	m_cameraPerspective = m_perspective * m_camera;
	//m_cameraPerspective = m_perspective;
}

void Camera::lookRight(float theta) {
	applyTransform(Mat4f::rotation(m_worldUp, theta));
}

void Camera::lookUp(float theta) {
	applyTransform(Mat4f::rotation(m_right, theta));
}

void Camera::roll(float theta) {
	applyTransform(Mat4f::rotation(m_look, theta));
}

void Camera::applyTransform(const Mat4f& transform) {
	m_up = transform * m_up;
	m_right = transform * m_right;
	m_look = transform * m_look;
	update();
}

void Camera::strafe(float delta) {
	m_eye += m_right * delta;
	update();
}

void Camera::jump(float delta) {
	m_eye += m_up * delta;
	update();
}

void Camera::move(float delta) {
	m_eye += m_look * delta;
	update();
}

// field of view is in degrees
Mat4f Camera::perspectiveFov(float fieldOfView, float aspectRatio, float zNear, float zFar) {
	float width = 1.0f / tanf(math::degreesToRadians(fieldOfView) * 0.5f);
	float height = aspectRatio / tanf(math::degreesToRadians(fieldOfView) * 0.5f);

	return Mat4f(width, 0.0f, 0.0f, 0.0f,
	             0.0f, height, 0.0f, 0.0f,
	             0.0f, 0.0f, zFar / (zNear - zFar), zFar * zNear / (zNear - zFar),
	             0.0f, 0.0f, -1.0f, 0.0f);
}

Mat4f Camera::viewMatrix(const Vec3f& eye, const Vec3f& look, const Vec3f& up, const Vec3f& right) {
	Vec3f l = look.normalize();
	Vec3f r = right.normalize();
	Vec3f u = up.normalize();

	return Mat4f(r.x, r.y, r.z, -Vec3f::dot(r, eye),
	             u.x, u.y, u.z, -Vec3f::dot(u, eye),
	             l.x, l.y, l.z, -Vec3f::dot(l, eye),
	             0.0f, 0.0f, 0.0f, 1.0f);
}

}  // namespace ml