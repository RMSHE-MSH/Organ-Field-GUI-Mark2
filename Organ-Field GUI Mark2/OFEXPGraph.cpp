#include "OFEXPGraph.h"
Window EXPG_win;

DoublePOINT OFEXPGraph::GravityCenterSolver(int BoxName) {
	DoublePOINT PointsTorqueSum{ NULL }; double MassSum{ NULL }; DoublePOINT GravityCenter{ NULL };

	//��Style.FunctionPointCustomRadius(���е�İ뾶)��Ϊ��ʱ;
	if (!Style.FunctionPointCustomRadius.empty()) {
		//�����е���������ͬʱ,��������λ��;
		//ע����EXPGraph�����ᵽ�ĵ㲢�����ʵ�,����������ܶ������������,�����ܶ�Ϊ����;
		//�������ĵ���⹫ʽ:���� = ϵͳ�������ľ� / ϵͳ������;
		//������������ü��㷨��ʽ��Style.FunctionPointCustomRadius[i]ԭ��ӦΪ�������,��ΪĬ�����е���ܶ���ͬ,������������ֻ���ֱ�����ϵ,ֱ��ʹ�����������������߼����ٶ�;

		//����TorqueSum��MassSum;
		for (int i = 0; i < EGFB[BoxName].PointsNum; ++i) {
			//Torque = Mass x r;
			//FunctionPointCustomRadius����������(Ĭ�����е��ܶ���ͬ,Ϊ����߼����ٶ���ֻ���ֱ�����ϵ);
			PointsTorqueSum.x = Style.FunctionPointCustomRadius[i] * xSourceCache[i] + PointsTorqueSum.x;
			PointsTorqueSum.y = Style.FunctionPointCustomRadius[i] * ySourceCache[i] + PointsTorqueSum.y;

			MassSum = Style.FunctionPointCustomRadius[i] + MassSum;
		}

		//GravityCenter = TorqueSum / MassSum;
		GravityCenter.x = PointsTorqueSum.x / MassSum;
		GravityCenter.y = PointsTorqueSum.y / MassSum;
	} else {
		//�����е�������ͬʱ,��������λ��;
		for (int i = 0; i < EGFB[BoxName].PointsNum; ++i) {
			PointsTorqueSum.x = xSourceCache[i] + PointsTorqueSum.x;
			PointsTorqueSum.y = ySourceCache[i] + PointsTorqueSum.y;
		}

		GravityCenter.x = PointsTorqueSum.x / EGFB[BoxName].PointsNum;
		GravityCenter.y = PointsTorqueSum.y / EGFB[BoxName].PointsNum;
	}

	EGFB[BoxName].GravityCenter = GravityCenter;
	return GravityCenter;
}

void OFEXPGraph::FarthestRadiusSolver(int BoxName) {
	float *relGravityRadius = new float[EGFB[BoxName].PointsNum]{ NULL };
	for (int i = 0; i < EGFB[BoxName].PointsNum; ++i) {
		relGravityRadius[i] = sqrt(pow((float(xSourceCache[i]) - float(EGFB[BoxName].GravityCenter.x)), 2) + pow(float(ySourceCache[i]) - float(EGFB[BoxName].GravityCenter.y), 2));
	}
	sort(relGravityRadius, relGravityRadius + EGFB[BoxName].PointsNum, greater<float>());

	float absoGravityRadius = sqrt(pow(float(EGFB[BoxName].GravityCenter.x), 2) + pow(float(EGFB[BoxName].GravityCenter.y), 2));

	if (relGravityRadius[0] >= absoGravityRadius) {
		EGFB[BoxName].FarthestRadius.x = EGFB[BoxName].FarthestRadius.y = relGravityRadius[0]; delete[] relGravityRadius;
	} else {
		EGFB[BoxName].FarthestRadius.x = EGFB[BoxName].FarthestRadius.y = absoGravityRadius; delete[] relGravityRadius;
	};
}

