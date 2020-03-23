#ifndef _KAMERA_BAGLANTI_KUR_
#define _KAMERA_BAGLANTI_KUR_

#include <opencv2/opencv.hpp>

void baglanti_kur(int *sonuc, char *video_stream_adresi)
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