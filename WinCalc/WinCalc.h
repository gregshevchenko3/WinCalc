// WinCalc.h : Include file for standard system include files,
// or project specific include files.

#pragma once
#define UNICODE
#define _UNICODE
#include <windows.h>
#include <winnt.h>
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>

#include <string>
#include <vector>
#include <sstream>
#include <WinUser.h>
#include <functional>

const int MAX_LENGTH = 10;
void AppendText(const HWND& hwnd, TCHAR* newText);
void DeletePrevChar(const HWND& hwnd);
void unary_operation(HWND hwnd, std::function<double(double)> op);
struct binary_operation {
	static double a;
	static std::function<double(double, double)> fn;
	static bool clean;
	static void binary_operation::calc(HWND hwnd, std::function<double(double, double)> fn);
};
struct window {
	HWND hwnd;
	int rStart, rEnd, cStart, cEnd;
};
class Grid {
	bool** isuse;
	int rcount, ccount, px_per_row, px_per_column;
	HWND parent;
	std::vector<window> childs;

	static Grid* instance;

	Grid(HWND parent, int row_count, int col_count);
	~Grid();
public:
	bool is_region_free(int rStart, int rEnd, int cStart, int cEnd);
	bool add(const wchar_t* cls, const wchar_t* wnd_name, DWORD style, int rStart, int rEnd, int cStart, int cEnd, int id_menu);
	void resize(unsigned int width, unsigned int height);
	HWND Get(int i) {
		if (i < childs.size() && i >= 0) {
			return childs[i].hwnd;
		}
		return 0;
	}

	static Grid* Instance(HWND parent, int rows, int cols);
	static Grid* Instance();
};

// TODO: Reference additional headers your program requires here.
