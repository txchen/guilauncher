#include <windows.h>
#include <tchar.h>
#include <string.h>
#include <strsafe.h>
#include <shellapi.h>

void ErrorExit(LPTSTR lpszFunction)
{
    LPTSTR lpMsgBuf;
    DWORD dw = GetLastError();
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

    LPTSTR lpDisplayBuf = (LPTSTR)LocalAlloc(LMEM_ZEROINIT,
        (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
    if (lpDisplayBuf != NULL)
    {
        StringCchPrintf(lpDisplayBuf,
            LocalSize(lpDisplayBuf) / sizeof(TCHAR),
            TEXT("%s\r\nError code: %d\r\nError description:%s"),
            lpszFunction, (int)dw, lpMsgBuf);
        MessageBox(NULL, lpDisplayBuf, TEXT("Error"), MB_OK);

        LocalFree(lpDisplayBuf);
    }
    LocalFree(lpMsgBuf);
    ExitProcess(dw);
}

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

    UINT runAsAdmin = GetPrivateProfileInt(L"GUILAUNCHER", L"runasadmin", 0, szFileName);

    SHELLEXECUTEINFO shExecInfo = {0};
    shExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
    shExecInfo.fMask = NULL;
    shExecInfo.hwnd = NULL;
    shExecInfo.lpVerb = runAsAdmin == 1 ? L"runas" : L"open";
    shExecInfo.lpFile = szCmd;
    shExecInfo.lpParameters = lpCmdLine;
    shExecInfo.lpDirectory = NULL;
    shExecInfo.nShow = SW_SHOW;
    shExecInfo.hInstApp = NULL;
    if (!ShellExecuteEx(&shExecInfo))
    {
        ErrorExit(L"Failed to launch executable");
    }
    return 0;
}
