#ifndef _GORUNTUYU_KAYDET_
#define _GORUNTUYU_KAYDET_

#include "includelar.h"

void goruntu_kaydet(cv::Mat goruntu_RGB)
{

	int x1_baslangic = 1455;
	int y1_baslangic = 686;
	int x2_baslangic = 1426;
	int y2_baslangic = 610;

	Rect RectangleToDraw(x2_baslangic, y2_baslangic, 10, 10);
	rectangle(goruntu_RGB, RectangleToDraw.tl(), RectangleToDraw.br(), 
	Scalar(0, 0, 255), 2, 8, 0);

	Rect RectangleToDraw2(x1_baslangic, y1_baslangic, 10, 10);
	rectangle(goruntu_RGB, RectangleToDraw2.tl(), RectangleToDraw2.br(), 
	Scalar(0, 0, 255), 2, 8, 0);


	char goruntu_ismi[256];
	std::string temp = to_string(counter);
	sprintf(goruntu_ismi, "%s%s%s", "goruntu_", temp.c_str(), ".jpeg");
	syslog(LOG_INFO, "%s(): goruntu basarali bir sekilde alindi ve kaydedildi: '%s'.", __func__, goruntu_ismi);
	mosq_mesaj_gonder(mosq, goruntu_ismi, mq_topic, &mq_baglantisi_kur);
	sprintf(goruntu_ismi, "%s%s%s%s", goruntu_kayit_dizini, "/goruntu_", temp.c_str(), ".jpeg");
	imwrite(goruntu_ismi, goruntu_RGB);
}

#endif