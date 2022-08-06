#pragma once
#include "Universal.h"

typedef struct TaskManagerType {
	string Processlist;				//系统进程列表;
	LPCTSTR Target_strProcessName = NULL;	//要结束的目标进程名称;
	BOOL ProcessKill_Result = FALSE;		//结束进程的结果(进程是否被结束);
}TaskManagerType;

class OFTaskManager {
private:
	BOOL ProcessKill_Tree(LPCTSTR strProcessName);	//用于结束进程的线程(目标进程名称);
	string ProcessTraversal_Tree();					//遍历系统的全部进程并获取进程信息的线程(获取系统进程列表);
public:
	//TaskManagerType类型的信息有两种方式能被外部调用;
	TaskManagerType TMT;
	TaskManagerType GetTaskManagerResult();

	void ProcessKill(LPCTSTR strProcessName);	//结束目标进程(目标进程名称);
	void ProcessTraversal();					//获取系统进程列表;
};