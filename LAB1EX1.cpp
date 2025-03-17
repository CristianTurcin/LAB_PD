#include <windows.h>
#include <iostream>
#include <string>

void QueryImagePath(HKEY hKey, LPCWSTR subKey)
{
    HKEY hSubKey;
    if (RegOpenKeyEx(hKey, subKey, 0, KEY_READ, &hSubKey) == ERROR_SUCCESS)
    {
        WCHAR imagePath[MAX_PATH];
        DWORD size = sizeof(imagePath);
        DWORD type = REG_SZ;

        if (RegQueryValueEx(hSubKey, L"ImagePath", nullptr, &type, (LPBYTE)imagePath, &size) == ERROR_SUCCESS)
        {
            std::wcout << L"Subkey: " << subKey << L"\n";
            std::wcout << L"ImagePath: " << imagePath << L"\n\n";
        }

        RegCloseKey(hSubKey);
    }
}

void EnumerateServices()
{
    HKEY hKey;
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\Services", 0, KEY_READ, &hKey) == ERROR_SUCCESS)
    {
        WCHAR subKeyName[MAX_PATH];
        DWORD index = 0;
        DWORD size = MAX_PATH;

        while (RegEnumKeyEx(hKey, index, subKeyName, &size, nullptr, nullptr, nullptr, nullptr) == ERROR_SUCCESS)
        {
            std::wstring fullPath = L"SYSTEM\\CurrentControlSet\\Services\\" + std::wstring(subKeyName);
            QueryImagePath(HKEY_LOCAL_MACHINE, fullPath.c_str());

            size = MAX_PATH; // Reset size for the next iteration
            index++;
        }

        RegCloseKey(hKey);
    }
    else
    {
        std::wcerr << L"Failed to open registry key!\n";
    }
}

int main()
{
    EnumerateServices();
    return 0;
}
