
#ifndef APPLICATION_BASE_GRAPHICSASSET_H_
#define APPLICATION_BASE_GRAPHICSASSET_H_

namespace ml {


class GraphicsDevice;
class GraphicsAsset
{
public:
	virtual void release() = 0;
	virtual void reset() = 0;
	virtual void onDeviceResize() {}
};

}  // namespace ml

#endif  // APPLICATION_BASE_GRAPHICSASSET_H_