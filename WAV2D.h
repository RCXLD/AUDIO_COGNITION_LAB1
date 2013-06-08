

struct WAVEFORMA
{//ģ��WAVEFORMATEX,WAVE�ļ���ͷ�����ݸ�ʽ
	unsigned short wFormat; /* format type */
	unsigned short nChann; /* number of channels (i.e. mono, stereo...) */
	unsigned long nSamplesPer; /* sample rate */
	unsigned long nAvgBytesPer; /* for buffer estimation */
	unsigned short nBlockAl; /* block size of data */
	unsigned short wBitsPerSam; /* number of bits per sample of mono data */
	unsigned short cbS; /* the count in bytes of the size of */
/* extra information (after cbSize) */
};

#if defined WIN32 || defined WIN64 || defined __i386__ || defined __alpha__
#define NL_LITTLE_ENDIAN
#endif

#ifdef NL_LITTLE_ENDIAN
#define nlSwaps(x) (unsigned short)(((((unsigned short)x) & 0x00ff) << 8) |\
((((unsigned short)x) & 0xff00) >> 8))
#define nlSwapl(x) (unsigned long)(((((unsigned long)x) & 0x000000ff) << 24) |\
((((unsigned long)x) & 0x0000ff00) << 8) |\
((((unsigned long)x) & 0x00ff0000) >> 8) |\
((((unsigned long)x) & 0xff000000) >> 24))

#else
/* no conversion needed for big endian */
#define nlSwaps(x) (x)
#define nlSwapl(x) (x)
#endif /* NL_LITTLE_ENDIAN */

static int readSamples(int f, short *buffer, int len)//����������
{
	int count;
	/* no conversion needed, just swap */
	count = _read(f, buffer, sizeof(short) * len); /*���ļ��е�ǰλ�ö���������buffer.count��
	�ɹ����ض������ֽ���*/
	if(count < 0)
	{
		printf("read error");
		return 1;
	}
	count /= 2;
	swapSamples(buffer, count);
	return count;
}
//buflen = readSamples(infile, samples, NUM_SAMPLES);//�������Ҫ������