
#ifndef APPLICATION_D3D11_D3D11CONSTANTBUFFER_H_
#define APPLICATION_D3D11_D3D11CONSTANTBUFFER_H_

namespace ml {

template<class T>
class D3D11ConstantBuffer : public GraphicsAsset
{
public:
	D3D11ConstantBuffer()
	{
        m_graphics = nullptr;
		m_buffer = nullptr;
	}

	~D3D11ConstantBuffer()
	{
		release();
	}

	void init(GraphicsDevice &g)
	{
        m_graphics = &g.castD3D11();
		reset();
	}

	void release()
	{
		SAFE_RELEASE(m_buffer);
	}

	void reset()
	{
		release();

		D3D11_BUFFER_DESC desc;
		ZeroMemory( &desc, sizeof(desc) );
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(T);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = 0;
		D3D_VALIDATE(m_graphics->getDevice().CreateBuffer( &desc, nullptr, &m_buffer ));
	}

    void updateAndBind(const T &data, UINT constantBufferIndex)
    {
        update(data);
		bind(constantBufferIndex);
    }

	void update(const T &data)
	{
        m_graphics->getContext().UpdateSubresource(m_buffer, 0, nullptr, &data, 0, 0);
	}

	void bind(UINT constantBufferIndex) 
	{
		bindVertexShader(constantBufferIndex);
		bindGeometryShader(constantBufferIndex);
		bindPixelShader(constantBufferIndex);
	}

	void unbind(unsigned int constantBufferIndex)
	{
		ID3D11Buffer* buffNULL[] = { nullptr };
		m_graphics->getContext()->GSSetConstantBuffers(constantBufferIndex, 1, buffNULL);
		m_graphics->getContext()->PSSetConstantBuffers(constantBufferIndex, 1, buffNULL);
	}

	void bindVertexShader(UINT constantBufferIndex)
	{
        m_graphics->getContext().VSSetConstantBuffers(constantBufferIndex, 1, &m_buffer);
	}

	void bindGeometryShader(UINT constantBufferIndex)
	{
		m_graphics->getContext().GSSetConstantBuffers(constantBufferIndex, 1, &m_buffer);
	}

	void bindPixelShader(UINT constantBufferIndex)
	{
        m_graphics->getContext().PSSetConstantBuffers(constantBufferIndex, 1, &m_buffer);
	}

private:
	D3D11GraphicsDevice *m_graphics;
	ID3D11Buffer *m_buffer;
};

}  // namespace ml

#endif  // APPLICATION_D3D11_D3D11CONSTANTBUFFER_H_
