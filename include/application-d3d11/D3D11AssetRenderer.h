
#ifndef APPLICATION_D3D11_D3D11ASSETRENDERER_H_
#define APPLICATION_D3D11_D3D11ASSETRENDERER_H_

#include <core-math/matrix4x4.h>
#include <core-math/vec4.h>
#include <core-graphics/polygon.h>
#include <application-base/graphicsDevice.h>
#include <application-d3d11/D3D11TriMesh.h>
#include <application-d3d11/D3D11ConstantBuffer.h>
#include <application-d3d11/D3D11ShaderManager.h>

namespace ml {

struct AssetRendererConstantBuffer
{
    mat4f worldViewProj;
    vec4f modelColor;
};

class D3D11AssetRenderer
{
public:
    void init(GraphicsDevice& g, bool useTexture = false);
    
    void renderMesh(const D3D11TriMesh &mesh, const mat4f &cameraPerspective, const vec3f &color = vec3f(1.0f, 1.0f, 1.0f));
    void renderMesh(const D3D11TriMesh &mesh, const mat4f &cameraPerspective, const mat4f &meshToWorld, const vec3f &color = vec3f(1.0f, 1.0f, 1.0f));

    void renderCylinder(const mat4f &cameraPerspective, const vec3f &p0, const vec3f &p1, float radius, const vec3f &color);
    void renderSphere(const mat4f &cameraPerspective, const vec3f &center, float radius, const vec3f &color);
    void renderBox(const mat4f &cameraPerspective, const vec3f &center, float radius, const vec3f &color);
    
    void renderPolygon(GraphicsDevice &g, const mat4f &cameraPerspective, const Polygonf &poly, const vec3f &color, float height);

    const D3D11TriMesh& getCylinder() const
    {
        return m_cylinder;
    }

    const D3D11TriMesh& getSphere() const
    {
        return m_sphere;
    }

    const D3D11TriMesh& getBox() const
    {
        return m_box;
    }

private:
    D3D11GraphicsDevice *m_graphics;
    const D3D11ShaderPair *m_shader;
    D3D11ConstantBuffer<AssetRendererConstantBuffer> m_constants;

    D3D11TriMesh m_cylinder;
    D3D11TriMesh m_sphere;
    D3D11TriMesh m_box;
};

}  // namespace ml

#endif  // APPLICATION_D3D11_D3D11ASSETRENDERER_H_
