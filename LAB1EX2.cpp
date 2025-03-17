#include <windows.h>
#include <iostream>
#include <string>

void QueryDriverImagePath(HKEY hKey, LPCWSTR subKey)
{
    HKEY hSubKey;
    if (RegOpenKeyEx(hKey, subKey, 0, KEY_READ, &hSubKey) == ERROR_SUCCESS)
    {
        DWORD typeValue = 0;
        DWORD typeSize = sizeof(typeValue);
        DWORD typeType = REG_DWORD;

        
        if (RegQueryValueEx(hSubKey, L"Type", nullptr, &typeType, (LPBYTE)&typeValue, &typeSize) == ERROR_SUCCESS)
        {
            if (typeValue == 0x01 || typeValue == 0x02)
            {
                WCHAR imagePath[MAX_PATH] = L"(None)";
                DWORD size = sizeof(imagePath);
                DWORD dataType = REG_SZ;

                // Citim valoarea "ImagePath"
                if (RegQueryValueEx(hSubKey, L"ImagePath", nullptr, &dataType, (LPBYTE)imagePath, &size) == ERROR_SUCCESS)
                {
                    std::wcout << L"Driver: " << subKey << L"\n";
                    std::wcout << L"ImagePath: " << imagePath << L"\n\n";
                }
            }
        }
        RegCloseKey(hSubKey);
    }
}

void EnumerateDrivers()
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
            QueryDriverImagePath(HKEY_LOCAL_MACHINE, fullPath.c_str());

            size = MAX_PATH;
            index++;
        }

        RegCloseKey(hKey);
    }
    else
    {
        std::wcerr << L"Eroare la deschiderea cheii de registry!\n";
    }
}

int main()
{
    EnumerateDrivers();
    return 0;
}
