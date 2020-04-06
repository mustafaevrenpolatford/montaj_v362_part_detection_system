#ifndef _MOSQ_MESAJ_GONDER_
#define _MOSQ_MESAJ_GONDER_
#include "includelar.hpp"

void mosq_mesaj_gonder(struct mosquitto *mosq, char *mesaj, char *topic, bool *mq_baglantisi_kur)
{
	int hata = mosquitto_publish(mosq, NULL, topic, strlen(mesaj), mesaj, 0, true);
	if (hata)
	{
		syslog(LOG_INFO, "mosquitto'ya mesaj gonderilemedi!: %s", mesaj);
		*mq_baglantisi_kur = 1;
	}
	//  We need a short delay here, to prevent the Mosquito library being
	//  torn down by the operating system before all the network operations
	//  are finished.
	usleep(1000); // cpu
}
#endif