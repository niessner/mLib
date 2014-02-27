
#include "main.h"

void AppTest::init(ApplicationData &app)
{
    const std::string testPLY = "scans/gates381.ply";
    const TriMesh triMesh = OpenMeshLoader::load(testPLY);
    m_mesh.load(app.graphics, triMesh);

    Vector<vec3f> points(5, [](UINT64 i) {return vec3f((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);});
    m_pointCloud.load(app.graphics, MeshUtil::createPointCloudTemplate(OpenMeshLoader::load(testPLY), points));

    m_vsColor.load(app.graphics, "shaders/test.shader");
    m_psColor.load(app.graphics, "shaders/test.shader");

    m_vsPointCloud.load(app.graphics, "shaders/pointCloud.shader");
    m_psPointCloud.load(app.graphics, "shaders/pointCloud.shader");

    m_constants.init(app.graphics);

    //vec3f eye(1.0f, 2.0f, 3.0f);
    vec3f eye(-0.5f, -0.5f, 1.5f);
    vec3f worldUp(0.0f, 0.0f, 1.0f);
    m_camera = Camera(eye, worldUp, vec3f::eX, 60.0f, (float)app.window.width() / app.window.height(), 0.01f, 1000.0f);
}

void AppTest::render(ApplicationData &app)
{
    ConstantBuffer constants;
    constants.worldViewProj = m_camera.cameraPerspective();
    m_constants.update(app.graphics, constants);

    m_vsColor.bind(app.graphics);
    m_psColor.bind(app.graphics);
    m_constants.bindVertexShader(app.graphics, 0);

    m_mesh.render(app.graphics);

    m_vsPointCloud.bind(app.graphics);
    m_psPointCloud.bind(app.graphics);
    m_constants.bindVertexShader(app.graphics, 0);

    m_pointCloud.render(app.graphics);
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
    const float distance = 0.3f;
    const float theta = 5.0f;

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

void AppTest::mouseWheel(ApplicationData &app, int wheelDelta)
{
    const float distance = 0.002f;
    m_camera.move(distance * -wheelDelta);
}

void AppTest::mouseMove(ApplicationData &app)
{
    const float distance = 0.01f;
    const float theta = 0.5f;

    vec2i posDelta = app.input.mouse.pos - app.input.prevMouse.pos;

    if(app.input.mouse.buttons[MouseButtonLeft])
    {
        m_camera.strafe(distance * posDelta.x);
        m_camera.jump(distance * -posDelta.y);
    }

    if(app.input.mouse.buttons[MouseButtonRight])
    {
        m_camera.lookRight(theta * -posDelta.x);
        m_camera.lookUp(theta * -posDelta.y);
    }


}
