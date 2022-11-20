#include "visualization.h"

using namespace cv;
using namespace std;

bool mouseClick = false;
Point A = Point(0, 0);

void onMouse(int event, int x, int y, int, void*)
{
	switch (event)
	{
	case EVENT_LBUTTONDOWN:
		A = Point(x, y);
		mouseClick = true;
		cout << "Point" << A << endl;
		break;
	case EVENT_LBUTTONUP:
		A = Point(x, y);
		mouseClick = false;
		//cout << "Point" << A << endl;
		break;
	}
}

void draw_lines(Mat& clr)
{
	line(clr, Point(left, up), Point(left, down), Scalar(0, 250, 250), 2);
	line(clr, Point(right, up), Point(right, down), Scalar(0, 250, 250), 2);
	line(clr, Point(left, down), Point(right, down), Scalar(0, 250, 250), 2);
	line(clr, Point(left, up), Point(right, up), Scalar(0, 250, 250), 2);
}