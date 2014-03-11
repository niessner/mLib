
#ifndef APPLICATION_BASE_GRAPHICSASSET_H_
#define APPLICATION_BASE_GRAPHICSASSET_H_

namespace ml {

class GraphicsDevice;
class GraphicsAsset
{
public:
	virtual void release(GraphicsDevice &g) = 0;
	virtual void reset(GraphicsDevice &g) = 0;
};

}  // namespace ml

#endif  // APPLICATION_BASE_GRAPHICSASSET_H_