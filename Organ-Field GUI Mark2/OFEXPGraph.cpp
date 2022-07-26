#include "OFEXPGraph.h"

DoublePOINT OFEXPGraph::GravityCenterSolver(int BoxName, int FunctionName) {
	DoublePOINT PointsSum{ NULL }; DoublePOINT GravityCenter{ NULL };
	for (int i = 0; i < EGFB[BoxName].EGFP[FunctionName].PointsNum; ++i) {
		PointsSum.x = xSourceCache[i] + PointsSum.x;
		PointsSum.y = ySourceCache[i] + PointsSum.y;
	}
	GravityCenter.x = PointsSum.x / EGFB[BoxName].EGFP[FunctionName].PointsNum;
	GravityCenter.y = PointsSum.y / EGFB[BoxName].EGFP[FunctionName].PointsNum;

	EGFB[BoxName].EGFP[FunctionName].GravityCenter = GravityCenter;
	return GravityCenter;
}

void OFEXPGraph::FarthestRadiusSolver(int BoxName, int FunctionName) {
	float *relGravityRadius = new float[EGFB[BoxName].EGFP[FunctionName].PointsNum]{ NULL };
	for (int i = 0; i < EGFB[BoxName].EGFP[FunctionName].PointsNum; ++i) {
		relGravityRadius[i] = sqrt(pow((float(xSourceCache[i]) - float(EGFB[BoxName].EGFP[FunctionName].GravityCenter.x)), 2) + pow(float(ySourceCache[i]) - float(EGFB[BoxName].EGFP[FunctionName].GravityCenter.y), 2));
	}
	sort(relGravityRadius, relGravityRadius + EGFB[BoxName].EGFP[FunctionName].PointsNum, greater<float>());

	float absoGravityRadius = sqrt(pow(float(EGFB[BoxName].EGFP[FunctionName].GravityCenter.x), 2) + pow(float(EGFB[BoxName].EGFP[FunctionName].GravityCenter.y), 2));

	if (relGravityRadius[0] >= absoGravityRadius) {
		EGFB[BoxName].EGFP[FunctionName].FarthestRadius = relGravityRadius[0]; delete[] relGravityRadius;
	} else {
		EGFB[BoxName].EGFP[FunctionName].FarthestRadius = absoGravityRadius; delete[] relGravityRadius;
	};
}

void OFEXPGraph::BoxSpaceTransformation(int BoxName, int FunctionName) {
	if (EGFB[BoxName].width < EGFB[BoxName].height) {
		EGFB[BoxName].EGFP[FunctionName].AspectRatio = (0.5 * (EGFB[BoxName].width)) / EGFB[BoxName].EGFP[FunctionName].FarthestRadius;
	} else {
		EGFB[BoxName].EGFP[FunctionName].AspectRatio = (0.5 * (EGFB[BoxName].height)) / EGFB[BoxName].EGFP[FunctionName].FarthestRadius;
	}

	EGFB[BoxName].absoOrigin.x = EGFB[BoxName].absoOrigin.x - EGFB[BoxName].EGFP[FunctionName].GravityCenter.x;
	EGFB[BoxName].absoOrigin.y = EGFB[BoxName].absoOrigin.y - EGFB[BoxName].EGFP[FunctionName].GravityCenter.y;
	EGFB[BoxName].EGFP[FunctionName].relOrigin.x = -EGFB[BoxName].EGFP[FunctionName].GravityCenter.x * EGFB[BoxName].EGFP[FunctionName].AspectRatio + EGFB[BoxName].absoOrigin.x + EGFB[BoxName].EGFP[FunctionName].GravityCenter.x;
	EGFB[BoxName].EGFP[FunctionName].relOrigin.y = -EGFB[BoxName].EGFP[FunctionName].GravityCenter.y * EGFB[BoxName].EGFP[FunctionName].AspectRatio + EGFB[BoxName].absoOrigin.y + EGFB[BoxName].EGFP[FunctionName].GravityCenter.y;

	for (int i = 0; i < EGFB[BoxName].EGFP[FunctionName].PointsNum; ++i) {
		xTransformCache.push_back((xSourceCache[i] - EGFB[BoxName].EGFP[FunctionName].GravityCenter.x) * EGFB[BoxName].EGFP[FunctionName].AspectRatio + EGFB[BoxName].absoOrigin.x + EGFB[BoxName].EGFP[FunctionName].GravityCenter.x);
		yTransformCache.push_back((ySourceCache[i] - EGFB[BoxName].EGFP[FunctionName].GravityCenter.y) * EGFB[BoxName].EGFP[FunctionName].AspectRatio + EGFB[BoxName].absoOrigin.y + EGFB[BoxName].EGFP[FunctionName].GravityCenter.y);
	}
}

