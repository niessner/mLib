
Camera::Camera(const vec3f &eye, const vec3f &worldUp, const vec3f &right, float fieldOfView, float aspect, float zNear, float zFar)
{
	m_eye = eye;
	m_worldUp = worldUp.normalize();
	m_right = right.normalize();
	m_look = (m_worldUp ^ m_right).normalize();
	m_up = (m_right ^ m_look).normalize();

	m_perspective = perspectiveFov(fieldOfView, aspect, zNear, zFar);

	update();
}

void Camera::update()
{
	m_camera = viewMatrix(m_eye, m_look, m_up, m_right);
	m_cameraPerspective = m_perspective * m_camera;
	//m_cameraPerspective = m_perspective;
}

void Camera::lookRight(float theta)
{
	applyTransform(mat4f::rotation(m_worldUp, theta));
}

void Camera::lookUp(float theta)
{
	applyTransform(mat4f::rotation(m_right, theta));
}

void Camera::roll(float theta)
{
	applyTransform(mat4f::rotation(m_look, theta));
}

void Camera::applyTransform(const mat4f &transform)
{
	m_up = transform * m_up;
	m_right = transform * m_right;
	m_look = transform * m_look;
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

// field of view is in degrees
mat4f Camera::perspectiveFov(float fieldOfView, float aspectRatio, float zNear, float zFar)
{
	float width = 1.0f / tanf(Math::degreesToRadians(fieldOfView) * 0.5f);
	float height = aspectRatio / tanf(Math::degreesToRadians(fieldOfView) * 0.5f);

	return mat4f(width, 0.0f, 0.0f, 0.0f,
				 0.0f, height, 0.0f, 0.0f,
				 0.0f, 0.0f, zFar / (zNear - zFar), zFar * zNear / (zNear - zFar),
				 0.0f, 0.0f, -1.0f, 0.0f);
}

mat4f Camera::viewMatrix(const vec3f &eye, const vec3f &look, const vec3f &up, const vec3f &right)
{
	vec3f l = look.normalize();
	vec3f r = right.normalize();
	vec3f u = up.normalize();

	return mat4f(r.x, r.y, r.z, -vec3f::dot(r, eye),
				 u.x, u.y, u.z, -vec3f::dot(u, eye),
				 l.x, l.y, l.z, -vec3f::dot(l, eye),
				 0.0f, 0.0f, 0.0f, 1.0f);
}