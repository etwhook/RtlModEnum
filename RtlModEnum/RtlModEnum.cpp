#include "Def.h"


DWORD GetProcessIdFromName(const char* targetProcessName) {

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    while (Process32Next(snapshot, &pe32)) {
        if (!strcmp(pe32.szExeFile, targetProcessName)) {
            CloseHandle(snapshot);
            return pe32.th32ProcessID;
        }
    }

    CloseHandle(snapshot);
    return -1;
}

int main(int argc, char* argv[])
{
    DWORD procId = GetProcessIdFromName(argv[1]);

    if (procId == -1) {
        printf("[-] Failed To Fetch Process Id.\n");
        return -1;
    }

	PRTL_DEBUG_INFORMATION dbgInfo = RtlCreateQueryDebugBuffer(0, FALSE);
	RtlQueryProcessDebugInformation(procId,
		0x01,
		dbgInfo);

	for (size_t i = 0; i < dbgInfo->Modules->NumberOfModules; i++)
	{
		auto moduleCurrent = dbgInfo->Modules->Modules[i];
		printf("[ %s ] [ %s ]\n", moduleCurrent.FullPathName, moduleCurrent.FullPathName + moduleCurrent.OffsetToFileName);
	}
}
