#include "board.hpp"
#include <windows.h>

#define CELL_SIZE_PIXELS 10
#define BOARD_WIDTH_CELLS 30
#define BOARD_HEIGHT_CELLS 30

Board board(BOARD_WIDTH_CELLS, BOARD_HEIGHT_CELLS);

LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch (Message) {
		case WM_LBUTTONDOWN: {
			int col = LOWORD(lParam) / CELL_SIZE_PIXELS;
			int row = HIWORD(lParam) / CELL_SIZE_PIXELS;
			
			board.changeCellState(row, col);
			RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE);

			break;
		}

		case WM_KEYDOWN: {
			switch (wParam) {
				case VK_RETURN: {
					board.iterate();
					RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE);

					break;
				}
			}
			
			break;
		}

		case WM_PAINT: {
			RECT windowRect;
			PAINTSTRUCT ps;
			
			GetClientRect(hwnd, &windowRect);
			HDC hdc = BeginPaint(hwnd, &ps);
			
			FillRect(hdc, &windowRect, (HBRUSH) (COLOR_WINDOW + 1));
			HBRUSH brush = CreateSolidBrush(0x000000);

			for (int i = 0; i < BOARD_HEIGHT_CELLS; i++) {
				for (int j = 0; j < BOARD_WIDTH_CELLS; j++) {
					int left = j * CELL_SIZE_PIXELS;
					int top = i * CELL_SIZE_PIXELS;
					int right = j * CELL_SIZE_PIXELS + CELL_SIZE_PIXELS;
					int bottom = i * CELL_SIZE_PIXELS + CELL_SIZE_PIXELS;

					if (board.isCellOccupied(i, j)) {
						RECT rect = {left: left, top: top, right: right, bottom: bottom};
						FillRect(hdc, &rect, brush);
					} else {
						// Create empty square; causes flickering window on update
						// Rectangle(hdc, left, top, right, bottom);
					}
				}
			}
			
			EndPaint(hwnd, &ps);
			break;
		}
		
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
		
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASS wc;
	HWND hwnd;
	MSG msg;
	
	memset(&wc, 0, sizeof(wc));
	wc.style		 = 0;
	wc.lpfnWndProc	 = WndProc;
	wc.cbClsExtra 	 = 0;
	wc.cbWndExtra	 = 0;
	wc.hInstance	 = hInstance;
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = TEXT("WindowClass");
	RegisterClass(&wc);
	
	hwnd = CreateWindow(
		TEXT("WindowClass"),
		TEXT("Game of Life"),
		(WS_VISIBLE | WS_OVERLAPPEDWINDOW) & (~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX & ~WS_SIZEBOX),
		CW_USEDEFAULT, CW_USEDEFAULT,
		CELL_SIZE_PIXELS * BOARD_WIDTH_CELLS + 6, CELL_SIZE_PIXELS * BOARD_HEIGHT_CELLS + 29,
		NULL,
		NULL,
		hInstance,
		NULL);
    
	ShowWindow(hwnd, TRUE);
    UpdateWindow(hwnd);
	
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	return msg.wParam;
}