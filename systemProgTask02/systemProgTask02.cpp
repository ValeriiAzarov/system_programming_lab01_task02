// systemProgTask02.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#define SIZE 1000
#define STEP 6

using namespace std;

void writeFile(HFILE file, LPCTSTR message, DWORD needs) {

	DWORD written;
	if (!WriteFile((HANDLE)file, (LPCVOID)message, needs, &written, NULL)) {
		throw 2;
	}
}

int _tmain(INT argc, TCHAR** argv)
{
	HFILE file, outFile;
	TCHAR* name = argv[1];
	TCHAR* outName = argv[2];
	
	if (argc != 4) {
		cout << "Invalid number of argument = " << argc << endl;
		return 0;
	}

	try {
		WCHAR wstr[SIZE + 1] = L"";
		CHAR str[SIZE + 1] = "";
		DWORD dw;
		BOOL result;

		file = (HFILE)CreateFile((LPTSTR)name, GENERIC_READ, 0,	NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		outFile = (HFILE)CreateFile((LPCTSTR)outName, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

		if (file == -1) {
			throw 3;
		}

		if (outFile == -1) {
			throw 1;
		}
				
		if (_tcscmp(argv[2], _T("-a")) == 0) {
			cout << "The following output provided by wcout: ";
			do {
				result = ReadFile((HANDLE)file, (LPVOID)&str, STEP, &dw, NULL);
				str[dw] = '\0';

				MultiByteToWideChar(CP_UTF8, 0, str, -1, wstr, dw);
				wstr[dw] = L'\0';

				wcout << wstr;
				writeFile(outFile, wstr, dw * sizeof(WCHAR));

			} while (result && dw != 0);
			wcout << endl;
		}
		else if (_tcscmp(argv[2], _T("-u")) == 0) {
			BOOL is_und = FALSE;
			cout << "The following output provided by cout: ";			
			do {
				result = ReadFile((HANDLE)file, (LPVOID)&wstr, STEP * sizeof(WCHAR), &dw, NULL);
				wstr[dw / sizeof(WCHAR)] = L'\0';
				BOOL is_und_curr;

				if (wstr[0] == 0xFEFF) {
					dw -= 2;
					WideCharToMultiByte(CP_ACP, 0, wstr + 1, -1, str, dw - 2, "&", &is_und_curr);
				}
				else {
					WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, dw, "&", &is_und_curr);
				}

				str[dw] = '\0';
				is_und |= is_und_curr;
				cout << str;
				writeFile(outFile, (LPTSTR)str, dw / sizeof(WCHAR));
			} while (result && dw != 0);
			cout << endl;
			if (is_und) {
				cout << "Some chars were replaced by ampersants" << endl;
			}
		}
	}
	catch (int error) {
		cout << "Error code = " << error << endl;
	}
	CloseHandle((HANDLE)file);
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
