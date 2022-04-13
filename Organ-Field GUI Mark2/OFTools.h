#pragma once
#include "Universal.h"

class OFTaskManager {
private:
	BOOL ProcessKill_Tree(LPCTSTR strProcessName);
	void ProcessTraversal_Tree();
public:
	BOOL ProcessKill(LPCTSTR strProcessName);
	void ProcessTraversal();
};