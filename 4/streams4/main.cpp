#include <windows.h>
#include <stdio.h>

#define THREADCOUNT 4

HANDLE ghWriteEvent;
HANDLE ghThreads[THREADCOUNT];

DWORD WINAPI ThreadProc(LPVOID);

void CreateEventsAndThreads(void) {
    int i;
    DWORD dwThreadID;

    // ������� ������ ������� � ������ �������. ����� ������ ������������� ���
    // ������ � ���������� ���������, ����� �� ����������� ������ � ����� �����

    ghWriteEvent = CreateEvent(
        NULL,               // �������� ������������ �� ���������
        TRUE,               // ������� ������� ������
        FALSE,              // ��������� ��������� �� ���������������
        TEXT("WriteEvent")  // ��� �������
    );

    if (ghWriteEvent == NULL) {
        printf("CreateEvent failed (%d)\n", GetLastError());
        return;
    }

    // ������� ��������� ������� ��� ������ �� ������

    for (i = 0; i < THREADCOUNT; i++) {
        // TODO: ����� ������� �������� ����� ����������� ������������� ���������
         // � ��������� ������, ��������, ������� � ������ ���
         // �������������� ��� �������������.
        ghThreads[i] = CreateThread(
            NULL,              // ������������ �� ���������
            0,                 // ������ ����� �� ���������
            ThreadProc,        // ��� ������� ������
            NULL,              // ��� ���������� ������
            0,                 // ����� ������� �� ���������
            &dwThreadID);

        if (ghThreads[i] == NULL) {
            printf("CreateThread failed (%d)\n", GetLastError());
            return;
        }
    }
}

void WriteToBuffer(VOID) {
    // ������: �������� � ����� �����.
    printf("Main thread writing to the shared buffer...\n");

    // ���������� ghWriteEvent � ����������
    if (!SetEvent(ghWriteEvent)) {
        printf("SetEvent failed (%d)\n", GetLastError());
        return;
    }
}

void CloseEvents() {
    // ������� ��� ����������� ������� (� ��������� ����� ������ ���� ���������� ����������).
    CloseHandle(ghWriteEvent);
}

int main(void) {
    DWORD dwWaitResult;

    // TODO: ������� ����� �����

     // ������� ������� � ������ THREADCOUNT ��� ������ �� ������

    CreateEventsAndThreads();

    // � ���� ������ ������ ������ �������� � ��������
     // �������� �������� ������� ����������� �������. ��� �� �����,
     // ������ � ����� ���������, ������ ��� ��� �������
     // ������� ������� ������.

    WriteToBuffer();

    printf("Main thread waiting for threads to exit...\n");

    // ���������� ������� ������ �������� ������, ����� �����
        // ���������.
    dwWaitResult = WaitForMultipleObjects(
        THREADCOUNT,   // ���������� handles � �������
        ghThreads,     // ������ ������������ �������
        TRUE,          // ���������, ���� ��� �� ����� ���������������
        INFINITE);

    switch (dwWaitResult) {
        // ��� ������� ������ �������� ������
    case WAIT_OBJECT_0:
        printf("All threads ended, cleaning up for application exit...\n");
        break;

        // ��������� ������
    default:
        printf("WaitForMultipleObjects failed (%d)\n", GetLastError());
        return 1;
    }

    Sleep(10000);
    // ������� ������� ��� �������
    CloseEvents();

    return 0;
}

DWORD WINAPI ThreadProc(LPVOID lpParam) {
    // lpParam � ���� ������� �� ������������
    UNREFERENCED_PARAMETER(lpParam);

    DWORD dwWaitResult;

    printf("Thread %d waiting for write event...\n", GetCurrentThreadId());

    dwWaitResult = WaitForSingleObject(
        ghWriteEvent, // event handle
        INFINITE);    // �������������� ��������

    switch (dwWaitResult)
    {
        // ������ ������� ������� ������
    case WAIT_OBJECT_0:
        // TODO: ��������� �� ������ ������
        printf("Thread %d reading from buffer\n",
            GetCurrentThreadId());
        break;

        // ������
    default:
        printf("Wait error (%d)\n", GetLastError());
        return 0;
    }

    // ������, ����� �� ��������� ������ ������, �� ����� ������������ ������
     // �������, ��������������� � ���, ��� ���� ����� ������ �� ������. ����
     // ������ ������ ���������� ���������� ������ ��� ������������� (
     // ���������� ���������������, ����� ����� �����������.)

    printf("Thread %d exiting\n", GetCurrentThreadId());
    return 1;
}