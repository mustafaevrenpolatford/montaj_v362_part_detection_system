#ifndef _KONTROL_FONKSIYONU_YAN_CITA_
#define _KONTROL_FONKSIYONU_YAN_CITA_

#include"includelar.hpp"

int kontrol_fonksiyonu_yan_cita(string goruntu_isleme_sonuc_yan_cita,char *pvsten_gelen_data)

{
int eslesme;
char *ptr;
ptr=strstr(pvsten_gelen_data,"CITA=RENKLI");
if (ptr!=NULL)
{
eslesme=1;
}
else
{
eslesme =0;
}
    
if ((goruntu_isleme_sonuc_yan_cita== "Ayni" && eslesme==1) || 
    (goruntu_isleme_sonuc_yan_cita == "Farkli" && eslesme==0))
 {
 return 1;
 }
else 
 {
 return 0;
 }

   


}





#endif
