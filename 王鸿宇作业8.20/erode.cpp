#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <math.h>
#include <opencv2/imgproc.hpp>
using namespace cv;
using namespace std;

Mat  Binarize_Otsu(Mat img)
{
	int height = img.rows;
	int width = img.cols;
	int value=0, th=0;
	double  w0 = 0, m0 = 0, w1 = 0, m1 = 0,sb = 0, sb_max = 0;
	Mat	out = Mat::zeros(width, height, CV_8UC1);
	for (int t = 0; t < 255; t++)
	{	
		w0 = 0, m0 = 0, w1 = 0, m1 = 0;
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				value = (int)img.at<uchar>(y, x);
				if (value < t)
				{
					w0++;
					m0 += value;
				}
				else
				{
					w1++;
					m1 += value;
				}

			}
		}
		m0 /= w0;
		m1 /= w1;
		w0 /= (height * width);
		w1 /= (height * width);
		sb = w0 * w1 * pow((m0 - m1), 2);
		if (sb > sb_max)
		{
			sb_max = sb;
			th = t;
		}
	}
	std::cout << "threshold:" << th << std::endl;

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			if (img.at<uchar>(y, x) > th)
			{
				out.at<uchar>(y, x) = 255;
			}
			else
			{
				out.at<uchar>(y, x) = 0;
			}
			

		}
	}
	return out;
}
	
		Mat Morphology_Dilate(Mat img, int Dilate_time)
		{
			int height = img.rows;
			int width = img.cols;
			Mat out = img.clone();
			Mat  tmp_img;
			for (int i = 0; i < Dilate_time; i++)
			{	
				tmp_img = out.clone();
				for (int y = 0; y < height; y++)
				{
					for (int x = 0; x < width; x++)
					{
						if ((x > 0) && (tmp_img.at<uchar>(y, x - 1) == 0))
						{
							out.at<uchar>(y, x) = 0;
							continue;
						}

						
						if ((y > 0) && (tmp_img.at<uchar>(y - 1, x) == 0))
						{
							out.at<uchar>(y, x) = 0;
							continue;
						}
						if ((x < width - 1) && (tmp_img.at<uchar>(y, x + 1) == 0))
						{
							out.at<uchar>(y, x) = 0;
							continue;
						}
						if ((y < height - 1) && (tmp_img.at<uchar>(y + 1, x) == 0))
						{
							out.at<uchar>(y, x) = 0;
							continue;
						}

					}
				}
			}
			return out;
		}





int main()
{

	Mat img = imread("E:/abc.jpg");
	cvtColor(img, img, COLOR_BGR2GRAY);
	Mat bin = Binarize_Otsu(img);
	Mat out = Morphology_Dilate(bin, 2);
	imshow("erode", out);
	waitKey(0);
	destroyAllWindows();
	return 0;

}
