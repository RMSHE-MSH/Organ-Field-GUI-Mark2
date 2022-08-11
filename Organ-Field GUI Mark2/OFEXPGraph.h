#pragma once
#include "Universal.h"
#include "Window.h"

//图形盒(GraphBOX)主题定义;
constexpr bool DarkTheme = 0;
constexpr bool lightTheme = 1;

//自适应缩放模式定义;
constexpr bool RadiusTransformMode = 0; //自适应x,y等比缩放;
constexpr bool AxisTransformMode = 1;	//自适应x,y非等比缩放;

//定义双精度坐标点;
typedef struct DoublePOINT { double x = NULL; double y = NULL; }DoublePOINT;

//图形绘制风格设置;
typedef struct DrawingStyle {
	struct GraphBOXStyle {
		bool ShowBackground = 1;			//显示GraphBOX背景(值为0时背景透明);
		bool ShowFuncGraphBOXedge = 1;		//显示GraphBOX边框;
		bool ShowAxis = 1;					//显示坐标轴;
		bool ShowAxisValue = 1;				//显示坐标轴数值;
		bool ShowBigGrid = 1;				//显示大栅格;
		bool ShowSmallGrid = 1;				//显示小栅格;
		bool ShowGravityCenter = 1;			//显示质心(重心)点;
		bool ShowFunctionLine = 0;			//显示函数离散点的两两连线;

		short GravityCenterRadius = 6;		//重心显示半径;
		short AxisValueTextHeight = 17;		//坐标轴数值字高;

		short GridLineWidth = 1;			//栅格线宽;
	}GraphBOXStyle;

	struct PointStyle {
		short StaticFunctionPointRadius = 1;		//'点'的静态半径;
		vector<float>DynamicFunctionPointRadius;	//'点'的动态半径(当动态半径不存在时默认使用静态半径);

		COLORREF StaticFunctionPointColor = NULL;	//'点'的静态颜色(静态颜色较优先,当静态颜色不存在时默认使用主题颜色);
		vector<COLORREF>DynamicFunctionPointColor;	//'点'的动态颜色(动态颜色最优先,当动态颜色不存在时默认使用静态颜色);
	}PointStyle;

	struct ThemeStyle {
		bool DefaultTheme = NULL;
		COLORREF BackgroundColor_light = RGB(255, 255, 255); COLORREF BackgroundColor_Dark = RGB(30, 30, 30);
		COLORREF FunctionColor_light = RGB(113, 175, 229); COLORREF FunctionColor_Dark = RGB(86, 156, 214);
		COLORREF FuncGraphBOXedgeColor_light = RGB(0, 0, 0); COLORREF FuncGraphBOXedgeColor_Dark = RGB(218, 218, 218);
		COLORREF AxisColor_light = RGB(0, 0, 0); COLORREF AxisColor_Dark = RGB(218, 218, 218);
		COLORREF AxisValueColor_light = RGB(30, 30, 30); COLORREF AxisValueColor_Dark = RGB(154, 154, 154);
		COLORREF BigGridColor_light = RGB(192, 192, 192); COLORREF BigGridColor_Dark = RGB(105, 105, 105);
		COLORREF SmallGridColor_light = RGB(235, 235, 235); COLORREF SmallGridColor_Dark = RGB(45, 45, 45);
		COLORREF GravityCenterColor_light = RGB(209, 52, 56); COLORREF GravityCenterColor_Dark = RGB(134, 27, 45);
		COLORREF LineColor_light = RGB(30, 30, 30); COLORREF LineColor_Dark = RGB(218, 218, 218);
	}ThemeStyle;

	struct LineStyle {
		short FunctionLineWidth = 1;				//函数线线宽;
		short LineType = PS_SOLID;					//函数线线型;

		COLORREF StaticLineColor = NULL;			//函数线静态颜色(静态颜色较优先,当静态颜色不存在时默认使用主题颜色);
		vector<COLORREF>DynamicFunctionLineColor;	//函数线动态颜色(动态颜色最优先,当动态颜色不存在时默认使用静态颜色);
	}LineStyle;
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

	//绘制函数离散点的两两连线(指定图形盒名称,设置函数个数,设置偏移数);
	int BoxName_DrawFunctionLine = NULL;
	int lineNum_DrawFunctionLine = NULL;
	int StaggerNum_DrawFunctionLine = 0;
	void DrawFunctionLine(int BoxName, int lineNum, int StaggerNum);
	/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------
	1.lineNum: 一般情况下画线的条数等于导入图形盒中的函数个数(当lineNum设置为1时则图形盒全局线条都统一为一种颜色);
	两点确定一条线,所有戴绘制的点都是储存在一个'数组'(vector)中的,一个图形盒导入了2个或以上的函数,如果连线点不经处理,那么所有的点将按照在数组中的位序两两相连,这将不会区分不同的函数;

	2.StaggerNum: 从绘制时的数组中观察,StaggerNum为两点连线时跳过的一组点(两个点)的个数,具体效果请自行观察(StaggerNum默认应为0);
	----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

	void FreeCache(int BoxName);

public:
	DrawingStyle Style;

	void CreateFunctionGraphBOX(int BoxName, int x, int y, int width, int height);
	void SetFunctionLine(int BoxName, int lineNum, int StaggerNum);					//绘制函数离散点的两两连线(指定图形盒名称,设置函数个数,设置偏移数);
	void ImportNewFunction(int BoxName, double xValue, double yValue);

	void SSAARender(int BoxName, const float Override, const short FPS);

	void AutoVisualDraw(int BoxName, bool TransformMode);

	void IMGtoVIDEO(int BoxName);
};