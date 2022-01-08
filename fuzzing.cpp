#include <stdio.h>
#include <windows.h>
#include <iostream>
#include <libloaderapi.h>
#pragma warning(disable: 4996)
typedef int(__cdecl* TARGET)(wchar_t* filename);
TARGET funcPtr;
extern "C" __declspec(dllexport) int fuzzme(wchar_t* path);
// LPCSTR = long pointer constant string = const char *

extern "C" __declspec(dllexport) __declspec(noinline) int fuzzme(wchar_t* path) {
	int result = funcPtr(path);
	return result;
}
wchar_t* charToWChar(const char* text)
{
	size_t size = strlen(text) + 1;
	wchar_t* wa = (wchar_t*)malloc(sizeof(wchar_t) * size);
	mbstowcs(wa, text, size);
	return wa;
}

int main(int argc, char* argv[]) {
	HMODULE DLLHandle = LoadLibrary(L"C:\\Program Files (x86)\\Unidocs\\ezPDFEditor\\ezPDF2HWP.dll"); // dll base address?

	if (DLLHandle == NULL) {
		printf("Cannot load ezPDF2HWP.dll\n");
		return -1;
	}
	int func = (int)DLLHandle + 0xB640;//0xb410;//0x1000B640
	funcPtr = (TARGET)func;
	printf("funcPtr addr: 0x%p\n", funcPtr);
	printf("file name: %s\n", argv[1]);

	int reusult = fuzzme(charToWChar(argv[1]));
	return 0;
}
