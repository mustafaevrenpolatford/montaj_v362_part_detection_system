#ifndef _MOSQ_BASLAT_
#define _MOSQ_BASLAT_
#include "includelar.hpp"

struct mosquitto *mosq_baslat(char *mq_sunucu, char *mq_port, char *mq_kullanici, char *mq_sifre, char *mq_id)
{
///mqtt bilgileri system loguna yazan kod baslangic///
    syslog(LOG_INFO, "%s(): -- mqtt bilgileri ---", __func__);
	syslog(LOG_INFO, "%s(): mq_sunucu: '%s'", __func__, mq_sunucu);
	syslog(LOG_INFO, "%s(): mq_port: '%s'", __func__, mq_port);
	syslog(LOG_INFO, "%s(): mq_kullanici: '%s'", __func__, mq_kullanici);
	syslog(LOG_INFO, "%s(): mq_sifre: '%s'", __func__, mq_sifre);
	syslog(LOG_INFO, "%s(): mq_id: '%s'", __func__, mq_id);
	syslog(LOG_INFO, "%s(): mq_topic: '%s'", __func__, mq_topic);
	syslog(LOG_INFO, "%s(): -- mqtt bilgileri sonu ---", __func__);
///mqtt bilgileri system loguna yazan kod bitis///

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