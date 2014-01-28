
class GraphicsAsset
{
public:
	virtual void release(GraphicsDevice &g) = 0;
	virtual void reset(GraphicsDevice &g) = 0;
};