#pragma once
#ifndef INCLUDE_CORE_GRAPHICS_CAMERA_H_
#define INCLUDE_CORE_GRAPHICS_CAMERA_H_

class Camera {
public:
	Camera() {}
	Camera(const Vec3f& eye, const Vec3f& worldUp, const Vec3f& right, float fieldOfView, float aspect, float zNear, float zFar);

	//! Construct camera from extrinsics matrix m (columns are x, y, z vectors and origin of camera in that order)
	Camera(const Mat4f& m, const float fieldOfView, const float aspect, const float zNear, const float zFar);

	void updateAspectRatio(float newAspect);
	void lookRight(float theta);
	void lookUp(float theta);
	void roll(float theta);

	void strafe(float delta);
	void jump(float delta);
	void move(float delta);

	Mat4f camera() {
		return m_camera;
	}

	Mat4f perspective() {
		return m_perspective;
	}

	Mat4f cameraPerspective() {
		return m_cameraPerspective;
	}

private:
	void applyTransform(const Mat4f& transform);
	void update();
	Mat4f perspectiveFov(float fieldOfView, float aspectRatio, float zNear, float zFar);
	Mat4f viewMatrix(const Vec3f& eye, const Vec3f& look, const Vec3f& up, const Vec3f& right);

	Vec3f m_eye, m_right, m_look, m_up;
	Vec3f m_worldUp;
	Mat4f m_camera;
	Mat4f m_perspective;
	Mat4f m_cameraPerspective;

	float m_fieldOfView, m_aspect, m_zNear, m_zFar;
};

#endif  // INCLUDE_CORE_GRAPHICS_CAMERA_H_
