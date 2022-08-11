#include "OFEXPGraph.h"
Window EXPG_win;

DoublePOINT OFEXPGraph::GravityCenterSolver(int BoxName) {
	DoublePOINT PointsTorqueSum{ NULL }; double MassSum{ NULL }; DoublePOINT GravityCenter{ NULL };

	//当Style.FunctionPointCustomRadius(所有点的半径)不为空时;
	if (!Style.PointStyle.DynamicFunctionPointRadius.empty()) {
		//当所有点的体积不相同时,计算质心位置;
		//注意在EXPGraph中所提到的点并不是质点,它有体积有密度有体积有质量,并且密度为常量;
		//根据质心的求解公式:质心 = 系统关于零点的矩 / 系统总质量;
		//但是我这里采用简化算法下式中DynamicFunctionPointRadius[i]原本应为点的质量,因为默认所有点的密度相同,所以我们这里只体现比例关系,直接使用面积代表质量能提高计算速度;

		//计算TorqueSum与MassSum;
		for (int i = 0; i < EGFB[BoxName].PointsNum; ++i) {
			//Torque = Mass x r;
			//DynamicFunctionPointRadius代表点的质量(默认所有点密度相同,为了提高计算速度我只体现比例关系);
			PointsTorqueSum.x = Style.PointStyle.DynamicFunctionPointRadius[i] * xSourceCache[i] + PointsTorqueSum.x;
			PointsTorqueSum.y = Style.PointStyle.DynamicFunctionPointRadius[i] * ySourceCache[i] + PointsTorqueSum.y;

			MassSum = Style.PointStyle.DynamicFunctionPointRadius[i] + MassSum;
		}

		//GravityCenter = TorqueSum / MassSum;
		GravityCenter.x = PointsTorqueSum.x / MassSum;
		GravityCenter.y = PointsTorqueSum.y / MassSum;
	} else {
		//当所有点的体积相同时,计算质心位置;
		for (int i = 0; i < EGFB[BoxName].PointsNum; ++i) {
			PointsTorqueSum.x = xSourceCache[i] + PointsTorqueSum.x;
			PointsTorqueSum.y = ySourceCache[i] + PointsTorqueSum.y;
		}
		//GravityCenter = Locat_Sum / PointsNum_Sum;
		GravityCenter.x = PointsTorqueSum.x / EGFB[BoxName].PointsNum;
		GravityCenter.y = PointsTorqueSum.y / EGFB[BoxName].PointsNum;
	}

	EGFB[BoxName].GravityCenter = GravityCenter;
	return GravityCenter;
}

void OFEXPGraph::FarthestRadiusSolver(int BoxName) {
	//计算函数任意点到质心的最小距离;
	float *relGravityRadius = new float[EGFB[BoxName].PointsNum] { NULL };
	float *FunctionPointCustomRadius = new float[EGFB[BoxName].PointsNum] { NULL };
	for (int i = 0; i < EGFB[BoxName].PointsNum; ++i) {
		relGravityRadius[i] = sqrt(pow((float(xSourceCache[i]) - float(EGFB[BoxName].GravityCenter.x)), 2) + pow(float(ySourceCache[i]) - float(EGFB[BoxName].GravityCenter.y), 2));

		//当FunctionPointCustomRadius不为空时(存在自定义半径时);
		if (!Style.PointStyle.DynamicFunctionPointRadius.empty())FunctionPointCustomRadius[i] = Style.PointStyle.DynamicFunctionPointRadius[i];//转存数据(如果直接sort DynamicFunctionPointRadius,那么vector的数据会被排序后面的绘图渲染无法正确进行);
	}

	//寻找最大半径(距离质心的最远点);
	sort(relGravityRadius, relGravityRadius + EGFB[BoxName].PointsNum, greater<float>());
	//寻找最大点半径(从所有点半径中);
	sort(FunctionPointCustomRadius, FunctionPointCustomRadius + EGFB[BoxName].PointsNum, greater<float>());

	//计算质心与原点的最小距离;
	float absoGravityRadius = sqrt(pow(float(EGFB[BoxName].GravityCenter.x), 2) + pow(float(EGFB[BoxName].GravityCenter.y), 2));

	//比较;
	if (relGravityRadius[0] >= absoGravityRadius) {
		EGFB[BoxName].FarthestRadius.x = EGFB[BoxName].FarthestRadius.y = relGravityRadius[0] + FunctionPointCustomRadius[0];
	} else {
		EGFB[BoxName].FarthestRadius.x = EGFB[BoxName].FarthestRadius.y = absoGravityRadius;
	};
	//释放内存;
	delete[] relGravityRadius; delete[] FunctionPointCustomRadius;
}

