/******************************************
*PROJECT CODE:FIR filter *
*CREATE DATE:4/12/2009 *
*CREAT BY:Zhao Zhengang *
*FUNCTION: speech data filtered *
*MODIFY DATE: *
*DOCUMENT: *
*OTHERS: *
******************************************/
#include <stdio.h> //文件读取操作头文件
#include "WAV2D.h" //wav文件头处理子程序文件
#define length 180 /*语音帧长为180点＝22.5ms@8kHz采样*/
typedef FILE *file;
void filter(short xin[],short xout[],int n,double h[]); /*滤波子程序声明，输入为16bit short型*/

/*21 点滤波器系数*/
static double hlow[21]={0,-0.000309121660436,-0.000940150229959,0.002708957967671,0.010126462877093,-0,-0.038569035519551,-0.042966320505829,0.079417738324859,0.290598949077059,0.399865039338186,0.290598949077059, 0.079417738324859,-0.042966320505829,-0.038569035519551,-0,0.010126462877093,0.002708957967671,-0.000940150229959,-0.000309121660436,0};
static double hhigh[21]={-1.24995293746598e-18,0.00345525288120460,-0.00393358484915531,-0.00722110437359730,0.0201145189915256,8.43718232789539e-18,-0.0517318083030214,0.0506430237108575,0.0855040614772554,-0.296517071230397,0.400813827342926,-0.296517071230397,0.0855040614772554,0.0506430237108575,-0.0517318083030214,8.43718232789539e-18,0.0201145189915256,-0.00722110437359730,-0.00393358484915531,0.00345525288120460,-1.24995293746598e-18}; //从Matlab获得
static double hband[21]={0,1.78087782196817e-19,0.00262558232924251,-4.41908793302974e-18,-0.0282804398069171,1.85008519680256e-17,0.107712762161892,-8.01033146808979e-17,-0.221792011243957,-7.44074136835274e-17,0.279178408915983,-7.44074136835274e-17,-0.221792011243957,-8.01033146808979e-17,0.107712762161892,1.85008519680256e-17,-0.0282804398069171,-4.41908793302974e-18,0.00262558232924251,1.78087782196817e-19,0}; //从Matlab获得
static int x1[length+22]; //定义数据缓存，包括帧对接

/*fir滤波浮点子程序,入口参数h为系数*/
void filter(short xin[],short xout[],int n,double h[])
{
	int i,j;
	double sum;
	for(i=0;i<length;i++) x1[n+i-1]=xin[i]; //数据读入
	for (i=0;i<length;i++) //按帧滤波
	{
		sum=0.0;
		for(j=0;j<n;j++) sum+=h[j]*x1[i-j+n-1]; //卷积运算
		xout[i]= (short)sum;
	}
	for(i=0;i<(n-1);i++)
	x1[n-i-2]=xin[length-1-i]; //帧头处理
}

/*主程序*/
int main( )
{
	struct WAVEFORMA WAVEFORMAT;
	FILE *fp1,*fp2,*fp3,*fp4;
	int i,frame;
	short indata[length],outdata[length];
	fp1=fopen("MARIO.wav","rb"); /*输入语音文件,二进制方式打开*/
	if(fp1==NULL)
	{ printf("\n文件打开失败!\n");
	exit(0);
	}
	fp2=fopen("outspeechlow.wav","wb"); /*低通滤波后语音文件*/
	fp3=fopen("outspeechhigh.wav","wb"); /*高通滤波后语音文件*/
	fp4=fopen("outspeechband.wav","wb"); /*带通滤波后语音文件*/
	frame=0;

	_read(fp1, &WAVEFORMAT, sizeof(WAVEFORMAT));//读wav文件头部，指针定位至数据起始
	while(feof(fp1)==0)
	{
		frame++;
		printf("frame=%d\n",frame);

		for(i=0;i<length;i++)
			indata[i]=getw(fp1); /*取一帧语音数据*/

		filter(indata,outdata,21,hlow); /*调用低通滤波子程序*/
		for(i=0;i<length;i++)
		{
			putw(outdata[i],fp2); /*将滤波后的样值写入文件*/
			//printf("%Lf\n",outdata[i]); /*验证输出数据*/
		}

		filter(indata,outdata,21,hhigh); /*调用高通滤波子程序*/
		for(i=0;i<length;i++)
			putw(outdata[i],fp3);

		filter(indata,outdata,21,hband); /*调用带通滤波子程序*/
		for(i=0;i<length;i++)
			putw(outdata[i],fp4);
	}
	fclose(fp1); /*关闭文件*/
	fclose(fp2);
	fclose(fp3);
	fclose(fp4);
	return (0);
}

/*
#definelength 180
void filter(shortxin[ ],shortxout[ ],int n,int h[ ]);
static int h[21]={0,-10,-31,-89,…}; //左移15位
static short x1[length+22];

//fir 滤波定点子程序
void filter(shortxin[ ],shortxout[ ],int n,int h[ ])
{
	int i,j;
	long sum;
	for(i=0;i<length;i++) x1[n+i-1]=xin[i];
	for (i=0;i<length;i++)
	{
		sum=0;
		for(j=0;j<n;j++) sum+=(long)h[j]*x1[i-j+n-1];
		xout[i]=sum>>15;
	}
	for(i=0;i<(n-1);i++) x1[n-i-2]=xin[length-i-1];
}
*/