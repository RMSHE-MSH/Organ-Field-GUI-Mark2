#pragma once
#include "Universal.h"

typedef struct TaskManagerType {
	string Processlist;				//ϵͳ�����б�;
	LPCTSTR Target_strProcessName = NULL;	//Ҫ������Ŀ���������;
	BOOL ProcessKill_Result = FALSE;		//�������̵Ľ��(�����Ƿ񱻽���);
}TaskManagerType;

class OFTaskManager {
private:
	BOOL ProcessKill_Tree(LPCTSTR strProcessName);	//���ڽ������̵��߳�(Ŀ���������);
	string ProcessTraversal_Tree();					//����ϵͳ��ȫ�����̲���ȡ������Ϣ���߳�(��ȡϵͳ�����б�);
public:
	//TaskManagerType���͵���Ϣ�����ַ�ʽ�ܱ��ⲿ����;
	TaskManagerType TMT;
	TaskManagerType GetTaskManagerResult();

	void ProcessKill(LPCTSTR strProcessName);	//����Ŀ�����(Ŀ���������);
	void ProcessTraversal();					//��ȡϵͳ�����б�;
};