
struct ConstantBuffer
{
	mat4f worldViewProj;
};

class AppTest : public ApplicationCallback
{
public:
	void init(ApplicationData &app);
	void render(ApplicationData &app);
	void keyDown(ApplicationData &app, UINT key);
	void keyPressed(ApplicationData &app, UINT key);
	void mouse(ApplicationData &app, int x, int y, int prevX, int prevY);

private:
	D3D11TriMesh m_mesh;
	D3D11VertexShader m_vertexShader;
	D3D11PixelShader m_pixelShader;
	D3D11ConstantBuffer<ConstantBuffer> m_constants;
	Camera m_camera;
};