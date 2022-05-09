#include "OFBlur.h"

ColorType OFBlur::RGBtoCOL(DWORD rgb) {
	ColorType result{ NULL };
	unsigned char temp = rgb % (1 << 8);
	result.Red = temp;
	rgb = rgb >> 8;
	temp = rgb % (1 << 8);
	result.Green = temp;
	rgb = rgb >> 8;
	temp = rgb % (1 << 8);
	result.Blue = temp;
	return result;
}

unsigned int OFBlur::COLtoRGB(ColorType col) { return RGB(col.Red, col.Green, col.Blue); }

ColorType **OFBlur::CreateColorArr(unsigned int height, unsigned int width) {
	ColorType **temp = new ColorType * [height];
	for (int i = 0; i < height; i++)
		temp[i] = new ColorType[width];
	return temp;
}

void OFBlur::ClearColorArr(ColorType **arr, unsigned int height) {
	for (int i = 0; i < height; i++)
		delete[] arr[i];
	delete[] arr;
}

void OFBlur::CopyImg(ColorType **Dest, ColorType **source, const unsigned int height, const unsigned int width, const unsigned int radius) {
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			Dest[i + radius][j + radius] = source[i][j];
	for (int i = 0; i < radius; i++)
	{
		for (int j = 0; j < radius; j++)
		{
			Dest[i][j] = source[0][0];
			Dest[i + radius + height][j] = source[height - 1][0];
			Dest[i][j + radius + width] = source[0][width - 1];
			Dest[i + radius + height][j + radius + width] = source[height - 1][width - 1];
		}
	}
	for (int i = 0; i < radius; i++)
	{
		for (int j = 0; j < width; j++)
		{
			Dest[i][j + radius] = source[0][j];
			Dest[(i + radius + height)][j + radius] = source[height - 1][j];
		}
	}
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < radius; j++)
		{
			Dest[i + radius][j] = source[i][0];
			Dest[i + radius][j + radius + width] = source[i][width - 1];
		}
	}
}

double **OFBlur::BuildDistanceTable(int radius, double SigmaDis, double *weightsum) {
	*weightsum = 0;
	double **result = new double *[2 * radius];
	for (int i = 0; i < 2 * radius; i++)
		result[i] = new double[2 * radius];
	for (int i = -radius; i < radius; i++)
	{
		for (int j = -radius; j < radius; j++)
		{
			// i * i + j * j 是距离的平方，SigmaDis 是距离标准差
			double value = exp(-(0.5 * (i * i + j * j) / (SigmaDis * SigmaDis)));
			result[i + radius][j + radius] = value;
			*weightsum += value;
		}
	}
	return result;
}

void OFBlur::ClearDistanceTable(double **distable, unsigned int radius) {
	for (int i = 0; i < 2 * radius; i++)
		delete[] distable[i];
	delete[] distable;
}

double *OFBlur::BuildColorTable(double SigmaCol) {
	double *result = new double[256];
	for (int i = 0; i < 256; i++)
		// i * i 是色差的平方
		result[i] = exp(-(0.5 * (i * i) / (SigmaCol * SigmaCol)));
	return result;
}

void OFBlur::ClearColorTable(double *colortable) { delete[]colortable; }

void OFBlur::AverageEngine(int BlurName) {
	//预处理;
	unsigned int width = IMG[BlurName].OrigIMG.getwidth(); unsigned int height = IMG[BlurName].OrigIMG.getheight();
	int radius = IMG[BlurName].radius;

	DWORD *pMem = GetImageBuffer(&IMG[BlurName].BlurIMG);
	ColorType **img = CreateColorArr(height, width);

	int k = -1;
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			img[i][j] = RGBtoCOL(pMem[++k]);
		}
	}

	//计算平均模糊;
	ColorType **temp = CreateColorArr(height + radius * 2, width + radius * 2);
	CopyImg(temp, img, height, width, radius);
	double sum = 4 * radius * radius;

	k = -1;
	for (int row = 0; row < height; ++row) {
		for (int col = 0; col < width; ++col) {
			double red_all = 0, green_all = 0, blue_all = 0;
			for (int area_row = -radius; area_row < radius; ++area_row) {
				for (int area_col = -radius; area_col < radius; ++area_col) {
					red_all += temp[row + area_row + radius][col + area_col + radius].Red;
					green_all += temp[row + area_row + radius][col + area_col + radius].Green;
					blue_all += temp[row + area_row + radius][col + area_col + radius].Blue;
				}
			}
			pMem[++k] = RGB((unsigned int)(red_all / sum), (unsigned int)(green_all / sum), (unsigned int)(blue_all / sum));
		}
	}

	//标记模糊运算完成;
	IMG[BlurName].BlurDone = 1;

	ClearColorArr(temp, height + radius * 2);
}

