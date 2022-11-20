#ifndef _Camera_H
#define _Camera_H
#pragma once

#include <k4a/k4a.h>
#include <k4arecord/record.h>
#include <k4arecord/playback.h>
#include <k4a/k4a.hpp>
#include <opencv2/opencv.hpp>

int camera(k4a::device& device, k4a::capture& capture);
#endif
