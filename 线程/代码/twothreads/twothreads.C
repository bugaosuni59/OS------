#include <stdio.h>
#include <windows.h>

/* the thread runs in this separate function */
DWORD WINAPI Bat()
{
	int i;

	for (i = 0; i <= 5; i++)
		printf("Bat:\%d\n",i);

	return 0;
}

DWORD WINAPI Cat()
{
	int i;

	for (i = 0; i <= 5; i++)
		printf("Cat:\%d\n",i);

	return 0;
}

int main()
{
	DWORD ThreadId_1, ThreadId_2;
	HANDLE ThreadHandle_1, ThreadHandle_2;
	int Param=0;

	ThreadHandle_1 = CreateThread(NULL, 0, Bat, NULL, 0, &ThreadId_1);

	ThreadHandle_2 = CreateThread(NULL, 0, Cat, NULL, 0, &ThreadId_2);
	
	if (ThreadHandle_1 != NULL) {
		WaitForSingleObject(ThreadHandle_1, INFINITE);
		CloseHandle(ThreadHandle_1);
	}
	
	if (ThreadHandle_2 != NULL) {
		WaitForSingleObject(ThreadHandle_2, INFINITE);
		CloseHandle(ThreadHandle_2);
	}	
	system("pause"); 
}
