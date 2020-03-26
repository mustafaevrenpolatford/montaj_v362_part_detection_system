#ifndef _MOSQ_BASLAT_
#define _MOSQ_BASLAT_

#include"includelar.h"

struct mosquitto *mosq_baslat()
{
    struct mosquitto *mosq = NULL;
    bool mq_baglantisi_kur = 1;
    char mq_sunucu[] = "10.20.5.248";
    char mq_port[] = "1883";
    char mq_kullanici[] = "sms_vekil";
    char mq_sifre[] = "sms_vekil_sifre";
    char *mq_id = NULL;
    char mq_topic[] = "montaj_gi";
	struct mosquitto *mosq;

	/*
	 * Mosquitto kutuphanesi yuklenir.
	 */
	mosquitto_lib_init();

	/*
	 * Create a new Mosquito runtime instance with a random client ID,
	 * and no application-specific callback data.
	 */
	mosq = mosquitto_new(mq_id, true, NULL);
	if (!mosq)
	{
		syslog(LOG_INFO, "Mosquitto kutuphanesi yuklenmedi!\n");
		return 0;
	}

	mosquitto_username_pw_set(mosq, mq_kullanici, mq_sifre);

	/*
	 * Establish a connection to the MQTT server. Do not use a keep-alive ping
	 */
	int keepalive = 300;
	int sonuc = mosquitto_connect(mosq, mq_sunucu, atoi(mq_port), keepalive);
	if (sonuc)
	{
		syslog(LOG_INFO, "Mosquitto sunucusuna baglanamadi!\n");
		return 0;
	}

	/*
	 * Specify the function to call when a new message is received
	 */
	// mosquitto_message_callback_set(mosq, mq_mesaji_alindi_cb);
	return mosq;
}

#endif