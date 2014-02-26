
class D3D11Utility
{
public:
	static ID3DBlob* CompileShader(const std::string &filename, const std::string &entryPoint, const std::string &shaderModel);
};