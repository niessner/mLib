
//
// ext-depthcamera headers
//
//#include "ext-depthcamera/calibratedSensorData.h"	//this is obsolete
//

#ifdef LINUX
using namespace std;
#include <cmath>
#include <cstdlib>
#endif

namespace stb {
	#define STB_IMAGE_IMPLEMENTATION
#include "ext-depthcamera/sensorData/stb_image.h"
	#undef STB_IMAGE_IMPLEMENTATION

	#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "ext-depthcamera/sensorData/stb_image_write.h"
	#undef STB_IMAGE_WRITE_IMPLEMENTATION
}