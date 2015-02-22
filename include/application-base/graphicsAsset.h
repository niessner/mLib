
#ifndef APPLICATION_BASE_GRAPHICSASSET_H_
#define APPLICATION_BASE_GRAPHICSASSET_H_

namespace ml {


class GraphicsDevice;
class GraphicsAsset
{
public:
	//TODO i think the graphics device parameters should go away as the device is stored anyway...
	virtual void release() = 0;
	virtual void reset() = 0;
	virtual void resize() {}
};

}  // namespace ml

#endif  // APPLICATION_BASE_GRAPHICSASSET_H_