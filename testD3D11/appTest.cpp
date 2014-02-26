
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

	vec3f eye(1.0f, 2.0f, 3.0f);
	vec3f worldUp(0.0f, 0.0f, 1.0f);
	m_camera = Camera(eye, worldUp, (vec3f::origin - eye) ^ worldUp, Math::degreesToRadians(60.0f), 0.01f, 1000.0f);
}

void AppTest::render(ApplicationData &app)
{
	m_vertexShader.bind(app.graphics);
	m_pixelShader.bind(app.graphics);



	m_mesh.render(app.graphics);
}

void AppTest::keyPress(ApplicationData &app, UINT key)
{

}
