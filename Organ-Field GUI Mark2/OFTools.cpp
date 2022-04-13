#include "OFTools.h"

BOOL OFTaskManager::ProcessKill_Tree(LPCTSTR strProcessName) {
	if (NULL == strProcessName) { return FALSE; }
	HANDLE handle32Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == handle32Snapshot) { return FALSE; }
	PROCESSENTRY32 pEntry;
	pEntry.dwSize = sizeof(PROCESSENTRY32);

	//Search for all the process and terminate it
	if (Process32First(handle32Snapshot, &pEntry)) {
		BOOL bFound = FALSE;
		if (!_tcsicmp(pEntry.szExeFile, strProcessName)) { bFound = TRUE; }

		while ((!bFound) && Process32Next(handle32Snapshot, &pEntry)) {
			if (!_tcsicmp(pEntry.szExeFile, strProcessName)) { bFound = TRUE; }
		}

		if (bFound) {
			CloseHandle(handle32Snapshot);
			HANDLE handLe = OpenProcess(PROCESS_TERMINATE, FALSE, pEntry.th32ProcessID);
			BOOL bResult = TerminateProcess(handLe, 0);
			return bResult;
		}
	}
	CloseHandle(handle32Snapshot);
	return FALSE;
}

void OFTaskManager::ProcessTraversal_Tree() {
	DWORD ProcessID; CHAR *ExeFile;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hSnapshot) {}
	PROCESSENTRY32 pi;
	pi.dwSize = sizeof(PROCESSENTRY32); //第一次使用必须初始化成员
	BOOL bRet = Process32First(hSnapshot, &pi);
	while (bRet) {
		ProcessID = pi.th32ProcessID; ExeFile = pi.szExeFile;

		cout << ProcessID << "    " << ExeFile << endl;

		bRet = Process32Next(hSnapshot, &pi);
	}
}

BOOL OFTaskManager::ProcessKill(LPCTSTR strProcessName) { thread OFTT0(&OFTaskManager::ProcessKill_Tree, this, strProcessName); OFTT0.detach(); }

void OFTaskManager::ProcessTraversal() { thread OFTT1(&OFTaskManager::ProcessTraversal_Tree, this); OFTT1.detach(); }