#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <math.h>
#define kernel_size 7
using namespace cv;
using namespace std;


Mat gaussian_filter(Mat img, double sigma)
{
	Mat out = Mat::zeros(img.rows, img.cols, CV_8UC3);
	int height = img.rows;
	int width = img.cols;
	int channel = img.channels();
	int _x=0, _y=0;
	float kernel[kernel_size][kernel_size];
	float value = 0;
	//初始化卷积核
	for (int y = 0; y < kernel_size; y++)
	{
		for (int x = 0; x < kernel_size; x++)
		{
			_x = x - kernel_size / 2;
			_y = y - kernel_size / 2;
			kernel[y][x] = 1 / (2 * 3.14 * sigma * sigma) * exp(-(_x * _x + _y * _y) / (2 * sigma * sigma));
			value += kernel[y][x];
		}
	}
	//归一化
	for (int y = 0; y < kernel_size; y++)
	{
		for (int x = 0; x < kernel_size; x++)
		{
			kernel[y][x] = kernel[y][x] /value;
		}
	}
	//过滤
	double v = 0;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			for (int c = 0; c < channel; c++)
			{
				v = 0;
				for (int dy = -kernel_size / 2; dy < kernel_size / 2 ; dy++)
				{
					for (int dx = -kernel_size / 2; dx < kernel_size / 2 ; dx++)
					{
						if (((x + dx) >= 0) && ((y + dy) >= 0) && ((x + dx) < width) && ((y + dy) < height))
						{
							v += (double)img.at<Vec3b>(y + dy, x + dx)[c] * kernel[dy + kernel_size / 2][dx + kernel_size / 2];
						}
					}
				}
				out.at<Vec3b>(y, x)[c] = v;
			}
		}
	}
	return out;
}


int main()
{

	Mat img = imread("E:/abc.jpg");
	Mat out = Mat::zeros(img.rows, img.cols, CV_8UC3);
	out = gaussian_filter(img, 0.8);
	namedWindow("高斯滤波", WINDOW_AUTOSIZE);
	imshow("高斯滤波", out);
	waitKey(0);
	destroyAllWindows();
	return 0;

}