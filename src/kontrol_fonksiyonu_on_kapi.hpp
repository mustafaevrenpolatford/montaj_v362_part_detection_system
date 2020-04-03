#ifndef _KONTROL_FONKSIYONU_ON_KAPI_
#define _KONTROL_FONKSIYONU_ON_KAPI_

#include"includelar.hpp"

int kontrol_fonksiyonu_on_kapi(string goruntu_isleme_sonuc_on_kapi,char *pvsten_gelen_data)

{
int eslesme;
char *ptr;
ptr=strstr(pvsten_gelen_data,"AM51-U22404-CAW");//boyali on kapi acma kolu parcasi
if (ptr!=NULL)//aranan string pvsten gelen datada var ise ptrda bir deger olur
{
eslesme=1;//arac boyali ise
}
else
{
eslesme =0;//arac boyasiz ise
}
    
if ((goruntu_isleme_sonuc_on_kapi == "Ayni" && eslesme==1) || 
    (goruntu_isleme_sonuc_on_kapi == "Farkli" && eslesme==0))
 {
 return 1;
 }
else 
 {
 return 0;
 }


}





#endif
