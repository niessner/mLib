
#ifndef APPLICATION_BASE_GRAPHICSASSET_H_
#define APPLICATION_BASE_GRAPHICSASSET_H_

namespace ml {



class GraphicsAsset
{
public:
	virtual void release() = 0;
	virtual void reset() = 0;

	virtual std::string getName() const {
		return "unknown graphics asset";
	}
};

}  // namespace ml

#endif  // APPLICATION_BASE_GRAPHICSASSET_H_