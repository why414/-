#pragma comment(lib, "k4a.lib")
#include <stdio.h>
#include <k4a/k4a.h>
#include <k4arecord/record.h>
#include <k4arecord/playback.h>
#include <iostream>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include <k4a/k4a.hpp>
#include <cstdlib>
#include <omp.h>
#include <fstream>
#include "serial_port.h"
#include "visualization.h"
#include "camera.h"
#include "process.h"

using namespace cv;
using namespace std;

int main()
{
    k4a::device device = k4a::device::open(K4A_DEVICE_DEFAULT);
    k4a::capture capture;

    camera(device, capture);//Start camera


    cv::Mat color_frame;
    cv::Mat depth_frame;
    cv::Mat HSV_frame;
    cv::Mat transformed_depth_frame;

    ofstream outfile("../data0.txt");

    k4a_calibration_t calibration = device.get_calibration(K4A_DEPTH_MODE_NFOV_UNBINNED, K4A_COLOR_RESOLUTION_720P);
    k4a::transformation transformation = k4a::transformation(calibration);

    //int left = 500, right = 1340, down = 300;
    cv::Mat maska = Mat::zeros(down + 1, right - left + 1, CV_8UC1);

    namedWindow("color");
    setMouseCallback("color", onMouse);

    while (true)
    {
        //double t1 = (double)getTickCount();
        if (device.get_capture(&capture, std::chrono::milliseconds(0)))
        {
            // __________________________________________________________//

            k4a::image rgbImage;
            k4a::image depthImage;
            k4a::image transformed_depth_image;
            Mat img_4;

            rgbImage = capture.get_color_image();
            depthImage = capture.get_depth_image();
            transformed_depth_image = transformation.depth_image_to_color_camera(depthImage);

            //_________________________________________________________//

            color_frame = cv::Mat(rgbImage.get_height_pixels(), rgbImage.get_width_pixels(), CV_8UC4, rgbImage.get_buffer());
            depth_frame = cv::Mat(depthImage.get_height_pixels(), depthImage.get_width_pixels(), CV_16U, depthImage.get_buffer());

            transformed_depth_frame = cv::Mat(transformed_depth_image.get_height_pixels(), transformed_depth_image.get_width_pixels(), CV_16U, transformed_depth_image.get_buffer());


            HSV_frame = Mat::zeros(color_frame.rows, color_frame.cols, CV_8UC3);
            preprocessing(transformed_depth_frame, color_frame, HSV_frame);

            process(transformed_depth_frame, color_frame, outfile); 


            if (mouseClick)
            {
                //cout << "H:" << (int)HSV_frame.at<Vec3b>(A.y, A.x)[0] << endl;
                cout << "depth: " << (int)transformed_depth_frame.at<ushort>(A.y, A.x) << endl;
                cout << "H:" << (int)HSV_frame.at<Vec3b>(A.y, A.x)[0] << "   S:" << (int)HSV_frame.at<Vec3b>(A.y, A.x)[1] << "   V:" << (int)HSV_frame.at<Vec3b>(A.y, A.x)[2] << endl;
            }
            draw_lines(color_frame);

            cv::imshow("color",color_frame);

			//cv::imshow("depth", depth_frame);
			//cv::imshow("transformed_depth_frame", transformed_depth_frame);
			//cv::imshow("maska", maska);
			//cv::imshow("transformed_depth_frame", transformed_depth_frame);
            color_frame.release();
            depth_frame.release();

            capture.reset();
            if (waitKey(1) == ' ')
            {
                break;

            }

        }
    }
        outfile.close();
        cv::destroyAllWindows();
        device.close();
        return 0;
}