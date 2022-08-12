#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	Mat img = imread("E:/abc.jpg");
	for(int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			int m = img.at<Vec3b>(i, j)[0];
			img.at<Vec3b>(i,j)[0] = img.at<Vec3b>(i, j)[2];
			img.at<Vec3b>(i, j)[2] = m;
		}
	}
	namedWindow("通道转换");
	imshow("通道转换",img);
	waitKey(0);
	destroyAllWindows();
	return 0;
}
