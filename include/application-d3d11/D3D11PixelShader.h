
class D3D11PixelShader : public GraphicsAsset
{
public:
	D3D11PixelShader()
	{
		m_shader = NULL;
		m_blob = NULL;
	}
	~D3D11PixelShader()
	{
		SAFE_RELEASE(m_shader);
		SAFE_RELEASE(m_blob);
	}
	void load(GraphicsDevice &g, const std::string &filename);

	void release(GraphicsDevice &g);
	void reset(GraphicsDevice &g);

	void bind(GraphicsDevice &g);

	UINT64 hash64();

private:
	ID3D11PixelShader *m_shader;
	ID3DBlob *m_blob;
	std::string m_filename;

	UINT64 m_hash64;
};