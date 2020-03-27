#include <cmath>
#include <csignal>
#include <iostream>
#include <sstream>
#include <string>
#include <syslog.h>
#include <time.h>
#include <unistd.h>
#include <vector>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <sys/time.h>
#include <fcntl.h>
#include <opencv2/opencv.hpp>
#include <mosquitto.h>
struct mosquitto *mosq_baslat(char *mq_sunucu, char *mq_port, char *mq_kullanici, char *mq_sifre, char *mq_id);
void mosq_mesaj_gonder(struct mosquitto *mosq, char *mesaj, char *topic, bool *mq_baglantisi_kur);
void mosq_bitir(struct mosquitto *mosq);

using namespace cv;
using namespace std;

#define pvs_Server_PORT 3334

void baglanti_kur(int *sonuc);
std::string goruntuyu_isle(cv::Mat goruntu);
char **str_split(char *a_str, const char a_delim);
int server_kur(int *server_fd, struct sockaddr_in *address_ptr, int addrlen, int PORT);
double what_time_is_it_now();
void goruntu_kaydet(cv::Mat goruntu_RGB);
cv::Mat kameradan_goruntu_al();
struct mosquitto *mosq_baslat(char *mq_sunucu, char *mq_port,char* mq_kullanici, char *mq_sifre, char *mq_id);
void mosq_mesaj_gonder(struct mosquitto *mosq, char *mesaj, char *topic, bool *mq_baglantisi_kur);
void mosq_bitir(struct mosquitto *mosq);

int counter = 0;
int cita_sonucu = 0;
const char ayrac = ':';
const char *pvs_gelen_veri_isimleri[] = {"VIN_NUMBER", "waste", "date", "hour", "minute", "second", "colour_car", "rim", "front_door_handle", "mirror_garnish", "rear_door_handle", "side_moulding"};
const char *video_stream_adresi = "rtsp://admin:Test1234@10.14.37.205:80";
const char *goruntu_kayit_dizini = "/home/gi/montaj_v362/sonuclar";
cv::VideoCapture capture;

struct mosquitto *mosq = NULL;
bool mq_baglantisi_kur = 1;
char mq_sunucu[] = "10.20.5.248";
char mq_port[] = "1883";
char mq_kullanici[] = "sms_vekil";
char mq_sifre[] = "sms_vekil_sifre";
char *mq_id = NULL;
char mq_topic[] = "montaj_gi";

