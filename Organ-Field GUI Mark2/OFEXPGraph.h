#pragma once
#include "Universal.h"
constexpr auto BigStandardUnit = 100;
constexpr auto SmallStandardUnit = 20;

typedef struct DoublePOINT { double x = NULL; double y = NULL; }DoublePOINT;

typedef struct DrawingStyle {
	bool ShowEXPGFuncGraphBOXedge = 1;
	bool ShowAxis = 1;
	bool ShowGravityCenter = 0;
	bool ShowFarthestRadius = 0;
	bool ShowFunction = 1;
}DrawingStyle;

typedef struct EXPFunctionPoints {
	int PointsNum = 0;
	DoublePOINT GravityCenter;
	float AspectRatio;
	float FarthestRadius;
	DoublePOINT relOrigin;
}EXPFunctionPoints;

typedef struct EXPGFuncGraphBOX {
	int x; int y; int width; int height;
	DoublePOINT absoOrigin;
	float GridZoomFactor = 1;

	EXPFunctionPoints *EGFP = new EXPFunctionPoints[32]{ NULL };
	DrawingStyle *EGDS = new DrawingStyle[32]{ NULL };
}EXPGFuncGraphBOX;

typedef struct AutoGridInfo {
	float yPathLineNum; float xPathLineNum;

	vector<float>xBigGridLocat; vector<float>yBigGridLocat;
	vector<float>xBigGridValue; vector<float>yBigGridValue;

	vector<float>xSmallGridLocat; vector<float>ySmallGridLocat;
}AutoGridInfo;

class OFEXPGraph {
private:
	EXPGFuncGraphBOX *EGFB = new EXPGFuncGraphBOX[32]{ NULL };

	vector<double>xSourceCache; vector<double>ySourceCache;
	vector<double>xTransformCache; vector<double>yTransformCache;

	DoublePOINT GravityCenterSolver(int BoxName, int FunctionName);
	void FarthestRadiusSolver(int BoxName, int FunctionName);
	void BoxSpaceTransformation(int BoxName, int FunctionName);

	AutoGridInfo AGI;
	void AutoGrid(int BoxName, int FunctionName);

	void DrawFunction(int BoxName, int FunctionName);

public:
	void CreateFunctionGraphBOX(int BoxName, int x, int y, int width, int height);
	void ImportNewFunction(int BoxName, int FunctionName, double xValue, double yValue);

	void AutoVisualDraw(int BoxName, int FunctionName);
};