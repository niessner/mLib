
#ifndef APPLICATION_D3D11_D3D11BUFFER_H_
#define APPLICATION_D3D11_D3D11BUFFER_H_

namespace ml {

	template<class T>
	class D3D11Buffer : public GraphicsAsset
	{
	public:
		D3D11Buffer()
		{
			m_graphics = nullptr;
			m_buffer = nullptr;
			m_srv = nullptr;
			m_uav = nullptr;
			
			m_bHasSRV = true;	//TODO add parameter
			m_bHasUAV = false;	//TODO add parameter
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

		D3D11Buffer(GraphicsDevice& g, const std::vector<T>& data)
		{
			m_buffer = nullptr;
			m_srv = nullptr;
			m_uav = nullptr;
			load(g, data);
		}
		void load(GraphicsDevice& g, const std::vector<T>& data);

		void release();
		void reset();

		void bindSRV(unsigned int slot = 0) const;
		void unbindSRV(unsigned int slot = 0) const;

		const std::vector<T>& getData() const
		{
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
