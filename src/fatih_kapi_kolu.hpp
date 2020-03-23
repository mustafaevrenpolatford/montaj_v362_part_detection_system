#include <opencv2/opencv.hpp>

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