#pragma once
#include "Universal.h"

typedef struct ColorType {
	unsigned char Red;
	unsigned char Green;
	unsigned char Blue;
}ColorType;

typedef struct BlurCache {
	//ԭʼͼƬ����;
	IMAGE OrigIMG;
	unsigned int x;
	unsigned int y;

	//ԭʼͼƬ��ɫ��Ļ���;
	IMAGE ColorFilterIMG;

	//ģ�����ͼƬ����;
	IMAGE BlurIMG;
	int radius;
	double SigmaDis;
	double SigmaCol;

	//������ʾģ����ͼƬ����;
	IMAGE ShowIMG;

	//ģ�������Ƿ����;
	bool BlurDone;
}BlurCache;

class OFBlur {
private:
	BlurCache IMG[512]{ NULL };

	ColorType RGBtoCOL(DWORD rgb);// �� RGB ת��Ϊ ColorType;
	unsigned int COLtoRGB(ColorType col);// �� ColorType ת��Ϊ RGB;
	ColorType **CreateColorArr(unsigned int height, unsigned int width);// ������ɫ����;
	void ClearColorArr(ColorType **arr, unsigned int height);// �����ɫ����;

	// �� source ��ɫ�����ø��Ƶķ������Ƶ� dest �����ע�� dest ����ĳߴ�� source ����ĳߴ��߸��� radius;
	void CopyImg(ColorType **Dest, ColorType **source, const unsigned int height, const unsigned int width, const unsigned int radius);

	// ���������weightsum ��Ȩ�����������и�˹ģ��ʱ����ֱ����;
	double **BuildDistanceTable(int radius, double SigmaDis, double *weightsum);

	void ClearDistanceTable(double **distable, unsigned int radius);// ��������;
	double *BuildColorTable(double SigmaCol);// ������ɫ��SigmaCol ����ɫ��׼��;
	void ClearColorTable(double *colortable);// �����ɫ��;

	//ƽ��ֵ�˲�;
	void AverageEngine(int BlurName);

	// ��˹�˲�;
	void GaussianEngine(int BlurName);

	// ���Ƶ��˲�;
	void SimilarityEngine(ColorType **img, const unsigned int width, const unsigned int height, const int radius, const double SigmaCol);

	// ˫���˲�;
	void BilateralEngine(ColorType **img, const unsigned int width, const unsigned int height, const int radius, const double SigmaDis, const double SigmaCol);

	void ShowBlur_Tree(int BlurName, float ALPHA);
public:
	//��ԭʼͼ��д��ģ������(ģ������,����,��ȸ߶�);
	void WriteBlurCache(int BlurName, int x, int y, int width, int height);

	//������ɫ�˾�(ģ������,RGB)ʹԭʼͼ�����ɫ���趨ֵ��ɫƫ��;
	void ColorFilter(int BlurName, int R, int G, int B);

	//������˹ģ��Ч��(ģ������,ģ���뾶,Sigmaֵ)ģ���뾶Խ��,SigmaԽС����Խ��;
	void CreateGaussBlur(int BlurName, const int radius, const double SigmaDis);

	void CreateAverageBlur(int BlurName, const int radius);

	//��ʾģ��Ч��(ģ������,����,��͸����0~1);
	void ShowBlur(int BlurName, float ALPHA);
	//��ʾģ��ǰԭʼͼ��(ģ������,����,��͸����0~1);
	void ShowOrig(int BlurName, float ALPHA);
};
