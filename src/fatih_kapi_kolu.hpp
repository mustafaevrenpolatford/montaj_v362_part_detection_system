#include <opencv2/opencv.hpp>

string goruntuyu_isle(cv::Mat goruntu)
{
	

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
		
	}


	cvtColor(CamFrame_RGB, CamFrame_Gry_crop_1, COLOR_BGR2GRAY);

	Rect RectangleToDraw(x2_baslangic, y2_baslangic, 10, 10);
	rectangle(CamFrame_RGB_crop_1, RectangleToDraw.tl(), RectangleToDraw.br(), Scalar(0, 0, 255), 2, 8, 0);

	Rect RectangleToDraw2(x1_baslangic, y1_baslangic, 10, 10);
	rectangle(CamFrame_RGB_crop_1, RectangleToDraw2.tl(), RectangleToDraw2.br(), Scalar(0, 0, 255), 2, 8, 0);


	Mat sutun1(CamFrame_Gry_crop_1.rows, 1, CV_8UC1);
	Mat sutun2(CamFrame_Gry_crop_1.rows, 1, CV_8UC1);

	for (int r = y1_baslangic; r < y1_bitis; ++r)
	{
		for (int c = x1_baslangic; c < x1_bitis; ++c)
		{
			sutun1.at<unsigned char>(r) = CamFrame_Gry_crop_1.at<unsigned char>(r, c);
			toplam1 = toplam1 + CamFrame_Gry_crop_1.at<unsigned char>(r, c);
		}
	}
	ortalama1 = toplam1 / ((x1_bitis - x1_baslangic) * (y1_bitis - y1_baslangic));
	
	for (int r = y2_baslangic; r < y2_bitis; ++r)
	{
		for (int c = x2_baslangic; c < x2_bitis; ++c)
		{
			sutun2.at<unsigned char>(r) = CamFrame_Gry_crop_1.at<unsigned char>(r, c);
			toplam2 = toplam2 + CamFrame_Gry_crop_1.at<unsigned char>(r, c);
			
		}
	}
	ortalama2 = toplam2 / ((x2_bitis - x2_baslangic) * (y2_bitis - y2_baslangic));
	
	syslog(LOG_INFO, "%s(): Ortalamalar: Ortalama1= [%d],  Ortalama2= [%d].", __func__, ortalama1,ortalama2);
	if (abs(ortalama1 - ortalama2) < 20)
	{
	
		myText = "Ayni";
	}
	else
	{
		
		myText = "Farkli";
	}
	
	toplam1 = 0;
	toplam2 = 0;
	return myText;
}