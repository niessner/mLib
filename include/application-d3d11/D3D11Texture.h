
#ifndef APPLICATION_D3D11_D3D11TEXTURE_H_
#define APPLICATION_D3D11_D3D11TEXTURE_H_

namespace ml {

class D3D11Texture : public GraphicsAsset
{
public:
    D3D11Texture()
	{
        m_texture = NULL;
        m_view = NULL;
	}
    ~D3D11Texture()
	{
        SAFE_RELEASE(m_texture);
        SAFE_RELEASE(m_view);
	}
	void load(GraphicsDevice &g, const Bitmap &bmp);

	void release(GraphicsDevice &g);
	void reset(GraphicsDevice &g);

    void bind(GraphicsDevice &g) const;

private:
    Bitmap m_bmp;
    ID3D11Texture2D *m_texture;
    ID3D11ShaderResourceView *m_view;
};

}  // namespace ml

#endif  // APPLICATION_D3D11_D3D11TEXTURE_H_
