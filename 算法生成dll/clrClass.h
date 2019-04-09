#pragma once
#include "OpenRadar.h"

//这是个托管C++类用于对C++本地代码类和函数等进行封装以在C#中使用
public ref class clrClass //必须声明为 public，否则类在程序集中不可见，关键词 ref 表示类是一个托管类，将编译为中间语言
{
public:
	clrClass(void);

	////数据读取
	//bool RadarRead(char *fileName);
	//void CreateRadarImage(IplImage* RadarImage);
	//调用opencv算法生成图片
	void saveImage();

private:
	OpenRadar * openRadar;//建立一个本地类实例 (在构造函数中实例化，这里可以理解为用于在clrClass 中“继承”CClassNative的公共成员)
};