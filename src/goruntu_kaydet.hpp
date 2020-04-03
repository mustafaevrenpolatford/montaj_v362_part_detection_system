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

string dosya_yolu_temp1 = dosya_yolu_main + "goruntu_";
string dosya_yolu_temp2 = dosya_yolu_temp1 + temp;
string dosya_yolu_final = dosya_yolu_temp2 + ".jpg";

imwrite(dosya_yolu_final, goruntu_RGB);
printf("Goruntu basariyla kaydedildi...\n");

return dosya_yolu_final;

}


#endif

