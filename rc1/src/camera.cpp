#include "camera.h"

using namespace k4a;
using namespace cv;
using namespace std;

int camera(k4a::device& device, k4a::capture& capture)
{
	const uint32_t device_count = k4a::device::get_installed_count();//查看设备个数
	if (device_count == 0)
	{
		printf("No K4A devices found\n");
		return -1;
	}
	k4a_device_configuration_t config = K4A_DEVICE_CONFIG_INIT_DISABLE_ALL;	//设定设备参数
	config.color_format = K4A_IMAGE_FORMAT_COLOR_BGRA32;
	//config.color_resolution = K4A_COLOR_RESOLUTION_1080P;
	config.color_resolution = K4A_COLOR_RESOLUTION_720P;
	config.depth_mode = K4A_DEPTH_MODE_NFOV_UNBINNED;
	config.camera_fps = K4A_FRAMES_PER_SECOND_15;
	config.synchronized_images_only = true;
	device.start_cameras(&config);
	return 0;
}