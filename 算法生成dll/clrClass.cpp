#include"clrClass.h"
#include <iostream>
#include <cmath>
#include "io.h"
using namespace std;
const int MAX_POINT_COUNT = 1200;
int Rho[MAX_POINT_COUNT] = { 0 };

clrClass::clrClass(void)
{
	openRadar = new OpenRadar();//这里一定注意要创建对象！	
}
void clrClass::saveImage()
{
	OpenRadar openRadar;
	char fileName[32] = "C:\\opencv\\data_0.csv";		//保存要读取文件的绝对路径
	char fileNameImage[32] = "C:\\opencv\\data_0.jpg";	//保存中间产生的图片到设好的绝对路径
	int frameCnt = 0;
	char key;
	//调用此函数使得到图片暂存的容器
	IplImage* RadarImage = cvCreateImage(cvSize(RadarImageWdith, RadarImageHeight), IPL_DEPTH_8U, 3);

	/*cvNamedWindow("Radar", 1);*/
	cvNamedWindow("BreakedRadar", 1);
	//cvNamedWindow("PloyLine",1);

	int lineCnt = 0;

	while (_access(fileName, 0) == 0)			//判断此路径文件是否存在且是否有权限访问，是则进入循环
	{
		//基于opencv和C++实现多个文件数据的加载，程序中需要将文件统一命名，而且文件名最后一定是数字，还未实现对字符文件名图像的加载。
		sprintf_s(fileName, "C:\\opencv\\data_%d.csv", frameCnt);
		openRadar.RadarRead(fileName);          //打开文件并读取数据

		//openRadar.CreateRadarImage(RadarImage, openRadar.RadarRho, openRadar.RadarTheta);	//将图片容器传入绘图函数，准备绘图
		//cvNamedWindow("Radar", 1);				//对打开的窗口命名
		//cvShowImage("Radar", RadarImage);		//展示出这幅图片

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
		key = cvWaitKey(30);					//每30ms扫描一次键盘
		if (key == 27)							//按ESC键退出，不再继续读取文件显示图片
		{
			break;
		}
		frameCnt++;
	}
	getchar();
	cvReleaseImage(&RadarImage);				//清空内存
	//cvDestroyWindow("Radar");			//销毁一个窗口
	cvDestroyWindow("BreakedRadar");					//销毁窗口
}