void OFEXPGraph::AxisFarthestSolver(int BoxName) {
	//分别计算两轴函数任意点到质心的最小距离;
	float *XrelGravity = new float[EGFB[BoxName].PointsNum] { NULL };
	float *YrelGravity = new float[EGFB[BoxName].PointsNum] { NULL };
	float *FunctionPointCustomRadius = new float[EGFB[BoxName].PointsNum] { NULL };
	for (int i = 0; i < EGFB[BoxName].PointsNum; ++i) {
		XrelGravity[i] = abs(float(xSourceCache[i]) - float(EGFB[BoxName].GravityCenter.x));
		YrelGravity[i] = abs((float(ySourceCache[i]) - float(EGFB[BoxName].GravityCenter.y)));

		//当FunctionPointCustomRadius不为空时(存在自定义半径时);
		if (!Style.PointStyle.DynamicFunctionPointRadius.empty())FunctionPointCustomRadius[i] = Style.PointStyle.DynamicFunctionPointRadius[i];//转存数据(如果直接sort DynamicFunctionPointRadius,那么vector的数据会被排序后面的绘图渲染无法正确进行);
	}

	//分别寻找两轴最大半径(距离质心的最远点);
	sort(XrelGravity, XrelGravity + EGFB[BoxName].PointsNum, greater<float>());
	sort(YrelGravity, YrelGravity + EGFB[BoxName].PointsNum, greater<float>());
	//寻找最大点半径(从所有点半径中);
	sort(FunctionPointCustomRadius, FunctionPointCustomRadius + EGFB[BoxName].PointsNum, greater<float>());

	//X;
	if (XrelGravity[0] >= float(EGFB[BoxName].GravityCenter.x)) {
		EGFB[BoxName].FarthestRadius.x = XrelGravity[0] + FunctionPointCustomRadius[0];
	} else {
		EGFB[BoxName].FarthestRadius.x = float(EGFB[BoxName].GravityCenter.x);
	};

	//Y;
	if (YrelGravity[0] >= float(EGFB[BoxName].GravityCenter.y)) {
		EGFB[BoxName].FarthestRadius.y = YrelGravity[0] + FunctionPointCustomRadius[0];
	} else {
		EGFB[BoxName].FarthestRadius.y = float(EGFB[BoxName].GravityCenter.y);
	};
	//释放内存;
	delete[] XrelGravity; delete[] YrelGravity; delete[] FunctionPointCustomRadius;
}

//自适应等比缩放模式;
void OFEXPGraph::BoxSpaceTransformation_Radius(int BoxName) {
	//在等比缩放时两轴缩放因子相同,即EGFB[BoxName].AspectRatio.y = EGFB[BoxName].AspectRatio.x;
	//保证函数质心与函数最远点半径(最大半径FarthestRadius)不大于BoxSpace的最小边距;
	if (EGFB[BoxName].width < EGFB[BoxName].height) {
		EGFB[BoxName].AspectRatio.y = EGFB[BoxName].AspectRatio.x = (0.5 * (EGFB[BoxName].width)) / EGFB[BoxName].FarthestRadius.x;//这里是等比例缩放经过FarthestRadiusSolver计算后的FarthestRadius.x与.y是一致的;
	} else {
		EGFB[BoxName].AspectRatio.y = EGFB[BoxName].AspectRatio.x = (0.5 * (EGFB[BoxName].height)) / EGFB[BoxName].FarthestRadius.x;//这里是等比例缩放经过FarthestRadiusSolver计算后的FarthestRadius.x与.y是一致的;
	}

	//对坐标原点进行变换;
	//这部分我写完后就忘了,自己看吧(注释有可能不对)!

	/*----------------------------------------------------------------------
	<参考系>
	1.以窗口左上角为原点的绝对参考系为第零参考系;
	2.在第零参考系下创建的另一参考系为第一参考系,即屏幕上实际显示的坐标轴和原点;
	3.在第一参考系下以质心为原点的参考系为第二参考系;
	-----------------------------------------------------------------------*/

	//计算第一参考系绝对原点坐标;
	EGFB[BoxName].absoOrigin.x = EGFB[BoxName].absoOrigin.x - EGFB[BoxName].GravityCenter.x;
	EGFB[BoxName].absoOrigin.y = EGFB[BoxName].absoOrigin.y - EGFB[BoxName].GravityCenter.y;

	//计算第一参考系相对第二参考系原点坐标;
	EGFB[BoxName].relOrigin.x = -EGFB[BoxName].GravityCenter.x * EGFB[BoxName].AspectRatio.x + EGFB[BoxName].absoOrigin.x + EGFB[BoxName].GravityCenter.x;
	EGFB[BoxName].relOrigin.y = -EGFB[BoxName].GravityCenter.y * EGFB[BoxName].AspectRatio.y + EGFB[BoxName].absoOrigin.y + EGFB[BoxName].GravityCenter.y;

	//对函数进行变换;
	for (int i = 0; i < EGFB[BoxName].PointsNum; ++i) {
		xTransformCache.push_back((xSourceCache[i] - EGFB[BoxName].GravityCenter.x) * EGFB[BoxName].AspectRatio.x + EGFB[BoxName].absoOrigin.x + EGFB[BoxName].GravityCenter.x);
		yTransformCache.push_back((ySourceCache[i] - EGFB[BoxName].GravityCenter.y) * EGFB[BoxName].AspectRatio.y + EGFB[BoxName].absoOrigin.y + EGFB[BoxName].GravityCenter.y);
	}
}

