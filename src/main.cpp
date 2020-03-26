#include"includelar.hpp"
#include"goruntuyu_isle.hpp"
#include"kameradan_goruntu_al.hpp"
#include"goruntu_kaydet.hpp"
#include"baglanti_kur.hpp"

using namespace std;
using namespace cv;

int main(int argc, char const *argv[])
{

printf("***Main program basladi***\n");

// Server baglantisi ayarlari


cv::Mat goruntu_RGB = kameradan_goruntu_al();//kameradan alinan goruntu matrise kaydedilir

string dosya_yolu_main=goruntu_kaydet(goruntu_RGB);//matris bu fonksiyona gonderilir ve fotografin kayit adresi return edilir

string goruntu_isleme_sonuc_on_kapi=goruntuyu_isle(dosya_yolu_main,1606,234,1606,288);// on kapi icin goruntu isleme

string goruntu_isleme_sonuc_yan_cita=goruntuyu_isle(dosya_yolu_main,1453,686,1426,686);// yan cita icin goruntu isleme



cout << "Main_sonuc_on_kapi:" << goruntu_isleme_sonuc_on_kapi << endl;
cout << "Main_sonuc_yan_cita:" << goruntu_isleme_sonuc_yan_cita << endl;


}
