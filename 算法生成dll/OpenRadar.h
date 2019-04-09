#pragma once
#include <iostream>
#include <cmath>
#include<cv.h>
#include<highgui.h>
using namespace std;
#include <vector>
#include "WeightedFit.h"
#define UTM30LX

static int RadarImageWdith  = 720;
static int RadarImageHeight = 720;
//������ɫ
/*Colour      Red      Green      Blue      ֵ     
    ��ɫ   White    255    255    255    16777215     
    ��ɫ    Red    255    0    0    255     
    ���ɫ    Dark    Red    128    0    0    128     
    ��ɫ    Green    0    255    0    65280     
    ����ɫ    Dark    Green    0    128    0    32768     
    ��ɫ    Blue    0    0    255    16711680       
    �Ϻ�ɫ    Magenta    255    0    255    16711935            
    ����    Dark    Cyan    0    128    128    8421376     
    ��ɫ    Yellow    255    255    0    65535     
    ��ɫ    Brown    128    128    0    32896  
 */
static int usualColor[15] = {16777215,255,128,65280,32768,
                             16711680,16711935,8421376,65535,32896 }; /*<10�ֳ��õ���ɫ*/

class OpenRadar
{
public:
    OpenRadar(void);
    ~OpenRadar(void);

    vector<int>RadarRho;
    vector<int>BreakedRadarRho;

    vector<double>RadarTheta;
    vector<double>BreakedRadarTheta;

    vector<int>SepRadarRho;//�ֶ�֮���Rho,����Ҳ���в��
    vector<double>SepRadarTheta;

    vector<int>RadarX;
    vector<int>RadarY;

    vector<int>BreakIndex;
    vector<LinePara>FittedLine;

    //���ݶ�ȡ
    bool RadarRead(char *fileName);
    void CreateRadarImage(IplImage* RadarImage,vector<int>& RadarRho,vector<double>& RadarTheta);
    //���ݷֶ�
    int BreakRadarRho();//��ֵ���ѡȡ
    int OpenRadar::PolyContourFit(int* X, int* Y, int n , float Eps);
    int BreakPolyLine();//�����߲������
    void FitLine(vector<LinePara>& FittedLine,vector<int>& RadarRho,vector<double>& RadarTheta);//ֱ�����
    void DrawRadarLine(vector<LinePara>& FittedLine,IplImage* RadarImage);
};

