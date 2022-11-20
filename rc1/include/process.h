#ifndef _Process_H
#define _Process_H
#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/video/tracking.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>
#include <cstdlib>
#include <omp.h>
#include <fstream>
#include <cmath>
#include "serial_port.h"
#include "visualization.h"
#define FramePerSecond 15

void preprocessing(cv::Mat& transformed_depth_frame, cv::Mat& color_frame, cv::Mat& HSV_frame);
void process(cv::Mat& transformed_depth_frame, cv::Mat& color_frame, std::ofstream& outfile);
#endif