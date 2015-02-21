
#ifndef APPLICATION_D3D11_D3D11CANVAS2D_H_
#define APPLICATION_D3D11_D3D11CANVAS2D_H_

namespace ml {

class D3D11Canvas2D : public GraphicsAsset
{
public:
	enum ElementType {
		ELEMENT_TYPE_MESH = 0,
		ELEMENT_TYPE_CIRCLE = 1
	};
	class Element
	{
	public:
		Element(GraphicsDevice& g, ElementType elementType, float depth) : m_graphics(&g.castD3D11()), m_depth(depth) {
			m_elementType = elementType;
		}

		//! render the element: takes care for shader binding, etc.,
		virtual void render() = 0;
		
		ElementType getType() const {
			return m_elementType;
		}
		
	protected:
		D3D11GraphicsDevice* m_graphics;
		float m_depth; // value should be between 0 and 1

	private:
		ElementType m_elementType;
	};

    class ElementMesh : public Element
    { 
	public:
		ElementMesh(GraphicsDevice& g, const bbox2i& box, const Bitmap &bmp, float depth) : Element(g, ELEMENT_TYPE_MESH, depth) {
			m_box = box;
			m_tex.load(g, bmp);
			m_mesh.load(g, ml::shapes::plane(vec3f(box.getMin(), depth), vec3f(box.getMax(), depth), vec3f::eZ));
		 }

		void render() {
			m_tex.bind();
			m_mesh.render();
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
			vec2f center;
			float radius;			
		};
		ElementCircle(GraphicsDevice &g, const vec2f& center, float radius, float depth) : Element(g, ELEMENT_TYPE_CIRCLE, depth) {
			ElementCircleConstants constants;
			constants.center = center;
			constants.radius = radius;
			m_constantBuffer.init(g);
			m_constantBuffer.update(constants);

			bbox2i box;
			box.include(math::floor(center - radius));
			box.include(math::ceil(center + radius));
			m_mesh.load(g, ml::shapes::plane(vec3f(box.getMin(), depth), vec3f(box.getMax(), depth), vec3f::eZ));
		}

		void render() {
			m_constantBuffer.bind(0);
			m_mesh.render();
		}
	private:
		D3D11ConstantBuffer<ElementCircleConstants> m_constantBuffer;		
		D3D11TriMesh m_mesh;
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
        clearElements();
	}

    void init(GraphicsDevice &g);

	void addCircle(const vec2f& center, float radius, float depth) {
		m_elements.push_back(new ElementCircle(*m_graphics, center, radius, depth));
	}

	void addElement(const bbox2i& box, const Bitmap &bmp, float depth) {
		m_elements.push_back(new ElementMesh(*m_graphics, box, bmp, depth));
	}

    bool intersects(const vec2i &mouseCoord, const vec2i &windowDimensions, const Cameraf &camera, const UIEvent &event);

	void release(GraphicsDevice &g);
	void reset(GraphicsDevice &g);

    void render();


	void clearElements() {
		for (size_t i = 0; i < m_elements.size(); i++) {
			SAFE_DELETE(m_elements[i]);
		}
		m_elements.clear();
	}
private:

    D3D11GraphicsDevice *m_graphics;
    std::vector<Element*> m_elements;
};

}  // namespace ml

#endif  // APPLICATION_D3D11_D3D11CANVAS2D_H_
