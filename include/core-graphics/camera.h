
class Camera
{
public:

	Camera() {}
	Camera(const vec3f &eye, const vec3f &worldUp, const vec3f &right, float fieldOfView, float aspect, float zNear, float zFar);

	void updateAspectRatio(float newAspect);
	void lookRight(float theta);
	void lookUp(float theta);
	void roll(float theta);

	void strafe(float delta);
	void jump(float delta);
	void move(float delta);

	mat4f camera()
	{
		return m_camera;
	}

	mat4f perspective()
	{
		return m_perspective;
	}

	mat4f cameraPerspective()
	{
		return m_cameraPerspective;
	}

private:
	void applyTransform(const mat4f &transform);
	void update();
	mat4f perspectiveFov(float fieldOfView, float aspectRatio, float zNear, float zFar);
	mat4f viewMatrix(const vec3f &eye, const vec3f &look, const vec3f &up, const vec3f &right);

	vec3f m_eye, m_right, m_look, m_up;
	vec3f m_worldUp;
	mat4f m_camera;
	mat4f m_perspective;
	mat4f m_cameraPerspective;

	float m_fieldOfView, m_aspect, m_zNear, m_zFar;
};
