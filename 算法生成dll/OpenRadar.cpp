#include "OpenRadar.h"
#define pi 3.141592653

OpenRadar::OpenRadar(void)
{
}
OpenRadar::~OpenRadar(void)
{
}
bool OpenRadar::RadarRead(char *fileName){
    FILE* fp = NULL;
    int rho = 0;
    double theta;
    double deltaTeta;
#ifdef UTM30LX
       theta = (135.0 + 90)*pi/180;
       deltaTeta = -0.25*pi/180;
#elif defined URG04LX
    theta = (120.0 + 90)*pi/180;
    deltaTeta = -0.36*pi/180;
#endif

    int totalCnt = 0;
    fp = fopen(fileName,"r");
    if (fp == NULL)
    {
        //cout<<"failed to read"<<endl;
        return false;
    }else {
        //cout<<"successed to read"<<endl;
        RadarRho.clear();
        RadarTheta.clear();

        while(!feof(fp))
        {

            fscanf_s(fp, "%d, ", &rho);
            RadarRho.push_back(rho);
            RadarTheta.push_back(theta);
            theta += deltaTeta;
            //printf("%d  ", rho);
        }
        //cout<<"Total Count: "<<RadarRho.size()<<endl;
        //
    }
    fclose(fp);
    return true;
}

void OpenRadar::CreateRadarImage(IplImage* RadarImage,vector<int>& RadarRho,vector<double>& RadarTheta){
    //RadarImage = cvCreateImage(cvSize(RadarImageWdith,RadarImageHeight),IPL_DEPTH_8U,1);
    cvZero(RadarImage);
    //�����ļ���һ��Բ��
    //int halfWidth  = RadarImageWdith/2;
    //int halfHeight = RadarImageHeight/2;
    int dx =  RadarImageWdith/2;
    int dy =  RadarImageHeight*3/4;

    cvCircle(RadarImage, cvPoint(dx,dy),3, CV_RGB(0,255,255), -1, 8,0);

    int x,y;
    double theta,rho;
    unsigned char * pPixel = 0;
    


    //��ɫ
    int colorIndex = 0,colorRGB;
    int R = 255,G = 0,B = 0;
    int pointCnt = 0;
  

    for (int i = 0; i < RadarRho.size();i++)
    {
        //theta = (pointCnt/4.0 - 45)*pi/180;
        theta = RadarTheta.at(i);
        rho = RadarRho.at(i);
        if (rho < 0)
        {
           //�״����ݶϵ��־
            colorRGB = usualColor[colorIndex];
            R = colorRGB/65536;
            G = (colorRGB%65536)/256;
            B = colorRGB%256; 
            colorIndex = (colorIndex + 1)%10;
        }else {
            pointCnt++;
        }

        x = (int)(rho*cos(theta)/10) + dx;
        y = (int)(-rho*sin(theta)/10)+ dy;

        if (x >= 0 && x < RadarImageWdith && y >= 0 && y < RadarImageHeight)
        {
            pPixel = (unsigned char*)RadarImage->imageData + y*RadarImage->widthStep + 3*x;
            pPixel[0] = B;
            pPixel[1] = G;
            pPixel[2] = R;
        }else{
            //cout<<"x: "<<x<<"  y: "<<y<<endl;
        }
    }

}

int OpenRadar::BreakRadarRho(){
    int breakCnt = 0;
    int rho = 0;
    
    int lastRho = RadarRho.at(0);
    double theta = RadarTheta.at(0);
    int dis = 0;
    int Dmax = 50;

    BreakedRadarRho.clear();
    BreakedRadarTheta.clear();

    BreakedRadarRho.push_back(lastRho);
    BreakedRadarTheta.push_back(theta);

    for (int i = 1; i< RadarRho.size();i++)
    {
       rho = RadarRho.at(i);
       theta = RadarTheta.at(i);

       BreakedRadarRho.push_back(rho);
       BreakedRadarTheta.push_back(theta);
       dis = abs(rho - lastRho);
       if (dis < Dmax)
       {
          
       }else {
          BreakedRadarRho.push_back(-1);
          BreakedRadarTheta.push_back(1000.0);
          breakCnt++;
       }

       lastRho = rho;
    }
    BreakedRadarRho.push_back(-1);
    BreakedRadarTheta.push_back(1000.0);
    //cout<<"breakCnt: "<<breakCnt<<endl;
    return breakCnt;
}

