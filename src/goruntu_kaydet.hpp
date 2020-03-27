#ifndef _GORUNTU_KAYDET_
#define _GORUNTU_KAYDET_

#include "includelar.hpp"
int counter;
string dosya_yolu_main = "/home/gi/fatih_calisma/kaydedilen_goruntuler/";

string goruntu_kaydet(cv::Mat goruntu_RGB)
{

if (counter < 10000)
	{
		counter++;
	}
	else
	{
		counter = 0;
	}


std::string temp = to_string(counter);


///kaydedilen fotografın ismini mq servera yollama kodu baslangic///

char goruntu_ismi[256];
sprintf(goruntu_ismi, "%s%s%s", "goruntu_", temp.c_str(), ".jpg");
mosq_mesaj_gonder(mosq, goruntu_ismi, mq_topic, &mq_baglantisi_kur);

///kaydedilen fotografın ismini mq servera yollama kodu bitis///

///return edilecek dosya yolunu olusturan kod baslangic///
string dosya_yolu_temp1 = dosya_yolu_main + "goruntu_";
string dosya_yolu_temp2 = dosya_yolu_temp1 + temp;
string dosya_yolu_final = dosya_yolu_temp2 + ".jpg";

///return edilecek dosya yolunu olusturan kod bitis///

imwrite(dosya_yolu_final, goruntu_RGB);
printf("Goruntu basariyla kaydedildi...\n");

return dosya_yolu_final;

}


#endif

