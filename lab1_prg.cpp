#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <Lmcons.h>
#include <cstdlib>
#include <tchar.h>
#define WINVER 0x0502
#define BUFSIZE MAX_PATH 
#define FILESYSNAMEBUFSIZE MAX_PATH
#define MAX_KEY_LENGTH 255
#pragma warning(disable : 4996)
#define BUFFER_SIZE 32767;
using namespace std;

void CPUFrequency()
{
    long double dur;
    LARGE_INTEGER frequency, t_end , t_beg;
    QueryPerformanceCounter(&t_beg);
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&t_end);
    printf("\n2.1 CPU frequency:     %lu  HZ: ", frequency);
    dur = (1000000 * (long double(t_end.QuadPart) - long double(t_beg.QuadPart)) / long double(frequency.QuadPart));
    cout << "\n2.2 Measurement duration: " << dur << "  mcs";
}

int main()
{
    setlocale(LC_ALL, "Russian");
    OSVERSIONINFO OS_ver;
    OS_ver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&OS_ver);
    printf("1.1 OS version Windows %i.%i\n", OS_ver.dwMajorVersion, OS_ver.dwMinorVersion);


    TCHAR a[MAX_PATH];
    GetSystemDirectory(a, MAX_PATH);
    cout << "1.2 System Directory: ";
    wcout << a << endl;

    WCHAR user[UNLEN + 1];
    DWORD ULEN = UNLEN;
    GetUserNameW(user, &ULEN);
    cout << "1.3 Username: ";
    wcout << user << endl;

    WCHAR cname[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD CLEN = MAX_COMPUTERNAME_LENGTH;
    GetComputerNameW(cname, &CLEN);
    cout << "    Computer name: ";
    wcout << cname << endl;


    wchar_t buffer[MAX_PATH + 1];
    wchar_t name[MAX_PATH + 1];
    DWORD BufferLength = MAX_PATH;
    ULARGE_INTEGER freespace;
    ULARGE_INTEGER volume;
    BOOL success;
    int t = 0;
    cout << "1.4 Volume characteristics: ";
    HANDLE search = FindFirstVolume(buffer, sizeof(buffer));
    do
    {
        GetVolumePathNamesForVolumeName(name, buffer, BufferLength, &BufferLength);
        if (t != 0)
        {
            printf(" \n %s", name);
            printf(" \n %S", buffer);

            if (GetDiskFreeSpaceEx(name, &freespace, &volume, NULL) != 0)
            {
                //printf(" Total size: %S", volume.QuadPart, " bytes", "\n", "Free space: %S ", freespace.QuadPart, " bytes", "\n");
                cout << " Total size: ";
                wcout << volume.QuadPart;
                cout << " bytes" << endl << "Free space: ";
                wcout << freespace.QuadPart;
                cout << " bytes";
            }
            else
            {
                cout << " Buffer is empty";
            }

        }
        t++;
    } while (FindNextVolume(search, name, sizeof(buffer)));
    FindVolumeClose(search);

    
    
    DWORD size_name;
    DWORD size_data = BUFFER_SIZE;
    HKEY hkey;
    DWORD i = 0;
    wchar_t names[32767];
    wchar_t data[32767];
    printf("\n1.5 Startup commands:\n");
    RegOpenKeyEx(HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), 0, KEY_ALL_ACCESS, &hkey);
    do
    {
        size_name = sizeof(names);
        if (RegEnumValue(hkey, i, names, &size_name, NULL, REG_NONE, NULL, NULL) == ERROR_SUCCESS)
        {
            RegQueryValueEx(hkey, names, NULL, REG_NONE, (LPBYTE)data, &size_data);
            cout << "   " << i + 1 << "   ";
            wprintf(names);
            cout << "    ";
            wprintf(data);
            cout << endl;
            i++;
        }
        else break;
    } while (1);
    RegCloseKey(hkey);
    CPUFrequency();
}