//自适应非等比缩放模式;
void OFEXPGraph::BoxSpaceTransformation_Axis(int BoxName) {
	EGFB[BoxName].AspectRatio.x = (0.5 * (EGFB[BoxName].width)) / EGFB[BoxName].FarthestRadius.x;
	EGFB[BoxName].AspectRatio.y = (0.5 * (EGFB[BoxName].height)) / EGFB[BoxName].FarthestRadius.y;

	//cout << EGFB[BoxName].FarthestRadius.x << "," << EGFB[BoxName].FarthestRadius.y << endl;

	EGFB[BoxName].absoOrigin.x = EGFB[BoxName].absoOrigin.x - EGFB[BoxName].GravityCenter.x;
	EGFB[BoxName].absoOrigin.y = EGFB[BoxName].absoOrigin.y - EGFB[BoxName].GravityCenter.y;
	EGFB[BoxName].relOrigin.x = -EGFB[BoxName].GravityCenter.x * EGFB[BoxName].AspectRatio.x + EGFB[BoxName].absoOrigin.x + EGFB[BoxName].GravityCenter.x;
	EGFB[BoxName].relOrigin.y = -EGFB[BoxName].GravityCenter.y * EGFB[BoxName].AspectRatio.y + EGFB[BoxName].absoOrigin.y + EGFB[BoxName].GravityCenter.y;

	for (int i = 0; i < EGFB[BoxName].PointsNum; ++i) {
		xTransformCache.push_back((xSourceCache[i] - EGFB[BoxName].GravityCenter.x) * EGFB[BoxName].AspectRatio.x + EGFB[BoxName].absoOrigin.x + EGFB[BoxName].GravityCenter.x);
		yTransformCache.push_back((ySourceCache[i] - EGFB[BoxName].GravityCenter.y) * EGFB[BoxName].AspectRatio.y + EGFB[BoxName].absoOrigin.y + EGFB[BoxName].GravityCenter.y);
	}
}

void OFEXPGraph::DrawFunctionLine(int BoxName, int lineNum, int StaggerNum) {
	if (Style.LineStyle.StaticLineColor != NULL) {//如果存在自定义'线'的静态颜色则优先使用静态颜色(相对于主题颜色),如果没有静态颜色则使用主题颜色;
		setlinecolor(Style.LineStyle.StaticLineColor);
	} else if (Style.ThemeStyle.DefaultTheme == lightTheme)setlinecolor(Style.ThemeStyle.LineColor_light); else setlinecolor(Style.ThemeStyle.LineColor_Dark);

	setlinestyle(Style.LineStyle.LineType | PS_ENDCAP_ROUND, Style.LineStyle.FunctionLineWidth * SSAAR[BoxName].SSAAOverride);//设置线宽;

	for (int i = lineNum; i < EGFB[BoxName].PointsNum; i += StaggerNum + 1) {
		//如果存在自定义'线'的动态颜色那么动态颜色最优先被使用;
		if (!Style.LineStyle.DynamicFunctionLineColor.empty())setlinecolor(Style.LineStyle.DynamicFunctionLineColor[i]);

		line(xTransformCache[i - lineNum] * SSAAR[BoxName].SSAAOverride, yTransformCache[i - lineNum] * SSAAR[BoxName].SSAAOverride, xTransformCache[i] * SSAAR[BoxName].SSAAOverride, yTransformCache[i] * SSAAR[BoxName].SSAAOverride);
	}
}

void OFEXPGraph::FreeCache(int BoxName) {
	EGFB[BoxName].PointsNum = 0;
	xSourceCache.clear(); ySourceCache.clear(); vector<double>().swap(xSourceCache); vector<double>().swap(ySourceCache);
	xTransformCache.clear(); yTransformCache.clear(); vector<double>().swap(xTransformCache); vector<double>().swap(yTransformCache);

	AGI.xBigGridLocat.clear(); AGI.yBigGridLocat.clear(); vector<float>().swap(AGI.xBigGridLocat); vector<float>().swap(AGI.yBigGridLocat);
	AGI.xBigGridValue.clear(); AGI.yBigGridValue.clear(); vector<float>().swap(AGI.xBigGridValue); vector<float>().swap(AGI.yBigGridValue);
	AGI.xSmallGridLocat.clear(); AGI.ySmallGridLocat.clear(); vector<float>().swap(AGI.xSmallGridLocat); vector<float>().swap(AGI.ySmallGridLocat);

	Style.PointStyle.DynamicFunctionPointColor.clear(); vector<COLORREF>().swap(Style.PointStyle.DynamicFunctionPointColor);
	Style.PointStyle.DynamicFunctionPointRadius.clear(); vector<float>().swap(Style.PointStyle.DynamicFunctionPointRadius);

	Style.LineStyle.DynamicFunctionLineColor.clear(); vector<COLORREF>().swap(Style.LineStyle.DynamicFunctionLineColor);

	if (SSAAR[BoxName].SSAARenderOpen == 1 && SSAAR[BoxName].FPS == 0) { delete SSAAR[BoxName].SSAAIMGCache; delete IMGCache; }
}

