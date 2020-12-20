
#ifndef APPLICATION_D3D11_D3D11UTILITY_H_
#define APPLICATION_D3D11_D3D11UTILITY_H_

#include <string>
#include <vector>
#include <utility>
#include <d3d11.h>


namespace ml {

class D3D11Utility
{
public:
	static ID3DBlob* CompileShader(
    const std::string &filename, 
    const std::string &entryPoint, 
    const std::string &shaderModel, 
    const std::vector<std::pair<std::string, std::string>>& macros = std::vector<std::pair<std::string, std::string>>());
};

}  // namespace ml

#endif  // APPLICATION_D3D11_D3D11UTILITY_H_