void OFEXPGraph::AxisFarthestSolver(int BoxName) {
	float *XrelGravity = new float[EGFB[BoxName].PointsNum]{ NULL };
	float *YrelGravity = new float[EGFB[BoxName].PointsNum]{ NULL };
	for (int i = 0; i < EGFB[BoxName].PointsNum; ++i) {
		XrelGravity[i] = abs(float(xSourceCache[i]) - float(EGFB[BoxName].GravityCenter.x));
		YrelGravity[i] = abs((float(ySourceCache[i]) - float(EGFB[BoxName].GravityCenter.y)));
	}
	sort(XrelGravity, XrelGravity + EGFB[BoxName].PointsNum, greater<float>());
	sort(YrelGravity, YrelGravity + EGFB[BoxName].PointsNum, greater<float>());

	//X;
	if (XrelGravity[0] >= float(EGFB[BoxName].GravityCenter.x)) {
		EGFB[BoxName].FarthestRadius.x = XrelGravity[0]; delete[] XrelGravity;
	} else {
		EGFB[BoxName].FarthestRadius.x = float(EGFB[BoxName].GravityCenter.x); delete[] XrelGravity;
	};

	//Y;
	if (YrelGravity[0] >= float(EGFB[BoxName].GravityCenter.y)) {
		EGFB[BoxName].FarthestRadius.y = YrelGravity[0]; delete[] YrelGravity;
	} else {
		EGFB[BoxName].FarthestRadius.y = float(EGFB[BoxName].GravityCenter.y); delete[] YrelGravity;
	};
}

void OFEXPGraph::BoxSpaceTransformation_Radius(int BoxName) {
	if (EGFB[BoxName].width < EGFB[BoxName].height) {
		EGFB[BoxName].AspectRatio.y = EGFB[BoxName].AspectRatio.x = (0.5 * (EGFB[BoxName].width)) / EGFB[BoxName].FarthestRadius.x;//�����ǵȱ������ž���FarthestRadiusSolver������FarthestRadius.x��.y��һ�µ�;
	} else {
		EGFB[BoxName].AspectRatio.y = EGFB[BoxName].AspectRatio.x = (0.5 * (EGFB[BoxName].height)) / EGFB[BoxName].FarthestRadius.x;//�����ǵȱ������ž���FarthestRadiusSolver������FarthestRadius.x��.y��һ�µ�;
	}

	EGFB[BoxName].absoOrigin.x = EGFB[BoxName].absoOrigin.x - EGFB[BoxName].GravityCenter.x;
	EGFB[BoxName].absoOrigin.y = EGFB[BoxName].absoOrigin.y - EGFB[BoxName].GravityCenter.y;
	EGFB[BoxName].relOrigin.x = -EGFB[BoxName].GravityCenter.x * EGFB[BoxName].AspectRatio.x + EGFB[BoxName].absoOrigin.x + EGFB[BoxName].GravityCenter.x;
	EGFB[BoxName].relOrigin.y = -EGFB[BoxName].GravityCenter.y * EGFB[BoxName].AspectRatio.y + EGFB[BoxName].absoOrigin.y + EGFB[BoxName].GravityCenter.y;

	for (int i = 0; i < EGFB[BoxName].PointsNum; ++i) {
		xTransformCache.push_back((xSourceCache[i] - EGFB[BoxName].GravityCenter.x) * EGFB[BoxName].AspectRatio.x + EGFB[BoxName].absoOrigin.x + EGFB[BoxName].GravityCenter.x);
		yTransformCache.push_back((ySourceCache[i] - EGFB[BoxName].GravityCenter.y) * EGFB[BoxName].AspectRatio.y + EGFB[BoxName].absoOrigin.y + EGFB[BoxName].GravityCenter.y);
	}
}

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

