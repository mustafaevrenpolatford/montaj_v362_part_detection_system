#include <mosquitto.h>

struct mosquitto *mosq_baslat(char *mq_sunucu, char *mq_port, char *mq_kullanici, char *mq_sifre, char *mq_id);
int mosq_pub(struct mosquitto *mosq, char topic[], char mesaj[]);
void mosq_bitir(struct mosquitto *mosq);

