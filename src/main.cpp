#include"includelar.hpp"
#include"goruntuyu_isle.hpp"
#include"kameradan_goruntu_al.hpp"
#include"goruntu_kaydet.hpp"
#include"baglanti_kur.hpp"
#include"mosq_baslat.hpp"
#include"mosq_mesaj_gonder.hpp"
#include"mosq_bitir.hpp"
#include"server_kur.hpp"
#include"str_split.hpp"
#include"kontrol_fonksiyonu_yan_cita.hpp"
#include"kontrol_fonksiyonu_on_kapi.hpp"
#include"kontrol_fonksiyonu_kayar_kapi.hpp"
#include"kontrol_fonksiyonu_ayna_kapagi.hpp"



using namespace std;
using namespace cv;

#define pvs_Server_PORT 3334

///Mq server degiskenleri baslangic///

struct mosquitto *mosq = NULL;
bool mq_baglantisi_kur = 1;
char mq_sunucu[] = "10.20.5.248";
char mq_port[] = "1883";
char mq_kullanici[] = "sms_vekil";
char mq_sifre[] = "sms_vekil_sifre";
char *mq_id = NULL;
char mq_topic[] = "montaj_gi";

///Mq server degiskenleri bitis///


int main(int argc, char const *argv[])
{

    char **pvs_degerler;
	int sonuc;
	int ack = (char)6;
    

printf("***Main program basladi***\n");

    openlog(argv[0], LOG_PID, LOG_USER);
	syslog(LOG_INFO, "%s uygulamasi basladi.", argv[0]);

///Server baglantisi ayarlari baslangic///

    syslog(LOG_INFO, "ack: int: '%d' - char:'%c' \n", ack, ack);
	syslog(LOG_INFO, "pvs Serveri kuruluyor: Server Port:'%d'\n", pvs_Server_PORT);
	struct sockaddr_in address;
	int server_fd, conn_fd;
	int valread = 0;
	char buffer_server[1024] = {0};
	int addrlen = sizeof(address);

/// Server baglantisi ayarlari bitis///


///Pvs server ile baglanti kurma kodlari baslangic///
    sonuc = server_kur(&server_fd, &address, addrlen, pvs_Server_PORT);// PVS ile baglanti kurulur. Eger baglanti kurulamazsa "-1" return edilir. Eger baglanti kurulursa "0" return edilir.
	if (sonuc < 0)
	{
		syslog(LOG_INFO, "Server kurulumu hatasi.\n");
		return -1;
	}
	syslog(LOG_INFO, "Server kuruldu: Server Port: '%d'\n", pvs_Server_PORT);


///Pvs server ile baglanti kurma kodlari bitis///

while (1)
{


///Mqtt baglantisini kuran kod baslangic///

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
///Mqtt baglantisini kuran kod bitis///


/// Server baglantisi ve pvsten data okuma kodlari baslangic///

    syslog(LOG_INFO, "%s(): -- ana dongu -- , pvs datasi bekleniyor..", __func__);
    conn_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
	if (conn_fd < 0)
	{
		syslog(LOG_INFO, "%s(): accept hatasi.", __func__);
		return -1;
	}

	valread = read(conn_fd, buffer_server, 1024);// read fonksiyonu pvsten gelen bilgileri buffer_server degiskenine yazar
    syslog(LOG_INFO, "%s(): PVS gelen data: '%s' , valread: '%d'", __func__, buffer_server, valread);// pvsten gelen data sistem log kayýtlarýna yazýlýr

/// Server baglantisi ve pvsten data okuma kodlari bitis///

///Pvsten gelen kodlarin ayristirilmasi ve pvse acknowledge gonderme kodlari baslangic///

    const char ayrac = ':';
    pvs_degerler = str_split(buffer_server, ayrac);//pvsten gelen veriler "pvs_degerler" arrayine yazýlýr
		if (pvs_degerler == NULL)
		{
			syslog(LOG_INFO, "%s(): pvs_degerler==NULL.", __func__);
			send(conn_fd, &ack, 1, 0);
			syslog(LOG_INFO, "%s(): acknowledge: '%c' gonderildi.\n", __func__, ack);
			close(conn_fd);
			free(pvs_degerler);
			continue;
		}


///Pvsten gelen kodlarin ayristirilmasi ve pvse acknowledge gonderme kodlari bitis///


///Kameraya baglanip goruntu alma ve kamera baglanti kontrol etme kodlari baslangic///

    cv::Mat goruntu_RGB = kameradan_goruntu_al();//kameradan alinan goruntu matrise kaydedilir

    if (goruntu_RGB.empty())
		{
			syslog(LOG_INFO, "%s(): kameradan goruntu alinamadi.", __func__);
			send(conn_fd, &ack, 1, 0);
			syslog(LOG_INFO, "%s(): acknowledge: '%c' gonderildi.\n", __func__, ack);
			close(conn_fd);
			free(pvs_degerler);
			continue;
		}

///Kameraya baglanip goruntu alma ve kamera baglanti kontrol etme kodlari bitis///


    string dosya_yolu_main=goruntu_kaydet(goruntu_RGB);//matris bu fonksiyona gonderilir ve fotografin kayit adresi return edilir

    string goruntu_isleme_sonuc_yan_cita=goruntuyu_isle(dosya_yolu_main,1453,686,1453,616);// yan cita icin goruntu isleme
	int final_sonuc_yan_cita=kontrol_fonksiyonu_yan_cita(goruntu_isleme_sonuc_yan_cita,pvs_degerler[11]);//yan cita icin pvs ve goruntu isleme sonuclarini kontrol etme
	
	string goruntu_isleme_sonuc_ayna_kapagi=goruntuyu_isle(dosya_yolu_main,1033,66,1033,266);// ayna kapagi icin goruntu isleme
	int final_sonuc_ayna_kapagi=kontrol_fonksiyonu_ayna_kapagi(goruntu_isleme_sonuc_ayna_kapagi,pvs_degerler[9]);//ayna kapagi icin pvs ve goruntu isleme sonuclarini kontrol etme

	string goruntu_isleme_sonuc_on_kapi = goruntuyu_isle(dosya_yolu_main,1606,234,1606,288);// on kapi icin goruntu isleme
	int final_sonuc_on_kapi=kontrol_fonksiyonu_on_kapi(goruntu_isleme_sonuc_on_kapi,pvs_degerler[8]);//on kapi icin pvs ve goruntu isleme sonuclarini kontrol etme
   
	if (pvs_degerler[10]!=NULL)//eger arabanin kayar kapisi var ise
	{

	string goruntu_isleme_sonuc_kayar_kapi = goruntuyu_isle(dosya_yolu_main,1751,249,1751,299);// kayar kapi icin goruntu isleme
	int final_sonuc_kayar_kapi=kontrol_fonksiyonu_kayar_kapi(goruntu_isleme_sonuc_kayar_kapi,pvs_degerler[10]);//kayar kapi icin pvs ve goruntu isleme sonuclarini kontrol etme	
	}	

    cout << "Main_sonuc_on_kapi:" << goruntu_isleme_sonuc_on_kapi << endl;
    cout << "Main_sonuc_yan_cita:" << goruntu_isleme_sonuc_yan_cita << endl;


/// Pvse acknowledge gonderme ve baglantiyi kapatma kodu baslangic///

	send(conn_fd, &ack, 1, 0);
	syslog(LOG_INFO, "%s(): acknowledge: '%c' gonderildi.\n", __func__, ack);
	close(conn_fd);
	free(pvs_degerler);

/// Pvse acknowledge gonderme ve baglantiyi kapatma kodu bitis///
}

}