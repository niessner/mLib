
#ifndef APPLICATION_D3D11_D3D11BUFFER_H_
#define APPLICATION_D3D11_D3D11BUFFER_H_

namespace ml {

	template<class T>
	class D3D11Buffer : public GraphicsAsset
	{
	public:
		D3D11Buffer(bool _withSRV = true, bool _withUAV = false)
		{
			m_graphics = nullptr;
			m_buffer = nullptr;
			m_srv = nullptr;
			m_uav = nullptr;
			
			m_bHasSRV = _withSRV;	//TODO add parameter
			m_bHasUAV = _withUAV;	//TODO add parameter
		}

		D3D11Buffer(GraphicsDevice& g, const std::vector<T>& data)
		{
			m_buffer = nullptr;
			m_srv = nullptr;
			m_uav = nullptr;
			load(g, data);
		}

		//
		// TODO: implement other copy constructors similar to D3D11Mesh
		//
		D3D11Buffer(D3D11Buffer&& t)
		{
			m_data = std::move(t.m_data);
			m_graphics = t.m_graphics;
			m_buffer = t.m_buffer; t.m_buffer = nullptr;
			m_srv = t.m_srv; t.m_srv = nullptr;
			m_uav = t.m_uav; t.m_uav = nullptr;
			m_bHasSRV = t.m_bHasSRV;
			m_bHasUAV = t.m_bHasUAV;
		}

		void operator=(D3D11Buffer&& t)
		{
			m_data = std::move(t.m_data);
			m_graphics = t.m_graphics;
			m_buffer = t.m_buffer; t.m_buffer = nullptr;
			m_srv = t.m_srv; t.m_srv = nullptr;
			m_uav = t.m_uav; t.m_uav = nullptr;
			m_bHasSRV = t.m_bHasSRV;
			m_bHasUAV = t.m_bHasUAV;
		}		

		~D3D11Buffer()
		{
			release();
		}

		//! adl swap
		friend void swap() {

		}

		void load(GraphicsDevice& g, const std::vector<T>& data);

		void release();
		void reset();

		//! binds the buffer as a shader resource view
		void bindSRV(unsigned int slot = 0) const;
		//! unbinds the shader resource view
		void unbindSRV(unsigned int slot = 0) const;

		const std::vector<T>& getData() const {
			return m_image;
		}

		bool hasSRV() const {
			return m_bHasSRV;
		}
		bool hasUAV() const {
			return m_bHasUAV;
		}

	private:
		D3D11GraphicsDevice* m_graphics;
		std::vector<T> m_data;
		ID3D11Buffer* m_buffer;
		ID3D11ShaderResourceView* m_srv;
		ID3D11UnorderedAccessView* m_uav;

		bool m_bHasSRV;
		bool m_bHasUAV;
	};

}  // namespace ml

#include "D3D11Buffer.inl"

#endif  // APPLICATION_D3D11_D3D11Buffer_H_
