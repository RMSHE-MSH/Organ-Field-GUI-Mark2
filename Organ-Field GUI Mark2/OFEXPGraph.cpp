#include "OFEXPGraph.h"
Window EXPG_win;

DoublePOINT OFEXPGraph::GravityCenterSolver(int BoxName) {
	DoublePOINT PointsTorqueSum{ NULL }; double MassSum{ NULL }; DoublePOINT GravityCenter{ NULL };

	//��Style.FunctionPointCustomRadius(���е�İ뾶)��Ϊ��ʱ;
	if (!Style.PointStyle.DynamicFunctionPointRadius.empty()) {
		//�����е���������ͬʱ,��������λ��;
		//ע����EXPGraph�����ᵽ�ĵ㲢�����ʵ�,����������ܶ������������,�����ܶ�Ϊ����;
		//�������ĵ���⹫ʽ:���� = ϵͳ�������ľ� / ϵͳ������;
		//������������ü��㷨��ʽ��DynamicFunctionPointRadius[i]ԭ��ӦΪ�������,��ΪĬ�����е���ܶ���ͬ,������������ֻ���ֱ�����ϵ,ֱ��ʹ�����������������߼����ٶ�;

		//����TorqueSum��MassSum;
		for (int i = 0; i < EGFB[BoxName].PointsNum; ++i) {
			//Torque = Mass x r;
			//DynamicFunctionPointRadius����������(Ĭ�����е��ܶ���ͬ,Ϊ����߼����ٶ���ֻ���ֱ�����ϵ);
			PointsTorqueSum.x = Style.PointStyle.DynamicFunctionPointRadius[i] * xSourceCache[i] + PointsTorqueSum.x;
			PointsTorqueSum.y = Style.PointStyle.DynamicFunctionPointRadius[i] * ySourceCache[i] + PointsTorqueSum.y;

			MassSum = Style.PointStyle.DynamicFunctionPointRadius[i] + MassSum;
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
		//GravityCenter = Locat_Sum / PointsNum_Sum;
		GravityCenter.x = PointsTorqueSum.x / EGFB[BoxName].PointsNum;
		GravityCenter.y = PointsTorqueSum.y / EGFB[BoxName].PointsNum;
	}

	EGFB[BoxName].GravityCenter = GravityCenter;
	return GravityCenter;
}

void OFEXPGraph::FarthestRadiusSolver(int BoxName) {
	//���㺯������㵽���ĵ���С����;
	float *relGravityRadius = new float[EGFB[BoxName].PointsNum] { NULL };
	float *FunctionPointCustomRadius = new float[EGFB[BoxName].PointsNum] { NULL };
	for (int i = 0; i < EGFB[BoxName].PointsNum; ++i) {
		relGravityRadius[i] = sqrt(pow((float(xSourceCache[i]) - float(EGFB[BoxName].GravityCenter.x)), 2) + pow(float(ySourceCache[i]) - float(EGFB[BoxName].GravityCenter.y), 2));

		//��FunctionPointCustomRadius��Ϊ��ʱ(�����Զ���뾶ʱ);
		if (!Style.PointStyle.DynamicFunctionPointRadius.empty())FunctionPointCustomRadius[i] = Style.PointStyle.DynamicFunctionPointRadius[i];//ת������(���ֱ��sort DynamicFunctionPointRadius,��ôvector�����ݻᱻ�������Ļ�ͼ��Ⱦ�޷���ȷ����);
	}

	//Ѱ�����뾶(�������ĵ���Զ��);
	sort(relGravityRadius, relGravityRadius + EGFB[BoxName].PointsNum, greater<float>());
	//Ѱ������뾶(�����е�뾶��);
	sort(FunctionPointCustomRadius, FunctionPointCustomRadius + EGFB[BoxName].PointsNum, greater<float>());

	//����������ԭ�����С����;
	float absoGravityRadius = sqrt(pow(float(EGFB[BoxName].GravityCenter.x), 2) + pow(float(EGFB[BoxName].GravityCenter.y), 2));

	//�Ƚ�;
	if (relGravityRadius[0] >= absoGravityRadius) {
		EGFB[BoxName].FarthestRadius.x = EGFB[BoxName].FarthestRadius.y = relGravityRadius[0] + FunctionPointCustomRadius[0];
	} else {
		EGFB[BoxName].FarthestRadius.x = EGFB[BoxName].FarthestRadius.y = absoGravityRadius;
	};
	//�ͷ��ڴ�;
	delete[] relGravityRadius; delete[] FunctionPointCustomRadius;
}

void OFEXPGraph::AxisFarthestSolver(int BoxName) {
	//�ֱ�������ắ������㵽���ĵ���С����;
	float *XrelGravity = new float[EGFB[BoxName].PointsNum] { NULL };
	float *YrelGravity = new float[EGFB[BoxName].PointsNum] { NULL };
	float *FunctionPointCustomRadius = new float[EGFB[BoxName].PointsNum] { NULL };
	for (int i = 0; i < EGFB[BoxName].PointsNum; ++i) {
		XrelGravity[i] = abs(float(xSourceCache[i]) - float(EGFB[BoxName].GravityCenter.x));
		YrelGravity[i] = abs((float(ySourceCache[i]) - float(EGFB[BoxName].GravityCenter.y)));

		//��FunctionPointCustomRadius��Ϊ��ʱ(�����Զ���뾶ʱ);
		if (!Style.PointStyle.DynamicFunctionPointRadius.empty())FunctionPointCustomRadius[i] = Style.PointStyle.DynamicFunctionPointRadius[i];//ת������(���ֱ��sort DynamicFunctionPointRadius,��ôvector�����ݻᱻ�������Ļ�ͼ��Ⱦ�޷���ȷ����);
	}

	//�ֱ�Ѱ���������뾶(�������ĵ���Զ��);
	sort(XrelGravity, XrelGravity + EGFB[BoxName].PointsNum, greater<float>());
	sort(YrelGravity, YrelGravity + EGFB[BoxName].PointsNum, greater<float>());
	//Ѱ������뾶(�����е�뾶��);
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
	//�ͷ��ڴ�;
	delete[] XrelGravity; delete[] YrelGravity; delete[] FunctionPointCustomRadius;
}

//����Ӧ�ȱ�����ģʽ;
void OFEXPGraph::BoxSpaceTransformation_Radius(int BoxName) {
	//�ڵȱ�����ʱ��������������ͬ,��EGFB[BoxName].AspectRatio.y = EGFB[BoxName].AspectRatio.x;
	//��֤���������뺯����Զ��뾶(���뾶FarthestRadius)������BoxSpace����С�߾�;
	if (EGFB[BoxName].width < EGFB[BoxName].height) {
		EGFB[BoxName].AspectRatio.y = EGFB[BoxName].AspectRatio.x = (0.5 * (EGFB[BoxName].width)) / EGFB[BoxName].FarthestRadius.x;//�����ǵȱ������ž���FarthestRadiusSolver������FarthestRadius.x��.y��һ�µ�;
	} else {
		EGFB[BoxName].AspectRatio.y = EGFB[BoxName].AspectRatio.x = (0.5 * (EGFB[BoxName].height)) / EGFB[BoxName].FarthestRadius.x;//�����ǵȱ������ž���FarthestRadiusSolver������FarthestRadius.x��.y��һ�µ�;
	}

	//������ԭ����б任;
	//�ⲿ����д��������,�Լ�����(ע���п��ܲ���)!

	/*----------------------------------------------------------------------
	<�ο�ϵ>
	1.�Դ������Ͻ�Ϊԭ��ľ��Բο�ϵΪ����ο�ϵ;
	2.�ڵ���ο�ϵ�´�������һ�ο�ϵΪ��һ�ο�ϵ,����Ļ��ʵ����ʾ���������ԭ��;
	3.�ڵ�һ�ο�ϵ��������Ϊԭ��Ĳο�ϵΪ�ڶ��ο�ϵ;
	-----------------------------------------------------------------------*/

	//�����һ�ο�ϵ����ԭ������;
	EGFB[BoxName].absoOrigin.x = EGFB[BoxName].absoOrigin.x - EGFB[BoxName].GravityCenter.x;
	EGFB[BoxName].absoOrigin.y = EGFB[BoxName].absoOrigin.y - EGFB[BoxName].GravityCenter.y;

	//�����һ�ο�ϵ��Եڶ��ο�ϵԭ������;
	EGFB[BoxName].relOrigin.x = -EGFB[BoxName].GravityCenter.x * EGFB[BoxName].AspectRatio.x + EGFB[BoxName].absoOrigin.x + EGFB[BoxName].GravityCenter.x;
	EGFB[BoxName].relOrigin.y = -EGFB[BoxName].GravityCenter.y * EGFB[BoxName].AspectRatio.y + EGFB[BoxName].absoOrigin.y + EGFB[BoxName].GravityCenter.y;

	//�Ժ������б任;
	for (int i = 0; i < EGFB[BoxName].PointsNum; ++i) {
		xTransformCache.push_back((xSourceCache[i] - EGFB[BoxName].GravityCenter.x) * EGFB[BoxName].AspectRatio.x + EGFB[BoxName].absoOrigin.x + EGFB[BoxName].GravityCenter.x);
		yTransformCache.push_back((ySourceCache[i] - EGFB[BoxName].GravityCenter.y) * EGFB[BoxName].AspectRatio.y + EGFB[BoxName].absoOrigin.y + EGFB[BoxName].GravityCenter.y);
	}
}

//����Ӧ�ǵȱ�����ģʽ;
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
	if (Style.LineStyle.StaticLineColor != NULL) {//��������Զ���'��'�ľ�̬��ɫ������ʹ�þ�̬��ɫ(�����������ɫ),���û�о�̬��ɫ��ʹ��������ɫ;
		setlinecolor(Style.LineStyle.StaticLineColor);
	} else if (Style.ThemeStyle.DefaultTheme == lightTheme)setlinecolor(Style.ThemeStyle.LineColor_light); else setlinecolor(Style.ThemeStyle.LineColor_Dark);

	setlinestyle(Style.LineStyle.LineType | PS_ENDCAP_ROUND, Style.LineStyle.FunctionLineWidth * SSAAR[BoxName].SSAAOverride);//�����߿�;

	for (int i = lineNum; i < EGFB[BoxName].PointsNum; i += StaggerNum + 1) {
		//��������Զ���'��'�Ķ�̬��ɫ��ô��̬��ɫ�����ȱ�ʹ��;
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
	SSAAR[BoxName].SSAARenderOpen = 1;//���SSAA��Ⱦ����;
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

	setlinestyle(PS_SOLID | PS_ENDCAP_SQUARE, Style.GraphBOXStyle.GridLineWidth * SSAAR[BoxName].SSAAOverride);

	//��û����ָ������������¸��ݱ���ɫ����Ĭ������(���������ָ����������򲻻��Զ�ѡ��);
	if (RGB(128, 128, 128) <= getbkcolor() && Style.ThemeStyle.DefaultTheme == NULL) Style.ThemeStyle.DefaultTheme = lightTheme; else Style.ThemeStyle.DefaultTheme = DarkTheme;

	//ShowBackgroundColor
	if (Style.GraphBOXStyle.ShowBackground == 1) {
		if (Style.ThemeStyle.DefaultTheme == lightTheme)setfillcolor(Style.ThemeStyle.BackgroundColor_light); else setfillcolor(Style.ThemeStyle.BackgroundColor_Dark);
		solidrectangle(EGFB[BoxName].x * SSAAR[BoxName].SSAAOverride, EGFB[BoxName].y * SSAAR[BoxName].SSAAOverride, (EGFB[BoxName].width + EGFB[BoxName].x) * SSAAR[BoxName].SSAAOverride, (EGFB[BoxName].height + EGFB[BoxName].y) * SSAAR[BoxName].SSAAOverride);
	}

	//����������ֵ����;
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

	//���Ƶ�֮�������;
	if (Style.GraphBOXStyle.ShowFunctionLine == 1)DrawFunctionLine(BoxName_DrawFunctionLine, lineNum_DrawFunctionLine, StaggerNum_DrawFunctionLine);

	//ShowFunctionPoints;
	if (Style.PointStyle.StaticFunctionPointColor != NULL) {//��������Զ���'��'�ľ�̬��ɫ������ʹ�þ�̬��ɫ(�����������ɫ),���û�о�̬��ɫ��ʹ��������ɫ;
		setfillcolor(Style.PointStyle.StaticFunctionPointColor);
	} else if (Style.ThemeStyle.DefaultTheme == lightTheme)setfillcolor(Style.ThemeStyle.FunctionColor_light); else setfillcolor(Style.ThemeStyle.FunctionColor_Dark);

	for (int i = 0; i < EGFB[BoxName].PointsNum; ++i) {
		//��������Զ���'��'�Ķ�̬��ɫ��ô��̬��ɫ�����ȱ�ʹ��;
		if (!Style.PointStyle.DynamicFunctionPointColor.empty())setfillcolor(Style.PointStyle.DynamicFunctionPointColor[i]);

		//������Զ�����뾶��ʹ���Զ���뾶,������ʹ��ͳһ�뾶;
		if (!Style.PointStyle.DynamicFunctionPointRadius.empty()) {
			//�ڿ������ŵ�������,������Բ���о����ĸ���������;
			float Recleft = (xTransformCache[i] - EGFB[BoxName].AspectRatio.x * Style.PointStyle.DynamicFunctionPointRadius[i]) * SSAAR[BoxName].SSAAOverride;
			float RecTop = (yTransformCache[i] - EGFB[BoxName].AspectRatio.y * Style.PointStyle.DynamicFunctionPointRadius[i]) * SSAAR[BoxName].SSAAOverride;
			float RecRight = (xTransformCache[i] + EGFB[BoxName].AspectRatio.x * Style.PointStyle.DynamicFunctionPointRadius[i]) * SSAAR[BoxName].SSAAOverride;
			float RecBottom = (yTransformCache[i] + EGFB[BoxName].AspectRatio.y * Style.PointStyle.DynamicFunctionPointRadius[i]) * SSAAR[BoxName].SSAAOverride;

			//������Բ;
			solidellipse(Recleft, RecTop, RecRight, RecBottom);

			//����Բ;
			//solidcircle(xTransformCache[i] * SSAAR[BoxName].SSAAOverride, yTransformCache[i] * SSAAR[BoxName].SSAAOverride, Style.FunctionPointCustomRadius[i] * SSAAR[BoxName].SSAAOverride);
		} else {
			solidcircle(xTransformCache[i] * SSAAR[BoxName].SSAAOverride, yTransformCache[i] * SSAAR[BoxName].SSAAOverride, Style.PointStyle.StaticFunctionPointRadius * SSAAR[BoxName].SSAAOverride);
		}
	}

	if (SSAAR[BoxName].SSAARenderOpen == 1)GetGraphBoxIMG(BoxName);//������Ⱦͼ����������Ŀ¼�µ�EXPGraphSSAAOutPut�ļ���
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

	if (Style.GraphBOXStyle.ShowAxis == 1)AutoGrid(BoxName);//����դ��任;
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