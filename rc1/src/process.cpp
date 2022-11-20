#include "process.h"
using namespace cv;
using namespace std;   

Mat img2, img3,img4,img5;
int dmin = 1, dmax = 90;//(*100)
bool slope;
bool width;

float length_img, depth, radius_img_to_real,height_real;
float focal = 503;
float r_std = 10;
float error1 = 100;//待修改
float error2 = 2.0;//待调整
float heightmin = 0;
int rmax = 0;
int bmax = 0;

void findup(Point2f center,Mat img,float height,float length_img)
{
	int x = center.x-length_img*0.5-error2;
	int y = center.y;
	int i = 0;
	for (i = y-height*0.5; i <y; y++)
	{
		int r = img.at<Vec3b>(i, x)[0];
		int g = img.at<Vec3b>(i, x)[1];
		int b = img.at<Vec3b>(i, x)[2];
		if (r > rmax )
		{
			cout << "red circle!" << endl;
		}
		if (b > bmax)
		{
			cout << "blue circle" << endl;
		}
	}
}

void preprocessing(cv::Mat& transformed_depth_frame, cv::Mat& color_frame, cv::Mat& HSV_frame)
{
	Mat imageROI_d, imageROI_c, imageROI_h, imageROI_m;
	imageROI_d = transformed_depth_frame(Range(up, down), Range(left, right));
	imageROI_c = color_frame(Range(up, down), Range(left, right));
	imageROI_h = HSV_frame(Range(up, down), Range(left, right));//�������޸�imageROI_h��ͬʱҲ���޸�HSV

	inRange(imageROI_d, dmin * 100, dmax * 100, img5);

	cvtColor(imageROI_c, imageROI_h, COLOR_RGB2HSV);
	vector<Mat> channels;
	split(imageROI_h, channels);
	threshold(channels.at(1), channels.at(1), 40, 255, THRESH_BINARY_INV);
	imageROI_c.copyTo(img2, channels.at(1));
	//imshow("image2", img2);
	Mat element1 = getStructuringElement(MORPH_RECT, Size(9, 9));
	morphologyEx(img2, img2, MORPH_CLOSE, element1);

	imshow("image2", img2);
	inRange(img2, Scalar(120, 120, 120), Scalar(255, 255, 255), img3);

	//imshow("img3", img3);

	Mat element = getStructuringElement(MORPH_RECT, Size(15, 15));
	morphologyEx(img3, img4, MORPH_OPEN, element);
}

void process(cv::Mat& transformed_depth_frame, cv::Mat& color_frame, std::ofstream& outfile)
{
/*	const int64 start = cv::getTickCount();
	Mat img = imread("C:/Users/h/Desktop/rc/����5.jpg");
	Mat sobel_x, sobel_y, sobel, thr, thr1;

	Mat img2, srcRoi, hsv, img3, img4, img5;
	srcRoi = img;
	cvtColor(img, hsv, COLOR_BGR2HSV);
	imshow("img", img);
	imshow("hsv", hsv);
	vector<Mat> channels;
	split(hsv, channels);
	threshold(channels.at(1), channels.at(1), 40, 255, THRESH_BINARY_INV);
	srcRoi.copyTo(img2, channels.at(1));
//	imshow("image2", img2);
	Mat element1 = getStructuringElement(MORPH_RECT, Size(9, 9));
	morphologyEx(img2, img2, MORPH_CLOSE, element1);
	imshow("image2", img2);
	inRange(img2, Scalar(120, 120, 120), Scalar(255, 255, 255), img3);

	imshow("img3", img3);

	Mat element = getStructuringElement(MORPH_RECT, Size(15, 15));
	morphologyEx(img3, img4, MORPH_OPEN, element);
	imshow("img4", img4);*/

/*	Sobel(img4, sobel_x, CV_32F, 1, 0, 1, 1, 0, BORDER_DEFAULT);
	Sobel(img4, sobel_y, CV_32F, 0, 1, 3, 1, 0, BORDER_DEFAULT);

	convertScaleAbs(sobel_x, sobel_x);
	convertScaleAbs(sobel_y, sobel_y);

	imshow("sobel_x", sobel_x);
	imshow("sobel_y", sobel_y);

	addWeighted(sobel_x, 0.7, sobel_y, 0.3, 0, sobel);
	imshow("sobel", sobel);*/


	vector<vector<Point>> contours;
	int i,j;
	Point2f mc;
	findContours(img4, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	for (i = 0; i < contours.size(); i++)
	{
		slope = false;
		width = false;
		RotatedRect point = minAreaRect(contours[i]);
		Point2f result[4];
		point.points(result);
		length_img = abs(result[0].x - result[1].x);
		Point2f middle;
		middle.x = (result[0].x + result[1].x) * 0.5;
		middle.y = (result[0].x + result[1].y) * 0.5;
		depth = (float)transformed_depth_frame.at<ushort>(middle.y, middle.x);
		float k = (result[0].y - result[1].y) / (result[0].x - result[1].x);
		if(abs(k)>10||abs(k)<0.5)
		{
			slope = true;
		}
		if ( abs((length_img*0.5*depth)/(focal-length_img*0.5)- r_std)< error1 && abs(result[0].y - result[2].y) > heightmin)
		{
			width = true;
		}
		if(slope==true&&width==true)
		{

			for (j = 0; j < 4; j++)
			{
				line(color_frame, result[j], result[(j + 1) % 4], Scalar(0, 0, 255), 2);
			}
			mc.x = 0.5 * (result[0].x + result[1].x);
			mc.y = 0.5 * (result[0].y + result[2].y);
			height_real = 0.5 * abs(result[0].y - result[2].y);
			circle(color_frame, mc, 3, Scalar(255, 0, 0), -1, 8, 0);
		}
	}
	/*findup(mc, color_frame,height_real,length_img);*/
//	float time = (getTickCount() - start) / getTickFrequency();
//	cout << time;
//	waitKey();
//	return 0;
}

