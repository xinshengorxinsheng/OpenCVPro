#include"clrClass.h"
#include <iostream>
#include <cmath>
#include "io.h"
using namespace std;
const int MAX_POINT_COUNT = 1200;
int Rho[MAX_POINT_COUNT] = { 0 };

clrClass::clrClass(void)
{
	openRadar = new OpenRadar();//����һ��ע��Ҫ��������	
}
void clrClass::saveImage()
{
	OpenRadar openRadar;
	char fileName[32] = "C:\\opencv\\data_0.csv";		//����Ҫ��ȡ�ļ��ľ���·��
	char fileNameImage[32] = "C:\\opencv\\data_0.jpg";	//�����м������ͼƬ����õľ���·��
	int frameCnt = 0;
	char key;
	//���ô˺���ʹ�õ�ͼƬ�ݴ������
	IplImage* RadarImage = cvCreateImage(cvSize(RadarImageWdith, RadarImageHeight), IPL_DEPTH_8U, 3);

	/*cvNamedWindow("Radar", 1);*/
	cvNamedWindow("BreakedRadar", 1);
	//cvNamedWindow("PloyLine",1);

	int lineCnt = 0;

	while (_access(fileName, 0) == 0)			//�жϴ�·���ļ��Ƿ�������Ƿ���Ȩ�޷��ʣ��������ѭ��
	{
		//����opencv��C++ʵ�ֶ���ļ����ݵļ��أ���������Ҫ���ļ�ͳһ�����������ļ������һ�������֣���δʵ�ֶ��ַ��ļ���ͼ��ļ��ء�
		sprintf_s(fileName, "C:\\opencv\\data_%d.csv", frameCnt);
		openRadar.RadarRead(fileName);          //���ļ�����ȡ����

		//openRadar.CreateRadarImage(RadarImage, openRadar.RadarRho, openRadar.RadarTheta);	//��ͼƬ���������ͼ������׼����ͼ
		//cvNamedWindow("Radar", 1);				//�Դ򿪵Ĵ�������
		//cvShowImage("Radar", RadarImage);		//չʾ�����ͼƬ

		openRadar.BreakRadarRho();
		openRadar.CreateRadarImage(RadarImage, openRadar.BreakedRadarRho, openRadar.BreakedRadarTheta);
		lineCnt = openRadar.BreakPolyLine();
		cout << "lineCnt: " << lineCnt << endl;

		openRadar.CreateRadarImage(RadarImage, openRadar.SepRadarRho, openRadar.SepRadarTheta);
		//cvShowImage("BreakedRadar", RadarImage);


		openRadar.FitLine(openRadar.FittedLine, openRadar.SepRadarRho, openRadar.SepRadarTheta);
		openRadar.DrawRadarLine(openRadar.FittedLine, RadarImage);
		//cvShowImage("BreakedRadar", RadarImage);

		sprintf_s(fileNameImage, "C:\\opencv\\data_%d.jpg", frameCnt);
		cvSaveImage(fileNameImage, RadarImage);
		key = cvWaitKey(30);					//ÿ30msɨ��һ�μ���
		if (key == 27)							//��ESC���˳������ټ�����ȡ�ļ���ʾͼƬ
		{
			break;
		}
		frameCnt++;
	}
	getchar();
	cvReleaseImage(&RadarImage);				//����ڴ�
	//cvDestroyWindow("Radar");			//����һ������
	cvDestroyWindow("BreakedRadar");					//���ٴ���
}