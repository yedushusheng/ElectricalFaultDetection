#include "fft.h"
#include <qmath.h>
#include <QDebug>

/*****************************************************************
函数原型：void FFT(double *RealArray,double *ImaArray,int n,int sign)
函数功能：对输入的复数（包含实部和虚部）进行快速傅里叶变换（FFT）
输入参数：RealArray为给定数据实部，ImaArray为给定数据虚部，
         n为傅立叶计算点数，取2的整数次幂，
         sign取1进行傅立叶计算，取-1进行反傅立叶变换
*****************************************************************/
double FFT(double *RealArray,double *ImaArray,int n,int sign)
{
    //qDebug()<<"FFT is used!";

    int i,j,k,l,m,n1,n2;
    double c,c1,e,s,s1,t,tr,ti;
    for(j=1,i=1;i<16;i++)
    {
        m=i;
        j=2*j;
        if(j==n) break;
    }
    n1=n-1;
    for(j=0,i=0;i<n1;i++)
    {
        if(i<j)
        {
            tr=*(RealArray+j);
            ti=*(ImaArray+j);
            *(RealArray+j)=*(RealArray+i);
            *(ImaArray+j)=*(ImaArray+i);
            *(RealArray+i)=tr;
            *(ImaArray+i)=ti;
        }
        k=n/2;
        while(k<(j+1))
        {
            j=j-k;
            k=k/2;
        }
        j=j+k;
    }
    n1=1;
    for(l=1;l<=m;l++)
    {
        n1=2*n1;
        n2=n1/2;
        e=3.14159265359/n2;
        c=1.0;
        s=0.0;
        c1=cos(e);
        s1=-sign*sin(e);
        for(j=0;j<n2;j++)
        {
            for(i=j;i<n;i+=n1)
            {
                k=i+n2;
                tr=c*(*(RealArray+k))-s*(*(ImaArray+k));
                ti=c*(*(ImaArray+k))+s*(*(RealArray+k));
                *(RealArray+k)=*(RealArray+i)-tr;
                *(ImaArray+k)=*(ImaArray+i)-ti;
                *(RealArray+i)=*(RealArray+i)+tr;
                *(ImaArray+i)=*(ImaArray+i)+ti;
            }
            t=c;
            c=c*c1-s*s1;
            s=t*s1+s*c1;
        }
    }

    if(sign==-1)
    {
        for(i=0;i<n;i++)
        {
            *(RealArray+i)/=n;
            *(ImaArray+i)/=n;
        }
    }
    return *RealArray,*ImaArray;
}
