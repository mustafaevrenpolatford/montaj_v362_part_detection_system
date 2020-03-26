#ifndef _GORUNTUYU_ISLE_
#define _GORUNTUYU_ISLE_

#include"includelar.hpp"
using namespace cv;
using namespace std;

string goruntuyu_isle(string dosya_yolu, int x1_baslangic, int y1_baslangic, int x2_baslangic, int y2_baslangic )
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


	Mat CamFrame_RGB = imread(dosya_yolu);

	Mat CamFrame_RGB_crop_1(ROI_width_1, ROI_height_1, CV_8UC1);
	Mat CamFrame_Gry_crop_1(ROI_width_1, ROI_height_1, CV_8UC1);

	//Kare icine alinacak bolge tanimlanir//////
	int toplam1 = 0;
	int x1_bitis = x1_baslangic + 90;
	int y1_bitis = y1_baslangic + 40;
	int ortalama1 = 0;

	int toplam2 = 0;
	int x2_bitis = x2_baslangic + 90;
	int y2_bitis = y2_baslangic + 40;
	int ortalama2 = 0;
	//////////////////////////
	
	
	cvtColor(CamFrame_RGB, CamFrame_Gry_crop_1, COLOR_BGR2GRAY);

	

	Rect RectangleToDraw(x2_baslangic, y2_baslangic, 10, 10);
	rectangle(CamFrame_RGB, RectangleToDraw.tl(), RectangleToDraw.br(), Scalar(0, 0, 255), 2, 8, 0);

	Rect RectangleToDraw2(x1_baslangic, y1_baslangic, 10, 10);
	rectangle(CamFrame_RGB, RectangleToDraw2.tl(), RectangleToDraw2.br(), Scalar(0, 0, 255), 2, 8, 0);


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

#endif
