#pragma once
#include "OpenRadar.h"

//���Ǹ��й�C++�����ڶ�C++���ش�����ͺ����Ƚ��з�װ����C#��ʹ��
public ref class clrClass //��������Ϊ public���������ڳ����в��ɼ����ؼ��� ref ��ʾ����һ���й��࣬������Ϊ�м�����
{
public:
	clrClass(void);

	////���ݶ�ȡ
	//bool RadarRead(char *fileName);
	//void CreateRadarImage(IplImage* RadarImage);
	//����opencv�㷨����ͼƬ
	void saveImage();

private:
	OpenRadar * openRadar;//����һ��������ʵ�� (�ڹ��캯����ʵ����������������Ϊ������clrClass �С��̳С�CClassNative�Ĺ�����Ա)
};