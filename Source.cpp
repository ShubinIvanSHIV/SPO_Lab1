#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <Lmcons.h>
#include <cstdlib>
#include <tchar.h>
#define WINVER 0x0502
#define BUFSIZE MAX_PATH
#define BUFSIZE MAX_PATH
#define FILESYSNAMEBUFSIZE MAX_PATH
#define INFO_BUFFER_SIZE 32767
#define MAX_KEY_LENGTH 255
#pragma warning(disable : 4996)
using namespace std;

int main()
{
    DWORD encodedVersion = GetVersion();
    double majorVersion = (LOBYTE(LOWORD(encodedVersion)));
    double minorVersion = (HIBYTE(LOWORD(encodedVersion)));
    cout << majorVersion << "." << minorVersion;
    //OSVERSIONINFO osvi = {0};
   // osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    //GetVersionInfo(&osvi);
    //cout << "OSversion: " << osvi.dwMajorVersion << "." << osvi.dwMinorVersion << "Build: " << osvi.dwBuildNumber;




    TCHAR a[MAX_PATH];
    GetSystemDirectory(a, MAX_PATH);
    cout << a << endl;

    WCHAR user[UNLEN + 1];
    DWORD ULEN = UNLEN;
    GetUserNameW(user, &ULEN);
    wcout << endl << user << endl;

    WCHAR cname[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD CLEN = MAX_COMPUTERNAME_LENGTH;
    GetComputerNameW(cname, &CLEN);
    wcout << cname << endl;


    TCHAR buffer[MAX_PATH];
    TCHAR name[MAX_PATH];
    DWORD BufferLength = MAX_PATH;
    ULARGE_INTEGER freespace;
    ULARGE_INTEGER volume;
    BOOL success;

    HANDLE search = FindFirstVolume(buffer, sizeof(buffer));
    do
    {
        GetVolumePathNamesForVolumeName(name, buffer, BufferLength, &BufferLength);
        printf("\n%s", name);
        printf("\n%s", buffer);
        /*cout << name;
        cout << buffer;*/
        if (GetDiskFreeSpaceEx(name, &freespace, &volume, NULL) != 0)
        {
            cout << " Total size: " << volume.QuadPart << " bytes" << "\nFree space: " << freespace.QuadPart << " bytes";
        }
        else
        {
            cout << " Buffer is empty";
        }
    } while (FindNextVolume(search, name, sizeof(buffer)));
    FindVolumeClose(search);

        DWORD dwSize;
        TCHAR szName[MAX_KEY_LENGTH];
        HKEY hKey;
        DWORD dwIndex = 0;
        DWORD retCode;
        DWORD BufferSize = 8192;
        PPERF_DATA_BLOCK PerfData = (PPERF_DATA_BLOCK)malloc(BufferSize);
        DWORD cbData = BufferSize;
    //Error "LSTATUS RegOpenKeyExW(HKEY,LPCWSTR,DWORD,REGSAM,PHKEY)": невозможно преобразовать аргумент 2 из "const char [46]" в "LPCWSTR"
        if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 
            0, KEY_ALL_ACCESS, &hKey) == !ERROR_SUCCESS)
        {
            printf("Function RegOpenKeyEx() failed! \n");
        }
        else printf("\n1.5. Startup commands:\n");
    
        while (1) {
            dwSize = sizeof(szName);
            retCode = RegEnumValue(hKey, dwIndex, szName, &dwSize, NULL, NULL, NULL, NULL);
            if (retCode == ERROR_SUCCESS)
            {
                RegQueryValueEx(hKey, szName, NULL, NULL, (LPBYTE)PerfData, &cbData);
                printf("     %d: %s:   %s\n", dwIndex + 1, szName, PerfData);
                    dwIndex++;
            }
            else break;
        }
        RegCloseKey(hKey);
    

    double PCFreq = 0.0;
    __int64 CounterStart = 0;
    void StartCounter();

    {
        LARGE_INTEGER li;
        if (!QueryPerformanceFrequency(&li))
            cout << "Function QueryPerfomanceFrequency() failed!\n";

        PCFreq = double(li.QuadPart);

        printf("\n2.1. CPU frequency %u Hz\n", li);
        QueryPerformanceCounter(&li);
        CounterStart = li.QuadPart;
    }
    double GetCounter();
    {
        LARGE_INTEGER li;
        QueryPerformanceCounter(&li);
        return double((li.QuadPart - CounterStart) * 1000000) / PCFreq;
    }
}
