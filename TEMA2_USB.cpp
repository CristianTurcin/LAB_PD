#include <windows.h>
#include <setupapi.h>
#include <devguid.h>
#include <regstr.h>
#include <iostream>
#include <string>

#pragma comment(lib, "setupapi.lib")


static const GUID GUID_DEVINTERFACE_USB_DEVICE =
{ 0xA5DCBF10L, 0x6530, 0x11D2,
  { 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED } };

int main() {
        std::wcout.imbue(std::locale(""));


    HDEVINFO deviceInfoSet = SetupDiGetClassDevs(
        &GUID_DEVINTERFACE_USB_DEVICE,
        NULL,
        NULL,
        DIGCF_PRESENT | DIGCF_DEVICEINTERFACE
    );

    if (deviceInfoSet == INVALID_HANDLE_VALUE) {
        std::cerr << "Eroare la obținerea dispozitivelor USB." << std::endl;
        return 1;
    }

    SP_DEVICE_INTERFACE_DATA deviceInterfaceData;
    deviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

    int index = 0;
    while (SetupDiEnumDeviceInterfaces(deviceInfoSet, NULL, &GUID_DEVINTERFACE_USB_DEVICE, index, &deviceInterfaceData)) {
        DWORD requiredSize = 0;

       
        SetupDiGetDeviceInterfaceDetail(deviceInfoSet, &deviceInterfaceData, NULL, 0, &requiredSize, NULL);

        PSP_DEVICE_INTERFACE_DETAIL_DATA deviceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(requiredSize);
        if (!deviceDetailData) {
            std::cerr << "Eroare la alocarea memoriei pentru detalii dispozitiv." << std::endl;
            break;
        }

        deviceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

        if (SetupDiGetDeviceInterfaceDetail(deviceInfoSet, &deviceInterfaceData, deviceDetailData, requiredSize, NULL, NULL)) {
            std::wcout << L"Dispozitiv USB găsit: " << deviceDetailData->DevicePath << std::endl;
        }
        else {
            std::cerr << "Eroare la obținerea detaliilor dispozitivului USB." << std::endl;
        }

        free(deviceDetailData);
        index++;
    }

    SetupDiDestroyDeviceInfoList(deviceInfoSet);
    return 0;
}