void OFEXPGraph::GetGraphBoxIMG(int BoxName) {
	getimage(IMGCache, EGFB[BoxName].x * SSAAR[BoxName].SSAAOverride, EGFB[BoxName].y * SSAAR[BoxName].SSAAOverride, (EGFB[BoxName].width + 1) * SSAAR[BoxName].SSAAOverride, (EGFB[BoxName].height + 1) * SSAAR[BoxName].SSAAOverride);

	CreateDirectory("EXPGraphSSAAOutPut", NULL);
	saveimage(_T(("EXPGraphSSAAOutPut\\" + to_string(BoxName) + to_string(SSAAR[BoxName].Frame) + ".png").c_str()), IMGCache);

	Resize(NULL, SourceWindowSize.x, SourceWindowSize.y);
	if (SSAAR[BoxName].FPS == 0) {
		clearcliprgn();
		loadimage(IMGCache, _T(("EXPGraphSSAAOutPut\\" + to_string(BoxName) + to_string(SSAAR[BoxName].Frame) + ".png").c_str()), EGFB[BoxName].width + EGFB[BoxName].x, EGFB[BoxName].height + EGFB[BoxName].y, true);
		putimage(EGFB[BoxName].x, EGFB[BoxName].y, IMGCache);
	}

	/*HDC easyx_win = GetImageHDC();
	HDC img_hdc = GetImageHDC(&IMGCache);
	StretchBlt(easyx_win, EGFB[BoxName].x, EGFB[BoxName].y, EGFB[BoxName].width, EGFB[BoxName].height, img_hdc, EGFB[BoxName].x * SSAAR[BoxName].SSAAOverride, EGFB[BoxName].y * SSAAR[BoxName].SSAAOverride, EGFB[BoxName].width * SSAAR[BoxName].SSAAOverride, EGFB[BoxName].height * SSAAR[BoxName].SSAAOverride, SRCCOPY);*/
}

void OFEXPGraph::SSAARender(int BoxName, const float Override, const short FPS) {
	SSAAR[BoxName].SSAARenderOpen = 1;//标记SSAA渲染启动;
	SSAAR[BoxName].SSAAOverride = Override; SSAAR[BoxName].FPS = FPS;

	//当超采样倍率SSAAOverride==0 或 FPS==0时 不会进行视频渲染(但进行图片渲染)
	if (SSAAR[BoxName].SSAAOverride != 0 && SSAAR[BoxName].FPS != 0) {
		if (SSAAR[BoxName].Frame == 0)ShowWindow(EXPG_win.GetWindowHWND(), 0);
		++SSAAR[BoxName].Frame;

		system("cls");
		cout << "EXPGraphSSAARendering...\n" << "BoxName:" << BoxName << "  SSAAOverride:" << Override << "  FPS:" << FPS << "  Frame:" << SSAAR[BoxName].Frame;
	} else if (SSAAR[BoxName].SSAAOverride == 0) { SSAAR[BoxName].SSAAOverride = 1; }

	SourceWindowSize.x = getwidth(); SourceWindowSize.y = getheight();
	Resize(SSAAR[BoxName].SSAAIMGCache, SourceWindowSize.x * SSAAR[BoxName].SSAAOverride, SourceWindowSize.y * SSAAR[BoxName].SSAAOverride);

	SetWorkingImage(SSAAR[BoxName].SSAAIMGCache);
}

