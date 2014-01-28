
class D3D11VertexShader : public GraphicsAsset
{
public:
	void load(D3D11GraphicsDevice &g, const String &filename);

	void release(GraphicsDevice &g);
	void reset(GraphicsDevice &g);

private:
	ID3D11VertexShader *m_vertexShader;
	String m_filename;
};