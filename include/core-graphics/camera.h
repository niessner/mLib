
class Camera
{
public:

	Camera() {}
	Camera(const vec3f &eye, const vec3f &worldUp, const vec3f &right, float fieldOfView, float zNear, float zFar);

	void lookRight(float theta);
	void lookUp(float theta);
	void roll(float theta);

	void strafe(float delta);
	void jump(float delta);
	void move(float delta);

	Matrix4 camera()
	{
		return m_camera;
	}

	Matrix4 perspective()
	{
		return m_perspective;
	}

	Matrix4 cameraPerspective()
	{
		return m_cameraPerspective;
	}

private:
	vec3f m_eye, m_right, m_look;
	vec3f m_worldUp;
	Matrix4f m_camera;
	Matrix4f m_perspective;
	Matrix4f m_cameraPerspective;
};
