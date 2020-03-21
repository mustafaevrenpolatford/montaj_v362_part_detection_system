#include"includelar.h"
#include"baglanti_kur.h"
#include"goruntu_kaydet.h"
#include"mosq_mesaj_gonder.h"
#include"kameradan_goruntu_al.h"
#include"mosq_baslat.h"
#include"goruntuyu_isle.h"
#include"str_split.h"
#include"server_kur.h"


using namespace cv;
using namespace std;


int counter = 0;
int cita_sonucu = 0;

cv::VideoCapture capture;

int main(int argc, char const *argv[])
{
	char **degerler;
	int sonuc;
	int ack = (char)6;
	string goruntu_ismi;

	openlog(argv[0], LOG_PID, LOG_USER);
	syslog(LOG_INFO, "%s uygulamasi basladi.", argv[0]);

	/*
	 * MQTT bilgileri-degiskenleri
	 */
	syslog(LOG_INFO, "%s(): -- mqtt bilgileri ---", __func__);
	syslog(LOG_INFO, "%s(): mq_sunucu: '%s'", __func__, mq_sunucu);
	syslog(LOG_INFO, "%s(): mq_port: '%s'", __func__, mq_port);
	syslog(LOG_INFO, "%s(): mq_kullanici: '%s'", __func__, mq_kullanici);
	syslog(LOG_INFO, "%s(): mq_sifre: '%s'", __func__, mq_sifre);
	syslog(LOG_INFO, "%s(): mq_id: '%s'", __func__, mq_id);
	syslog(LOG_INFO, "%s(): mq_topic: '%s'", __func__, mq_topic);
	syslog(LOG_INFO, "%s(): -- mqtt bilgileri sonu ---", __func__);

	/*  Server Kur */
	syslog(LOG_INFO, "ack: int: '%d' - char:'%c' \n", ack, ack);
	syslog(LOG_INFO, "pvs Serveri kuruluyor: Server Port:'%d'\n", pvs_Server_PORT);
	struct sockaddr_in address;
	int server_fd, conn_fd;
	int valread = 0;
	char buffer_server[1024] = {0};
	int addrlen = sizeof(address);
	int r,g,b;
	sonuc = server_kur(&server_fd, &address, addrlen, pvs_Server_PORT);
	if (sonuc < 0)
	{
		syslog(LOG_INFO, "Server kurulumu hatasi.\n");
		return -1;
	}
	syslog(LOG_INFO, "Server kuruldu: Server Port: '%d'\n", pvs_Server_PORT);
	std::string sonucText ;
	while (1)
	{
		/*
		 * MQTT baglantisini kur.
		 * Baglanti koptugunda yine kurulur.
		 */
		if (mq_baglantisi_kur)
		{
			syslog(LOG_INFO, "%s(): mq baglantisi kuruluyor: '%s'", __func__, mq_sunucu);
			mosq = mosq_baslat();
			if (mosq)
			{
				syslog(LOG_INFO, "%s(): broker baglantisi '%s' ile kuruldu.", __func__, mq_sunucu);
				mq_baglantisi_kur = 0;
			}
			else
			{
				syslog(LOG_INFO, "%s(): broker baglantisi '%s' ile kurulamadi! Tekrar deneniyor...", __func__, mq_sunucu);
			}
		}

		syslog(LOG_INFO, "%s(): -- ana dongu -- , pvs datasi bekleniyor..", __func__);
		conn_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
		if (conn_fd < 0)
		{
			syslog(LOG_INFO, "%s(): accept hatasi.", __func__);
			return -1;
		}

		valread = read(conn_fd, buffer_server, 1024);
		syslog(LOG_INFO, "%s(): PVS gelen data: '%s' , valread: '%d'", __func__, buffer_server, valread);

		//gelen datayi "ayrac" a gore ayristir.
		degerler = str_split(buffer_server, ayrac);
		if (degerler == NULL)
		{
			syslog(LOG_INFO, "%s(): degerler==NULL.", __func__);
			send(conn_fd, &ack, 1, 0);
			syslog(LOG_INFO, "%s(): acknowledge: '%c' gonderildi.\n", __func__, ack);
			close(conn_fd);
			free(degerler);
			continue;
		}

		cv::Mat goruntu_RGB = kameradan_goruntu_al();
		if (goruntu_RGB.empty())
		{
			syslog(LOG_INFO, "%s(): kameradan goruntu alinamadi.", __func__);
			send(conn_fd, &ack, 1, 0);
			syslog(LOG_INFO, "%s(): acknowledge: '%c' gonderildi.\n", __func__, ack);
			close(conn_fd);
			free(degerler);
			continue;
		}

		std::string kamera_kontrol_sonucu = goruntuyu_isle(goruntu_RGB);

		syslog(LOG_INFO, "%s(): kamera_kontrol_sonucu: '%s'", __func__, kamera_kontrol_sonucu.c_str());
		if ((kamera_kontrol_sonucu == "Ayni" && strstr(degerler[11], "CITA=RENKLI")) || (kamera_kontrol_sonucu == "Farkli" && !strstr(degerler[11], "CITA=RENKLI")))
		{
			sonucText = "Sus citasi dogru takildi";
			syslog(LOG_INFO, "Sus citasi basarili bir sekilde takildi.***");
			cita_sonucu = 1;
			r = 0;
			g = 255;
			b = 0;
		}
		else
		{
			std::string pvs_cita_bilgisi = degerler[11];
			sonucText = "Sus citasi yanlis takildi. PVS:" + pvs_cita_bilgisi;
			syslog(LOG_INFO, "***Kamera kontrol sonucu ile PVS bilgileri farkli. Lutfen sus citasini kontrol edin.***");
			cita_sonucu = 0;
			r = 255;
			g = 0;
			b = 0;
		}
		cv::circle(goruntu_RGB, cv::Point(60,1040), 40, CV_RGB(r,g,b), -1);
		//cv::rectangle(goruntu_RGB,cv::Point(40,1050),cv::Point(700,1050),CV_RGB(r,g,b),3);
		cv::putText(goruntu_RGB, sonucText, cv::Point(150, 1050), cv::FONT_HERSHEY_DUPLEX, 2.0, CV_RGB(r, g, b), 2);

		goruntu_kaydet(goruntu_RGB);

		//imshow("goruntu_RGB",goruntu_RGB);
		//waitKey(1);

		send(conn_fd, &ack, 1, 0);
		syslog(LOG_INFO, "%s(): acknowledge: '%c' gonderildi.\n", __func__, ack);
		close(conn_fd);
		free(degerler);
		syslog(LOG_INFO, "%s(): PVS_cita_bilgisi: '%s'.", __func__, degerler[11]);
	}

	return 0;
}


