void OFEXPGraph::AutoGrid(int BoxName, int FunctionName) {
	if (EGFB[BoxName].GridZoomFactor == 0)EGFB[BoxName].GridZoomFactor = 1;

	float PyPathLineNum = (EGFB[BoxName].height + EGFB[BoxName].y - EGFB[BoxName].EGFP[FunctionName].relOrigin.y) / EGFB[BoxName].EGFP[FunctionName].AspectRatio / EGFB[BoxName].GridZoomFactor;
	float NyPathLineNum = (EGFB[BoxName].EGFP[FunctionName].relOrigin.y - EGFB[BoxName].y) / EGFB[BoxName].EGFP[FunctionName].AspectRatio / EGFB[BoxName].GridZoomFactor;
	float PxPathLineNum = (EGFB[BoxName].width + EGFB[BoxName].x - EGFB[BoxName].EGFP[FunctionName].relOrigin.x) / EGFB[BoxName].EGFP[FunctionName].AspectRatio / EGFB[BoxName].GridZoomFactor;
	float NxPathLineNum = (EGFB[BoxName].EGFP[FunctionName].relOrigin.x - EGFB[BoxName].x) / EGFB[BoxName].EGFP[FunctionName].AspectRatio / EGFB[BoxName].GridZoomFactor;
	AGI.yPathLineNum = PyPathLineNum + NyPathLineNum; AGI.xPathLineNum = PxPathLineNum + NxPathLineNum;

	//BigGridValue;
	for (int i = 0; i < PyPathLineNum; ++i) {//-y;
		AGI.yBigGridLocat.push_back(EGFB[BoxName].EGFP[FunctionName].relOrigin.y + i * EGFB[BoxName].EGFP[FunctionName].AspectRatio * EGFB[BoxName].GridZoomFactor);
		AGI.yBigGridValue.push_back(-i * EGFB[BoxName].GridZoomFactor);
	}
	for (int i = 0; i < NyPathLineNum; ++i) {//+y;
		AGI.yBigGridLocat.push_back(EGFB[BoxName].EGFP[FunctionName].relOrigin.y - i * EGFB[BoxName].EGFP[FunctionName].AspectRatio * EGFB[BoxName].GridZoomFactor);
		AGI.yBigGridValue.push_back(i * EGFB[BoxName].GridZoomFactor);
	}

	for (int i = 0; i < PxPathLineNum; ++i) {//-x;
		AGI.xBigGridLocat.push_back(EGFB[BoxName].EGFP[FunctionName].relOrigin.x + i * EGFB[BoxName].EGFP[FunctionName].AspectRatio * EGFB[BoxName].GridZoomFactor);
		AGI.xBigGridValue.push_back(i * EGFB[BoxName].GridZoomFactor);
	}
	for (int i = 0; i < NxPathLineNum; ++i) {//+x;
		AGI.xBigGridLocat.push_back(EGFB[BoxName].EGFP[FunctionName].relOrigin.x - i * EGFB[BoxName].EGFP[FunctionName].AspectRatio * EGFB[BoxName].GridZoomFactor);
		AGI.xBigGridValue.push_back(-i * EGFB[BoxName].GridZoomFactor);
	}

	//SmallGridValue;
	for (int i = 0; i < PyPathLineNum * 5; ++i) {//-y;
		AGI.ySmallGridLocat.push_back(EGFB[BoxName].EGFP[FunctionName].relOrigin.y + i * EGFB[BoxName].EGFP[FunctionName].AspectRatio * EGFB[BoxName].GridZoomFactor / 5);
	}
	for (int i = 0; i < NyPathLineNum * 5; ++i) {//+y;
		AGI.ySmallGridLocat.push_back(EGFB[BoxName].EGFP[FunctionName].relOrigin.y - i * EGFB[BoxName].EGFP[FunctionName].AspectRatio * EGFB[BoxName].GridZoomFactor / 5);
	}

	for (int i = 0; i < PxPathLineNum * 5; ++i) {//-x;
		AGI.xSmallGridLocat.push_back(EGFB[BoxName].EGFP[FunctionName].relOrigin.x + i * EGFB[BoxName].EGFP[FunctionName].AspectRatio * EGFB[BoxName].GridZoomFactor / 5);
	}
	for (int i = 0; i < NxPathLineNum * 5; ++i) {//+x;
		AGI.xSmallGridLocat.push_back(EGFB[BoxName].EGFP[FunctionName].relOrigin.x - i * EGFB[BoxName].EGFP[FunctionName].AspectRatio * EGFB[BoxName].GridZoomFactor / 5);
	}
}

