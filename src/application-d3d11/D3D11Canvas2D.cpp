
namespace ml {

void D3D11Canvas2D::init(GraphicsDevice &g)
{
    m_graphics = &g.castD3D11();


	const std::string mLibShaderDir = util::getMLibDir() + "data/shaders/";

	m_graphics->getShaderManager().registerShader(mLibShaderDir + "defaultCanvas.hlsl", "defaultCanvasMesh", "meshVS", "vs_4_0", "meshPS", "ps_4_0");
	m_graphics->getShaderManager().registerShader(mLibShaderDir + "defaultCanvas.hlsl", "defaultCanvasCircle", "circleVS", "vs_4_0", "circlePS", "ps_4_0");
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
		if (e->getType() == ELEMENT_TYPE_MESH) {
			m_graphics->getShaderManager().bindShaders("defaultCanvasMesh");
		}
		else if (e->getType() == ELEMENT_TYPE_CIRCLE) {
			m_graphics->getShaderManager().bindShaders("defaultCanvasCircle");
		}
		else
		{
			throw MLIB_EXCEPTION("unknown shader");
		}
		e->render();
    }
}

}  // namespace ml
