#pragma once
#include "Universal.h"
#include "Window.h"

constexpr bool DarkTheme = 0;
constexpr bool lightTheme = 1;

constexpr bool RadiusTransformMode = 0; //自适应x,y等比缩放;
constexpr bool AxisTransformMode = 1;	//自适应x,y非等比缩放;

typedef struct DoublePOINT { double x = NULL; double y = NULL; }DoublePOINT;

typedef struct DrawingStyle {
	bool ShowBackgroundColor = 1;
	bool ShowFuncGraphBOXedge = 1;
	bool ShowAxis = 1;
	bool ShowAxisValue = 1;
	bool ShowBigGrid = 1;
	bool ShowSmallGrid = 1;
	bool ShowGravityCenter = 1;

	vector<COLORREF>FunctionPointCustomColor;
	vector<COLORREF>FunctionPointCustomRadius;

	bool DrawingTheme = lightTheme;
	COLORREF BackgroundColor_light = RGB(255, 255, 255); COLORREF BackgroundColor_Dark = RGB(30, 30, 30);
	COLORREF FunctionColor_light = RGB(113, 175, 229); COLORREF FunctionColor_Dark = RGB(86, 156, 214);
	COLORREF FuncGraphBOXedgeColor_light = RGB(0, 0, 0); COLORREF FuncGraphBOXedgeColor_Dark = RGB(218, 218, 218);
	COLORREF AxisColor_light = RGB(0, 0, 0); COLORREF AxisColor_Dark = RGB(218, 218, 218);
	COLORREF AxisValueColor_light = RGB(30, 30, 30); COLORREF AxisValueColor_Dark = RGB(154, 154, 154);
	COLORREF BigGridColor_light = RGB(192, 192, 192); COLORREF BigGridColor_Dark = RGB(105, 105, 105);
	COLORREF SmallGridColor_light = RGB(235, 235, 235); COLORREF SmallGridColor_Dark = RGB(45, 45, 45);
	COLORREF GravityCenterColor_light = RGB(209, 52, 56); COLORREF GravityCenterColor_Dark = RGB(134, 27, 45);

	short LineWidth = 1;
	short FunctionPointRadius = 1;
	short GravityCenterRadius = 6;
	short AxisValueTextHeight = 17;
}DrawingStyle;

typedef struct EXPGFuncGraphBOX {
	int x; int y; int width; int height;
	DoublePOINT absoOrigin;
	DoublePOINT GridZoomFactor;

	int PointsNum = 0;
	DoublePOINT GravityCenter;
	DoublePOINT AspectRatio;
	DoublePOINT FarthestRadius;
	DoublePOINT relOrigin;

	DrawingStyle *EGDS = new DrawingStyle[32]{ NULL };
}EXPGFuncGraphBOX;

typedef struct AutoGridInfo {
	float yPathLineNum = NULL; float xPathLineNum = NULL;

	vector<float>xBigGridLocat; vector<float>yBigGridLocat;
	vector<float>xBigGridValue; vector<float>yBigGridValue;

	vector<float>xSmallGridLocat; vector<float>ySmallGridLocat;
}AutoGridInfo;

typedef struct EXPGSSAARender {
	bool SSAARenderOpen = 0;
	IMAGE *SSAAIMGCache = NULL;
	float SSAAOverride = 1;
	short FPS = 0;
	long long Frame = 0;
};

class OFEXPGraph {
private:
	IMAGE *IMGCache = new IMAGE; POINT SourceWindowSize{ NULL };

	EXPGFuncGraphBOX *EGFB = new EXPGFuncGraphBOX[32]{ NULL };
	EXPGSSAARender *SSAAR = new EXPGSSAARender[32]{ NULL };

	vector<double>xSourceCache; vector<double>ySourceCache;
	vector<double>xTransformCache; vector<double>yTransformCache;

	DoublePOINT GravityCenterSolver(int BoxName);

	void FarthestRadiusSolver(int BoxName);
	void AxisFarthestSolver(int BoxName);

	void BoxSpaceTransformation_Radius(int BoxName);
	void BoxSpaceTransformation_Axis(int BoxName);

	AutoGridInfo AGI;
	void AutoGrid(int BoxName);

	void GetGraphBoxIMG(int BoxName);

	void DrawFunction(int BoxName);

	void FreeCache(int BoxName);

public:
	DrawingStyle Style;

	void CreateFunctionGraphBOX(int BoxName, int x, int y, int width, int height);
	void ImportNewFunction(int BoxName, double xValue, double yValue);

	void SSAARender(int BoxName, const float Override, const short FPS);

	void AutoVisualDraw(int BoxName, bool TransformMode);

	void IMGtoVIDEO(int BoxName);
};