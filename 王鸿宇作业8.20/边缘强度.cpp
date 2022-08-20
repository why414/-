#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <math.h>
#include <opencv2/imgproc.hpp>
#define kernel_size 3
using namespace cv;
using namespace std;


Mat gaussian_filter(Mat img, double sigma)
{
	Mat out = Mat::zeros(img.rows, img.cols, CV_8UC3);
	int height = img.rows;
	int width = img.cols;
	float kernel[kernel_size][kernel_size];
	float value = 0;
	//初始化卷积核
	for (int y = 0; y < kernel_size; y++)
	{
		for (int x = 0; x < kernel_size; x++)
		{
			int _x = x - kernel_size / 2;
			int _y = y - kernel_size / 2;
			kernel[y][x] = 1 / (2 * 3.14 * sigma * sigma) * exp(-(_x * _x + _y * _y) / (2 * sigma * sigma));
			value += kernel[y][x];
		}
	}
	//归一化
	for (int y = 0; y < kernel_size; y++)
	{
		for (int x = 0; x < kernel_size; x++)
		{
			kernel[y][x] /= value;
		}
	}
	//过滤
	double v = 0;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{

			v = 0;
			for (int dy = -kernel_size / 2; dy < kernel_size / 2; dy++)
			{
				for (int dx = -kernel_size / 2; dx < kernel_size / 2; dx++)
				{
					if (((x + dx) >= 0) && ((y + dy) >= 0) && ((x + dx) < width) && ((y + dy) < height))
					{
						v += (double)img.at<uchar>(y + dy, x + dx) * kernel[dy + kernel_size / 2][dx + kernel_size / 2];
					}
				}
			}
			out.at<uchar>(y, x) = v;
		}
	}

	return out;
}

Mat sobel_filter(cv::Mat img, bool horizontal)
{
	int height = img.rows;
	int width = img.cols;
	int channel = img.channels();
	double kernel[kernel_size][kernel_size] = { {1, 2, 1}, {0, 0, 0}, {-1, -2, -1} };
	cv::Mat out = cv::Mat::zeros(height, width, CV_8UC1);

	if (horizontal)
	{
		kernel[0][1] = 0;
		kernel[0][2] = -1;
		kernel[1][0] = 2;
		kernel[1][2] = -2;
		kernel[2][0] = 1;
		kernel[2][1] = 0;
	}
	double v = 0;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{

			v = 0;
			for (int dy = -kernel_size / 2; dy < kernel_size / 2; dy++)
			{
				for (int dx = -kernel_size / 2; dx < kernel_size / 2; dx++)
				{
					if (((x + dx) >= 0) && ((y + dy) >= 0) && ((x + dx) < width) && ((y + dy) < height))
					{
						v += (double)img.at<uchar>(y + dy, x + dx) * kernel[dy + kernel_size / 2][dx + kernel_size / 2];
					}
				}
			}
			out.at<uchar>(y, x) = v;
		}
	}
	return out;
}
Mat get_edge(Mat fx, Mat fy)
{

	int height = fx.rows;
	int width = fx.cols;
	Mat out = Mat::zeros(height, width, CV_8UC1);
	double _fx, _fy;

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			_fx = (double)fx.at<uchar>(y, x);
			_fy = (double)fy.at<uchar>(y, x);

			out.at<uchar>(y, x) = (uchar)(sqrt(_fx * _fx + _fy * _fy));
		}
	}
	return out;
}
Mat get_angle(cv::Mat fx, cv::Mat fy) {

	int height = fx.rows;
	int width = fx.cols;

	cv::Mat out = cv::Mat::zeros(height, width, CV_8UC1);

	double _fx, _fy;
	double angle;

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			_fx = fmax((double)fx.at<uchar>(y, x), 0.000001);
			_fy = (double)fy.at<uchar>(y, x);

			angle = atan2(_fy, _fx);
			angle = angle / 3.14 * 180;

			if (angle < -22.5) {
				angle = 180 + angle;
			}
			else if (angle >= 157.5) {
				angle = angle - 180;
			}
			if (angle <= 22.5) {
				out.at<uchar>(y, x) = 0;
			}
			else if (angle <= 67.5) {
				out.at<uchar>(y, x) = 45;
			}
			else if (angle <= 112.5) {
				out.at<uchar>(y, x) = 90;
			}
			else {
				out.at<uchar>(y, x) = 135;
			}
		}
	}

	return out;
}

Mat non_maximum_suppression(cv::Mat angle, cv::Mat edge)
{
	Mat out = Mat::zeros(angle.rows, angle.cols, CV_8UC1);
	int dx1, dx2, dy1, dy2;
	int now_angle;
	for (int y = 0; y < angle.rows; y++)
	{
		for (int x = 0; x < angle.cols; x++)
		{
			now_angle = angle.at<uchar>(x, y);
			if (now_angle == 0)
			{
				dx1 = -1;
				dy1 = 0;
				dx2 = 1;
				dy2 = 0;
			}
			else if (now_angle == 45)
			{
				dx1 = -1;
				dy1 = 1;
				dx2 = 1;
				dy2 = -1;
			}
			else if (now_angle == 90)
			{
				dx1 = 0;
				dy1 = -1;
				dx2 = 0;
				dy2 = 1;
			}
			else
			{
				dx1 = -1;
				dy1 = -1;
				dx2 = 1;
				dy2 = 1;
			}

			if (x == 0)
			{
				dx1 = fmax(dx1, 0);
				dx2 = fmax(dx2, 0);
			}
			if (x == (angle.cols - 1))
			{
				dx1 = fmin(dx1, 0);
				dx2 = fmin(dx2, 0);
			}
			if (y == 0) {
				dy1 = fmax(dy1, 0);
				dy2 = fmax(dy2, 0);
			}
			if (y == (angle.rows - 1))
			{
				dy1 = fmin(dy1, 0);
				dy2 = fmin(dy2, 0);
			}


			if (fmax(fmax(edge.at<uchar>(y, x), edge.at<uchar>(y + dy1, x + dx1)), edge.at<uchar>(y + dy2, x + dx2)) == edge.at<uchar>(y, x)) {
				out.at<uchar>(y, x) = edge.at<uchar>(y, x);
			}
		}
	}

	return out;
}

Mat histerisis(Mat edge, int HT, int LT) {
	int height = edge.rows;
	int width = edge.cols;
	int channle = edge.channels();
	Mat _edge = Mat::zeros(height, width, CV_8UC1);

	int now_pixel;

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			now_pixel = edge.at<uchar>(y, x);
			if (now_pixel >= HT) {
				_edge.at<uchar>(y, x) = 255;
			}
			else if (HT > now_pixel > LT) 
			{
				for (int dy = -1; dy < 2; dy++) 
				{
					for (int dx = -1; dx < 2; dx++) 
					{
						if (edge.at<uchar>(fmin(fmax(y + dy, 0), height-1), fmin(fmax(x + dx, 0), width-1)) >= HT) 
						{
							_edge.at<uchar>(y, x) = 255;
						}
					}
				}
			}
			else
			{
				_edge.at<uchar>(y, x) = 0;
			}
		}
	}
	return _edge;
}







int main()
{

	Mat img = imread("E:/abc.jpg");
	cv::cvtColor(img, img, COLOR_BGR2GRAY);
	Mat out = gaussian_filter(img, 1.4);
	Mat fy = sobel_filter(out, false);
	Mat fx = sobel_filter(out, true);
	Mat edge = get_edge(fx, fy);
	Mat angle = get_angle(fx, fy);
	imshow("edge, edge);
	imshow("angle",angle);
	waitKey(0);
	destroyAllWindows();
	return 0;

}