void OFEXPGraph::AutoGrid(int BoxName) {
	//根据函数点与质心的最远距离(FarthestRadius)和函数缩放比例计算栅格缩放比例;
	if (EGFB[BoxName].FarthestRadius.y > EGFB[BoxName].FarthestRadius.x) {//当XY轴为非等比例缩放时从两轴之间选择最远的FarthestRadius;
		if (EGFB[BoxName].FarthestRadius.y < 50) {
			//当FarthestRadius<50时栅格显示大小不固定;
			EGFB[BoxName].GridZoomFactor.x = (50 + EGFB[BoxName].FarthestRadius.x) / EGFB[BoxName].AspectRatio.x;
			EGFB[BoxName].GridZoomFactor.y = (50 + EGFB[BoxName].FarthestRadius.y) / EGFB[BoxName].AspectRatio.y;
		} else {
			//当FarthestRadius>=50时栅格的显示大小固定;
			EGFB[BoxName].GridZoomFactor.x = 100 / EGFB[BoxName].AspectRatio.x;
			EGFB[BoxName].GridZoomFactor.y = 100 / EGFB[BoxName].AspectRatio.y;
		}
	} else {
		if (EGFB[BoxName].FarthestRadius.x < 50) {
			//当FarthestRadius<50时栅格显示大小不固定;
			EGFB[BoxName].GridZoomFactor.x = (50 + EGFB[BoxName].FarthestRadius.x) / EGFB[BoxName].AspectRatio.x;
			EGFB[BoxName].GridZoomFactor.y = (50 + EGFB[BoxName].FarthestRadius.y) / EGFB[BoxName].AspectRatio.y;
		} else {
			//当FarthestRadius>=50时栅格的显示大小固定;
			EGFB[BoxName].GridZoomFactor.x = 100 / EGFB[BoxName].AspectRatio.x;
			EGFB[BoxName].GridZoomFactor.y = 100 / EGFB[BoxName].AspectRatio.y;
		}
	}

	//统计栅格数目;
	float PyPathLineNum = (EGFB[BoxName].height + EGFB[BoxName].y - EGFB[BoxName].relOrigin.y) / EGFB[BoxName].AspectRatio.y / EGFB[BoxName].GridZoomFactor.y;
	float NyPathLineNum = (EGFB[BoxName].relOrigin.y - EGFB[BoxName].y) / EGFB[BoxName].AspectRatio.y / EGFB[BoxName].GridZoomFactor.y;
	float PxPathLineNum = (EGFB[BoxName].width + EGFB[BoxName].x - EGFB[BoxName].relOrigin.x) / EGFB[BoxName].AspectRatio.x / EGFB[BoxName].GridZoomFactor.x;
	float NxPathLineNum = (EGFB[BoxName].relOrigin.x - EGFB[BoxName].x) / EGFB[BoxName].AspectRatio.x / EGFB[BoxName].GridZoomFactor.x;
	AGI.yPathLineNum = PyPathLineNum + NyPathLineNum; AGI.xPathLineNum = PxPathLineNum + NxPathLineNum;

	//BigGridValue;
	for (int i = 0; i < PyPathLineNum; ++i) {//-y;
		AGI.yBigGridLocat.push_back(EGFB[BoxName].relOrigin.y + i * EGFB[BoxName].AspectRatio.y * EGFB[BoxName].GridZoomFactor.y);
		AGI.yBigGridValue.push_back(-i * EGFB[BoxName].GridZoomFactor.y);
	}
	for (int i = 0; i < NyPathLineNum; ++i) {//+y;
		AGI.yBigGridLocat.push_back(EGFB[BoxName].relOrigin.y - i * EGFB[BoxName].AspectRatio.y * EGFB[BoxName].GridZoomFactor.y);
		AGI.yBigGridValue.push_back(i * EGFB[BoxName].GridZoomFactor.y);
	}

	for (int i = 0; i < PxPathLineNum; ++i) {//-x;
		AGI.xBigGridLocat.push_back(EGFB[BoxName].relOrigin.x + i * EGFB[BoxName].AspectRatio.x * EGFB[BoxName].GridZoomFactor.x);
		AGI.xBigGridValue.push_back(i * EGFB[BoxName].GridZoomFactor.x);
	}
	for (int i = 0; i < NxPathLineNum; ++i) {//+x;
		AGI.xBigGridLocat.push_back(EGFB[BoxName].relOrigin.x - i * EGFB[BoxName].AspectRatio.x * EGFB[BoxName].GridZoomFactor.x);
		AGI.xBigGridValue.push_back(-i * EGFB[BoxName].GridZoomFactor.x);
	}

	//SmallGridValue;
	for (int i = 0; i < PyPathLineNum * 5; ++i) {//-y;
		AGI.ySmallGridLocat.push_back(EGFB[BoxName].relOrigin.y + i * EGFB[BoxName].AspectRatio.y * EGFB[BoxName].GridZoomFactor.y / 5);
	}
	for (int i = 0; i < NyPathLineNum * 5; ++i) {//+y;
		AGI.ySmallGridLocat.push_back(EGFB[BoxName].relOrigin.y - i * EGFB[BoxName].AspectRatio.y * EGFB[BoxName].GridZoomFactor.y / 5);
	}

	for (int i = 0; i < PxPathLineNum * 5; ++i) {//-x;
		AGI.xSmallGridLocat.push_back(EGFB[BoxName].relOrigin.x + i * EGFB[BoxName].AspectRatio.x * EGFB[BoxName].GridZoomFactor.x / 5);
	}
	for (int i = 0; i < NxPathLineNum * 5; ++i) {//+x;
		AGI.xSmallGridLocat.push_back(EGFB[BoxName].relOrigin.x - i * EGFB[BoxName].AspectRatio.x * EGFB[BoxName].GridZoomFactor.x / 5);
	}
}

