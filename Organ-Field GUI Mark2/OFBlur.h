#pragma once
#include "Universal.h"

typedef struct ColorType {
	unsigned char Red;
	unsigned char Green;
	unsigned char Blue;
}ColorType;

typedef struct BlurCache {
	//原始图片缓存;
	IMAGE OrigIMG;
	unsigned int x;
	unsigned int y;

	//原始图片滤色后的缓存;
	IMAGE ColorFilterIMG;

	//模糊后的图片缓存;
	IMAGE BlurIMG;
	int radius;
	double SigmaDis;
	double SigmaCol;

	//最终显示模糊的图片缓存;
	IMAGE ShowIMG;

	//模糊运算是否完成;
	bool BlurDone;
}BlurCache;

class OFBlur {
private:
	BlurCache IMG[512]{ NULL };

	ColorType RGBtoCOL(DWORD rgb);// 将 RGB 转化为 ColorType;
	unsigned int COLtoRGB(ColorType col);// 将 ColorType 转化为 RGB;
	ColorType **CreateColorArr(unsigned int height, unsigned int width);// 创建颜色数组;
	void ClearColorArr(ColorType **arr, unsigned int height);// 清除颜色数组;

	// 将 source 颜色数组用复制的方法复制到 dest 数组里，注意 dest 数组的尺寸比 source 数组的尺寸宽高各大 radius;
	void CopyImg(ColorType **Dest, ColorType **source, const unsigned int height, const unsigned int width, const unsigned int radius);

	// 创建距离表，weightsum 是权重总数，进行高斯模糊时可以直接用;
	double **BuildDistanceTable(int radius, double SigmaDis, double *weightsum);

	void ClearDistanceTable(double **distable, unsigned int radius);// 清除距离表;
	double *BuildColorTable(double SigmaCol);// 创建颜色表，SigmaCol 是颜色标准差;
	void ClearColorTable(double *colortable);// 清除颜色表;

	//平均值滤波;
	void AverageEngine(int BlurName);

	// 高斯滤波;
	void GaussianEngine(int BlurName);

	// 相似点滤波;
	void SimilarityEngine(ColorType **img, const unsigned int width, const unsigned int height, const int radius, const double SigmaCol);

	// 双边滤波;
	void BilateralEngine(ColorType **img, const unsigned int width, const unsigned int height, const int radius, const double SigmaDis, const double SigmaCol);

	void ShowBlur_Tree(int BlurName, float ALPHA);
public:
	//将原始图像写入模糊缓存(模糊名称,坐标,宽度高度);
	void WriteBlurCache(int BlurName, int x, int y, int width, int height);

	//线性颜色滤镜(模糊名称,RGB)使原始图像的颜色往设定值颜色偏移;
	void ColorFilter(int BlurName, int R, int G, int B);

	//创建高斯模糊效果(模糊名称,模糊半径,Sigma值)模糊半径越大,Sigma越小质量越好;
	void CreateGaussBlur(int BlurName, const int radius, const double SigmaDis);

	void CreateAverageBlur(int BlurName, const int radius);

	//显示模糊效果(模糊名称,坐标,不透明度0~1);
	void ShowBlur(int BlurName, float ALPHA);
	//显示模糊前原始图像(模糊名称,坐标,不透明度0~1);
	void ShowOrig(int BlurName, float ALPHA);
};
