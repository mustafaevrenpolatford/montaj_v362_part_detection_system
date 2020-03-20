#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>

#include "mosq.h"


void mq_mesaji_alindi_cb(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message);


struct mosquitto *mosq_baslat(char *mq_sunucu, char *mq_port, char *mq_kullanici, char *mq_sifre, char *mq_id)
{
    struct mosquitto *mosq;
    int keepalive=300;

    // Initialize the Mosquitto library
    mosquitto_lib_init();

    // Create a new Mosquito runtime instance with a random client ID,
    //  and no application-specific callback data.
    mosq=mosquitto_new(NULL, true, NULL);
    if(!mosq)
    {
        printf( "Can't initialize Mosquitto library!\n");
        return 0;
    }

    mosquitto_username_pw_set(mosq, mq_kullanici, mq_sifre);

    // Establish a connection to the MQTT server. Do not use a keep-alive ping
    int ret = mosquitto_connect(mosq, mq_sunucu, atoi(mq_port), keepalive);
    if(ret)
    {
        printf( "Can't connect to Mosquitto server!\n");
        return 0;
    }

    // Specify the function to call when a new message is received
    mosquitto_message_callback_set(mosq, mq_mesaji_alindi_cb);

    return mosq;
}


void mosq_bitir(struct mosquitto *mosq)
{
  // Tidy up
  mosquitto_disconnect(mosq);
  mosquitto_destroy(mosq);
  mosquitto_lib_cleanup();
}


int mosq_pub(struct mosquitto *mosq, char topic[], char mesaj[])
{
    int ret;

    // Publish the message to the topic
    ret = mosquitto_publish(mosq, NULL, topic, strlen(mesaj), mesaj, 0, false);
    if(ret)
    {
      printf( "Can't publish to Mosquitto server!\n");
      return 1;
    }

  // We need a short delay here, to prevent the Mosquito library being
  //  torn down by the operating system before all the network operations
  //  are finished.
  return 0;
}