// ���ж������ϣ� Points : �����ϵĵ�      n -- ��������Ŀ  Eps -- ��Ͼ���
// ����ֵ�� ������������Ҫ�ֶΣ��򷵻طֶε��ڸ����������е����������򣬷��� 0 ��ʾ����Ҫ�ֶ�
// �����������㷨���㸴��������һ���ط�
// Ϊ����߳�������Ч�ʣ��Ե㵽ֱ�ߵľ��������иĽ���
// ���������е�ֱ�����ɵ����еĵ������
// Ϊ�˼���㵽ֱ�ߵľ��룬
// ��������ϵ��ת����ֱ����ת��x�᷽�������㵽ֱ�ߵľ��뼴Ϊ������
// ��������ת���yֵ�ľ���ֵ
// ͬʱ��������ת�����ڸô�������Ϊ��ֵ��ֻ��һ�μ��㣬����Ҫ��εĿ��������Ǽ���
int OpenRadar::PolyContourFit( int* X, int* Y, int n , float Eps ) // ���������㣬�ö������ϸ�������    
{
    double dis = sqrt((double)(((X[0] - X[n - 1])*(X[0] - X[n - 1])) +  
                     ((Y[0] - Y[n - 1])* (Y[0] - Y[n - 1]))));
    double cosTheta = (X[n- 1] - X[0]) / dis;
    double sinTheta = - ( Y[n- 1] - Y[0] )/dis;
    double MaxDis = 0;
    int i ;
    int MaxDisInd = -1;
    double dbDis;
    for(i = 1 ; i < n - 1 ; i++)
    {
        // ����������ת������ת��ĵ㵽x��ľ���
        dbDis = abs( (Y[i] - Y[0]) * cosTheta + (X[i] - X[0])* sinTheta);
        if( dbDis > MaxDis)
        {
            MaxDis = dbDis;
            MaxDisInd = i;
        }
    }
    if(MaxDis > Eps)
    {
        return MaxDisInd;
        //        cout << "Line 1 : " << endl;
        //        cout << "Start :" << Points[0].x << "  " << Points[0].y  << " --- " << Points[MaxDisInd].x << "  " << Points[MaxDisInd].y << endl;
        //        cout << "�Ƕȣ� "<<180 * atan2(Points[0].y - Points[MaxDisInd].y , Points[0].x - Points[MaxDisInd].x ) / 3.1415926;
        //        cout << "Line 2 :" << endl;
        //        cout << "Start :" << Points[MaxDisInd].x << "  " << Points[MaxDisInd].y  << " --- " << Points[n - 1].x << "  " << Points[n - 1].y << endl;
        //        cout << "�Ƕȣ� "<< 180 * atan2(Points[n - 1].y - Points[MaxDisInd].y , Points[n - 1].x - Points[MaxDisInd].x ) / 3.1415926;
    }
    //    else{
    //        cout << "Line 1 : " << endl;
    //        cout << "Start :" << Points[0].x << "  " << Points[0].y  << " --- " << Points[n - 1].x << "  " << Points[n - 1].y << endl;
    //        cout << "�Ƕȣ� "<<180 * atan2(Points[n - 1].y - Points[0].y , Points[n - 1].x - Points[0].x ) / 3.1415926;

    //    }
    return 0;
}

