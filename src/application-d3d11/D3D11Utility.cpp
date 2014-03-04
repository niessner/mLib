
ID3DBlob* D3D11Utility::CompileShader(const std::string &filename, const std::string &entryPoint, const std::string &shaderModel)
{
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	shaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* blob = NULL;
	ID3DBlob* errorBlob = NULL;
    MLIB_ASSERT_STR(Utility::fileExists(filename), "File not found: " + filename);
	HRESULT hr = D3DX11CompileFromFile( std::wstring(filename.begin(), filename.end()).c_str(), NULL, NULL, entryPoint.c_str(), shaderModel.c_str(), 
		shaderFlags, 0, NULL, &blob, &errorBlob, NULL );
	if( FAILED(hr) )
	{
		if( errorBlob != NULL )
		{
			Console::log() << "Shader compilation failed for " << filename << std::endl
			               << errorBlob->GetBufferPointer() << std::endl;
		}
		MLIB_ERROR("Shader compilation failed for " + filename);
	}
	if( errorBlob ) errorBlob->Release();

	return blob;
}