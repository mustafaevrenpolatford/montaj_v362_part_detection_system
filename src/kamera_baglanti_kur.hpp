#ifndef _KAMERA_BAGLANTI_KUR
#define _KAMERA_BAGLANTI_KUR

#include <opencv2/opencv.hpp>
const char *video_stream_adresi = "rtsp://admin:Test1234@10.14.37.205:80";
void baglanti_kur(int *sonuc)
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

#endif