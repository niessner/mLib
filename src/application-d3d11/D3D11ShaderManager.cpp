
namespace ml {

void D3D11ShaderManager::init(GraphicsDevice &g)
{
    m_graphics = &g.castD3D11();
}

void D3D11ShaderManager::registerShader(const std::string &filename, const std::string &shaderName)
{
    MLIB_ASSERT_STR(m_graphics != NULL, "shader manager not initialized");

    auto &shaders = m_shaders[shaderName];
    shaders.vs.load(*m_graphics, filename);
    shaders.ps.load(*m_graphics, filename);
}

}