void OFEXPGraph::DrawFunction(int BoxName) {
	BeginBatchDraw();
	//FlushBatchDraw();

	setlinestyle(PS_SOLID | PS_ENDCAP_SQUARE, Style.GraphBOXStyle.GridLineWidth * SSAAR[BoxName].SSAAOverride);

	//在没有人指定主题的条件下根据背景色设置默认主题(如果已有人指定主题则程序不会自动选择);
	if (RGB(128, 128, 128) <= getbkcolor() && Style.ThemeStyle.DefaultTheme == NULL) Style.ThemeStyle.DefaultTheme = lightTheme; else Style.ThemeStyle.DefaultTheme = DarkTheme;

	//ShowBackgroundColor
	if (Style.GraphBOXStyle.ShowBackground == 1) {
		if (Style.ThemeStyle.DefaultTheme == lightTheme)setfillcolor(Style.ThemeStyle.BackgroundColor_light); else setfillcolor(Style.ThemeStyle.BackgroundColor_Dark);
		solidrectangle(EGFB[BoxName].x * SSAAR[BoxName].SSAAOverride, EGFB[BoxName].y * SSAAR[BoxName].SSAAOverride, (EGFB[BoxName].width + EGFB[BoxName].x) * SSAAR[BoxName].SSAAOverride, (EGFB[BoxName].height + EGFB[BoxName].y) * SSAAR[BoxName].SSAAOverride);
	}

	//设置坐标轴值字体;
	if (Style.GraphBOXStyle.ShowAxisValue == 1) {
		setbkmode(TRANSPARENT);
		LOGFONT f; gettextstyle(&f);
		f.lfHeight = Style.GraphBOXStyle.AxisValueTextHeight * SSAAR[BoxName].SSAAOverride;
		f.lfQuality = ANTIALIASED_QUALITY | PROOF_QUALITY, f.lfCharSet = CHINESEBIG5_CHARSET;
		_tcscpy_s(f.lfFaceName, _T("Consolas"));
		settextstyle(&f); if (Style.ThemeStyle.DefaultTheme == lightTheme)settextcolor(Style.ThemeStyle.AxisValueColor_light); else settextcolor(Style.ThemeStyle.AxisValueColor_Dark);
	}

	//ShowSmallGrid;
	if (Style.GraphBOXStyle.ShowSmallGrid == 1) {
		if (Style.ThemeStyle.DefaultTheme == lightTheme)setlinecolor(Style.ThemeStyle.SmallGridColor_light); else setlinecolor(Style.ThemeStyle.SmallGridColor_Dark);
		for (int i = 0; i < AGI.xPathLineNum * 5; ++i) {//x;
			line(AGI.xSmallGridLocat[i] * SSAAR[BoxName].SSAAOverride, EGFB[BoxName].y * SSAAR[BoxName].SSAAOverride, AGI.xSmallGridLocat[i] * SSAAR[BoxName].SSAAOverride, (EGFB[BoxName].height + EGFB[BoxName].y) * SSAAR[BoxName].SSAAOverride);
		}
		for (int i = 0; i < AGI.yPathLineNum * 5; ++i) {//y;
			line(EGFB[BoxName].x * SSAAR[BoxName].SSAAOverride, AGI.ySmallGridLocat[i] * SSAAR[BoxName].SSAAOverride, (EGFB[BoxName].width + EGFB[BoxName].x) * SSAAR[BoxName].SSAAOverride, AGI.ySmallGridLocat[i] * SSAAR[BoxName].SSAAOverride);
		}
	}

	//ShowBigGrid;
	if (Style.GraphBOXStyle.ShowBigGrid == 1) {
		char s[32]{}; if (Style.ThemeStyle.DefaultTheme == lightTheme)setlinecolor(Style.ThemeStyle.BigGridColor_light); else setlinecolor(Style.ThemeStyle.BigGridColor_Dark);
		for (int i = 0; i < AGI.xPathLineNum; ++i) {//x;
			line(AGI.xBigGridLocat[i] * SSAAR[BoxName].SSAAOverride, EGFB[BoxName].y * SSAAR[BoxName].SSAAOverride, AGI.xBigGridLocat[i] * SSAAR[BoxName].SSAAOverride, (EGFB[BoxName].height + EGFB[BoxName].y) * SSAAR[BoxName].SSAAOverride);

			if (Style.GraphBOXStyle.ShowAxisValue == 1)sprintf(s, "%.2f", AGI.xBigGridValue[i]); outtextxy((AGI.xBigGridLocat[i] + 3) * SSAAR[BoxName].SSAAOverride, EGFB[BoxName].relOrigin.y * SSAAR[BoxName].SSAAOverride, s);
		}
		for (int i = 0; i < AGI.yPathLineNum; ++i) {//y;
			line(EGFB[BoxName].x * SSAAR[BoxName].SSAAOverride, AGI.yBigGridLocat[i] * SSAAR[BoxName].SSAAOverride, (EGFB[BoxName].width + EGFB[BoxName].x) * SSAAR[BoxName].SSAAOverride, AGI.yBigGridLocat[i] * SSAAR[BoxName].SSAAOverride);

			if (Style.GraphBOXStyle.ShowAxisValue == 1)sprintf(s, "%.2f", AGI.yBigGridValue[i]); outtextxy((EGFB[BoxName].relOrigin.x + 3) * SSAAR[BoxName].SSAAOverride, AGI.yBigGridLocat[i] * SSAAR[BoxName].SSAAOverride, s);
		}
	}

	//ShowAixs;
	if (Style.GraphBOXStyle.ShowAxis == 1) {
		if (Style.ThemeStyle.DefaultTheme == lightTheme)setlinecolor(Style.ThemeStyle.AxisColor_light); else setlinecolor(Style.ThemeStyle.AxisColor_Dark);
		line(EGFB[BoxName].x * SSAAR[BoxName].SSAAOverride, EGFB[BoxName].relOrigin.y * SSAAR[BoxName].SSAAOverride, (EGFB[BoxName].width + EGFB[BoxName].x) * SSAAR[BoxName].SSAAOverride, EGFB[BoxName].relOrigin.y * SSAAR[BoxName].SSAAOverride);
		line(EGFB[BoxName].relOrigin.x * SSAAR[BoxName].SSAAOverride, EGFB[BoxName].y * SSAAR[BoxName].SSAAOverride, EGFB[BoxName].relOrigin.x * SSAAR[BoxName].SSAAOverride, (EGFB[BoxName].height + EGFB[BoxName].y) * SSAAR[BoxName].SSAAOverride);
	}

	//ShowEdge;
	if (Style.GraphBOXStyle.ShowFuncGraphBOXedge == 1)rectangle(EGFB[BoxName].x * SSAAR[BoxName].SSAAOverride, EGFB[BoxName].y * SSAAR[BoxName].SSAAOverride, (EGFB[BoxName].width + EGFB[BoxName].x) * SSAAR[BoxName].SSAAOverride, (EGFB[BoxName].height + EGFB[BoxName].y) * SSAAR[BoxName].SSAAOverride);

	//ShowGravityCenter;
	if (Style.GraphBOXStyle.ShowGravityCenter == 1) {
		if (Style.ThemeStyle.DefaultTheme == lightTheme)setfillcolor(Style.ThemeStyle.GravityCenterColor_light); else setfillcolor(Style.ThemeStyle.GravityCenterColor_Dark);
		solidcircle((EGFB[BoxName].GravityCenter.x + EGFB[BoxName].absoOrigin.x) * SSAAR[BoxName].SSAAOverride, (EGFB[BoxName].GravityCenter.y + EGFB[BoxName].absoOrigin.y) * SSAAR[BoxName].SSAAOverride, Style.GraphBOXStyle.GravityCenterRadius * SSAAR[BoxName].SSAAOverride);
	}

	//绘制点之间的连线;
	if (Style.GraphBOXStyle.ShowFunctionLine == 1)DrawFunctionLine(BoxName_DrawFunctionLine, lineNum_DrawFunctionLine, StaggerNum_DrawFunctionLine);

	//ShowFunctionPoints;
	if (Style.PointStyle.StaticFunctionPointColor != NULL) {//如果存在自定义'点'的静态颜色则优先使用静态颜色(相对于主题颜色),如果没有静态颜色则使用主题颜色;
		setfillcolor(Style.PointStyle.StaticFunctionPointColor);
	} else if (Style.ThemeStyle.DefaultTheme == lightTheme)setfillcolor(Style.ThemeStyle.FunctionColor_light); else setfillcolor(Style.ThemeStyle.FunctionColor_Dark);

	for (int i = 0; i < EGFB[BoxName].PointsNum; ++i) {
		//如果存在自定义'点'的动态颜色那么动态颜色最优先被使用;
		if (!Style.PointStyle.DynamicFunctionPointColor.empty())setfillcolor(Style.PointStyle.DynamicFunctionPointColor[i]);

		//如果有自定义绘点半径则使用自定义半径,否则则使用统一半径;
		if (!Style.PointStyle.DynamicFunctionPointRadius.empty()) {
			//在考虑缩放的条件下,计算椭圆外切矩形四个顶点坐标;
			float Recleft = (xTransformCache[i] - EGFB[BoxName].AspectRatio.x * Style.PointStyle.DynamicFunctionPointRadius[i]) * SSAAR[BoxName].SSAAOverride;
			float RecTop = (yTransformCache[i] - EGFB[BoxName].AspectRatio.y * Style.PointStyle.DynamicFunctionPointRadius[i]) * SSAAR[BoxName].SSAAOverride;
			float RecRight = (xTransformCache[i] + EGFB[BoxName].AspectRatio.x * Style.PointStyle.DynamicFunctionPointRadius[i]) * SSAAR[BoxName].SSAAOverride;
			float RecBottom = (yTransformCache[i] + EGFB[BoxName].AspectRatio.y * Style.PointStyle.DynamicFunctionPointRadius[i]) * SSAAR[BoxName].SSAAOverride;

			//绘制椭圆;
			solidellipse(Recleft, RecTop, RecRight, RecBottom);

			//绘制圆;
			//solidcircle(xTransformCache[i] * SSAAR[BoxName].SSAAOverride, yTransformCache[i] * SSAAR[BoxName].SSAAOverride, Style.FunctionPointCustomRadius[i] * SSAAR[BoxName].SSAAOverride);
		} else {
			solidcircle(xTransformCache[i] * SSAAR[BoxName].SSAAOverride, yTransformCache[i] * SSAAR[BoxName].SSAAOverride, Style.PointStyle.StaticFunctionPointRadius * SSAAR[BoxName].SSAAOverride);
		}
	}

	if (SSAAR[BoxName].SSAARenderOpen == 1)GetGraphBoxIMG(BoxName);//保存渲染图导程序自身目录下的EXPGraphSSAAOutPut文件夹
	EndBatchDraw();
}

