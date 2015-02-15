
#ifndef APPLICATION_D3D11_D3D11SHADERMANAGER_H_
#define APPLICATION_D3D11_D3D11SHADERMANAGER_H_

namespace ml {

struct D3D11ShaderPair
{
    D3D11PixelShader ps;
    D3D11VertexShader vs;
};

class D3D11ShaderManager
{
public:
    D3D11ShaderManager()
    {
        m_graphics = NULL;
    }

    void init(GraphicsDevice &g);

    void registerShader(const std::string &filename, const std::string &shaderName);

    const D3D11ShaderPair& getShaders(const std::string &shaderName) const
    {
        MLIB_ASSERT_STR(m_shaders.count(shaderName) > 0, "shader not found in shader manager");
        return m_shaders.find(shaderName)->second;
    }

    void bindShaders(const std::string &shaderName) const
    {
        auto &shaders = getShaders(shaderName);
        shaders.ps.bind(*m_graphics);
        shaders.vs.bind(*m_graphics);
    }

private:
    D3D11GraphicsDevice *m_graphics;
    std::map<std::string, D3D11ShaderPair> m_shaders;
};

}  // namespace ml

#endif  // APPLICATION_D3D11_D3D11VERTEXSHADER_H_
