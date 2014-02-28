
template <class T>
class p3d
{
public:
	p3d() {
		array[0] = array[1] = array[2] = 0;
	}

	p3d(const T &x, const T &y, const T &z) {
		array[0] = x;
		array[1] = y;
		array[2] = z;
	}

	static const p3d<T> origin;
	
	union {
		struct {
			T x,y,z;    // standard names for components
		};
		T array[3];     // array access
	};
};

typedef p3d<float> p3df;

int main()
{
	p3df p = p3df::origin;
	return 0;
}

template<> const p3df p3df::origin(0.0f, 0.0f, 0.0f);