void OFEXPGraph::CreateFunctionGraphBOX(int BoxName, int x, int y, int width, int height) {
	EGFB[BoxName].x = x; EGFB[BoxName].y = y; EGFB[BoxName].width = width; EGFB[BoxName].height = height;
	EGFB[BoxName].absoOrigin.x = (EGFB[BoxName].width / static_cast<double>(2) + x); EGFB[BoxName].absoOrigin.y = (EGFB[BoxName].height / static_cast<double>(2) + y);
}

void OFEXPGraph::SetFunctionLine(int BoxName, int lineNum, int StaggerNum) {
	Style.GraphBOXStyle.ShowFunctionLine = 1;
	BoxName_DrawFunctionLine = BoxName;
	lineNum_DrawFunctionLine = lineNum;
	StaggerNum_DrawFunctionLine = StaggerNum;
}

void OFEXPGraph::ImportNewFunction(int BoxName, double xValue, double yValue) {
	xSourceCache.push_back(xValue); ySourceCache.push_back(-yValue); ++EGFB[BoxName].PointsNum;
}

void OFEXPGraph::AutoVisualDraw(int BoxName, bool TransformMode) {
	GravityCenterSolver(BoxName);

	if (TransformMode == RadiusTransformMode) {
		FarthestRadiusSolver(BoxName);
		BoxSpaceTransformation_Radius(BoxName);
	} else {
		AxisFarthestSolver(BoxName);
		BoxSpaceTransformation_Axis(BoxName);
	}

	if (Style.GraphBOXStyle.ShowAxis == 1)AutoGrid(BoxName);//计算栅格变换;
	DrawFunction(BoxName);
	FreeCache(BoxName);
}

