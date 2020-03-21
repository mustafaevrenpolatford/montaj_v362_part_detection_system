#ifndef _KAMERADAN_GORUNTU_AL_
#define _KAMERADAN_GORUNTU_AL_

#include "includelar.h"
cv::Mat kameradan_goruntu_al()
{
	int sonuc;
	cv::Mat goruntu_RGB;

	baglanti_kur(&sonuc);
	if (sonuc == 0)
	{
		syslog(LOG_INFO, "%s(): Kamera acilamadi.", __func__);
		return goruntu_RGB;
	}
	syslog(LOG_INFO, "%s(): Kamera acildi.", __func__);

	capture.read(goruntu_RGB);

	if (counter < 10000)
	{
		counter++;
	}
	else
	{
		counter = 0;
	}

	capture.release();
	return goruntu_RGB;
}

#endif