void OFEXPGraph::DrawFunction(int BoxName, int FunctionName) {
	//BeginBatchDraw();
	FlushBatchDraw();

	setbkmode(TRANSPARENT);
	LOGFONT f; gettextstyle(&f);
	f.lfHeight = 18;
	f.lfQuality = ANTIALIASED_QUALITY | PROOF_QUALITY, f.lfCharSet = CHINESEBIG5_CHARSET;
	_tcscpy_s(f.lfFaceName, _T("Consolas"));
	settextstyle(&f); settextcolor(RGB(0, 0, 0));

	AutoGrid(BoxName, FunctionName);
	setlinecolor(RGB(240, 240, 240));
	for (int i = 0; i < AGI.xPathLineNum * 5; ++i) {
		line(AGI.xSmallGridLocat[i], EGFB[BoxName].y, AGI.xSmallGridLocat[i], EGFB[BoxName].height + EGFB[BoxName].y);
	}
	for (int i = 0; i < AGI.yPathLineNum * 5; ++i) {
		line(EGFB[BoxName].x, AGI.ySmallGridLocat[i], EGFB[BoxName].width + EGFB[BoxName].x, AGI.ySmallGridLocat[i]);
	}

	char s[5]; setlinecolor(RGB(192, 192, 192));
	for (int i = 0; i < AGI.xPathLineNum; ++i) {
		line(AGI.xBigGridLocat[i], EGFB[BoxName].y, AGI.xBigGridLocat[i], EGFB[BoxName].height + EGFB[BoxName].y);
		sprintf(s, "%d", (int)AGI.xBigGridValue[i]); outtextxy(AGI.xBigGridLocat[i] + 3, EGFB[BoxName].EGFP[FunctionName].relOrigin.y, s);
	}
	for (int i = 0; i < AGI.yPathLineNum; ++i) {
		line(EGFB[BoxName].x, AGI.yBigGridLocat[i], EGFB[BoxName].width + EGFB[BoxName].x, AGI.yBigGridLocat[i]);
		sprintf(s, "%d", (int)AGI.yBigGridValue[i]); outtextxy(EGFB[BoxName].EGFP[FunctionName].relOrigin.x + 3, AGI.yBigGridLocat[i], s);
	}

	setlinecolor(RGB(0, 0, 0));
	line(EGFB[BoxName].x, EGFB[BoxName].EGFP[FunctionName].relOrigin.y, EGFB[BoxName].width + EGFB[BoxName].x, EGFB[BoxName].EGFP[FunctionName].relOrigin.y);
	line(EGFB[BoxName].EGFP[FunctionName].relOrigin.x, EGFB[BoxName].y, EGFB[BoxName].EGFP[FunctionName].relOrigin.x, EGFB[BoxName].height + EGFB[BoxName].y);

	setfillcolor(RGB(209, 52, 56));
	solidcircle(EGFB[BoxName].EGFP[FunctionName].GravityCenter.x + EGFB[BoxName].absoOrigin.x, EGFB[BoxName].EGFP[FunctionName].GravityCenter.y + EGFB[BoxName].absoOrigin.y, 6);

	rectangle(EGFB[BoxName].x, EGFB[BoxName].y, EGFB[BoxName].width + EGFB[BoxName].x, EGFB[BoxName].height + EGFB[BoxName].y);

	setfillcolor(RGB(113, 175, 229));
	for (int i = 0; i < EGFB[BoxName].EGFP[FunctionName].PointsNum; ++i) { solidcircle(xTransformCache[i], yTransformCache[i], 1); }
	setfillcolor(RGB(43, 136, 216));
	for (int i = 0; i < EGFB[BoxName].EGFP[FunctionName].PointsNum; ++i) { solidcircle(xTransformCache[i], yTransformCache[i], 0.5); }

	EndBatchDraw();
}

void OFEXPGraph::CreateFunctionGraphBOX(int BoxName, int x, int y, int width, int height) {
	EGFB[BoxName].x = x; EGFB[BoxName].y = y; EGFB[BoxName].width = width; EGFB[BoxName].height = height;
	EGFB[BoxName].absoOrigin.x = (EGFB[BoxName].width / static_cast<double>(2) + x); EGFB[BoxName].absoOrigin.y = (EGFB[BoxName].height / static_cast<double>(2) + y);
}

void OFEXPGraph::ImportNewFunction(int BoxName, int FunctionName, double xValue, double yValue) {
	xSourceCache.push_back(xValue); ySourceCache.push_back(-yValue); ++EGFB[BoxName].EGFP[FunctionName].PointsNum;
}

void OFEXPGraph::AutoVisualDraw(int BoxName, int FunctionName) {
	GravityCenterSolver(BoxName, FunctionName);
	FarthestRadiusSolver(BoxName, FunctionName);
	BoxSpaceTransformation(BoxName, FunctionName);
	DrawFunction(BoxName, FunctionName);

	//cout << "EGFB[" << BoxName << "].AspectRatio = " << EGFB[BoxName].EGFP[FunctionName].AspectRatio << endl;

	EGFB[BoxName].EGFP[FunctionName].PointsNum = 0;
	xSourceCache.clear(); ySourceCache.clear(); vector<double>().swap(xSourceCache); vector<double>().swap(ySourceCache);
	xTransformCache.clear(); yTransformCache.clear(); vector<double>().swap(xTransformCache); vector<double>().swap(yTransformCache);

	AGI.xBigGridLocat.clear(); AGI.yBigGridLocat.clear(); vector<float>().swap(AGI.xBigGridLocat); vector<float>().swap(AGI.yBigGridLocat);
	AGI.xBigGridValue.clear(); AGI.yBigGridValue.clear(); vector<float>().swap(AGI.xBigGridValue); vector<float>().swap(AGI.yBigGridValue);
	AGI.xSmallGridLocat.clear(); AGI.ySmallGridLocat.clear(); vector<float>().swap(AGI.xSmallGridLocat); vector<float>().swap(AGI.ySmallGridLocat);
}