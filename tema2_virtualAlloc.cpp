#include <windows.h>
#include <iostream>

int main() {
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);

    DWORD pageSize = sysInfo.dwPageSize;
    std::cout << "Dimensiunea unei pagini este: " << pageSize << " bytes" << std::endl;

    int N;
    std::cout << "Introdu numarul de pagini de alocat: ";
    std::cin >> N;

    if (N <= 0) {
        std::cerr << "Numar invalid de pagini." << std::endl;
        return 1;
    }

    
    LPVOID allocatedMemory = VirtualAlloc(
        NULL,
        N * pageSize,
        MEM_RESERVE | MEM_COMMIT,
        PAGE_READWRITE
    );

    if (allocatedMemory == NULL) {
        std::cerr << "Eroare la alocarea memoriei cu VirtualAlloc." << std::endl;
        return 1;
    }

    std::cout << "Memorie alocata la adresa: " << allocatedMemory << std::endl;

    
    memset(allocatedMemory, 0, N * pageSize);

    
    VirtualFree(allocatedMemory, 0, MEM_RELEASE);

    std::cout << "Memoria a fost eliberata." << std::endl;

    return 0;
}
