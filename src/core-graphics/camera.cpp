
Camera::Camera(const vec3f &eye, const vec3f &worldUp, const vec3f &right, float fieldOfView, float aspect, float zNear, float zFar)
{
	m_eye = eye;
	m_worldUp = worldUp.normalize();
	m_right = right.normalize();
	m_look = (m_worldUp ^ m_right).normalize();
	m_up = (m_right ^ m_look).normalize();

	m_perspective = Matrix4f::perspectiveFov(fieldOfView, aspect, zNear, zFar);

	update();
}

void Camera::update()
{
	m_camera = Matrix4f::camera(m_eye, m_look, m_up, m_right);
	//m_cameraPerspective = m_camera * m_perspective;
}

void Camera::lookRight(float theta)
{
	applyTransform(Matrix4f::rotation(m_worldUp, theta));
}

void Camera::lookUp(float theta)
{
	applyTransform(Matrix4f::rotation(m_right, theta));
}

void Camera::roll(float theta)
{
	applyTransform(Matrix4f::rotation(m_look, theta));
}

void Camera::applyTransform(const Matrix4f &transform)
{
	m_up = transform.transformPoint(m_up);
	m_right = transform.transformPoint(m_right);
	m_look = transform.transformPoint(m_look);
	update();
}

void Camera::strafe(float delta)
{
	m_eye += m_right * delta;
	update();
}

void Camera::jump(float delta)
{
	m_eye += m_up * delta;
	update();
}

void Camera::move(float delta)
{
	m_eye += m_look * delta;
	update();
}
