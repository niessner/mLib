
#include "main.h"

void AppTest::init(ApplicationData &app)
{
	Vector<MeshVertex> vertices;
	vertices.pushBack(MeshVertex(vec3f(0.0f, 0.5f, 0.5f)));
	vertices.pushBack(MeshVertex(vec3f(0.5f, -0.5f, 0.5f)));
	vertices.pushBack(MeshVertex(vec3f(-0.5f, -0.5f, 0.5f)));

	Vector<UINT> indices;
	indices.pushBack(0);
	indices.pushBack(1);
	indices.pushBack(2);

	m_mesh.load(app.graphics, vertices, indices);
	m_vertexShader.load(app.graphics, "shaders/test.shader");
	m_pixelShader.load(app.graphics, "shaders/test.shader");
	m_constants.init(app.graphics);

	//vec3f eye(1.0f, 2.0f, 3.0f);
	vec3f eye(0.0f, 0.0f, 0.0f);
	vec3f worldUp(0.0f, 1.0f, 0.0f);
	m_camera = Camera(eye, worldUp, worldUp ^ (vec3f::eZ - eye), 60.0f, 1.0f, 0.01f, 1000.0f);
}

void AppTest::render(ApplicationData &app)
{
	m_vertexShader.bind(app.graphics);
	m_pixelShader.bind(app.graphics);

	ConstantBuffer constants;
	constants.worldViewProj = m_camera.cameraPerspective();
	m_constants.update(app.graphics, constants);
	m_constants.bindVertexShader(app.graphics, 0);

	m_mesh.render(app.graphics);
}

void AppTest::keyDown(ApplicationData &app, UINT key)
{
	const float delta = 0.025f;
	const float theta = 0.02f;

	if(key == KEY_W) m_camera.move(-delta);
	if(key == KEY_S) m_camera.move(delta);
	if(key == KEY_A) m_camera.strafe(-delta);
	if(key == KEY_D) m_camera.strafe(delta);

	if(key == KEY_NUMPAD2) m_camera.lookUp(theta);
	if(key == KEY_NUMPAD8) m_camera.lookUp(-theta);
	if(key == KEY_NUMPAD4) m_camera.lookRight(-theta);
	if(key == KEY_NUMPAD6) m_camera.lookRight(theta);
}

void AppTest::keyPressed(ApplicationData &app, UINT key)
{
	const float delta = 0.025f;
	const float theta = 0.02f;

	if(key == KEY_W) m_camera.move(-delta);
	if(key == KEY_S) m_camera.move(delta);
	if(key == KEY_A) m_camera.strafe(-delta);
	if(key == KEY_D) m_camera.strafe(delta);

	if(key == KEY_NUMPAD2) m_camera.lookUp(theta);
	if(key == KEY_NUMPAD8) m_camera.lookUp(-theta);
	if(key == KEY_NUMPAD4) m_camera.lookRight(-theta);
	if(key == KEY_NUMPAD6) m_camera.lookRight(theta);
}

void AppTest::mouse(ApplicationData &app, int x, int y, int prevX, int prevY)
{

}