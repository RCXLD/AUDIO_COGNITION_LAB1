/******************************************
*PROJECT CODE:FIR filter *
*CREATE DATE:4/12/2009 *
*CREAT BY:Zhao Zhengang *
*FUNCTION: speech data filtered *
*MODIFY DATE: *
*DOCUMENT: *
*OTHERS: *
******************************************/
#include <stdio.h> //�ļ���ȡ����ͷ�ļ�
#include "WAV2D.h" //wav�ļ�ͷ�����ӳ����ļ�
#define length 180 /*����֡��Ϊ180�㣽22.5ms@8kHz����*/
typedef FILE *file;
void filter(short xin[],short xout[],int n,double h[]); /*�˲��ӳ�������������Ϊ16bit short��*/

/*21 ���˲���ϵ��*/
static double hlow[21]={0,-0.000309121660436,-0.000940150229959,0.002708957967671,0.010126462877093,-0,-0.038569035519551,-0.042966320505829,0.079417738324859,0.290598949077059,0.399865039338186,0.290598949077059, 0.079417738324859,-0.042966320505829,-0.038569035519551,-0,0.010126462877093,0.002708957967671,-0.000940150229959,-0.000309121660436,0};
static double hhigh[21]={-1.24995293746598e-18,0.00345525288120460,-0.00393358484915531,-0.00722110437359730,0.0201145189915256,8.43718232789539e-18,-0.0517318083030214,0.0506430237108575,0.0855040614772554,-0.296517071230397,0.400813827342926,-0.296517071230397,0.0855040614772554,0.0506430237108575,-0.0517318083030214,8.43718232789539e-18,0.0201145189915256,-0.00722110437359730,-0.00393358484915531,0.00345525288120460,-1.24995293746598e-18}; //��Matlab���
static double hband[21]={0,1.78087782196817e-19,0.00262558232924251,-4.41908793302974e-18,-0.0282804398069171,1.85008519680256e-17,0.107712762161892,-8.01033146808979e-17,-0.221792011243957,-7.44074136835274e-17,0.279178408915983,-7.44074136835274e-17,-0.221792011243957,-8.01033146808979e-17,0.107712762161892,1.85008519680256e-17,-0.0282804398069171,-4.41908793302974e-18,0.00262558232924251,1.78087782196817e-19,0}; //��Matlab���
static int x1[length+22]; //�������ݻ��棬����֡�Խ�

/*fir�˲������ӳ���,��ڲ���hΪϵ��*/
void filter(short xin[],short xout[],int n,double h[])
{
	int i,j;
	double sum;
	for(i=0;i<length;i++) x1[n+i-1]=xin[i]; //���ݶ���
	for (i=0;i<length;i++) //��֡�˲�
	{
		sum=0.0;
		for(j=0;j<n;j++) sum+=h[j]*x1[i-j+n-1]; //�������
		xout[i]= (short)sum;
	}
	for(i=0;i<(n-1);i++)
	x1[n-i-2]=xin[length-1-i]; //֡ͷ����
}

/*������*/
int main( )
{
	struct WAVEFORMA WAVEFORMAT;
	FILE *fp1,*fp2,*fp3,*fp4;
	int i,frame;
	short indata[length],outdata[length];
	fp1=fopen("MARIO.wav","rb"); /*���������ļ�,�����Ʒ�ʽ��*/
	if(fp1==NULL)
	{ printf("\n�ļ���ʧ��!\n");
	exit(0);
	}
	fp2=fopen("outspeechlow.wav","wb"); /*��ͨ�˲��������ļ�*/
	fp3=fopen("outspeechhigh.wav","wb"); /*��ͨ�˲��������ļ�*/
	fp4=fopen("outspeechband.wav","wb"); /*��ͨ�˲��������ļ�*/
	frame=0;

	_read(fp1, &WAVEFORMAT, sizeof(WAVEFORMAT));//��wav�ļ�ͷ����ָ�붨λ��������ʼ
	while(feof(fp1)==0)
	{
		frame++;
		printf("frame=%d\n",frame);

		for(i=0;i<length;i++)
			indata[i]=getw(fp1); /*ȡһ֡��������*/

		filter(indata,outdata,21,hlow); /*���õ�ͨ�˲��ӳ���*/
		for(i=0;i<length;i++)
		{
			putw(outdata[i],fp2); /*���˲������ֵд���ļ�*/
			//printf("%Lf\n",outdata[i]); /*��֤�������*/
		}

		filter(indata,outdata,21,hhigh); /*���ø�ͨ�˲��ӳ���*/
		for(i=0;i<length;i++)
			putw(outdata[i],fp3);

		filter(indata,outdata,21,hband); /*���ô�ͨ�˲��ӳ���*/
		for(i=0;i<length;i++)
			putw(outdata[i],fp4);
	}
	fclose(fp1); /*�ر��ļ�*/
	fclose(fp2);
	fclose(fp3);
	fclose(fp4);
	return (0);
}

/*
#definelength 180
void filter(shortxin[ ],shortxout[ ],int n,int h[ ]);
static int h[21]={0,-10,-31,-89,��}; //����15λ
static short x1[length+22];

//fir �˲������ӳ���
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