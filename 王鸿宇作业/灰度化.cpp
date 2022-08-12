#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	Mat img = imread("E:/abc.jpg");
	Mat out = Mat::zeros(img.rows, img.cols, CV_8UC1);
	for(int i = 0; i < out.rows; i++)
	{
		for (int j = 0; j < out.cols; j++)
		{
			out.at<Vec3b>(i, j) = 0.2126 * (float)img.at<Vec3b>(i, j)[2] 
				+ 0.7152 * (float)img.at<Vec3b>(i, j)[1] 
				+ 0.0722 * (float)img.at<Vec3b>(i, j)[0];
		}
	}
	namedWindow("灰度化", WINDOW_AUTOSIZE);
	imshow("灰度化", out);
	waitKey(0);
	destroyAllWindows();
	return 0;
}