void OFBlur::GaussianEngine(int BlurName) {
	//预处理;
	unsigned int width = IMG[BlurName].OrigIMG.getwidth(); unsigned int height = IMG[BlurName].OrigIMG.getheight();
	int radius = IMG[BlurName].radius; double SigmaDis = IMG[BlurName].SigmaDis;

	DWORD *pMem = GetImageBuffer(&IMG[BlurName].BlurIMG);
	ColorType **img = CreateColorArr(height, width);

	int k = -1;
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			img[i][j] = RGBtoCOL(pMem[++k]);
		}
	}

	//计算高斯模糊;
	ColorType **temp = CreateColorArr(height + radius * 2, width + radius * 2);
	double WeightSum;
	double **distanceTable = BuildDistanceTable(radius, SigmaDis, &WeightSum);
	CopyImg(temp, img, height, width, radius);

	k = -1;
	for (int row = 0; row < height; ++row) {
		for (int col = 0; col < width; ++col) {
			double red_all = 0, green_all = 0, blue_all = 0;
			for (int area_row = -radius; area_row < radius; ++area_row) {
				for (int area_col = -radius; area_col < radius; ++area_col) {
					red_all += temp[row + area_row + radius][col + area_col + radius].Red * distanceTable[area_row + radius][area_col + radius];
					green_all += temp[row + area_row + radius][col + area_col + radius].Green * distanceTable[area_row + radius][area_col + radius];
					blue_all += temp[row + area_row + radius][col + area_col + radius].Blue * distanceTable[area_row + radius][area_col + radius];
				}
			}
			pMem[++k] = RGB((unsigned int)(red_all / WeightSum), (unsigned int)(green_all / WeightSum), (unsigned int)(blue_all / WeightSum));
		}
	}

	//标记模糊运算完成;
	IMG[BlurName].BlurDone = 1;

	//清除缓存区;
	ClearColorArr(temp, height + radius * 2);
	ClearDistanceTable(distanceTable, radius);
	ClearColorArr(img, height);
}

void OFBlur::SimilarityEngine(ColorType **img, const unsigned int width, const unsigned int height, const int radius, const double SigmaCol) {
	ColorType **temp = CreateColorArr(height + radius * 2, width + radius * 2);
	double *colorTable = BuildColorTable(SigmaCol);
	CopyImg(temp, img, height, width, radius);
	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width; col++)
		{
			ColorType currentPoint = temp[row + radius][col + radius];
			double red_all = 0, green_all = 0, blue_all = 0;
			double Weight_Red = 0, Weight_Green = 0, Weight_Blue = 0;
			for (int area_row = -radius; area_row < radius; area_row++)
			{
				for (int area_col = -radius; area_col < radius; area_col++)
				{
					double middle = colorTable[abs(temp[row + area_row + radius][col + area_col + radius].Red - currentPoint.Red)];
					red_all += temp[row + area_row + radius][col + area_col + radius].Red * middle;;
					Weight_Red += middle;
					middle = colorTable[abs(temp[row + area_row + radius][col + area_col + radius].Green - currentPoint.Green)];
					green_all += temp[row + area_row + radius][col + area_col + radius].Green * middle;
					Weight_Green += middle;
					middle = colorTable[abs(temp[row + area_row + radius][col + area_col + radius].Blue - currentPoint.Blue)];
					blue_all += temp[row + area_row + radius][col + area_col + radius].Blue * middle;
					Weight_Blue += middle;
				}
			}
			putpixel(col, row,
					 RGB((unsigned int)(red_all / Weight_Red),
						 (unsigned int)(green_all / Weight_Green),
						 (unsigned int)(blue_all / Weight_Blue)));
		}
	}
	ClearColorArr(temp, height + radius * 2);
	ClearColorTable(colorTable);
}

