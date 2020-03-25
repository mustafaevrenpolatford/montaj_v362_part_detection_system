#include <cmath>
#include <csignal>
#include <iostream>
#include <sstream>
#include <string>
#include <syslog.h>
#include <unistd.h>
#include <vector>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <opencv2/opencv.hpp>
#include <mosquitto.h>

const char *video_stream_adresi = "rtsp://admin:Test1234@10.14.37.205:80";
int sonuc;
cv::VideoCapture capture;

void kamera_baglanti_kur(int *sonuc)
{
	capture.set(CAP_PROP_BUFFERSIZE, 1);
	int frameRate = capture.get(CAP_PROP_FPS);
	capture.set(CAP_PROP_POS_FRAMES, frameRate);

	if (!capture.open(video_stream_adresi))
	{
		*sonuc = 0;
		return;
	}
	*sonuc = 1;
}