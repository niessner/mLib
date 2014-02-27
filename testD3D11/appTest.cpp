
#include "main.h"

void AppTest::init(ApplicationData &app)
{
  const std::string testPLY = "scans/gates381.ply";
  const TriMesh triMesh = OpenMeshLoader::load(testPLY);
	m_mesh.load(app.graphics, triMesh);
	m_vertexShader.load(app.graphics, "shaders/test.shader");
	m_pixelShader.load(app.graphics, "shaders/test.shader");
	m_constants.init(app.graphics);

	//vec3f eye(1.0f, 2.0f, 3.0f);
	vec3f eye(0.0f, 0.0f, 0.0f);
	vec3f worldUp(0.0f, 0.0f, 1.0f);
	m_camera = Camera(eye, worldUp, worldUp ^ (vec3f::eX - eye), 60.0f, (float)app.window.width() / app.window.height(), 0.01f, 1000.0f);
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

void AppTest::resize(ApplicationData &app)
{
	m_camera.updateAspectRatio((float)app.window.width() / app.window.height());
}

void AppTest::keyDown(ApplicationData &app, UINT key)
{

}

void AppTest::keyPressed(ApplicationData &app, UINT key)
{
	const float distance = 0.025f;
	const float theta = 2.0f;

	if(key == KEY_W) m_camera.move(-distance);
	if(key == KEY_S) m_camera.move(distance);
	if(key == KEY_A) m_camera.strafe(-distance);
	if(key == KEY_D) m_camera.strafe(distance);

	if(key == KEY_UP) m_camera.lookUp(theta);
	if(key == KEY_DOWN) m_camera.lookUp(-theta);
	if(key == KEY_LEFT) m_camera.lookRight(theta);
	if(key == KEY_RIGHT) m_camera.lookRight(-theta);
}

void AppTest::mouseDown(ApplicationData &app, MouseButtonType button)
{

}

void AppTest::mouseMove(ApplicationData &app)
{
	const float distance = 0.025f;
	const float theta = 0.5f;

	vec2i mouseDelta = app.input.mouse.pos - app.input.prevMouse.pos;

	if(app.input.mouse.buttons[MouseButtonLeft])
	{
		m_camera.strafe(distance * mouseDelta.x);
		m_camera.jump(distance * -mouseDelta.y);
	}

	if(app.input.mouse.buttons[MouseButtonRight])
	{
		m_camera.lookRight(theta * -mouseDelta.x);
		m_camera.lookUp(theta * -mouseDelta.y);
	}
}
