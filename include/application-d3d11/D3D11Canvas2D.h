
#ifndef APPLICATION_D3D11_D3D11CANVAS2D_H_
#define APPLICATION_D3D11_D3D11CANVAS2D_H_

namespace ml {

class D3D11Canvas2D : public GraphicsAsset
{
public:
    struct Intersection
    {
        Intersection()
        {
            coord.x = coord.y = std::numeric_limits<int>::min();
        }
        bool valid() const
        {
            return (coord.x != std::numeric_limits<int>::min());
        }
        Intersection(const std::string &id, const vec2i &_coord) : elementId(id), coord(_coord) {}
        std::string elementId;
        vec2i coord;
    };

    enum ElementType
    {
        ELEMENT_TYPE_BILLBOARD = 0,
        ELEMENT_TYPE_CIRCLE = 1
    };

    class ElementBillboard;
    class Element
    {
    public:
        Element(GraphicsDevice& g, const std::string &id, ElementType elementType, float depth) : m_graphics(&g.castD3D11()), m_id(id), m_depth(depth) {
            m_elementType = elementType;
        }

        virtual ~Element() {}

        //! render the element: takes care for shader binding, etc.,
        virtual void render() = 0;
        virtual void onDeviceResize() {};
        virtual bool intersects(const vec2i &mouseCoord, Intersection &intersection) const
        {
            return false;
        }

        ElementType getType() const {
            return m_elementType;
        }

        const ElementBillboard& castBillboard() const {
            MLIB_ASSERT(m_elementType == ELEMENT_TYPE_BILLBOARD);
            return *((ElementBillboard*)this);
        }

    protected:
        D3D11GraphicsDevice* m_graphics;
        float m_depth; // value should be between 0 and 1
        std::string m_id;

    private:
        ElementType m_elementType;
    };

    class ElementBillboard : public Element
    {
    public:
        ElementBillboard(GraphicsDevice& g, const std::string &id, const bbox2i& box, const ColorImageR8G8B8A8 &image, float depth) : Element(g, id, ELEMENT_TYPE_BILLBOARD, depth)
        {
            const std::string mLibShaderDir = util::getMLibDir() + "data/shaders/";
            m_graphics->getShaderManager().registerShader(mLibShaderDir + "defaultCanvas.hlsl", "defaultCanvasMesh", "meshVS", "vs_4_0", "meshPS", "ps_4_0");

            m_id = id;
            m_box = box;
            m_tex.load(g, image);
            onDeviceResize();
        }

        void onDeviceResize();
        void render();
        bool intersects(const vec2i &mouseCoord, Intersection &intersection) const;

        const bbox2i& getBox() const
        {
            return m_box;
        }

    private:
        bbox2i m_box;
        D3D11Texture2D m_tex;
        D3D11TriMesh m_mesh;
    };

    class ElementCircle : public Element
    {
    public:
        struct ElementCircleConstants {
            vec4f color;
            vec2f center;
            float radius;
            float dummy;
        };
        ElementCircle(GraphicsDevice &g, const std::string &id, const vec2f& center, float radius, const vec4f& color, float depth) : Element(g, id, ELEMENT_TYPE_CIRCLE, depth) {

            const std::string mLibShaderDir = util::getMLibDir() + "data/shaders/";
            m_graphics->getShaderManager().registerShader(mLibShaderDir + "defaultCanvas.hlsl", "defaultCanvasCircle", "circleVS", "vs_4_0", "circlePS", "ps_4_0");

            m_constants.center = center;
            m_constants.radius = radius;
            m_constants.color = color;
            m_constantBuffer.init(g);
            m_constantBuffer.update(m_constants);

            onDeviceResize();
        }

        void onDeviceResize() {
            bbox2f box;
            box.include(m_graphics->pixelToNDC(math::floor(m_constants.center - m_constants.radius)));
            box.include(m_graphics->pixelToNDC(math::ceil(m_constants.center + m_constants.radius)));
            m_mesh.load(*m_graphics, ml::shapes::plane(vec3f(box.getMin(), m_depth), vec3f(box.getMax(), m_depth), vec3f::eZ));
        }

        void render() {
            m_graphics->getShaderManager().bindShaders("defaultCanvasCircle");
            m_constantBuffer.bind(0);
            m_mesh.render();
        }
    private:
        ElementCircleConstants m_constants;
        D3D11ConstantBuffer<ElementCircleConstants> m_constantBuffer;
        D3D11TriMesh m_mesh;
    };

    D3D11Canvas2D()
    {
        m_graphics = nullptr;
    }
    ~D3D11Canvas2D()
    {
        clearElements();
    }

    void init(GraphicsDevice &g);

    void addCircle(const std::string &elementId, const vec2f& centerInPixels, float radiusInPixels, const vec4f& color, float depth) {
        m_namedElements[elementId] = new ElementCircle(*m_graphics, elementId, centerInPixels, radiusInPixels, color, depth);
    }

    void addBillboard(const std::string &elementId, const bbox2i& box, const ColorImageR8G8B8A8 &image, float depth) {
        m_namedElements[elementId] = new ElementBillboard(*m_graphics, elementId, box, image, depth);
    }

    void addCircle(const vec2f& centerInPixels, float radiusInPixels, const vec4f& color, float depth) {
        m_unnamedElements.push_back(new ElementCircle(*m_graphics, "", centerInPixels, radiusInPixels, color, depth));
    }

    void addBillboard(const bbox2i& box, const ColorImageR8G8B8A8 &image, float depth) {
        m_unnamedElements.push_back(new ElementBillboard(*m_graphics, "", box, image, depth));
    }

    Intersection intersectionFirst(const vec2i &mouseCoord) const;
    vector<Intersection> intersectionAll(const vec2i &mouseCoord) const;

    void release();
    void reset();
    void onDeviceResize();

    void render();

    void clearElements()
    {
        for (Element *e : m_unnamedElements)
            SAFE_DELETE(e);
        for (auto &e : m_namedElements)
            SAFE_DELETE(e.second);
        m_namedElements.clear();
        m_unnamedElements.clear();
    }

    const Element& getElementById(const std::string &elementId) const
    {
        auto it = m_namedElements.find(elementId);
        MLIB_ASSERT_STR(it != m_namedElements.end(), "Element not found");
        return *(it->second);
    }

private:
    D3D11GraphicsDevice *m_graphics;
    std::map<std::string, Element*> m_namedElements;
    std::vector<Element*> m_unnamedElements;
};

}  // namespace ml

#endif  // APPLICATION_D3D11_D3D11CANVAS2D_H_
