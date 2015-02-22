
namespace ml {

void D3D11Canvas2D::init(GraphicsDevice &g)
{
    m_graphics = &g.castD3D11();
}



bool D3D11Canvas2D::intersects(const vec2i &mouseCoord, const vec2i &windowDimensions, const Cameraf &camera, const UIEvent &event)
{
    return false;
}

void D3D11Canvas2D::release(GraphicsDevice &g)
{

}

void D3D11Canvas2D::reset(GraphicsDevice &g)
{

}

void D3D11Canvas2D::render()
{    
    for (auto *e : m_elements)
    {
		e->render();
    }
}

}  // namespace ml
