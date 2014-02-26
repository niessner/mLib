
class AppTest : public ApplicationCallback
{
public:
	void init(ApplicationData &app);
	void render(ApplicationData &app);
	void keyPress(ApplicationData &app, UINT key);

private:
	D3D11TriMesh m_mesh;
	D3D11VertexShader m_vertexShader;
	D3D11PixelShader m_pixelShader;
	Camera m_camera;
};