//�����߲������
int OpenRadar::BreakPolyLine(){
    int rho = 0;
    double theta = 0.0;
    int X[1200] = {0};
    int Y[1200] = {0};
    int rhoCopy[1200] = {0};
    double thetaCopy[1200] = {0};
    int pointCnt = 0;
    int lineCnt = 0;
    int N = 0;
    SepRadarRho.clear();
    SepRadarTheta.clear();

    for (int i = 0; i < BreakedRadarRho.size();i++)
    {
        rho = BreakedRadarRho.at(i);
        theta = BreakedRadarTheta.at(i);

        if (rho < 0)
        {
            if (pointCnt > 200)//��Ŀ�Ƚ��ٵĵ�ֱ������
            {
                N = PolyContourFit(X , Y, pointCnt, 1000);
                //cout<<"N: "<<N<<endl;
                //cout<<"pointCnt: "<<pointCnt<<endl;
                //�������ֱ�ߣ���Ҫ����µĶϵ�
                if (N == 0 )
                {
                    lineCnt++;
                    //cout<<"Line"<<endl;
                    for (int j = 0; j < pointCnt;j++)
                    {
                       
                       SepRadarRho.push_back(rhoCopy[j]);
                       SepRadarTheta.push_back(thetaCopy[j]);
                    }
                    SepRadarRho.push_back(-1);
                    SepRadarTheta.push_back(1000.0);

                }else if (N > 0)
                {
                    lineCnt += 2;

                    for (int j = 0; j < N;j++)
                    {
                        SepRadarRho.push_back(rhoCopy[j]);
                        SepRadarTheta.push_back(thetaCopy[j]);
                    }
                    SepRadarRho.push_back(-1);
                    SepRadarTheta.push_back(1000.0);

                    for (int j = N; j< pointCnt;j++)
                    {
                        SepRadarRho.push_back(rhoCopy[j]);
                        SepRadarTheta.push_back(thetaCopy[j]);
                    }
                    SepRadarRho.push_back(-1);
                    SepRadarTheta.push_back(1000.0);
                }   
            }
            pointCnt = 0;
            continue;
        }
        X[pointCnt] = rho*cos(theta);
        Y[pointCnt] = rho*sin(theta);
        rhoCopy[pointCnt]   = rho;
        thetaCopy[pointCnt] = theta;
        pointCnt++;
    }
    //cout<<"lineCnt: "<<lineCnt<<endl;
    return lineCnt;
}


void OpenRadar::FitLine(vector<LinePara>& FittedLine,vector<int>& RadarRho,vector<double>& RadarTheta){

    int rho = 0;
    double theta = 0.0;
    int X[1200] = {0};
    int Y[1200] = {0};
    int pointCnt = 0;
    LinePara tmpLinePara;
    FittedLine.clear();
    for (int i = 0 ; i < RadarRho.size();i++)
    {
        rho = RadarRho.at(i);
        theta = RadarTheta.at(i);

        if (rho < 0)
        {
            WeightedFit(X ,Y ,pointCnt,&tmpLinePara);
            FittedLine.push_back(tmpLinePara);
            pointCnt = 0;
            continue;
        }

        X[pointCnt] = rho*cos(theta);
        Y[pointCnt] = rho*sin(theta);
        pointCnt++;
    }
    for (int i = 0; i < FittedLine.size();i++)
    {
        cout<<"a: "<<FittedLine.at(i).a<<"  b: "<<FittedLine.at(i).b<<" ";
        cout<<"x1: "<<FittedLine.at(i).startPoint.x<<" "
            <<"y1: "<<FittedLine.at(i).startPoint.y<<" "
            <<"x1: "<<FittedLine.at(i).endPoint.x<<" "
            <<"y1: "<<FittedLine.at(i).endPoint.y<<endl;
    }

}

void OpenRadar::DrawRadarLine(vector<LinePara>& FittedLine,IplImage* RadarImage){
    //�����ļ���һ��Բ��
    int dx =  RadarImageWdith/2;
    int dy =  RadarImageHeight*3/4;
    cvCircle(RadarImage, cvPoint(dx,dy),3, CV_RGB(0,255,255), -1, 8,0);
    int x1,y1,x2,y2;
    //��ɫ
    int colorIndex = 0,colorRGB;
    int R = 255,G = 0,B = 0;
    for (int i = 0; i < FittedLine.size();i++)
    {
        //�״����ݶϵ��־
        colorRGB = usualColor[colorIndex];
        R = colorRGB/65536;
        G = (colorRGB%65536)/256;
        B = colorRGB%256; 
        colorIndex = (colorIndex + 1)%10;

        x1 = FittedLine.at(i).startPoint.x;
        y1 = FittedLine.at(i).startPoint.y;

        x2 = FittedLine.at(i).endPoint.x;
        y2 = FittedLine.at(i).endPoint.y;

        //ת��ΪRadarͼ����ϵ��
        x1 = x1/10  + dx;
        y1 = -y1/10 + dy;
        x2 = x2/10  + dx;
        y2 = -y2/10 + dy;
        cout<<"x1: "<<x1<<" y1: "<<y1<<" x2: "<<x2<<" y2: "<<y2<<endl;
        cvLine(RadarImage,cvPoint(x2,y2),cvPoint(x1,y1),CV_RGB(R,G,B),2,8,0);
    }
}