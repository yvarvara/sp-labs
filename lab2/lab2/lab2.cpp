#include <cstdlib>
#include <clocale>
#include <cstdio>
#include <windows.h>
#include <tchar.h>
#include <iostream>

using namespace std;

BOOL FindFileByClaster(TCHAR* volume, LONGLONG cluster) {

	HANDLE hDevice = CreateFile(volume,			// им€ создаваемого/открываемого файла
		GENERIC_READ,							// тип доступа к объекту
		FILE_SHARE_READ | FILE_SHARE_WRITE,		// режим совместного доступа
		NULL,									// SECURITY_ATTRIBUTES 
												// (NULL - дескриптор не может быть унаследован)
		OPEN_EXISTING,							// выполн€емые действи€ с файлом
												// открытие, если файл существует (иначе - ошибка)
		FILE_FLAG_BACKUP_SEMANTICS,				// атрибуты файла 
												// (FILE_FLAG_BACKUP_SEMANTICS - дл€ получение дескриптора каталога)
		NULL);									// дескриптор файла шаблона (должен быть NULL)


	// функци€ завершилась ошибкой
	if (hDevice == INVALID_HANDLE_VALUE) {
		wprintf(L"Catalogue open error\n");
		return FALSE;
	}

	//входные данные
	LOOKUP_STREAM_FROM_CLUSTER_INPUT input = { 0 };
	input.NumberOfClusters = 1;
	input.Cluster[0].QuadPart = cluster;

	//буфер дл€ результатов
	BYTE output[5000] = {};
	DWORD dwRes = 0;
	LOOKUP_STREAM_FROM_CLUSTER_OUTPUT result = { 0 };

	//отправление управл€юещего кода указанному устройству
	BOOL bRes = DeviceIoControl((HANDLE)hDevice,			// дескриптор устройства
		FSCTL_LOOKUP_STREAM_FROM_CLUSTER,					// управл€ющий код дл€ операции
		(LPVOID)&input,										// указатель на буфер входных данных
		(DWORD)sizeof(input),								// размер буфера входных данных
		(LPVOID)output,										// буфер выходных данных
		(DWORD)5000,										// размер буфера выходных данных
		(LPDWORD)&dwRes,									// указатель на переменную, котора€ получает размер данных,
															// сохраненных в буфере выходных данных 
		NULL);												// структура OVERLAPPED (игнорируетс€)

	// операци€ завершилась ошибкой
	if (bRes == FALSE) {
		wprintf(L"Device IO control error\n");
		return FALSE;
	}

	//копирование результата из массива в структуру LOOKUP_STREAM_FROM_CLUSTER_OUTPUT
	memcpy(&result, output, sizeof(LOOKUP_STREAM_FROM_CLUSTER_OUTPUT));

	// число соответствий заданному критерию 
	if (result.NumberOfMatches == 0) {
		wprintf(L"File not found\n");
		return FALSE;
	}

	//переходим к адресу первой структуры LOOKUP_STREAM_FROM_CLUSTER_ENTRY

	BYTE* p = (BYTE*)output + result.Offset;	// смещение в структуре до первого вхождени€ 
	LOOKUP_STREAM_FROM_CLUSTER_ENTRY* pentry = (LOOKUP_STREAM_FROM_CLUSTER_ENTRY*)p;

	wprintf(L"File: %s\n", pentry->FileName);	// им€ файла, представленное кластером

	CloseHandle(hDevice); // закрываем дескриптор объекта
	return TRUE;
}

int main() {
	LONGLONG inp = 0;
	printf_s("Enter cluster number:\n");
	scanf_s("%llu", &inp);

	FindFileByClaster((TCHAR*)L"\\\\.\\C:", inp);
	system("PAUSE");
}