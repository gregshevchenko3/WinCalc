// WinCalc.cpp : Defines the entry point for the application.
//

#include "WinCalc.h"
#include <iostream>
#include <iomanip>

Grid* Grid::instance = nullptr;
double binary_operation::a;
std::function<double(double, double)> binary_operation::fn;
bool binary_operation::clean = true;

enum buttons {
	id0 = 0x8001, id1, id2, id3, id4, id5, id6, id7, id8, id9, id_point,
	id_plus_minus, id_inv, id_sqr, id_sqrt,
	id_clear, id_delete_prev,
	id_plus, id_minus, id_mod, id_div, id_mul, id_res,
};

LRESULT CALLBACK winproc(HWND hwnd, UINT wm, WPARAM wp, LPARAM lp);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PTSTR argv, int argc) {

	std::wstring clsName = L"Calc";

	WNDCLASSEX wnd;
	MSG msg;
	HWND hWnd;

	SIZE sz;
	sz.cx = 480;
	sz.cy = 640;

	wnd.cbClsExtra = wnd.cbWndExtra = 0;
	wnd.cbSize = sizeof wnd;
	wnd.lpszClassName = clsName.c_str();
	wnd.hInstance = hInstance;
	wnd.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wnd.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wnd.hIcon = wnd.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	wnd.lpszMenuName = nullptr;
	wnd.style = CS_HREDRAW | CS_VREDRAW;
	wnd.lpfnWndProc = winproc;

	if (!RegisterClassEx(&wnd)) {
		MessageBox(nullptr, (L"Could not register class: \"" + clsName + L"\".").c_str(), L"RegisterClass()", MB_ICONERROR);
		return 0;
	}
	if (!(hWnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, clsName.c_str(), L"WinCalc", WS_TILEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, sz.cx, sz.cy, nullptr,
		nullptr, hInstance, nullptr))) {
		MessageBox(nullptr, (L"Could not create window: \"" + clsName + L"\".").c_str(), L"CreateWindowEx()", MB_ICONERROR);
		return 0;
	}
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}
LRESULT winproc(HWND hwnd, UINT wm, WPARAM wp, LPARAM lp) {
	std::wstringstream ss;
	Grid* grid;
	switch (wm) {
	case WM_CREATE:
		grid = Grid::Instance(hwnd, 7, 4);

		grid->add(L"EDIT", nullptr, WS_BORDER | ES_RIGHT | ES_NUMBER, 0, 1, 0, 4, 0);
		SetWindowText(grid->Get(0), L"0");

		grid->add(L"BUTTON", L"%", BS_PUSHBUTTON, 1, 1, 0, 1, id_mod);
		grid->add(L"BUTTON", L"1/x", BS_PUSHBUTTON, 2, 1, 0, 1, id_inv);
		grid->add(L"BUTTON", L"7", BS_PUSHBUTTON, 3, 1, 0, 1, id7);
		grid->add(L"BUTTON", L"4", BS_PUSHBUTTON, 4, 1, 0, 1, id4);
		grid->add(L"BUTTON", L"1", BS_PUSHBUTTON, 5, 1, 0, 1, id1);
		grid->add(L"BUTTON", L"+/-", BS_PUSHBUTTON, 6, 1, 0, 1, id_plus_minus);

		grid->add(L"BUTTON", L"x²", BS_PUSHBUTTON, 2, 1, 1, 1, id_sqr);
		grid->add(L"BUTTON", L"8", BS_PUSHBUTTON, 3, 1, 1, 1, id8);
		grid->add(L"BUTTON", L"5", BS_PUSHBUTTON, 4, 1, 1, 1, id5);
		grid->add(L"BUTTON", L"2", BS_PUSHBUTTON, 5, 1, 1, 1, id2);
		grid->add(L"BUTTON", L"0", BS_PUSHBUTTON, 6, 1, 1, 1, id0);

		grid->add(L"BUTTON", L"C", BS_PUSHBUTTON, 1, 1, 2, 1, id_clear);
		grid->add(L"BUTTON", L"²√x", BS_PUSHBUTTON, 2, 1, 2, 1, id_sqrt);
		grid->add(L"BUTTON", L"9", BS_PUSHBUTTON, 3, 1, 2, 1, id9);
		grid->add(L"BUTTON", L"6", BS_PUSHBUTTON, 4, 1, 2, 1, id6);
		grid->add(L"BUTTON", L"3", BS_PUSHBUTTON, 5, 1, 2, 1, id3);
		grid->add(L"BUTTON", L",", BS_PUSHBUTTON, 6, 1, 2, 1, id_point);

		grid->add(L"BUTTON", L"←", BS_PUSHBUTTON, 1, 1, 3, 1, id_delete_prev);
		grid->add(L"BUTTON", L"/", BS_PUSHBUTTON, 2, 1, 3, 1, id_div);
		grid->add(L"BUTTON", L"*", BS_PUSHBUTTON, 3, 1, 3, 1, id_mul);
		grid->add(L"BUTTON", L"-", BS_PUSHBUTTON, 4, 1, 3, 1, id_minus);
		grid->add(L"BUTTON", L"+", BS_PUSHBUTTON, 5, 1, 3, 1, id_plus);
		grid->add(L"BUTTON", L"=", BS_PUSHBUTTON, 6, 1, 3, 1, id_res);
		break;
	case WM_COMMAND:	
		grid = Grid::Instance();
		binary_operation operation;
		switch (wp) {
			case id0:
				if (GetWindowTextLength(grid->Get(0)) == 0)
					break;
			case id1:
			case id2:
			case id3:
			case id4:
			case id5:
			case id6:
			case id7:
			case id8:
			case id9: {
				if (binary_operation::clean) {
					SetWindowText(grid->Get(0), L"0");
					binary_operation::clean = false;
				}
				wchar_t buffer[2]{ 0 };
				swprintf(buffer, sizeof(buffer), L"%i", wp - id0);
				AppendText(grid->Get(0), buffer);
				break;
			}
			case id_point: 
				AppendText(grid->Get(0), L".");
				break;
			case id_clear:
				SetWindowText(grid->Get(0), L"0");
				binary_operation::fn = std::function<double(double, double)>();
				binary_operation::a = 0;
				break;
			case id_delete_prev:
				if(binary_operation::clean)
				DeletePrevChar(grid->Get(0));
				break;
			case id_plus_minus:
				unary_operation(grid->Get(0), [=](double a) {return -a; });
				break;
			case id_inv:
				unary_operation(grid->Get(0), [=](double a) {return 1/a; });
				break;
			case id_sqr:
				unary_operation(grid->Get(0), [=](double a) {return a * a; });
				break;
			case id_sqrt: 
				unary_operation(grid->Get(0), [=](double a) {return (a < 0) ? 0 : sqrt(a); });
				break;
			case id_plus:
				binary_operation::calc(grid->Get(0), [=](double a, double b) {return a + b; });
				break;
			case id_minus:
				binary_operation::calc(grid->Get(0), [=](double a, double b) {return a - b; });
				break;
			case id_mul:
				binary_operation::calc(grid->Get(0), [=](double a, double b) {return a * b; });
				break;
			case id_div:
				binary_operation::calc(grid->Get(0), [=](double a, double b) {return a / b; });
				break;
			case id_mod:
				binary_operation::calc(grid->Get(0), [=](double a, double b) {return (int)a % (int)b; });
				break;
			case id_res:
				binary_operation::calc(grid->Get(0), [=](double a, double b) {return a; });
				binary_operation::fn = std::function<double(double, double)>();
				break;
		}
		break;
	case WM_SIZE: 
		{
			unsigned int w = LOWORD(lp);
			unsigned int h = HIWORD(lp);
			Grid::Instance()->resize(w, h);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, wm, wp, lp);
}

Grid::Grid(HWND parent, int row_count, int col_count) : rcount(row_count), ccount(col_count)
{
	RECT rect;
	GetClientRect(parent, &rect);
	this->parent = parent;
	px_per_row = (rect.bottom - rect.top) / row_count;
	px_per_column = (rect.right - rect.left) / col_count;
	isuse = new bool* [row_count];
	for (auto r = 0; r < row_count; r++)
		isuse[r] = new bool[col_count] {false};
}
Grid::~Grid() {
	if (isuse) {
		for (auto r = 0; r < rcount; r++)
			if (isuse[r]) delete[] isuse[r];
		delete[] isuse;
	}
}
bool Grid::add(const wchar_t* cls, const wchar_t* wnd_name, DWORD style, int rStart, int rEnd, int cStart, int cEnd, int menu) {
	bool result = is_region_free(rStart, rEnd, cStart, cEnd);
	if (result) {
		HWND child = CreateWindow(cls, wnd_name, style | WS_CHILD, px_per_column * cStart, px_per_row * rStart,
			px_per_column * cEnd, px_per_row * rEnd, parent, (HMENU)menu, (HINSTANCE)GetWindowLongPtr(parent, GWLP_HINSTANCE), nullptr);
		HFONT hFont = CreateFont(px_per_row, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
		SendMessage(child, WM_SETFONT, WPARAM(hFont), true);
		struct window wnd;
		wnd.hwnd = child;
		wnd.cStart = cStart;
		wnd.cEnd = cEnd;
		wnd.rEnd = rEnd;
		wnd.rStart = rStart;
		wnd.hFont = hFont;
		childs.push_back(wnd);
		ShowWindow(child, SW_SHOW);
		for (auto r = rStart; r < rEnd; r++)
			for (auto c = cStart; c < cEnd; c++)
				isuse[r][c] = true;
	}
	return result;
}
void Grid::resize(unsigned int width, unsigned int height) {
	px_per_row = height / rcount;
	px_per_column = width / ccount;
	for (struct window w: this->childs) {
		DeleteObject(w.hFont);
		HFONT hFont = CreateFont(px_per_row, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
		SendMessage(w.hwnd, WM_SETFONT, WPARAM(hFont), true);
		MoveWindow(w.hwnd, px_per_column * w.cStart, px_per_row * w.rStart, px_per_column * w.cEnd, px_per_row * w.rEnd, true);
	}
}
bool Grid::is_region_free(int rStart, int rEnd, int cStart, int cEnd) {
	if (rStart >= rcount || rEnd > rcount  || rStart < 0 || rEnd < 0) return false;
	if (cStart >= ccount || cEnd > ccount || cStart < 0 || cEnd < 0) return false;
	bool result = true;
	for (auto r = rStart; r < rEnd; r++)
		for (auto c = cStart; c < cEnd; c++)
			result = result && !isuse[r][c];
	return result;
};
Grid* Grid::Instance(HWND parent, int rows, int cols) {
	if (!instance) {
		instance = new Grid(parent, rows, cols);
	}
	return instance;
}
Grid* Grid::Instance() {
	return instance;
}
double get_number(HWND hwnd) {
	int outLength = GetWindowTextLength(hwnd) + 1;
	TCHAR* buf = (TCHAR*)GlobalAlloc(GPTR, outLength * sizeof(TCHAR));
	GetWindowText(hwnd, buf, outLength);
	double n;
	std::wstringstream wss_in(buf);
	wss_in >> n;
	GlobalFree(buf);
	return n;
}
void AppendText(const HWND& hwnd, WCHAR* newText)
{
	int outLength = GetWindowTextLength(hwnd);
	if (outLength == MAX_LENGTH)return;
	outLength += lstrlen(newText) + 1;
	TCHAR* buf = (TCHAR*)GlobalAlloc(GPTR, outLength * sizeof(TCHAR));
	if (!buf) return;
	GetWindowText(hwnd, buf, outLength);
	double n = 0;
	std::wstringstream wss;
	if (GetWindowTextLength(hwnd) == 1 && !wcscmp(buf, L"0") && wcscmp(newText, L".")) {
		wss = std::wstringstream(newText);
	}
	else {
		_tcscat_s(buf, outLength, newText);
		wss = std::wstringstream(buf);
	}
	wss >> n;
	if(wss.eof()){
		wss << n;
		SetWindowText(hwnd, wss.str().c_str());
	}
	GlobalFree(buf);
}
void DeletePrevChar(const HWND& hwnd) {
	int len = GetWindowTextLength(hwnd); 
	TCHAR* buf = (TCHAR*)GlobalAlloc(GPTR, len * sizeof(TCHAR));
	if (!buf) return;
	GetWindowText(hwnd, buf, len);
	if(len > 1)
		SetWindowText(hwnd, buf);
	else
		SetWindowText(hwnd, L"0");
	GlobalFree(buf);
}
void unary_operation(HWND hwnd, std::function<double(double)> op) {
	std::wostringstream wss_out;
	wss_out << std::setprecision(4) << op(get_number(hwnd));
	SetWindowText(hwnd,  wss_out.str().c_str());
}
void binary_operation::calc(HWND hwnd, std::function<double(double, double)> fn) {
	if (binary_operation::fn == nullptr) {
		binary_operation::a = get_number(hwnd);
		binary_operation::fn = fn;
	}
	else {
		std::wostringstream wss_out;
		wss_out << std::setprecision(4) << (binary_operation::a = binary_operation::fn(binary_operation::a, get_number(hwnd)));
		SetWindowText(hwnd, wss_out.str().c_str());
		binary_operation::fn = fn;
	}
	binary_operation::clean = true;
}
