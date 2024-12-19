#include <Windows.h>
#include <stdio.h>


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
		return EXIT_FAILURE;
	}


	
	pThreadAttList = (PPROC_THREAD_ATTRIBUTE_LIST)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sThreadSize);
	if (pThreadAttList == NULL) {
		printf("[!] HeapAlloc Failed With Error : %d \n",GetLastError());
		return EXIT_FAILURE;
	}

	if (!InitializeProcThreadAttributeList(NULL, 1, NULL, &sThreadSize)) {
		printf("Initializing proc thread attribute list has failed !!\n");
		return EXIT_FAILURE;
	}




	if (!CreateProcessA(NULL, lpPath, NULL, NULL, FALSE, EXTENDED_STARTUPINFO_PRESENT, NULL, NULL, &SiEx.StartupInfo, &Pi)) {
		printf("Can't create the process !!!!\n");
		return EXIT_FAILURE;
	}

}