void OFBlur::BilateralEngine(ColorType **img, const unsigned int width, const unsigned int height, const int radius, const double SigmaDis, const double SigmaCol) {
	ColorType **temp = CreateColorArr(height + radius * 2, width + radius * 2);
	double WeightSum;
	double **distanceTable = BuildDistanceTable(radius, SigmaDis, &WeightSum);
	double *colorTable = BuildColorTable(SigmaCol);
	CopyImg(temp, img, height, width, radius);

	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width; col++)
		{
			ColorType currentPoint = temp[row + radius][col + radius];
			double red_all = 0, green_all = 0, blue_all = 0;
			double Weight_Red = 0, Weight_Green = 0, Weight_Blue = 0;
			for (int area_row = -radius; area_row < radius; area_row++)
			{
				for (int area_col = -radius; area_col < radius; area_col++)
				{
					double middle = distanceTable[area_row + radius][area_col + radius] *
						colorTable[abs(temp[row + area_row + radius][col + area_col + radius].Red - currentPoint.Red)];
					red_all += temp[row + area_row + radius][col + area_col + radius].Red * middle;;
					Weight_Red += middle;
					middle = distanceTable[area_row + radius][area_col + radius] *
						colorTable[abs(temp[row + area_row + radius][col + area_col + radius].Green - currentPoint.Green)];
					green_all += temp[row + area_row + radius][col + area_col + radius].Green * middle;
					Weight_Green += middle;
					middle = distanceTable[area_row + radius][area_col + radius] *
						colorTable[abs(temp[row + area_row + radius][col + area_col + radius].Blue - currentPoint.Blue)];
					blue_all += temp[row + area_row + radius][col + area_col + radius].Blue * middle;
					Weight_Blue += middle;
				}
			}
			putpixel(col, row,
					 RGB((unsigned int)(red_all / Weight_Red),
						 (unsigned int)(green_all / Weight_Green),
						 (unsigned int)(blue_all / Weight_Blue)));
		}
	}
	ClearColorArr(temp, height + radius * 2);
	ClearColorTable(colorTable);
	ClearDistanceTable(distanceTable, radius);
}

void OFBlur::ShowBlur_Tree(int BlurName, float ALPHA) {
	if (IMG[BlurName].BlurDone == 0)while (1) { Sleep(1); if (IMG[BlurName].BlurDone == 1) break; }

	DWORD *pMemO = GetImageBuffer(&IMG[BlurName].OrigIMG);
	DWORD *pMemB = GetImageBuffer(&IMG[BlurName].BlurIMG);
	DWORD *pMemS = GetImageBuffer(&IMG[BlurName].ShowIMG);

	for (int i = 0; i < IMG[BlurName].OrigIMG.getwidth() * IMG[BlurName].OrigIMG.getheight(); ++i) {
		int r = (1 - ALPHA) * GetRValue(pMemO[i]) + ALPHA * GetRValue(pMemB[i]);
		int g = (1 - ALPHA) * GetGValue(pMemO[i]) + ALPHA * GetGValue(pMemB[i]);
		int b = (1 - ALPHA) * GetBValue(pMemO[i]) + ALPHA * GetBValue(pMemB[i]);
		pMemS[i] = RGB((BYTE)r, (BYTE)g, (BYTE)b);
	}

	//putimage(x, y, &IMG[BlurName].ShowIMG);
	putimage(IMG[BlurName].x, IMG[BlurName].y, &IMG[BlurName].ShowIMG);
}

void OFBlur::WriteBlurCache(int BlurName, int x, int y, int width, int height) {
	getimage(&IMG[BlurName].OrigIMG, x, y, width, height);
	IMG[BlurName].x = x; IMG[BlurName].y = y;
	IMG[BlurName].ColorFilterIMG = IMG[BlurName].OrigIMG;
}