void OFEXPGraph::IMGtoVIDEO(int BoxName) {
	//获取系统时间;
	time_t timep; time(&timep);
	struct tm *nowTime = localtime(&timep);
	string nowTimeStr = to_string(1900 + nowTime->tm_year) + to_string(nowTime->tm_mon) + to_string(nowTime->tm_mday) + to_string(nowTime->tm_hour) + to_string(nowTime->tm_min) + to_string(nowTime->tm_sec);

	//生成FFmpeg命令;
	string Command = "ffmpeg -r " + to_string(SSAAR[BoxName].FPS) + " -f image2 -i EXPGraphSSAAOutPut/" + to_string(BoxName) + "%d.png EXPGraphSSAAOutPut_" + to_string(BoxName) + "_" + nowTimeStr + ".mp4";
	cout << "\nFFmpegCommand: " << Command << endl;
	int state = system(Command.c_str());//执行FFmpeg命令;

	if (state == 0) {
		//获取程序自身路径;
		char szFilePath[MAX_PATH + 1] = { 0 };
		GetModuleFileNameA(NULL, szFilePath, MAX_PATH);
		(strrchr(szFilePath, '\\'))[0] = 0; // 删除文件名，只获得路径字串
		string path = szFilePath;

		//视频输出后删除EXPGraphSSAAOutPut文件夹;

		//遍历EXPGraphSSAAOutPut文件夹并删除所有图像文件;
		for (int i = 0; i <= SSAAR[BoxName].Frame; ++i)remove((path + "\\EXPGraphSSAAOutPut\\" + to_string(BoxName) + to_string(i) + ".png").c_str());
		RemoveDirectory((path + "\\EXPGraphSSAAOutPut").c_str());//删除空的EXPGraphSSAAOutPut文件夹;
	}
}