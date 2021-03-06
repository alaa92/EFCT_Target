#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdio.h>
#include "Functions.h"

class _Class
{
	int x = 0;
	int y = 0;
	char str[16] = { 0 };

public:
	_Class(int x, int y, const char* str)
	{
		this->x = x;
		this->y = y;
		strcpy_s(this->str, 16, str);
	}

	void MemberFunc()
	{
		pl("Member function.");
		printf("str: %s\nx: %i\ny %i\n\n", str, x, y);
	}
	void UpdateStr(const char* str)
	{
		strcpy_s(this->str, 16, str);
	}
};
static _Class* pObject = nullptr;

// this function was really just for development of the tutorial... Not important.
void CopyDefines()
{
	char buffer[0x200]{ 0 };
#ifdef _WIN64
#define HookOffset 0x1541
#else
#define HookOffset 0x1442
#endif

	sprintf_s(buffer, 0x200,
		"#define Func1\t\t0x%p\n#define Func2\t\t0x%p\n#define Func3\t\t0x%p\n#define Func4\t\t0x%p\n#define ObjectPtr\t0x%p\n#define MemberFunc\t0x%p\n#define UpdateStr\t0x%p\n#define HookLoc\t\t0x%p",
		Function1,
		Function2,
		Function3,
		Function4,
		&pObject,
		&_Class::MemberFunc,
		&_Class::UpdateStr,
		(void*)((uintptr_t)GetModuleHandle(0) + HookOffset));

	printf(buffer);

	// who needs error checking!
	OpenClipboard(NULL);
	EmptyClipboard();
	HGLOBAL hMemory = GlobalAlloc(GMEM_MOVEABLE, 0x200);
	void* pMem = GlobalLock(hMemory);
	memcpy_s(pMem, 0x200, buffer, 0x200);
	GlobalUnlock(hMemory);
	SetClipboardData(CF_TEXT, hMemory);
	CloseClipboard();
}

int main()
{
	_Class object = { 5, 7, "traxin" };
	pObject = &object;

	pl("Press END to quit.");
	pl("Keys are: F1, F2, F3, F4 and F5\n");
	pl("Press F9 to copy Function/Object defines to clipboard.");

	PrintFuncAddresses();

	printf("ObjectPtr address: 0x%p\n", &pObject);
	printf("_Class::MemberFunc: 0x%p\n", &_Class::MemberFunc);
	printf("_Class::UpdateStr:  0x%p\n", &_Class::UpdateStr);

	while (!GetAsyncKeyState(VK_F12))
	{
		if (GetAsyncKeyState(VK_F1) & 1)
		{
			Function1();
		}
		else if (GetAsyncKeyState(VK_F2) & 1)
		{
			Function2(9);
		}
		else if (GetAsyncKeyState(VK_F3) & 1)
		{
			Function3("GuidedHacking is awesome.");
		}
		else if (GetAsyncKeyState(VK_F4) & 1)
		{
			Function4(8, 9);
		}
		else if (GetAsyncKeyState(VK_F5) & 1)
		{
			object.MemberFunc();
		}
		else if (GetAsyncKeyState(VK_F9) & 1)
		{
			//Copy data to clipboard whatever.
			CopyDefines();
		}
	}
	return 0;
}

