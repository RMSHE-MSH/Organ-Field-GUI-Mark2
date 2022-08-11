#pragma once
#include "Universal.h"
#include "Window.h"

//ͼ�κ�(GraphBOX)���ⶨ��;
constexpr bool DarkTheme = 0;
constexpr bool lightTheme = 1;

//����Ӧ����ģʽ����;
constexpr bool RadiusTransformMode = 0; //����Ӧx,y�ȱ�����;
constexpr bool AxisTransformMode = 1;	//����Ӧx,y�ǵȱ�����;

//����˫���������;
typedef struct DoublePOINT { double x = NULL; double y = NULL; }DoublePOINT;

//ͼ�λ��Ʒ������;
typedef struct DrawingStyle {
	struct GraphBOXStyle {
		bool ShowBackground = 1;			//��ʾGraphBOX����(ֵΪ0ʱ����͸��);
		bool ShowFuncGraphBOXedge = 1;		//��ʾGraphBOX�߿�;
		bool ShowAxis = 1;					//��ʾ������;
		bool ShowAxisValue = 1;				//��ʾ��������ֵ;
		bool ShowBigGrid = 1;				//��ʾ��դ��;
		bool ShowSmallGrid = 1;				//��ʾСդ��;
		bool ShowGravityCenter = 1;			//��ʾ����(����)��;
		bool ShowFunctionLine = 0;			//��ʾ������ɢ�����������;

		short GravityCenterRadius = 6;		//������ʾ�뾶;
		short AxisValueTextHeight = 17;		//��������ֵ�ָ�;

		short GridLineWidth = 1;			//դ���߿�;
	}GraphBOXStyle;

	struct PointStyle {
		short StaticFunctionPointRadius = 1;		//'��'�ľ�̬�뾶;
		vector<float>DynamicFunctionPointRadius;	//'��'�Ķ�̬�뾶(����̬�뾶������ʱĬ��ʹ�þ�̬�뾶);

		COLORREF StaticFunctionPointColor = NULL;	//'��'�ľ�̬��ɫ(��̬��ɫ������,����̬��ɫ������ʱĬ��ʹ��������ɫ);
		vector<COLORREF>DynamicFunctionPointColor;	//'��'�Ķ�̬��ɫ(��̬��ɫ������,����̬��ɫ������ʱĬ��ʹ�þ�̬��ɫ);
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
		short FunctionLineWidth = 1;				//�������߿�;
		short LineType = PS_SOLID;					//����������;

		COLORREF StaticLineColor = NULL;			//�����߾�̬��ɫ(��̬��ɫ������,����̬��ɫ������ʱĬ��ʹ��������ɫ);
		vector<COLORREF>DynamicFunctionLineColor;	//�����߶�̬��ɫ(��̬��ɫ������,����̬��ɫ������ʱĬ��ʹ�þ�̬��ɫ);
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

	//���ƺ�����ɢ�����������(ָ��ͼ�κ�����,���ú�������,����ƫ����);
	int BoxName_DrawFunctionLine = NULL;
	int lineNum_DrawFunctionLine = NULL;
	int StaggerNum_DrawFunctionLine = 0;
	void DrawFunctionLine(int BoxName, int lineNum, int StaggerNum);
	/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------
	1.lineNum: һ������»��ߵ��������ڵ���ͼ�κ��еĺ�������(��lineNum����Ϊ1ʱ��ͼ�κ�ȫ��������ͳһΪһ����ɫ);
	����ȷ��һ����,���д����Ƶĵ㶼�Ǵ�����һ��'����'(vector)�е�,һ��ͼ�κе�����2�������ϵĺ���,������ߵ㲻������,��ô���еĵ㽫�����������е�λ����������,�⽫�������ֲ�ͬ�ĺ���;

	2.StaggerNum: �ӻ���ʱ�������й۲�,StaggerNumΪ��������ʱ������һ���(������)�ĸ���,����Ч�������й۲�(StaggerNumĬ��ӦΪ0);
	----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

	void FreeCache(int BoxName);

public:
	DrawingStyle Style;

	void CreateFunctionGraphBOX(int BoxName, int x, int y, int width, int height);
	void SetFunctionLine(int BoxName, int lineNum, int StaggerNum);					//���ƺ�����ɢ�����������(ָ��ͼ�κ�����,���ú�������,����ƫ����);
	void ImportNewFunction(int BoxName, double xValue, double yValue);

	void SSAARender(int BoxName, const float Override, const short FPS);

	void AutoVisualDraw(int BoxName, bool TransformMode);

	void IMGtoVIDEO(int BoxName);
};