void OFEXPGraph::FreeCache(int BoxName) {
	EGFB[BoxName].PointsNum = 0;
	xSourceCache.clear(); ySourceCache.clear(); vector<double>().swap(xSourceCache); vector<double>().swap(ySourceCache);
	xTransformCache.clear(); yTransformCache.clear(); vector<double>().swap(xTransformCache); vector<double>().swap(yTransformCache);

	AGI.xBigGridLocat.clear(); AGI.yBigGridLocat.clear(); vector<float>().swap(AGI.xBigGridLocat); vector<float>().swap(AGI.yBigGridLocat);
	AGI.xBigGridValue.clear(); AGI.yBigGridValue.clear(); vector<float>().swap(AGI.xBigGridValue); vector<float>().swap(AGI.yBigGridValue);
	AGI.xSmallGridLocat.clear(); AGI.ySmallGridLocat.clear(); vector<float>().swap(AGI.xSmallGridLocat); vector<float>().swap(AGI.ySmallGridLocat);

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
	SSAAR[BoxName].SSAARenderOpen = 1;
	SSAAR[BoxName].SSAAOverride = Override; SSAAR[BoxName].FPS = FPS;

	//������������SSAAOverride==0 �� FPS==0ʱ ���������Ƶ��Ⱦ(������ͼƬ��Ⱦ)
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
	//���ݺ����������ĵ���Զ����(FarthestRadius)�ͺ������ű�������դ�����ű���;
	if (EGFB[BoxName].FarthestRadius.y > EGFB[BoxName].FarthestRadius.x) {//��XY��Ϊ�ǵȱ�������ʱ������֮��ѡ����Զ��FarthestRadius;
		if (EGFB[BoxName].FarthestRadius.y < 50) {
			//��FarthestRadius<50ʱդ����ʾ��С���̶�;
			EGFB[BoxName].GridZoomFactor.x = (50 + EGFB[BoxName].FarthestRadius.x) / EGFB[BoxName].AspectRatio.x;
			EGFB[BoxName].GridZoomFactor.y = (50 + EGFB[BoxName].FarthestRadius.y) / EGFB[BoxName].AspectRatio.y;
		} else {
			//��FarthestRadius>=50ʱդ�����ʾ��С�̶�;
			EGFB[BoxName].GridZoomFactor.x = 100 / EGFB[BoxName].AspectRatio.x;
			EGFB[BoxName].GridZoomFactor.y = 100 / EGFB[BoxName].AspectRatio.y;
		}
	} else {
		if (EGFB[BoxName].FarthestRadius.x < 50) {
			//��FarthestRadius<50ʱդ����ʾ��С���̶�;
			EGFB[BoxName].GridZoomFactor.x = (50 + EGFB[BoxName].FarthestRadius.x) / EGFB[BoxName].AspectRatio.x;
			EGFB[BoxName].GridZoomFactor.y = (50 + EGFB[BoxName].FarthestRadius.y) / EGFB[BoxName].AspectRatio.y;
		} else {
			//��FarthestRadius>=50ʱդ�����ʾ��С�̶�;
			EGFB[BoxName].GridZoomFactor.x = 100 / EGFB[BoxName].AspectRatio.x;
			EGFB[BoxName].GridZoomFactor.y = 100 / EGFB[BoxName].AspectRatio.y;
		}
	}

	//ͳ��դ����Ŀ;
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

	setlinestyle(PS_SOLID | PS_ENDCAP_SQUARE, Style.LineWidth * SSAAR[BoxName].SSAAOverride);

	if (RGB(128, 128, 128) <= getbkcolor()) Style.DrawingTheme = lightTheme; else Style.DrawingTheme = DarkTheme;//���ݱ���ɫ��������;

	//ShowBackgroundColor
	if (Style.ShowBackgroundColor == 1) {
		if (Style.DrawingTheme == lightTheme)setfillcolor(Style.BackgroundColor_light); else setfillcolor(Style.BackgroundColor_Dark);
		solidrectangle(EGFB[BoxName].x * SSAAR[BoxName].SSAAOverride, EGFB[BoxName].y * SSAAR[BoxName].SSAAOverride, (EGFB[BoxName].width + EGFB[BoxName].x) * SSAAR[BoxName].SSAAOverride, (EGFB[BoxName].height + EGFB[BoxName].y) * SSAAR[BoxName].SSAAOverride);
	}

	//����������ֵ����;
	if (Style.ShowAxisValue == 1) {
		setbkmode(TRANSPARENT);
		LOGFONT f; gettextstyle(&f);
		f.lfHeight = Style.AxisValueTextHeight * SSAAR[BoxName].SSAAOverride;
		f.lfQuality = ANTIALIASED_QUALITY | PROOF_QUALITY, f.lfCharSet = CHINESEBIG5_CHARSET;
		_tcscpy_s(f.lfFaceName, _T("Consolas"));
		settextstyle(&f); if (Style.DrawingTheme == lightTheme)settextcolor(Style.AxisValueColor_light); else settextcolor(Style.AxisValueColor_Dark);
	}

	//ShowSmallGrid;
	if (Style.ShowSmallGrid == 1) {
		if (Style.DrawingTheme == lightTheme)setlinecolor(Style.SmallGridColor_light); else setlinecolor(Style.SmallGridColor_Dark);
		for (int i = 0; i < AGI.xPathLineNum * 5; ++i) {//x;
			line(AGI.xSmallGridLocat[i] * SSAAR[BoxName].SSAAOverride, EGFB[BoxName].y * SSAAR[BoxName].SSAAOverride, AGI.xSmallGridLocat[i] * SSAAR[BoxName].SSAAOverride, (EGFB[BoxName].height + EGFB[BoxName].y) * SSAAR[BoxName].SSAAOverride);
		}
		for (int i = 0; i < AGI.yPathLineNum * 5; ++i) {//y;
			line(EGFB[BoxName].x * SSAAR[BoxName].SSAAOverride, AGI.ySmallGridLocat[i] * SSAAR[BoxName].SSAAOverride, (EGFB[BoxName].width + EGFB[BoxName].x) * SSAAR[BoxName].SSAAOverride, AGI.ySmallGridLocat[i] * SSAAR[BoxName].SSAAOverride);
		}
	}

	//ShowBigGrid;
	if (Style.ShowBigGrid == 1) {
		char s[32]{}; if (Style.DrawingTheme == lightTheme)setlinecolor(Style.BigGridColor_light); else setlinecolor(Style.BigGridColor_Dark);
		for (int i = 0; i < AGI.xPathLineNum; ++i) {//x;
			line(AGI.xBigGridLocat[i] * SSAAR[BoxName].SSAAOverride, EGFB[BoxName].y * SSAAR[BoxName].SSAAOverride, AGI.xBigGridLocat[i] * SSAAR[BoxName].SSAAOverride, (EGFB[BoxName].height + EGFB[BoxName].y) * SSAAR[BoxName].SSAAOverride);

			if (Style.ShowAxisValue == 1)sprintf(s, "%.2f", AGI.xBigGridValue[i]); outtextxy((AGI.xBigGridLocat[i] + 3) * SSAAR[BoxName].SSAAOverride, EGFB[BoxName].relOrigin.y * SSAAR[BoxName].SSAAOverride, s);
		}
		for (int i = 0; i < AGI.yPathLineNum; ++i) {//y;
			line(EGFB[BoxName].x * SSAAR[BoxName].SSAAOverride, AGI.yBigGridLocat[i] * SSAAR[BoxName].SSAAOverride, (EGFB[BoxName].width + EGFB[BoxName].x) * SSAAR[BoxName].SSAAOverride, AGI.yBigGridLocat[i] * SSAAR[BoxName].SSAAOverride);

			if (Style.ShowAxisValue == 1)sprintf(s, "%.2f", AGI.yBigGridValue[i]); outtextxy((EGFB[BoxName].relOrigin.x + 3) * SSAAR[BoxName].SSAAOverride, AGI.yBigGridLocat[i] * SSAAR[BoxName].SSAAOverride, s);
		}
	}

	//ShowAixs;
	if (Style.ShowAxis == 1) {
		if (Style.DrawingTheme == lightTheme)setlinecolor(Style.AxisColor_light); else setlinecolor(Style.AxisColor_Dark);
		line(EGFB[BoxName].x * SSAAR[BoxName].SSAAOverride, EGFB[BoxName].relOrigin.y * SSAAR[BoxName].SSAAOverride, (EGFB[BoxName].width + EGFB[BoxName].x) * SSAAR[BoxName].SSAAOverride, EGFB[BoxName].relOrigin.y * SSAAR[BoxName].SSAAOverride);
		line(EGFB[BoxName].relOrigin.x * SSAAR[BoxName].SSAAOverride, EGFB[BoxName].y * SSAAR[BoxName].SSAAOverride, EGFB[BoxName].relOrigin.x * SSAAR[BoxName].SSAAOverride, (EGFB[BoxName].height + EGFB[BoxName].y) * SSAAR[BoxName].SSAAOverride);
	}

	//ShowEdge;
	if (Style.ShowFuncGraphBOXedge == 1)rectangle(EGFB[BoxName].x * SSAAR[BoxName].SSAAOverride, EGFB[BoxName].y * SSAAR[BoxName].SSAAOverride, (EGFB[BoxName].width + EGFB[BoxName].x) * SSAAR[BoxName].SSAAOverride, (EGFB[BoxName].height + EGFB[BoxName].y) * SSAAR[BoxName].SSAAOverride);

	//ShowGravityCenter;
	if (Style.ShowGravityCenter == 1) {
		if (Style.DrawingTheme == lightTheme)setfillcolor(Style.GravityCenterColor_light); else setfillcolor(Style.GravityCenterColor_Dark);
		solidcircle((EGFB[BoxName].GravityCenter.x + EGFB[BoxName].absoOrigin.x) * SSAAR[BoxName].SSAAOverride, (EGFB[BoxName].GravityCenter.y + EGFB[BoxName].absoOrigin.y) * SSAAR[BoxName].SSAAOverride, Style.GravityCenterRadius * SSAAR[BoxName].SSAAOverride);
	}

	//ShowFunction;
	if (Style.DrawingTheme == lightTheme && Style.FunctionPointCustomColor.empty())setfillcolor(Style.FunctionColor_light); else setfillcolor(Style.FunctionColor_Dark);
	for (int i = 0; i < EGFB[BoxName].PointsNum; ++i) {
		if (!Style.FunctionPointCustomColor.empty())setfillcolor(Style.FunctionPointCustomColor[i]);

		if (!Style.FunctionPointCustomRadius.empty()) {
			solidcircle(xTransformCache[i] * SSAAR[BoxName].SSAAOverride, yTransformCache[i] * SSAAR[BoxName].SSAAOverride, Style.FunctionPointCustomRadius[i] * SSAAR[BoxName].SSAAOverride);
		} else {
			solidcircle(xTransformCache[i] * SSAAR[BoxName].SSAAOverride, yTransformCache[i] * SSAAR[BoxName].SSAAOverride, Style.FunctionPointRadius * SSAAR[BoxName].SSAAOverride);
		}
	}
	/*setfillcolor(RGB(43, 136, 216));
	for (int i = 0; i < EGFB[BoxName].PointsNum; ++i) { solidcircle(xTransformCache[i], yTransformCache[i], 0.5); }*/

	if (SSAAR[BoxName].SSAARenderOpen == 1)GetGraphBoxIMG(BoxName);//������Ⱦͼ����������Ŀ¼�µ�EXPGraphSSAAOutPut�ļ���
	EndBatchDraw();
}

