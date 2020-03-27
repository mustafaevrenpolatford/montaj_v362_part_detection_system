#include"includelar.hpp"
#include"goruntuyu_isle.hpp"
#include"kameradan_goruntu_al.hpp"
#include"goruntu_kaydet.hpp"
#include"baglanti_kur.hpp"

using namespace std;
using namespace cv;

#define pvs_Server_PORT 3334

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

/// Server baglantisi ve pvsten data okuma kodlari baslangic///

    syslog(LOG_INFO, "%s(): -- ana dongu -- , pvs datasi bekleniyor..", __func__);
    conn_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
	if (conn_fd < 0)
	{
		syslog(LOG_INFO, "%s(): accept hatasi.", __func__);
		return -1;
	}

	valread = read(conn_fd, buffer_server, 1024);// read fonksiyonu pvsten gelen bilgileri buffer_server degiskenine yazar
    syslog(LOG_INFO, "%s(): PVS gelen data: '%s' , valread: '%d'", __func__, buffer_server, valread);// pvsten gelen data sistem log kayıtlarına yazılır

/// Server baglantisi ve pvsten data okuma kodlari bitis///

///Pvsten gelen kodlarin ayristirilmasi ve pvse acknowledge gonderme kodlari baslangic///

    const char ayrac = ':';
    pvs_degerler = str_split(buffer_server, ayrac);//pvsten gelen veriler "pvs_degerler" arrayine yazılır
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


///Kameraya baglanip goruntu alma ve kontrol etme kodlari baslangic///

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

///Kameraya baglanip goruntu alma ve kontrol etme kodlari bitis///

    string dosya_yolu_main=goruntu_kaydet(goruntu_RGB);//matris bu fonksiyona gonderilir ve fotografin kayit adresi return edilir

    string goruntu_isleme_sonuc_on_kapi=goruntuyu_isle(dosya_yolu_main,1606,234,1606,288);// on kapi icin goruntu isleme

    string goruntu_isleme_sonuc_yan_cita=goruntuyu_isle(dosya_yolu_main,1453,686,1426,686);// yan cita icin goruntu isleme



    cout << "Main_sonuc_on_kapi:" << goruntu_isleme_sonuc_on_kapi << endl;
    cout << "Main_sonuc_yan_cita:" << goruntu_isleme_sonuc_yan_cita << endl;


}

}
