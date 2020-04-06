#include <cstdlib>
#include <clocale>
#include <cstdio>
#include <windows.h>
#include <tchar.h>
#include <iostream>

using namespace std;

BOOL FindFileByClaster(TCHAR* volume, LONGLONG cluster) {

	HANDLE hDevice = CreateFile(volume,			// ��� ������������/������������ �����
		GENERIC_READ,							// ��� ������� � �������
		FILE_SHARE_READ | FILE_SHARE_WRITE,		// ����� ����������� �������
		NULL,									// SECURITY_ATTRIBUTES 
												// (NULL - ���������� �� ����� ���� �����������)
		OPEN_EXISTING,							// ����������� �������� � ������
												// ��������, ���� ���� ���������� (����� - ������)
		FILE_FLAG_BACKUP_SEMANTICS,				// �������� ����� 
												// (FILE_FLAG_BACKUP_SEMANTICS - ��� ��������� ����������� ��������)
		NULL);									// ���������� ����� ������� (������ ���� NULL)


	// ������� ����������� �������
	if (hDevice == INVALID_HANDLE_VALUE) {
		wprintf(L"Catalogue open error\n");
		return FALSE;
	}

	//������� ������
	LOOKUP_STREAM_FROM_CLUSTER_INPUT input = { 0 };
	input.NumberOfClusters = 1;
	input.Cluster[0].QuadPart = cluster;

	//����� ��� �����������
	BYTE output[5000] = {};
	DWORD dwRes = 0;
	LOOKUP_STREAM_FROM_CLUSTER_OUTPUT result = { 0 };

	//����������� ������������� ���� ���������� ����������
	BOOL bRes = DeviceIoControl((HANDLE)hDevice,			// ���������� ����������
		FSCTL_LOOKUP_STREAM_FROM_CLUSTER,					// ����������� ��� ��� ��������
		(LPVOID)&input,										// ��������� �� ����� ������� ������
		(DWORD)sizeof(input),								// ������ ������ ������� ������
		(LPVOID)output,										// ����� �������� ������
		(DWORD)5000,										// ������ ������ �������� ������
		(LPDWORD)&dwRes,									// ��������� �� ����������, ������� �������� ������ ������,
															// ����������� � ������ �������� ������ 
		NULL);												// ��������� OVERLAPPED (������������)

	// �������� ����������� �������
	if (bRes == FALSE) {
		wprintf(L"Device IO control error\n");
		return FALSE;
	}

	//����������� ���������� �� ������� � ��������� LOOKUP_STREAM_FROM_CLUSTER_OUTPUT
	memcpy(&result, output, sizeof(LOOKUP_STREAM_FROM_CLUSTER_OUTPUT));

	// ����� ������������ ��������� �������� 
	if (result.NumberOfMatches == 0) {
		wprintf(L"File not found\n");
		return FALSE;
	}

	//��������� � ������ ������ ��������� LOOKUP_STREAM_FROM_CLUSTER_ENTRY

	BYTE* p = (BYTE*)output + result.Offset;	// �������� � ��������� �� ������� ��������� 
	LOOKUP_STREAM_FROM_CLUSTER_ENTRY* pentry = (LOOKUP_STREAM_FROM_CLUSTER_ENTRY*)p;

	wprintf(L"File: %s\n", pentry->FileName);	// ��� �����, �������������� ���������

	CloseHandle(hDevice); // ��������� ���������� �������
	return TRUE;
}

int main() {
	LONGLONG inp = 0;
	printf_s("Enter cluster number:\n");
	scanf_s("%llu", &inp);

	FindFileByClaster((TCHAR*)L"\\\\.\\C:", inp);
	system("PAUSE");
}