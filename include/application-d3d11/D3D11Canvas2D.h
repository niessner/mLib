
#ifndef APPLICATION_D3D11_D3D11CANVAS2D_H_
#define APPLICATION_D3D11_D3D11CANVAS2D_H_

namespace ml {

class D3D11Canvas2D : public GraphicsAsset
{
public:
    struct Element
    {
        std::string id;
        bbox2i box;
        float depth; // value should be between 0 and 1
        D3D11Texture2D tex;
        D3D11TriMesh mesh;
    };
    struct UIEvent
    {
        std::string elementId;
        vec2i coord;
    };

    D3D11Canvas2D()
	{
        m_graphics = nullptr;
	}
    ~D3D11Canvas2D()
	{
        
	}
    void init(GraphicsDevice &g, UINT width, UINT height);

    void addElement(const std::string &elementId, const bbox2i &box, float depth, const Bitmap &bmp);

    bool intersects(const vec2i &mouseCoord, const vec2i &windowDimensions, const Cameraf &camera, const UIEvent &event);

	void release(GraphicsDevice &g);
	void reset(GraphicsDevice &g);

    void render();

    GraphicsAssetType type() const
    {
        return GraphicsAssetTexture;
    }

private:
    D3D11GraphicsDevice *m_graphics;
    
    D3D11RenderTarget m_renderTarget;
    std::map<std::string, Element> m_elements;
};

}  // namespace ml

#endif  // APPLICATION_D3D11_D3D11CANVAS2D_H_
