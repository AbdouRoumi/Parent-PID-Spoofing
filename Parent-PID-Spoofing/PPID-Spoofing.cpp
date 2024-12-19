#include <Windows.h>
#include <stdio.h>


BOOL CreatePPidSpoofedProcess(IN LPCSTR lpProcessName) {
	HANDLE hProcess;
	CHAR					lpPath[MAX_PATH * 2];
	CHAR					WnDr[MAX_PATH];
	CHAR					CurrentDir[MAX_PATH];

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


	if (!CreateProcessA(NULL, lpPath, NULL, NULL, FALSE, EXTENDED_STARTUPINFO_PRESENT, NULL, NULL, &SiEx.StartupInfo, &Pi)) {
		printf("Can't create the process !!!!\n");
		return EXIT_FAILURE;
	}

}
