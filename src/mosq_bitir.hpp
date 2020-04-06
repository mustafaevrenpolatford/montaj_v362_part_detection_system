#ifndef _MOSQ_BITIR_
#define _MOSQ_BITIR_
#include "includelar.hpp"

void mosq_bitir(struct mosquitto *mosq)
{
	syslog(LOG_INFO, "Mosquitto baglantisi sonlandiriliyor.\n");
	mosquitto_disconnect(mosq);
	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();
}
#endif