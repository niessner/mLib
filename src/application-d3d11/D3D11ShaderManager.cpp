
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
  const std::string& shaderModelPS,
  const std::vector<std::pair<std::string, std::string>>& shaderMacros
  )
{
    MLIB_ASSERT_STR(m_graphics != NULL, "shader manager not initialized");

	// in case the shader exists return
	if (m_shaders.count(shaderName) == 0) {
		auto &shaders = m_shaders[shaderName];
		shaders.vs.load(*m_graphics, filename, entryPointVS, shaderModelVS, shaderMacros);
		shaders.ps.load(*m_graphics, filename, entryPointPS, shaderModelPS, shaderMacros);
	}
}

}