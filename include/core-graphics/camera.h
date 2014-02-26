
class Camera
{
public:

	Camera() {}
	Camera(const vec3f &eye, const vec3f &worldUp, const vec3f &right, float fieldOfView, float aspect, float zNear, float zFar);

	void lookRight(float theta);
	void lookUp(float theta);
	void roll(float theta);

	void strafe(float delta);
	void jump(float delta);
	void move(float delta);

	Matrix4f camera()
	{
		return m_camera;
	}

	Matrix4f perspective()
	{
		return m_perspective;
	}

	Matrix4f cameraPerspective()
	{
		return m_cameraPerspective;
	}

private:
	void applyTransform(const Matrix4f &transform);
	void update();

	vec3f m_eye, m_right, m_look, m_up;
	vec3f m_worldUp;
	Matrix4f m_camera;
	Matrix4f m_perspective;
	Matrix4f m_cameraPerspective;
};
