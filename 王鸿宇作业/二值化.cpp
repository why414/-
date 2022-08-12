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
			out.at<uchar>(i, j) = 0.2126 * (float)img.at<Vec3b>(i, j)[2]
				+ 0.7152 * (float)img.at<Vec3b>(i, j)[1]
				+ 0.0722 * (float)img.at<Vec3b>(i, j)[0];

			if (out.at<uchar>(i, j) < 128) out.at<uchar>(i, j) = 0;
			else  out.at<uchar>(i, j) = 255;

		}
	}
	namedWindow("二值化", WINDOW_AUTOSIZE);
	imshow("二值化", out);
	waitKey(0);
	destroyAllWindows();
	return 0;


}