int main(int argc, char const *argv[])
{
	char **pvs_degerler;
	int sonuc;
	int ack = (char)6;
	

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
	
	while (1)
	{
		/*
		 * MQTT baglantisini kur.
		 * Baglanti koptugunda yine kurulur.
		 */
		if (mq_baglantisi_kur)
		{
			syslog(LOG_INFO, "%s(): mq baglantisi kuruluyor: '%s'", __func__, mq_sunucu);
			mosq = mosq_baslat(mq_sunucu, mq_port, mq_kullanici, mq_sifre, mq_id);
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
		pvs_degerler = str_split(buffer_server, ayrac);
		if (pvs_degerler == NULL)
		{
			syslog(LOG_INFO, "%s(): pvs_degerler==NULL.", __func__);
			send(conn_fd, &ack, 1, 0);
			syslog(LOG_INFO, "%s(): acknowledge: '%c' gonderildi.\n", __func__, ack);
			close(conn_fd);
			free(pvs_degerler);
			continue;
		}

		cv::Mat goruntu_RGB = kameradan_goruntu_al();
		if (goruntu_RGB.empty())
		{
			syslog(LOG_INFO, "%s(): kameradan goruntu alinamadi.", __func__);
			send(conn_fd, &ack, 1, 0);
			syslog(LOG_INFO, "%s(): acknowledge: '%c' gonderildi.\n", __func__, ack);
			close(conn_fd);
			free(pvs_degerler);
			continue;
		}

		std::string kamera_kontrol_sonucu = goruntuyu_isle(goruntu_RGB);

		syslog(LOG_INFO, "%s(): kamera_kontrol_sonucu: '%s'", __func__, kamera_kontrol_sonucu.c_str());
		if ((kamera_kontrol_sonucu == "Ayni" && strstr(pvs_degerler[11], "CITA=RENKLI")) || (kamera_kontrol_sonucu == "Farkli" && !strstr(pvs_degerler[11], "CITA=RENKLI")))
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
			std::string pvs_cita_bilgisi = pvs_degerler[11];
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
		free(pvs_degerler);
		syslog(LOG_INFO, "%s(): PVS_cita_bilgisi: '%s'.", __func__, pvs_degerler[11]);
	}

	return 0;
}

void baglanti_kur(int *sonuc)
{
	capture.set(CAP_PROP_BUFFERSIZE, 1);
	int frameRate = capture.get(CAP_PROP_FPS);
	capture.set(CAP_PROP_POS_FRAMES, frameRate);

	if (!capture.open(video_stream_adresi))
	{
		*sonuc = 0;
		return;
	}
	*sonuc = 1;
}

string goruntuyu_isle(cv::Mat goruntu)
{
	// Kameraya gore lisilacak alan(ROI) ayarlanir.
	Rect CamROI_1;
	int ROI_x_1;
	int ROI_y_1;
	int ROI_width_1;
	int ROI_height_1;
	string myText;
	ROI_x_1 = 500;
	ROI_y_1 = 650;
	ROI_width_1 = 300;
	ROI_height_1 = 180;
	CamROI_1 = Rect(ROI_x_1, ROI_y_1, ROI_width_1, ROI_height_1);

	//string full_dizin = goruntu_ismi; // burasi duzelecektir.
	//printf("full_dizin=[%s]\n", full_dizin.c_str());

	int Goruntu_genislik = capture.get(CAP_PROP_FRAME_WIDTH);
	int Goruntu_yukseklik = capture.get(CAP_PROP_FRAME_HEIGHT);

	Mat CamFrame_RGB(Goruntu_genislik, Goruntu_yukseklik, CV_8UC3);
	Mat CamFrame_RGB_crop_1(ROI_width_1, ROI_height_1, CV_8UC1);
	Mat CamFrame_Gry_crop_1(ROI_width_1, ROI_height_1, CV_8UC1);

	//B�lgeler tan�mlar//////
	int toplam1 = 0;
	int x1_baslangic = 1453;
	int x1_bitis = x1_baslangic + 90;
	int y1_baslangic = 686;
	int y1_bitis = y1_baslangic + 40;
	int ortalama1 = 0;

	int toplam2 = 0;
	int x2_baslangic = 1426;
	int x2_bitis = x2_baslangic + 90;
	int y2_baslangic = 610;
	int y2_bitis = y2_baslangic + 40;
	int ortalama2 = 0;
	//////////////////////////
	CamFrame_RGB = goruntu;

	if (CamFrame_RGB.empty())
	{
		syslog(LOG_INFO, "Goruntu okuma hatasi / Network baglanti sorunu\n");
		capture.release();
		sleep(4);
		capture.open(video_stream_adresi);
		syslog(LOG_INFO, "Kamera kapatildi ve kamera acma denendi.");
		//return;
	}

	//CamFrame_RGB_crop_1 = CamFrame_RGB(CamROI_1);
	cvtColor(CamFrame_RGB, CamFrame_Gry_crop_1, COLOR_BGR2GRAY);

	Rect RectangleToDraw(x2_baslangic, y2_baslangic, 10, 10);
	rectangle(CamFrame_RGB_crop_1, RectangleToDraw.tl(), RectangleToDraw.br(), Scalar(0, 0, 255), 2, 8, 0);

	Rect RectangleToDraw2(x1_baslangic, y1_baslangic, 10, 10);
	rectangle(CamFrame_RGB_crop_1, RectangleToDraw2.tl(), RectangleToDraw2.br(), Scalar(0, 0, 255), 2, 8, 0);

	//imwrite("/home/mpolat/montaj_cita_tanima/build/CamFrame_RGB_crop_1.bmp", CamFrame_RGB_crop_1);
	//sleep(1);
	//printf("Kaydedildi.\n");
	//break;

	Mat sutun1(CamFrame_Gry_crop_1.rows, 1, CV_8UC1);
	Mat sutun2(CamFrame_Gry_crop_1.rows, 1, CV_8UC1);

	for (int r = y1_baslangic; r < y1_bitis; ++r)
	{
		for (int c = x1_baslangic; c < x1_bitis; ++c)
		{
			sutun1.at<unsigned char>(r) = CamFrame_Gry_crop_1.at<unsigned char>(r, c);
			toplam1 = toplam1 + CamFrame_Gry_crop_1.at<unsigned char>(r, c);
			//printf("grey.at<unsigned char>(r[%d], c[%d])==[%d]\n",r,c,grey.at<unsigned char>(r, c));
		}
	}
	ortalama1 = toplam1 / ((x1_bitis - x1_baslangic) * (y1_bitis - y1_baslangic));
	//printf("ortalama1[%d]\n", ortalama1);
	//////
	for (int r = y2_baslangic; r < y2_bitis; ++r)
	{
		for (int c = x2_baslangic; c < x2_bitis; ++c)
		{
			sutun2.at<unsigned char>(r) = CamFrame_Gry_crop_1.at<unsigned char>(r, c);
			toplam2 = toplam2 + CamFrame_Gry_crop_1.at<unsigned char>(r, c);
			//printf("grey.at<unsigned char>(r[%d], c[%d])==[%d]\n",r,c,grey.at<unsigned char>(r, c));
		}
	}
	ortalama2 = toplam2 / ((x2_bitis - x2_baslangic) * (y2_bitis - y2_baslangic));
	//printf("ortalama2[%d]\n", ortalama2);
	syslog(LOG_INFO, "%s(): Ortalamalar: Ortalama1= [%d],  Ortalama2= [%d].", __func__, ortalama1,ortalama2);
	if (abs(ortalama1 - ortalama2) < 20)
	{
		//printf("Par�alar Ayn� Tondad�r.\n");
		myText = "Ayni";
	}
	else
	{
		//printf("Par�alar Farkl� Tondad�r.\n");
		//putText(img = CamFrame_Gry_crop_1, text = "Hii", org = (30,30), fontFace = FONT_HERSHEY_DUPLEX, fontScale = 3, color = (0, 255, 0))
		//putText(CamFrame_Gry_crop_1, "Hii", cvPoint(30,30),FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200,200,250), 1, CV_AA);
		myText = "Farkli";
	}
	/*cv::putText(CamFrame_RGB_crop_1, //target image
				myText,				 //text
				cv::Point(30, 30),   //top-left position
				cv::FONT_HERSHEY_DUPLEX,
				1.0,
				CV_RGB(0, 255, 0), //font color
				2);*/
	//imshow("MONTAJ FINAL :)", CamFrame_RGB_crop_1);
	//waitKey(1);

	toplam1 = 0;
	toplam2 = 0;
	return myText;
}

int server_kur(int *server_fd, struct sockaddr_in *address_ptr, int addrlen, int PORT)
{
	int server_fd_temp;
	int opt = 1;
	// Creating socket file descriptor
	server_fd_temp = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd_temp == 0)
	{
		syslog(LOG_INFO, "socket failed\n");
		return -1;
	}

	if (setsockopt(server_fd_temp, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
	{
		syslog(LOG_INFO, "setsockopt\n");
		return -1;
	}

	memset(address_ptr, '0', addrlen);
	address_ptr->sin_family = AF_INET;
	address_ptr->sin_addr.s_addr = INADDR_ANY;
	address_ptr->sin_port = htons(PORT);

	if (bind(server_fd_temp, (struct sockaddr *)address_ptr, addrlen) < 0)
	{
		syslog(LOG_INFO, "bind failed\n");
		return -1;
	}

	if (listen(server_fd_temp, 3) < 0)
	{
		syslog(LOG_INFO, "listen\n");
		return -1;
	}
	*server_fd = server_fd_temp;

	return 0;
}

char **str_split(char *a_str, const char a_delim)
{
	char **result = 0;
	int i;
	int count = 0;
	char *tmp = a_str;
	char *last_comma = 0;
	char delim[2];
	delim[0] = a_delim;
	delim[1] = 0;

	// Count how many elements will be extracted.
	while (*tmp)
	{
		if (a_delim == *tmp)
		{
			count++;
			last_comma = tmp;
		}
		tmp++;
	}

	// Add space for trailing token.
	count += last_comma < (a_str + strlen(a_str) - 1);

	//Add space for terminating null string so caller
	//knows where the list of returned strings ends.
	count++;

	result = (char **)calloc(count, sizeof(char *));

	if (result)
	{
		int idx = 0;
		char *token = strtok(a_str, delim);

		while (token)
		{
			assert(idx < count);
			*(result + idx++) = strdup(token);
			token = strtok(0, delim);
		}
		assert(idx == count - 1);
		*(result + idx) = 0;

		for (i = 0; i < count - 1; ++i)
		{
			syslog(LOG_INFO, "%s(): %s: '%s'", __func__, pvs_gelen_veri_isimleri[i], result[i]);
		}
		return result;
	}

	return NULL;
}


void goruntu_kaydet(cv::Mat goruntu_RGB)
{

	int x1_baslangic = 1453;
	int y1_baslangic = 686;
	int x2_baslangic = 1426;
	int y2_baslangic = 610;

	Rect RectangleToDraw(x2_baslangic, y2_baslangic, 10, 10);
	rectangle(goruntu_RGB, RectangleToDraw.tl(), RectangleToDraw.br(), 
	Scalar(0, 0, 255), 2, 8, 0);

	Rect RectangleToDraw2(x1_baslangic, y1_baslangic, 10, 10);
	rectangle(goruntu_RGB, RectangleToDraw2.tl(), RectangleToDraw2.br(), 
	Scalar(0, 0, 255), 2, 8, 0);


	char goruntu_ismi[256];
	std::string temp = to_string(counter);
	sprintf(goruntu_ismi, "%s%s%s", "goruntu_", temp.c_str(), ".jpeg");
	syslog(LOG_INFO, "%s(): goruntu basarali bir sekilde alindi ve kaydedildi: '%s'.", __func__, goruntu_ismi);
	mosq_mesaj_gonder(mosq, goruntu_ismi, mq_topic, &mq_baglantisi_kur);
	sprintf(goruntu_ismi, "%s%s%s%s", goruntu_kayit_dizini, "/goruntu_", temp.c_str(), ".jpeg");
	imwrite(goruntu_ismi, goruntu_RGB);
}

cv::Mat kameradan_goruntu_al()
{
	int sonuc;
	cv::Mat goruntu_RGB;

	baglanti_kur(&sonuc);
	if (sonuc == 0)
	{
		syslog(LOG_INFO, "%s(): Kamera acilamadi.", __func__);
		return goruntu_RGB;
	}
	syslog(LOG_INFO, "%s(): Kamera acildi.", __func__);

	capture.read(goruntu_RGB);

	if (counter < 10000)
	{
		counter++;
	}
	else
	{
		counter = 0;
	}

	capture.release();
	return goruntu_RGB;
}

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

struct mosquitto *mosq_baslat(char *mq_sunucu, char *mq_port, char *mq_kullanici, char *mq_sifre, char *mq_id)
{
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

void mosq_bitir(struct mosquitto *mosq)
{
	syslog(LOG_INFO, "Mosquitto baglantisi sonlandiriliyor.\n");
	mosquitto_disconnect(mosq);
	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();
}
