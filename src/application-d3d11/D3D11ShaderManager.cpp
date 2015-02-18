
namespace ml {

void D3D11ShaderManager::init(GraphicsDevice &g)
{
    m_graphics = &g.castD3D11();
}

void D3D11ShaderManager::registerShader(
  const std::string&filename, 
  const std::string& shaderName, 
  const std::string& entryPointVS, 
  const std::string& shaderModelVS, 
  const std::string& entryPointPS,
  const std::string& shaderModelPS)
{
    MLIB_ASSERT_STR(m_graphics != NULL, "shader manager not initialized");

    auto &shaders = m_shaders[shaderName];
    shaders.vs.load(*m_graphics, filename, entryPointVS, shaderModelVS);
    shaders.ps.load(*m_graphics, filename, entryPointPS, shaderModelPS);
}

}