void OFEXPGraph::CreateFunctionGraphBOX(int BoxName, int x, int y, int width, int height) {
	EGFB[BoxName].x = x; EGFB[BoxName].y = y; EGFB[BoxName].width = width; EGFB[BoxName].height = height;
	EGFB[BoxName].absoOrigin.x = (EGFB[BoxName].width / static_cast<double>(2) + x); EGFB[BoxName].absoOrigin.y = (EGFB[BoxName].height / static_cast<double>(2) + y);
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

	if (Style.ShowAxis == 1)AutoGrid(BoxName);//����դ��任;
	DrawFunction(BoxName);
	FreeCache(BoxName);
}

void OFEXPGraph::IMGtoVIDEO(int BoxName) {
	//��ȡϵͳʱ��;
	time_t timep; time(&timep);
	struct tm *nowTime = localtime(&timep);
	string nowTimeStr = to_string(1900 + nowTime->tm_year) + to_string(nowTime->tm_mon) + to_string(nowTime->tm_mday) + to_string(nowTime->tm_hour) + to_string(nowTime->tm_min) + to_string(nowTime->tm_sec);

	//����FFmpeg����;
	string Command = "ffmpeg -r " + to_string(SSAAR[BoxName].FPS) + " -f image2 -i EXPGraphSSAAOutPut/" + to_string(BoxName) + "%d.png EXPGraphSSAAOutPut_" + to_string(BoxName) + "_" + nowTimeStr + ".mp4";
	cout << "\nFFmpegCommand: " << Command << endl;
	int state = system(Command.c_str());//ִ��FFmpeg����;

	if (state == 0) {
		//��ȡ��������·��;
		char szFilePath[MAX_PATH + 1] = { 0 };
		GetModuleFileNameA(NULL, szFilePath, MAX_PATH);
		(strrchr(szFilePath, '\\'))[0] = 0; // ɾ���ļ�����ֻ���·���ִ�
		string path = szFilePath;

		//��Ƶ�����ɾ��EXPGraphSSAAOutPut�ļ���;

		//����EXPGraphSSAAOutPut�ļ��в�ɾ������ͼ���ļ�;
		for (int i = 0; i <= SSAAR[BoxName].Frame; ++i)remove((path + "\\EXPGraphSSAAOutPut\\" + to_string(BoxName) + to_string(i) + ".png").c_str());
		RemoveDirectory((path + "\\EXPGraphSSAAOutPut").c_str());//ɾ���յ�EXPGraphSSAAOutPut�ļ���;
	}
}