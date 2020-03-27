#ifndef _KONTROL_FONKSIYONU_YAN_CITA_
#define _KONTROL_FONKSIYONU_YAN_CITA_

#include"includelar.hpp"

int kontrol_fonksiyonu_yan_cita(string goruntu_isleme_sonuc_yan_cita,char pvsten_gelen_data)

{
    if ((goruntu_isleme_sonuc_yan_cita== "Ayni" && strstr(pvsten_gelen_data, "CITA=RENKLI")) || 
    (goruntu_isleme_sonuc_yan_cita == "Farkli" && !strstr(pvsten_gelen_data, "CITA=RENKLI")))
    {
        return 1;
    }
    else 
    {
        return 0;
    }
   


}





#endif