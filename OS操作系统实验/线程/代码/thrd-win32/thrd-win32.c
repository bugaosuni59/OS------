#include <stdio.h>
#include <windows.h>

DWORD Sum; /* data is shared by the thread(s) */

/* the thread runs in this separate function */
DWORD WINAPI Summation(PVOID Param)          /* Param为传递的参数的地址 */ 
{
	DWORD Upper = *(DWORD *)Param;
	DWORD i;

	for (i = 0; i <= Upper; i++)
		Sum += i;

	return 0;
}

int main(int argc, char *argv[])
{
	DWORD ThreadId;
	HANDLE ThreadHandle;
	int Param;

	// do some basic error checking
	if (argc != 2) {
		fprintf(stderr,"An integer parameter is required\n");
		return -1;
	}

	Param = atoi(argv[1]);

	if (Param < 0) {
		fprintf(stderr, "an integer >= 0 is required \n");
		return -1;
	}

	// create the thread
	ThreadHandle = CreateThread(NULL, 0, Summation, &Param, 0, &ThreadId); 
    /* 创建的线程执行“Summation”模块，传递的参数是Param */ 

	if (ThreadHandle != NULL) {
		WaitForSingleObject(ThreadHandle, INFINITE);      /*等待线程结束*/ 
		CloseHandle(ThreadHandle);                                   
		printf("sum = %d\n",Sum);
	}
	
	system("pause"); 
}
