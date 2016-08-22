
namespace ml
{
	template <class T>
	void D3D11Buffer<T>::load(GraphicsDevice& g, const std::vector<T>& data)
	{
		m_graphics = &g.castD3D11();
		release();

		g.castD3D11().registerAsset(this);
		m_data = data;

		reset();
	}

	template <class T>
	void D3D11Buffer<T>::release()
	{
		SAFE_RELEASE(m_buffer);
		SAFE_RELEASE(m_srv);
		SAFE_RELEASE(m_uav);
	}

	template <class T>
	void D3D11Buffer<T>::reset()
	{
		release();

		if (m_data.size() == 0) return;

		auto &device = m_graphics->getDevice();
		auto &context = m_graphics->getContext();

		D3D11_BUFFER_DESC desc;
		desc.ByteWidth = sizeof(T)*m_data.size();
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = sizeof(T);
		desc.Usage = D3D11_USAGE_DEFAULT;	// read/write GPU
		//desc.Usage = D3D11_USAGE_DYNAMIC;	// read-only GPU, write CPU
		desc.BindFlags = 0;
		if (hasSRV()) desc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
		if (hasUAV()) desc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;
		
		D3D11_SUBRESOURCE_DATA initialData;
		initialData.pSysMem = m_data.data();
		initialData.SysMemPitch = 0;
		initialData.SysMemSlicePitch = 0;

		D3D_VALIDATE(device.CreateBuffer(&desc, &initialData, &m_buffer));
		if (hasSRV())	D3D_VALIDATE(device.CreateShaderResourceView(m_buffer, nullptr, &m_srv));
		if (hasUAV())	D3D_VALIDATE(device.CreateUnorderedAccessView(m_buffer, nullptr, &m_uav));
		
		//context.UpdateSubresource(m_texture, 0, nullptr, m_image.getData(), (UINT)m_image.getWidth() * sizeof(vec4uc), (UINT)m_image.getWidth() * (UINT)m_image.getHeight() * sizeof(vec4uc));
		//context.GenerateMips(m_srv);
	}

	template <class T>
	void D3D11Buffer<T>::bind(unsigned int slot /* = 0 */) const
	{
		if (m_srv == nullptr) return;
		m_graphics->getContext().PSSetShaderResources(slot, 1, &m_srv);
	}

	template <class T>
	void D3D11Buffer<T>::unbind(unsigned int slot /* = 0 */) const
	{
		if (m_srv == nullptr) return;
		ID3D11ShaderResourceView* const srvs[] = { NULL };
		m_graphics->getContext().PSSetShaderResources(slot, 1, &srvs);
	}
}