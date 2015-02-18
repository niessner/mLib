
namespace ml {

void D3D11Canvas2D::init(GraphicsDevice &g, UINT width, UINT height)
{
    m_graphics = &g.castD3D11();

    m_renderTarget.load(*m_graphics, width, height);
}

void D3D11Canvas2D::addElement(const std::string &elementId, const bbox2i &box, float depth, const Bitmap &bmp)
{
    Element &e = m_elements[elementId];

    e.id = elementId;
    e.box = box;
    e.depth = depth;
    e.tex.load(*m_graphics, bmp);
    e.mesh.load(*m_graphics, ml::shapes::plane(vec3f(box.getMin(), depth), vec3f(box.getMax(), depth), vec3f::eZ));
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
    m_renderTarget.bind();
    
    for (const auto &e : m_elements)
    {
        e.second.mesh.render();
    }
}

}  // namespace ml
