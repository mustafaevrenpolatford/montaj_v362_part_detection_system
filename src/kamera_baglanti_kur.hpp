#ifndef KAMERA_BAGLANTI_KUR_HPP
#define KAMERA_BAGLANTI_KUR_HPP

#include <opencv2/opencv.hpp>

void kamera_baglanti_kur(int *sonuc);
extern const char *video_stream_adresi = "rtsp://admin:Test1234@10.14.37.205:80";
using namespace cv;
extern cv::VideoCapture capture;

#endif