
namespace ml {

void D3D11Canvas2D::init(GraphicsDevice &g)
{
    m_graphics = &g.castD3D11();
	m_graphics->registerAsset(this);	//register to get resize, reset, and release events
}



bool D3D11Canvas2D::intersects(const vec2i &mouseCoord, const vec2i &windowDimensions, const Cameraf &camera, const UIEvent &event)
{
    return false;
}

void D3D11Canvas2D::release()
{
	clearElements();
}

void D3D11Canvas2D::reset()
{

}

void D3D11Canvas2D::onDeviceResize()
{
	for (auto *e : m_elements)
	{
        e->onDeviceResize();
	}
}

void D3D11Canvas2D::render()
{    
    for (auto *e : m_elements)
    {
		e->render();
    }
}

}  // namespace ml
