#include <Windows.h>
#include <iostream>
#include "resource.h"

HINSTANCE hInst;  // Instancia actual

LRESULT CALLBACK LoginCallback(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK AgendaCallback(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK CitasCallback(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK PerfilCallback(HWND, UINT, WPARAM, LPARAM);

BOOL Menu(INT, HWND);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, PSTR cmdLine, INT cShow) {

	//Inizialisación
	HWND hWindow = CreateDialogW(hInst, MAKEINTRESOURCE(DLG_LOGIN), NULL, LoginCallback);
	MSG Msg;
	ZeroMemory(&Msg, sizeof(Msg));

	// Ventana y ciclo de mensajes
	ShowWindow(hWindow, cShow);
	while (GetMessage(&Msg, hWindow, NULL, NULL)) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
}
LRESULT CALLBACK LoginCallback(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {
		case WM_COMMAND: {
			int ID = LOWORD(wParam);
			if (Menu(ID, hwnd))
				return FALSE;
			switch (ID) {
					// Casos de Login
				case BTN_LOGIN: {
					HWND window = CreateDialog(hInst, MAKEINTRESOURCE(DLG_AGENDA), NULL, AgendaCallback);
					ShowWindow(window, SW_SHOW);
					EndDialog(hwnd, 0);
				}break;
				case WM_CLOSE:
				case WM_DESTROY: {
					int result = MessageBox(hwnd, L"¿Desea cerrar el programa?", L"Advertencia", 1);
					if (result != 1)
						break;
					PostQuitMessage(0);
				}break;
			}
		}break;
	}
	return FALSE;
}
LRESULT CALLBACK AgendaCallback(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {
		case WM_COMMAND: {
			int ID = LOWORD(wParam);
			if (Menu(ID, hwnd)) {
				return FALSE;
			}				
			switch (ID) {
				// Casos de Agenda	
			}
		}break;
	}
	return FALSE;
}
LRESULT CALLBACK CitasCallback(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {
	case WM_COMMAND: {
		int ID = LOWORD(wParam);
		if (Menu(ID, hwnd))
			return FALSE;

		switch (ID) {
			// Casos de Citas
			case BTN_NUEVA:{

			}break;
		}
	}break;
	}
	return FALSE;
}
LRESULT CALLBACK PerfilCallback(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {
	case WM_COMMAND: {
		int ID = LOWORD(wParam);
		if (Menu(ID, hwnd))
			return FALSE;

		switch (ID) {
			// Casos de Perfil	
		}
	}break;
	}
	return FALSE;
}
BOOL Menu(INT opcion, HWND window) {
	switch (opcion) {
		case MENU_AGENDA: {
			HWND window = CreateDialog(hInst, MAKEINTRESOURCE(DLG_AGENDA), NULL, AgendaCallback);
			ShowWindow(window, SW_SHOW);
		}break;
		case MENU_CITAS: {
			HWND window = CreateDialog(hInst, MAKEINTRESOURCE(DLG_CITAS), NULL, CitasCallback);
			ShowWindow(window, SW_SHOW);
		}break;
		case MENU_PERFIL: {
			HWND window = CreateDialog(hInst, MAKEINTRESOURCE(DLG_PERFIL), NULL, PerfilCallback);
			ShowWindow(window, SW_SHOW);
		}break;
		case MENU_SALIR: {
			PostQuitMessage(0);
		}break;
		default: return FALSE;
	}return TRUE;
}