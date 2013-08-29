#include <windows.h>
#include <tchar.h>
#include <string.h>

int WINAPI wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nShowCmd)
{
    TCHAR szFileName[MAX_PATH];
    GetModuleFileName(NULL, szFileName, MAX_PATH);
    int exeFileNameLen = _tcslen(szFileName);
    szFileName[exeFileNameLen - 3] = 'i';
    szFileName[exeFileNameLen - 2] = 'n';
    szFileName[exeFileNameLen - 1] = 'i';
    
    TCHAR szCmd[MAX_PATH];
    GetPrivateProfileString(L"GUILAUNCHER", L"exe", NULL, szCmd, MAX_PATH, szFileName);
    if (_tcslen(szCmd) == 0)
    {
        MessageBox(NULL, L"Failed to read [GUILAUNCHER]->exe value from ini file", L"Configuration Error", MB_ICONINFORMATION);
        return 2;
    }
    
    if (_tcslen(lpCmdLine) > 0)
    {
        _tcscat(szCmd, L" ");
        _tcscat(szCmd, lpCmdLine);
    }
    
    PROCESS_INFORMATION pi;
	STARTUPINFO si;    
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
    
    if (!CreateProcess(
		NULL,	/* No module name (use command line) */
		szCmd,	/* Command line */
		NULL,	/* Process handle not inheritable */
		NULL,	/* Thread handle not inheritable */
		FALSE,	/* Set handle inheritance to FALSE */
		0,	/* No creation flags */
		NULL,	/* Use parent's environment block */
		NULL,	/* Use parent's starting directory */
		&si,	/* Pointer to STARTUPINFO structure */
		&pi	/* Pointer to PROCESS_INFORMATION structure */
	))
    {
        MessageBox(NULL, L"Failed to launch exe", L"Error", MB_ICONINFORMATION);
    }
    
    CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);    
    return 0;
}
