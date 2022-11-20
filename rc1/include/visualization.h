#ifndef _Visualization_H
#define _Visualization_H
#pragma once

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include <cstdlib>
#include <omp.h>

#define left 333
#define right 893
#define up  80
#define down 600

extern bool mouseClick;
extern cv::Point A;
void onMouse(int event, int x, int y, int, void*);
void draw_lines(cv::Mat& clr);
#endif
