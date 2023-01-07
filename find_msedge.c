/* find_msedge --- Find the location of Microsoft Edge web browser */
#include <windows.h>
#include <stdio.h>

#ifndef _countof
    #define _countof(array) (sizeof(array) / sizeof(array[0]))
#endif

BOOL get_msedge_path(HKEY hBaseKey, LPTSTR pszPath, DWORD cchPathMax)
{
    HKEY hKey;
    LONG error;

    pszPath[0] = 0;
    error = RegOpenKeyEx(hBaseKey,
                         TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\msedge.exe"),
                         0, KEY_READ, &hKey);
    if (error != ERROR_SUCCESS)
        return FALSE;

    cchPathMax *= sizeof(TCHAR);
    error = RegQueryValueEx(hKey, NULL, NULL, NULL, (LPBYTE)pszPath, &cchPathMax);
    pszPath[cchPathMax - 1] = 0;

    RegCloseKey(hKey);
    return (error == ERROR_SUCCESS) && (pszPath[0] != 0);
}

BOOL find_msedge(LPTSTR pszPath, DWORD cchPathMax)
{
    return get_msedge_path(HKEY_CURRENT_USER, pszPath, cchPathMax) ||
           get_msedge_path(HKEY_LOCAL_MACHINE, pszPath, cchPathMax);
}

int main(void)
{
    TCHAR szPath[MAX_PATH];
    BOOL ret = find_msedge(szPath, _countof(szPath));
    if (ret)
    {
#ifdef UNICODE
        printf("found: %ls\n", szPath);
#else
        printf("found: %s\n", szPath);
#endif
    }
    else
    {
        puts("not found");
    }

    return 0;
}