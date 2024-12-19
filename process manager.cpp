#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
using namespace std;

// Function to list all running processes
void ListRunningProcesses() {
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;

    // Create a snapshot of all processes
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        cout<< "Error: Cannot create snapshot!" <<endl;
        return;
    }

    pe32.dwSize = sizeof(PROCESSENTRY32);

    // Get the first process
    if (!Process32First(hProcessSnap, &pe32)) {
        cout<< "Error: Unable to retrieve process information." <<endl;
        CloseHandle(hProcessSnap);
        return;
    }

    cout << "%-10s %-30s\n" << "Process ID" << "Process Name" <<endl;

    // Loop through all processes
    do {
        std::wcout << pe32.th32ProcessID << L"\t" << pe32.szExeFile <<endl;
    } while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);
}

// Function to terminate a process by its ID
void TerminateProcessByID(DWORD processID) {
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, processID);
    if (hProcess == nullptr) {
        cout<< "Error: Unable to open process with ID " << processID << ". Check permissions." <<endl;
        return;
    }

    if (TerminateProcess(hProcess, 0)) {
        cout<< "Process with ID " << processID << " terminated successfully." <<endl;
    } else {
        cout<< "Error: Failed to terminate process with ID " << processID << "." <<endl;
    }

    CloseHandle(hProcess);
}

int main() {
    while (true) {
        cout << "1. List Running Processes\n";
        cout << "2. Terminate a Process\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        int choice;
    	cin >> choice;

        switch (choice) {
        case 1:
            ListRunningProcesses();
            break;
        case 2: {
            cout << "Enter the Process ID to terminate: ";
            DWORD processID;
            cin >> processID;
            TerminateProcessByID(processID);
            break;
        }
        case 3:
            cout << "Exiting..." <<endl;
            return 0;
        default:
            cout << "Invalid choice. Please try again." <<endl;
        }
    }
    return 0;
}