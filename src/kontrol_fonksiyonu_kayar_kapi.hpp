#ifndef _KONTROL_FONKSIYONU_KAYAR_KAPI_
#define _KONTROL_FONKSIYONU_KAYAR_KAPI_

#include"includelar.hpp"

int kontrol_fonksiyonu_kayar_kapi(string goruntu_isleme_sonuc_kayar_kapi,char *pvsten_gelen_data)

{
int eslesme;
char *ptr;
ptr=strstr(pvsten_gelen_data,"BK21-V26600-BAW");//boyali kayar kapi acma kolu parcasi
if (ptr!=NULL)//aranan string pvsten geln datada var ise ptrda bir deger olur
{
eslesme=1;//arac boyali ise
}
else
{
eslesme =0;//arac boyasiz ise
}
    
if ((goruntu_isleme_sonuc_kayar_kapi == "Ayni" && eslesme==1) || 
    (goruntu_isleme_sonuc_kayar_kapi == "Farkli" && eslesme==0))
 {
 return 1;
 }
else 
 {
 return 0;
 }


}





#endif