void OFBlur::ColorFilter(int BlurName, int R, int G, int B) {
	DWORD *pMem = GetImageBuffer(&IMG[BlurName].ColorFilterIMG);
	float WeightR = NULL, WeightG = NULL, WeightB = NULL;
	for (int i = 0; i < IMG[BlurName].OrigIMG.getwidth() * IMG[BlurName].OrigIMG.getheight(); ++i) {
		/*if (Gr <= 30) { WeightR = (1 / 30) * Gr; } else if (Gr >= 225) { WeightR = (-1 / 30) * Gr + 8.5; } else { WeightR = 1; }
		if (Gg <= 30) { WeightG = (1 / 30) * Gg; } else if (Gg >= 225) { WeightG = (-1 / 30) * Gg + 8.5; } else { WeightG = 1; }
		if (Gb <= 30) { WeightB = (1 / 30) * Gb; } else if (Gb >= 225) { WeightB = (-1 / 30) * Gb + 8.5; } else { WeightB = 1; }

		float r = (Gr + static_cast<float>(R / 2) * WeightR); if (r > 255) { r = 255; } else if (r < 0) { r = 0; }
		float g = (Gg + static_cast<float>(G / 2) * WeightG); if (g > 255) { g = 255; } else if (g < 0) { g = 0; }
		float b = (Gb + static_cast<float>(B / 2) * WeightB); if (b > 255) { b = 255; } else if (b < 0) { b = 0; }*/

		float r = (GetRValue(pMem[i]) + static_cast<float>(R) * 0.5); if (r > 255) { r = 255; } else if (r < 0) { r = 0; }
		float g = (GetGValue(pMem[i]) + static_cast<float>(G) * 0.5); if (g > 255) { g = 255; } else if (g < 0) { g = 0; }
		float b = (GetBValue(pMem[i]) + static_cast<float>(B) * 0.5); if (b > 255) { b = 255; } else if (b < 0) { b = 0; }

		pMem[i] = RGB((BYTE)r, (BYTE)g, (BYTE)b);
	}
}

void OFBlur::CreateGaussBlur(int BlurName, const int radius, const double SigmaDis) {
	//标记模糊运算开始;
	IMG[BlurName].BlurDone = 0;

	IMG[BlurName].radius = radius; IMG[BlurName].SigmaDis = SigmaDis;
	IMG[BlurName].ShowIMG = IMG[BlurName].BlurIMG = IMG[BlurName].ColorFilterIMG;

	thread OFBlurTG(&OFBlur::GaussianEngine, this, BlurName); OFBlurTG.detach();
}

void OFBlur::CreateAverageBlur(int BlurName, const int radius) {
	//标记模糊运算开始;
	IMG[BlurName].BlurDone = 0;

	IMG[BlurName].radius = radius;
	IMG[BlurName].ShowIMG = IMG[BlurName].BlurIMG = IMG[BlurName].ColorFilterIMG;

	thread OFBlurTA(&OFBlur::AverageEngine, this, BlurName); OFBlurTA.detach();
}

void OFBlur::ShowBlur(int BlurName, float ALPHA) {
	thread OFBrSB(&OFBlur::ShowBlur_Tree, this, BlurName, ALPHA); OFBrSB.detach();
}

void OFBlur::ShowOrig(int BlurName, float ALPHA) {
	DWORD *pMemO = GetImageBuffer(&IMG[BlurName].OrigIMG);
	DWORD *pMemB = GetImageBuffer(&IMG[BlurName].BlurIMG);
	DWORD *pMemS = GetImageBuffer(&IMG[BlurName].ShowIMG);

	for (int i = 0; i < IMG[BlurName].OrigIMG.getwidth() * IMG[BlurName].OrigIMG.getheight(); ++i) {
		int r = (1 - ALPHA) * GetRValue(pMemB[i]) + ALPHA * GetRValue(pMemO[i]);
		int g = (1 - ALPHA) * GetGValue(pMemB[i]) + ALPHA * GetGValue(pMemO[i]);
		int b = (1 - ALPHA) * GetBValue(pMemB[i]) + ALPHA * GetBValue(pMemO[i]);
		pMemS[i] = RGB((BYTE)r, (BYTE)g, (BYTE)b);
	}
	putimage(IMG[BlurName].x, IMG[BlurName].y, &IMG[BlurName].ShowIMG);
}