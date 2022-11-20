#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>

using namespace cv;
using namespace std;

void drawx(Mat img, Mat sobel_x);
void drawy(Mat img, Mat sobel_y);

int main()
{
	Mat img=imread("C:/Users/h/Desktop/rc/Á¢Öù6.jpg");
	Mat sobel_x, sobel_y,sobel,thr,thr1;

	Sobel(img, sobel_x, CV_32F, 1, 0, 1, 1, 0, BORDER_DEFAULT);
	Sobel(img, sobel_y, CV_32F, 0, 1, 1, 1, 0, BORDER_DEFAULT);

	convertScaleAbs(sobel_x, sobel_x);
	convertScaleAbs(sobel_y, sobel_y);

	imshow("sobel_x", sobel_x);
//	imshow("sobel_y", sobel_y);

//	addWeighted(sobel_x, 0.7, sobel_y, 0.3, 0, sobel);
//	imshow("sobel", sobel);
	drawx(img, sobel_x);
//	drawy(img, sobel_y);


	imshow("output", img);

	waitKey();
	return 0;
}


void drawx(Mat img,Mat sobel_x)
{
	Mat thr, thr1;
	threshold(sobel_x, thr, 50, 255, THRESH_BINARY);
	imshow("thr", thr);
	cvtColor(thr, thr1, CV_BGR2GRAY);
	vector<Vec4f> plines;
	Scalar color = Scalar(0, 0, 255);
	HoughLinesP(thr1, plines, 1.0, CV_PI / 180.0, 10, 100, 100);
	bool choose1;
	bool choose2;
	for (size_t i = 0; i < plines.size(); i++)
	{
		Vec4f hlines = plines[i];
		for (size_t j = i + 1; j < plines.size(); j++)
		{
			bool choose1 = false;
			bool choose2 = false;
			Vec4f hlines1 = plines[j];
			int k1 = (hlines1[1] - hlines1[3]) / (hlines1[0] - hlines1[2]);
			int k2 = (hlines[1] - hlines[3]) / (hlines[0] - hlines[2]);
			if (abs(abs(k1) - abs(k2)) < 5 && abs(k1) > 15) { choose1 = true; }
			/*float r ;
			float depth = ;
			dy=hlines[3]-hlines[1];
			r=dy*depth/(f-dy);
			if (r-R<error) { choose2 = true; }*/
			choose2 = true;
			if (choose1 == true && choose2 == true)
			{
				line(img, Point(hlines[0], hlines[1]), Point(hlines[2], hlines[3]), color, 2, LINE_AA);
				//				line(img, Point(hlines[0], hlines[1]), Point(hlines1[0], hlines1[1]), color, 2, LINE_AA);
				//				line(img, Point(hlines[2], hlines[3]), Point(hlines1[2], hlines1[3]), color, 2, LINE_AA);
				line(img, Point(hlines1[0], hlines1[1]), Point(hlines1[2], hlines1[3]), color, 2, LINE_AA);
			}
		}
	}
}


void drawy(Mat img, Mat sobel_y)
{
	Mat thr, thr1;
	threshold(sobel_y, thr, 50, 255, THRESH_BINARY);
	imshow("thr", thr);
	cvtColor(thr, thr1, CV_BGR2GRAY);
	vector<Vec4f> plines;
	Scalar color = Scalar(0, 0, 255);
	HoughLinesP(thr1, plines, 1.0, CV_PI / 180.0, 10, 0, 7);
	bool choose1;
	bool choose2;
	for (size_t i = 0; i < plines.size(); i++)
	{
		Vec4f hlines = plines[i];
		for (size_t j = i + 1; j < plines.size(); j++)
		{
			bool choose1 = false;
			bool choose2 = false;
			Vec4f hlines1 = plines[j];
			int k1 = (hlines1[1] - hlines1[3]) / (hlines1[0] - hlines1[2]);
			int k2 = (hlines[1] - hlines[3]) / (hlines[0] - hlines[2]);
			if (abs(abs(k1) - abs(k2)) < 5 && abs(k1) <10) { choose1 = true; }
			float d;
			float depth;
			d = 30;
			if (d>20)
			{
				choose2 = true;
			}
			if (choose1 == true && choose2 == true)
			{
				line(img, Point(hlines[0], hlines[1]), Point(hlines[2], hlines[3]), color, 2, LINE_AA);
				//				line(img, Point(hlines[0], hlines[1]), Point(hlines1[0], hlines1[1]), color, 2, LINE_AA);
				//				line(img, Point(hlines[2], hlines[3]), Point(hlines1[2], hlines1[3]), color, 2, LINE_AA);
				line(img, Point(hlines1[0], hlines1[1]), Point(hlines1[2], hlines1[3]), color, 2, LINE_AA);
			}
		}
	}
}