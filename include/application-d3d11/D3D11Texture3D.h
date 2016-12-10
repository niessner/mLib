
#ifndef APPLICATION_D3D11_D3D11TEXTURE3D_H_
#define APPLICATION_D3D11_D3D11TEXTURE3D_H_

namespace ml {

class D3D11Texture3D : public GraphicsAsset
{
public:
    D3D11Texture3D()
	{
        m_graphics = nullptr;
        m_texture = nullptr;
        m_srv = nullptr;
	}
    //
    // TODO: implement other copy constructors similar to D3D11Mesh
    //
    D3D11Texture3D(D3D11Texture3D &&t)
    {
        m_data = std::move(t.m_data);
        m_graphics = t.m_graphics;
        m_srv = t.m_srv; t.m_srv = nullptr;
        m_texture = t.m_texture; t.m_texture = nullptr;
    }
    void operator = (D3D11Texture3D &&t)
    {
        m_graphics = t.m_graphics;
        m_data = std::move(t.m_data);
        m_srv = t.m_srv; t.m_srv = nullptr;
        m_texture = t.m_texture; t.m_texture = nullptr;
    }
    ~D3D11Texture3D()
	{
		releaseGPU();
	}
    D3D11Texture3D(GraphicsDevice &g, const Grid3<RGBColor> &data)
    {
        m_texture = nullptr;
        m_srv = nullptr;
        load(g, data);
    }
    void load(GraphicsDevice &g, const Grid3<RGBColor> &data);

	void releaseGPU();
	void createGPU();

    void bind(unsigned int slot = 0) const;


    const Grid3<RGBColor>& data() const
    {
        return m_data;
    }

private:
	D3D11GraphicsDevice *m_graphics;
    Grid3<RGBColor> m_data;
    ID3D11Texture3D *m_texture;
    ID3D11ShaderResourceView *m_srv;
};

}  // namespace ml

#endif  // APPLICATION_D3D11_D3D11TEXTURE3D_H_
