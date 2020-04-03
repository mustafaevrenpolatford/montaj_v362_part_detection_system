#ifndef _KONTROL_FONKSIYONU_AYNA_KAPAGI_
#define _KONTROL_FONKSIYONU_AYNA_KAPAGI_

#include"includelar.hpp"

int kontrol_fonksiyonu_ayna_kapagi(string goruntu_isleme_sonuc_ayna_kapagi,char *pvsten_gelen_data)

{
int eslesme;
char *ptr;
ptr=strstr(pvsten_gelen_data,"GOVDE RENGI");//boyali ayna kapagi
if (ptr!=NULL)//eger ayna boyali  ise ptrda bir deger olur
{
eslesme=1;//ayna boyali ise
}
else
{
eslesme =0;//ayna boyasiz ise
}
    
if ((goruntu_isleme_sonuc_ayna_kapagi == "Ayni" && eslesme==1) || 
    (goruntu_isleme_sonuc_ayna_kapagi == "Farkli" && eslesme==0))
 {
 return 1;
 }
else 
 {
 return 0;
 }


}





#endif
