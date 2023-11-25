#include <Windows.h>
#include <iostream>
#include <oleauto.h> // Librería para administrar fechas
#include <commctrl.h>
#include <windowsx.h>
#include "resource.h"

// Declaración de estructuras
struct VETERINARIO {
	char Nombre[100];
	int Cedula;
	int Clave;
	char FotoRuta[200];
	char Password[20];
};
struct NODOVET {
	VETERINARIO* Dato;
	NODOVET* Siguiente;
	NODOVET* Anterior;
};
struct VETERINARIOS {
	NODOVET* Origen;
	NODOVET* Fin;
}LISTAVET;
struct CITA {
	int ClaveVet;
	SYSTEMTIME* Dia;
	SYSTEMTIME* Hora;
	double Fecha;
	char NombreCliente[100];
	int Telefono;
	char Especie[20];
	char NombreMascota[30];
	char Motivo[500];
	char Estatus[20];
	float Costo;
};
struct NODOCITA {
	CITA* Dato;
	NODOCITA* Siguiente;
	NODOCITA* Anterior;
};
struct CITAS {
	NODOCITA* Origen;
	NODOCITA* Fin;
}LISTACITA;

HINSTANCE hInst;  // Instancia actual
int ActiveVet = 000; // Veterinario actual (Bruh)

// Declaración de funciones
LRESULT CALLBACK LoginCallback(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK AgendaCallback(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK CitasCallback(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK PerfilModCallback(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK PerfilCrearCallback(HWND, UINT, WPARAM, LPARAM);
BOOL Menu(INT, HWND);

VETERINARIO* crearVet(char*, int, int, char*, char*);
void modVet(char*, int, int, char*, char*);
NODOVET* nuevoNodoVet(VETERINARIO*);
void agregarVetFinal(VETERINARIO*);
NODOVET* buscarPorClave(int);
CITA* crearCita(HWND, int);
void modCita(HWND, int);
NODOCITA* nuevoNodoCita(CITA*);
void agregarCita(CITA*);
NODOCITA* buscarCitaPorDia(int claveVet, SYSTEMTIME* dia);
NODOCITA* buscarCitaPorFecha(int claveVet, int fecha);

bool ValidarLetras(const char*, int);
bool ValidarNumeros(const char*, int, const char*, int, int);
bool ValidarTelefono(const char*, int);
bool ValidarPrecio(const char*, int);

// Función principal/Callbacks/Menu
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, PSTR cmdLine, INT cShow) {

	//Inicialización
	HWND hWindow = CreateDialog(hInst, MAKEINTRESOURCE(DLG_LOGIN), NULL, LoginCallback);
	MSG Msg;
	ZeroMemory(&Msg, sizeof(Msg));
	agregarVetFinal(crearVet((char*)"Administrador", 1234567, 001, (char*)"X", (char*)"1"));
	LISTACITA.Origen = NULL;
	LISTACITA.Fin = NULL;

	// Ventana y ciclo de mensajes
	ShowWindow(hWindow, cShow);
	while (GetMessage(&Msg, nullptr, NULL, NULL)) {
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
					NODOVET* Busqueda = buscarPorClave(GetDlgItemInt(hwnd, EDIT_LOGIN_CLAVE, 0, 0));
					if (Busqueda != NULL) {

						HWND hPassword = GetDlgItem(hwnd, EDIT_LOGIN_PASSWORD);
						char wPassword[20];
						GetDlgItemText(hwnd, EDIT_LOGIN_PASSWORD, wPassword, 20);

						MessageBox(hwnd, Busqueda->Dato->Password, wPassword, MB_OK);

						if (strcmp(Busqueda->Dato->Password, wPassword) == 0){
							ActiveVet = Busqueda->Dato->Clave;
							HWND window = CreateDialog(hInst, MAKEINTRESOURCE(DLG_AGENDA_PORFECHA), NULL, AgendaCallback);
							ShowWindow(window, SW_SHOW);
							EndDialog(hwnd, 0);
						}else
							MessageBox(hwnd, "Contraseña inválida", "Vuelva a intentarlo", MB_OK);						
					}else
						MessageBox(hwnd, "Ingrese un usuario válido", "No se encontró el usuario", MB_OK);
				}break;
				case WM_CLOSE:
				case WM_DESTROY: {
					int result = MessageBox(hwnd, "¿Desea cerrar el programa?", "Advertencia", 1);
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
	/*HWND hCalendar = GetDlgItem(hwnd, CALENDAR_AGENDA);
	SYSTEMTIME* fechaCalendar = { 0 };
	DateTime_GetSystemtime(hCalendar, fechaCalendar);*/

	switch (msg) {
	case WM_INITDIALOG: { 
		NODOVET* busqueda = buscarPorClave(ActiveVet); 
		SetDlgItemText(hwnd, EDIT_VET_AGENDA, busqueda->Dato->Nombre); 
		SetDlgItemText(hwnd, EDIT_DIRECCION, busqueda->Dato->FotoRuta); 

		//Abrir imagen desde la ruta guardada
		OPENFILENAME ofn; 
		ZeroMemory(&ofn, sizeof(ofn)); 
		SetDlgItemText(hwnd, EDIT_DIRECCION, busqueda->Dato->FotoRuta); 
		HBITMAP imagen = 
			(HBITMAP)LoadImage( 
				hInst, 
				busqueda->Dato->FotoRuta, 
				IMAGE_BITMAP, 
				75, 75,
				LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_LOADFROMFILE); 
		if (imagen != NULL) 
			SendMessage(GetDlgItem(hwnd, PC_AGENDA_FOTO), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)imagen); 



	}
		case WM_COMMAND: {

		

			int ID = LOWORD(wParam);
			if (Menu(ID, hwnd)) {
				return FALSE;
			}				
			switch (ID) {
				// Casos de Agenda	
				case CALENDAR_AGENDA: {
					
				}break;
				case CB_AGENDA_HORA: {
					HWND hCalendar = GetDlgItem(hwnd, CALENDAR_AGENDA);
					SYSTEMTIME* fechaCalendar = { 0 };
					MonthCal_GetCurSel(hCalendar, fechaCalendar);


				}break;
			}
		}break;
	}
	return FALSE;
}
LRESULT CALLBACK CitasCallback(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {
	case WM_INITDIALOG: { 
		NODOVET* busqueda = buscarPorClave(ActiveVet); 
		SetDlgItemText(hwnd, EDIT_VET_CITA, busqueda->Dato->Nombre);  
		SetDlgItemText(hwnd, EDIT_DIRECCION, busqueda->Dato->FotoRuta); 

		//Abrir imagen desde la ruta guardada
		OPENFILENAME ofn; 
		ZeroMemory(&ofn, sizeof(ofn)); 
		SetDlgItemText(hwnd, EDIT_DIRECCION, busqueda->Dato->FotoRuta); 
		HBITMAP imagen = 
			(HBITMAP)LoadImage( 
				hInst, 
				busqueda->Dato->FotoRuta, 
				IMAGE_BITMAP, 
				75, 75,
				LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_LOADFROMFILE); 
		if (imagen != NULL) 
			SendMessage(GetDlgItem(hwnd, PC_CITA_FOTO), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)imagen);  
	}
	case WM_COMMAND: {
		int ID = LOWORD(wParam);
		if (Menu(ID, hwnd))
			return FALSE;		
		
		switch (ID) {
			// Casos de Citas
			case BTN_CREAR: {
				//Validar Nombre del dueño
				HWND hVET_NOMBRE = GetDlgItem(hwnd, EDIT_NOMBRE); 
				int Length_NOMBRE = GetWindowTextLength(hVET_NOMBRE); 
				char wVET_NOMBRE[100];  
				GetDlgItemText(hwnd, EDIT_NOMBRE, wVET_NOMBRE, 100); 
				GetWindowText(hVET_NOMBRE, wVET_NOMBRE, Length_NOMBRE + 1); 
				//Validad nombre de la mascota
				HWND hCITA_MASCOTA = GetDlgItem(hwnd, EDIT_MASCOTA); 
				int Length_MASCOTA = GetWindowTextLength(hCITA_MASCOTA); 
				char wCITA_MASCOTA[100];  
				GetDlgItemText(hwnd, EDIT_MASCOTA, wCITA_MASCOTA, 100); 
				GetWindowText(hCITA_MASCOTA, wCITA_MASCOTA, Length_MASCOTA + 1);  
				//Validar numero telefonico 
				HWND hTEL = GetDlgItem(hwnd, EDIT_TEL);  
				int LengthTEL = GetWindowTextLength(hTEL);
				char wTEL[20];
				GetDlgItemText(hwnd, EDIT_TEL, wTEL, 20);  
				GetWindowText(hTEL, wTEL, LengthTEL + 1); 
				//Validar precio
				HWND hPRECIO = GetDlgItem(hwnd, EDIT_PRECIO); 
				int LengthPRECIO = GetWindowTextLength(hPRECIO);   
				char wPRECIO[1000];  
				GetDlgItemText(hwnd, EDIT_PRECIO, wPRECIO, 1000); 
				GetWindowText(hPRECIO, wPRECIO, LengthPRECIO + 1); 

				if (!ValidarLetras(wVET_NOMBRE, Length_NOMBRE)|| !ValidarLetras(wCITA_MASCOTA, Length_MASCOTA)|| !ValidarTelefono(wTEL, LengthTEL)||!ValidarPrecio(wPRECIO, LengthPRECIO)) {
					break;
				}else {
					// Guardado de datos nuevos
					agregarCita(crearCita(hwnd, ActiveVet));
					MessageBox(NULL, "La cita ha sido agregada correctamente", "Cita:", MB_OK | MB_ICONINFORMATION); 
				}

			}break;
			case BTN_MODIFICAR: {
				//Validar Nombre del dueño
				HWND hVET_NOMBRE = GetDlgItem(hwnd, EDIT_NOMBRE);
				int Length_NOMBRE = GetWindowTextLength(hVET_NOMBRE);
				char wVET_NOMBRE[100];
				GetDlgItemText(hwnd, EDIT_NOMBRE, wVET_NOMBRE, 100);
				GetWindowText(hVET_NOMBRE, wVET_NOMBRE, Length_NOMBRE + 1);
				//Validad nombre de la mascota
				HWND hCITA_MASCOTA = GetDlgItem(hwnd, EDIT_MASCOTA);
				int Length_MASCOTA = GetWindowTextLength(hCITA_MASCOTA);
				char wCITA_MASCOTA[100];
				GetDlgItemText(hwnd, EDIT_MASCOTA, wCITA_MASCOTA, 100);
				GetWindowText(hCITA_MASCOTA, wCITA_MASCOTA, Length_MASCOTA + 1);
				//Validar numero telefonico 
				HWND hTEL = GetDlgItem(hwnd, EDIT_TEL);
				int LengthTEL = GetWindowTextLength(hTEL);
				char wTEL[20];
				GetDlgItemText(hwnd, EDIT_TEL, wTEL, 20);
				GetWindowText(hTEL, wTEL, LengthTEL + 1);
				//Validar precio
				HWND hPRECIO = GetDlgItem(hwnd, EDIT_PRECIO);
				int LengthPRECIO = GetWindowTextLength(hPRECIO);
				char wPRECIO[1000];
				GetDlgItemText(hwnd, EDIT_PRECIO, wPRECIO, 1000);
				GetWindowText(hPRECIO, wPRECIO, LengthPRECIO + 1);

				if (!ValidarLetras(wVET_NOMBRE, Length_NOMBRE) || !ValidarLetras(wCITA_MASCOTA, Length_MASCOTA) || !ValidarTelefono(wTEL, LengthTEL) || !ValidarPrecio(wPRECIO, LengthPRECIO)) {
					break;
				}
				else {
					// Guardado de datos modificados
					modCita(hwnd, ActiveVet);
					MessageBox(NULL, "La cita ha sido Modificada correctamente", "Cita:", MB_OK | MB_ICONINFORMATION);				
				}

			}break;

		}
	}break;
	}
	return FALSE;
}
LRESULT CALLBACK PerfilModCallback(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {
	case WM_INITDIALOG: {
		NODOVET* busqueda = buscarPorClave(ActiveVet);
		SetDlgItemText(hwnd, EDIT_VET_NOMBRE, busqueda->Dato->Nombre);
		SetDlgItemInt(hwnd, EDIT_CEDULA, busqueda->Dato->Cedula, NULL);
		SetDlgItemInt(hwnd, EDIT_CLAVE, busqueda->Dato->Clave, NULL);
		SetDlgItemText(hwnd, EDIT_PERFIL_PASSWORD, busqueda->Dato->Password);
		SetDlgItemText(hwnd, EDIT_DIRECCION, busqueda->Dato->FotoRuta);

		//Abrir imagen desde la ruta guardada
		OPENFILENAME ofn;
		ZeroMemory(&ofn, sizeof(ofn));	
		SetDlgItemText(hwnd, EDIT_DIRECCION, busqueda->Dato->FotoRuta);
		HBITMAP imagen =
			(HBITMAP)LoadImage(
				hInst,
				busqueda->Dato->FotoRuta,
				IMAGE_BITMAP,
				75, 75,
				LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_LOADFROMFILE);
		if (imagen != NULL)
			SendMessage( GetDlgItem(hwnd, PC_VET_FOTO), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)imagen);
	}
	case WM_COMMAND: {
		int ID = LOWORD(wParam);
		if (Menu(ID, hwnd))
			return FALSE;

		switch (ID) {
			// Casos de Perfil
			case BTN_SEARCH: {
				WCHAR ruta[300] = { 0 };
				OPENFILENAME ofn;
				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize = sizeof(OPENFILENAME);
				ofn.hwndOwner = hwnd;
				ofn.lpstrFile = (LPSTR)ruta;
				ofn.nMaxFile = 1000;
				ofn.lpstrFilter = "Bitmap\0*.bmp\0";
				ofn.nFilterIndex = 1;
				ofn.lpstrTitle = NULL;
				ofn.nMaxFileTitle = 0;
				ofn.lpstrInitialDir = NULL;
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
				if (GetOpenFileName(&ofn)) {
					SetDlgItemText(hwnd, EDIT_DIRECCION, (LPSTR)ruta);
					HBITMAP imagen =
						(HBITMAP)LoadImage(
							hInst,
							(LPSTR)ruta,
							IMAGE_BITMAP,
							75, 75,
							LR_DEFAULTCOLOR |
							LR_DEFAULTSIZE |
							LR_LOADFROMFILE);
					if (imagen != NULL)
						SendMessage(
							GetDlgItem(hwnd, PC_VET_FOTO),
							STM_SETIMAGE,
							IMAGE_BITMAP,
							(LPARAM)imagen);
				}
			}break;
			case BTN_PERFIL_APLICAR: {
				HWND hVET_NOMBRE = GetDlgItem(hwnd, EDIT_VET_NOMBRE);
				int Length_NOMBRE = GetWindowTextLength(hVET_NOMBRE);
				char wVET_NOMBRE[100];
				GetDlgItemText(hwnd, EDIT_VET_NOMBRE, wVET_NOMBRE, 100);
				GetWindowText(hVET_NOMBRE, wVET_NOMBRE, Length_NOMBRE + 1);
				//validacion de cedula
				HWND hCEDULA = GetDlgItem(hwnd, EDIT_CEDULA);
				int Length_CEDULA = GetWindowTextLength(hCEDULA);
				char wCEDULA[20];
				GetDlgItemText(hwnd, EDIT_CEDULA, wCEDULA, 20);
				GetWindowText(hCEDULA, wCEDULA, Length_CEDULA + 1);
				//validacion de clave de usuario
				HWND hCLAVE = GetDlgItem(hwnd, EDIT_CLAVE);
				int LengthCLAVE = GetWindowTextLength(hCLAVE);
				char wCLAVE[20];
				GetDlgItemText(hwnd, EDIT_CLAVE, wCLAVE, 20);
				GetWindowText(hCLAVE, wCLAVE, LengthCLAVE + 1);
				//validacion de contraseña
				HWND hCONTRASEÑA = GetDlgItem(hwnd, EDIT_PERFIL_PASSWORD);
				int LengthPASS = GetWindowTextLength(hCONTRASEÑA);
				if (!ValidarLetras(wVET_NOMBRE, Length_NOMBRE)) {
					break;
				}
				else if (!ValidarNumeros(wCEDULA, Length_CEDULA, wCLAVE, LengthCLAVE, LengthPASS)) {
					MessageBox(NULL, "Asegurate de ingresar correctamente los datos", "Aviso", MB_OK | MB_ICONERROR);
				}
				else {
					///Algoritmo para el guardado de datos///
					HWND hRUTA = GetDlgItem(hwnd, EDIT_DIRECCION);
					char wRUTA[200], wPASSWORD[20];
					GetDlgItemText(hwnd, EDIT_DIRECCION, wRUTA, 200);
					GetWindowText(hRUTA, wRUTA, GetWindowTextLength(hRUTA)+1);
					GetDlgItemText(hwnd, EDIT_PERFIL_PASSWORD_CREAR, wPASSWORD, 20);
					GetWindowText(hCONTRASEÑA, wPASSWORD, GetWindowTextLength(hCONTRASEÑA) + 1);
					modVet(wVET_NOMBRE, GetDlgItemInt(hwnd, EDIT_CEDULA, NULL, NULL), GetDlgItemInt(hwnd, EDIT_CLAVE, NULL, NULL), wRUTA, wPASSWORD);
					MessageBox(NULL, "Tus datos han sido modificados correctamente", "Bienvenido!!!", MB_OK | MB_ICONINFORMATION);
				}
			}break;
			case WM_CLOSE: 
			case WM_DESTROY: { 
				int result = MessageBox(hwnd, "¿Desea cerrar el programa?", "Advertencia", 1); 
				if (result != 1)
					break;
				PostQuitMessage(0); 
			}break;
		}
	}break;
	}
	return FALSE;
}
LRESULT CALLBACK PerfilCrearCallback(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {
	case WM_COMMAND: {
		int ID = LOWORD(wParam);
		if (Menu(ID, hwnd))
			return FALSE;

		switch (ID) {
			// Casos de Perfil	
		case BTN_SEARCH: {
			WCHAR ruta[200] = { 0 };
			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFile = (LPSTR)ruta;
			ofn.nMaxFile = 1000;
			ofn.lpstrFilter = "Bitmap\0*.bmp\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			if (GetOpenFileName(&ofn)) {
				SetDlgItemText(hwnd, EDIT_DIRECCION_CREAR, (LPSTR)ruta);
				HBITMAP imagen =
					(HBITMAP)LoadImage(
						hInst,
						(LPSTR)ruta,
						IMAGE_BITMAP,
						75, 75,
						LR_DEFAULTCOLOR |
						LR_DEFAULTSIZE |
						LR_LOADFROMFILE);
				if (imagen != NULL)
					SendMessage(
						GetDlgItem(hwnd, PC_VET_FOTO_CREAR),
						STM_SETIMAGE,
						IMAGE_BITMAP,
						(LPARAM)imagen);
			}
		}break;
		case BTN_PERFIL_CREAR: {
			HWND hVET_NOMBRE = GetDlgItem(hwnd, EDIT_VET_NOMBRE_CREAR);
			int Length_NOMBRE = GetWindowTextLength(hVET_NOMBRE);
			char wVET_NOMBRE[100];
			GetDlgItemText(hwnd, EDIT_VET_NOMBRE, wVET_NOMBRE, 100);
			GetWindowText(hVET_NOMBRE, wVET_NOMBRE, Length_NOMBRE + 1);
			//validacion de cedula
			HWND hCEDULA = GetDlgItem(hwnd, EDIT_CEDULA_CREAR);
			int Length_CEDULA = GetWindowTextLength(hCEDULA);
			char wCEDULA[100];
			GetDlgItemText(hwnd, EDIT_CEDULA, wCEDULA, 20);
			GetWindowText(hCEDULA, wCEDULA, Length_CEDULA + 1);
			//validacion de clave de usuario
			HWND hCLAVE = GetDlgItem(hwnd, EDIT_CLAVE_CREAR);
			int LengthCLAVE = GetWindowTextLength(hCLAVE);
			char wCLAVE[20];
			GetDlgItemText(hwnd, EDIT_CLAVE, wCLAVE, 20);
			GetWindowText(hCLAVE, wCLAVE, LengthCLAVE + 1);
			//validacion de contraseña
			HWND hCONTRASEÑA = GetDlgItem(hwnd, EDIT_PERFIL_PASSWORD_CREAR);
			int LengthPASS = GetWindowTextLength(hCONTRASEÑA);
			if (!ValidarLetras(wVET_NOMBRE, Length_NOMBRE)) {
				break;
			}
			else if (!ValidarNumeros(wCEDULA, Length_CEDULA, wCLAVE, LengthCLAVE, LengthPASS)) {
				MessageBox(NULL, "Asegurate de ingresar correctamente los datos", "Aviso", MB_OK | MB_ICONERROR);
			}
			else {
				///Algoritmo para el guardado de datos///*
				HWND hRUTA = GetDlgItem(hwnd, EDIT_DIRECCION_CREAR);
				char wRUTA[200], wPASSWORD[20];
				GetDlgItemText(hwnd, EDIT_DIRECCION_CREAR, wRUTA, 200);
				GetWindowText(hRUTA, wRUTA, GetWindowTextLength(hRUTA) + 1);
				GetDlgItemText(hwnd, EDIT_PERFIL_PASSWORD_CREAR, wPASSWORD, 20);
				GetWindowText(hCONTRASEÑA, wPASSWORD, GetWindowTextLength(hCONTRASEÑA) + 1);
				agregarVetFinal(crearVet(wVET_NOMBRE, GetDlgItemInt(hwnd, EDIT_CEDULA_CREAR, NULL, NULL), GetDlgItemInt(hwnd, EDIT_CLAVE_CREAR, NULL, NULL), wRUTA, wPASSWORD));
				MessageBox(NULL, "Los datos del nuevo veterinario se han guardado", "Perfil creado con exito!!!", MB_OK | MB_ICONINFORMATION);		
			}
		}break;
		case WM_CLOSE:
		case WM_DESTROY: {
			int result = MessageBox(hwnd, "¿Desea cerrar el programa?", "Advertencia", 1);
			if (result != 1)
				break;
			PostQuitMessage(0);
		}break;
		}
	}break;
	}
	return FALSE;
}
BOOL Menu(INT opcion, HWND window0) {
	switch (opcion) {
		case MENU_AGENDA_PORFECHA: {		
			DestroyWindow(window0);
			HWND window1 = CreateDialog(hInst, MAKEINTRESOURCE(DLG_AGENDA_PORFECHA), NULL, AgendaCallback);
			ShowWindow(window1, SW_SHOW);
		}break;
		case MENU_AGENDA_PORRANGO: {
			DestroyWindow(window0);
			HWND window1 = CreateDialog(hInst, MAKEINTRESOURCE(DLG_AGENDA_PORRANGO), NULL, AgendaCallback);
			ShowWindow(window1, SW_SHOW);
		}break;
		case MENU_CITAS_NUEVA: {
			DestroyWindow(window0);
			HWND window1 = CreateDialog(hInst, MAKEINTRESOURCE(DLG_CITAS_NUEVA), NULL, CitasCallback);
			ShowWindow(window1, SW_SHOW);
		}break;
		case MENU_CITAS_MODIFICAR: {
			DestroyWindow(window0);
			HWND window1 = CreateDialog(hInst, MAKEINTRESOURCE(DLG_CITAS_MODIFICAR), NULL, CitasCallback);
			ShowWindow(window1, SW_SHOW);
		}break;
		case MENU_CITAS_ELIMINAR: {
			DestroyWindow(window0);
			HWND window1 = CreateDialog(hInst, MAKEINTRESOURCE(DLG_CITAS_ELIMINAR), NULL, CitasCallback);
			ShowWindow(window1, SW_SHOW);
		}break;
		case MENU_PERFIL_MODIFICAR: {
			DestroyWindow(window0);
			HWND window1 = CreateDialog(hInst, MAKEINTRESOURCE(DLG_PERFIL_MODIFICAR), NULL, PerfilModCallback);
			ShowWindow(window1, SW_SHOW);
			
		}break;
		case MENU_PERFIL_CREAR: {
			DestroyWindow(window0);
			HWND window1 = CreateDialog(hInst, MAKEINTRESOURCE(DLG_PERFIL_CREAR), NULL, PerfilCrearCallback);
			ShowWindow(window1, SW_SHOW);
		}break;
		case ID_SALIR_LOGOUT: {
			int result = MessageBox(window0, "¿Desea cerrar sesión?", "Advertencia", 1);
			if (result != 1)
				break;
			DestroyWindow(window0);
			HWND window1 = CreateDialog(hInst, MAKEINTRESOURCE(DLG_LOGIN), NULL, LoginCallback);
			ShowWindow(window1, SW_SHOW);
		}break;
		case ID_SALIR_EXIT: {
			int result = MessageBox(window0, "¿Desea cerrar el programa?", "Advertencia", 1);
			if (result != 1)
				break;
			PostQuitMessage(0);
		}break;
		default: return FALSE;
	}return TRUE;
}

// Funciones de creación/busqueda/modificación
VETERINARIO* crearVet(char* nombre, int cedula, int clave, char* fotoRuta, char* password) {
	VETERINARIO* nuevo = new VETERINARIO;
	strcpy_s(nuevo->Nombre, 100, nombre);
	nuevo->Cedula = cedula;
	nuevo->Clave = clave;
	strcpy_s(nuevo->FotoRuta, 200, fotoRuta);
	strcpy_s(nuevo->Password, 20, password);
	return nuevo;
}
void modVet(char* nombre, int cedula, int clave, char* fotoRuta, char* password) {
	NODOVET* busqueda = buscarPorClave(ActiveVet);
	strcpy_s(busqueda->Dato->Nombre, 100, nombre);
	busqueda->Dato->Cedula = cedula;
	busqueda->Dato->Clave = clave;
	strcpy_s(busqueda->Dato->FotoRuta, 200, fotoRuta);
	strcpy_s(busqueda->Dato->Password, 20, password);
	ActiveVet = busqueda->Dato->Clave = clave;
}
NODOVET* nuevoNodoVet(VETERINARIO* dato) {
	NODOVET* nodo = new NODOVET;
	nodo->Dato = dato;
	nodo->Siguiente = NULL;
	nodo->Anterior = NULL;
	return nodo;
}
void agregarVetFinal(VETERINARIO* dato) {
	NODOVET* nodo = nuevoNodoVet(dato);
	if (LISTAVET.Origen == NULL) {
		LISTAVET.Origen = nodo;
		LISTAVET.Fin = nodo;
		nodo->Siguiente = NULL;
		nodo->Anterior = NULL;
	}
	else {
		LISTAVET.Fin->Siguiente = nodo;
		nodo->Anterior = LISTAVET.Fin;
		nodo->Siguiente = NULL;
		LISTAVET.Fin = nodo;		
	}
}
NODOVET* buscarPorClave(int buscar) {
	if (LISTAVET.Origen == NULL)
		return NULL;
	NODOVET* indice = LISTAVET.Origen;

	while (indice != NULL) {
		if (indice->Dato->Clave == buscar) {
			return indice;
			break;
		}
		indice = indice->Siguiente;
	}
	return NULL;
}
CITA* crearCita(HWND hwnd, int claveVet){
	HWND hDia = GetDlgItem(hwnd, DTP_CREAR_FECHA);
	HWND hHora = GetDlgItem(hwnd, DTP_CREAR_HORA);
	HWND hName = GetDlgItem(hwnd, EDIT_NOMBRE);
	HWND hTel = GetDlgItem(hwnd, EDIT_TEL);
	HWND hMascota = GetDlgItem(hwnd, EDIT_MASCOTA);
	HWND hEspecie = GetDlgItem(hwnd, CB_ESPECIE);
	HWND hStatus = GetDlgItem(hwnd, CB_ESTATUS);
	HWND hPrecio = GetDlgItem(hwnd, EDIT_PRECIO);
	HWND hMotivo = GetDlgItem(hwnd, EDIT_MOTIVO);

	char Nombre[100], NombreMascota[30], Motivo[500], Especie[20], Estatus[20], Precio[10];
	int Telefono = 0; float Costo = 0; 
	double hora = 0;
	double dia = 0;
	double fecha = 0;
	SYSTEMTIME diaCitas = { 0 };
	SYSTEMTIME horaCitas = { 0 };

	GetDlgItemText(hwnd, EDIT_NOMBRE, Nombre, GetWindowTextLength(hName));
	GetDlgItemText(hwnd, EDIT_MASCOTA, NombreMascota, GetWindowTextLength(hMascota));
	GetDlgItemText(hwnd, EDIT_MOTIVO, Motivo, GetWindowTextLength(hMotivo));
	GetDlgItemText(hwnd, EDIT_PRECIO, Precio, GetWindowTextLength(hPrecio));
	GetDlgItemInt(hwnd, EDIT_TEL, NULL, NULL);
	ComboBox_GetText(hEspecie, Especie, GetWindowTextLength(hEspecie));
	ComboBox_GetText(hEspecie, Estatus, GetWindowTextLength(hStatus));
	DateTime_GetSystemtime(hDia, &diaCitas);
	DateTime_GetSystemtime(hHora, &horaCitas); // Aqui
	SystemTimeToVariantTime(&diaCitas, &dia);
	SystemTimeToVariantTime(&horaCitas, &hora);
	fecha = ((int)dia) + (hora - ((int)hora));

	// Ingreso a la lista
	CITA* nuevo = new CITA;
	nuevo->ClaveVet = claveVet;
	nuevo->Dia = &diaCitas;
	nuevo->Hora = &horaCitas;
	nuevo->Fecha = fecha;
	strcpy_s(nuevo->NombreCliente, 100, Nombre);	
	nuevo->Telefono = Telefono;
	strcpy_s(nuevo->Especie, 20, Especie);
	strcpy_s(nuevo->NombreMascota, 30, NombreMascota);
	strcpy_s(nuevo->Motivo, 500, Motivo);
	strcpy_s(nuevo->Estatus, 20, Estatus);
	nuevo->Costo = atof(Precio); //Probar
	return nuevo;
}
void modCita(HWND hwnd, int claveVet) {
	HWND hDia = GetDlgItem(hwnd, DTP_CREAR_FECHA);
	HWND hHora = GetDlgItem(hwnd, DTP_CREAR_HORA);
	HWND hName = GetDlgItem(hwnd, EDIT_NOMBRE);
	HWND hTel = GetDlgItem(hwnd, EDIT_TEL);
	HWND hMascota = GetDlgItem(hwnd, EDIT_MASCOTA);
	HWND hEspecie = GetDlgItem(hwnd, CB_ESPECIE);
	HWND hStatus = GetDlgItem(hwnd, CB_ESTATUS);
	HWND hPrecio = GetDlgItem(hwnd, EDIT_PRECIO);
	HWND hMotivo = GetDlgItem(hwnd, EDIT_MOTIVO);

	char Nombre[100], NombreMascota[30], Motivo[500], Especie[20], Estatus[20], Precio[10];
	int Telefono = 0; float Costo = 0;
	double hora = 0;
	double dia = 0;
	double fecha = 0;
	SYSTEMTIME diaCitas = { 0 };
	SYSTEMTIME horaCitas = { 0 };

	GetDlgItemText(hwnd, EDIT_NOMBRE, Nombre, GetWindowTextLength(hName));
	GetDlgItemText(hwnd, EDIT_MASCOTA, NombreMascota, GetWindowTextLength(hMascota));
	GetDlgItemText(hwnd, EDIT_MOTIVO, Motivo, GetWindowTextLength(hMotivo));
	GetDlgItemText(hwnd, EDIT_PRECIO, Precio, GetWindowTextLength(hPrecio));
	GetDlgItemInt(hwnd, EDIT_TEL, NULL, NULL);
	ComboBox_GetText(hEspecie, Especie, GetWindowTextLength(hEspecie));
	ComboBox_GetText(hEspecie, Estatus, GetWindowTextLength(hStatus));
	DateTime_GetSystemtime(hDia, &diaCitas);
	DateTime_GetSystemtime(hHora, &horaCitas); // Aqui
	SystemTimeToVariantTime(&diaCitas, &dia);
	SystemTimeToVariantTime(&horaCitas, &hora);
	fecha = ((int)dia) + (hora - ((int)hora));
	NODOCITA* original = buscarCitaPorFecha(claveVet, fecha);

	// Ingreso a la lista
	original->Dato->Fecha = fecha;
	strcpy_s(original->Dato->NombreCliente, 100, Nombre);
	original->Dato->Telefono = Telefono;
	strcpy_s(original->Dato->Especie, 20, Especie);
	strcpy_s(original->Dato->NombreMascota, 30, NombreMascota);
	strcpy_s(original->Dato->Motivo, 500, Motivo);
	strcpy_s(original->Dato->Estatus, 20, Estatus);
	original->Dato->Costo = atof(Precio); //Probar
}
NODOCITA* nuevoNodoCita(CITA* dato) {
	NODOCITA* nodo = new NODOCITA;
	nodo->Dato = dato;
	nodo->Siguiente = NULL;
	nodo->Anterior = NULL;
	return nodo;
}
void agregarCita(CITA* dato) {
	NODOCITA* nodo = nuevoNodoCita(dato);
	if (LISTACITA.Origen == NULL) {
		LISTACITA.Origen = nodo;
		LISTACITA.Fin = nodo;
		nodo->Siguiente = NULL;
		nodo->Anterior = NULL;
	}
	else if (nodo->Dato->Fecha < LISTACITA.Origen->Dato->Fecha) {
		nodo->Siguiente = LISTACITA.Origen;
		nodo->Anterior = NULL;
		LISTACITA.Origen->Anterior = nodo;
		LISTACITA.Origen = nodo;
	}
	else if (nodo->Dato->Fecha > LISTACITA.Fin->Dato->Fecha) {
		nodo->Anterior = LISTACITA.Fin;
		nodo->Siguiente = NULL;
		LISTACITA.Fin->Siguiente = nodo;
		LISTACITA.Fin = nodo;
	}
	else {
		NODOCITA* temp = LISTACITA.Origen;
		while (temp != NULL) {
			if (nodo->Dato->Fecha > temp->Dato->Fecha && nodo->Dato->Fecha < temp->Siguiente->Dato->Fecha)
				break;
			temp = temp->Siguiente;
		}
		nodo->Anterior = temp;
		nodo->Siguiente = temp->Siguiente; // Dereferencing NULL??
		temp->Siguiente->Anterior = nodo;
		temp->Siguiente = nodo;
	}
}
NODOCITA* buscarCitaPorDia(int claveVet, SYSTEMTIME* dia) {
	if (LISTACITA.Origen == NULL)
		return NULL;
	NODOCITA* indice = LISTACITA.Origen;

	while (indice != NULL) {
		if (indice->Dato->Dia == dia) {
			return indice;
		}
		indice = indice->Siguiente;
	}
	return NULL;
	
}
NODOCITA* buscarCitaPorFecha(int claveVet, int fecha){
	if (LISTACITA.Origen == NULL)
		return NULL;
	NODOCITA* indice = LISTACITA.Origen;

	while (indice != NULL) {
		if (indice->Dato->Fecha == fecha) {
			return indice;
		}
		indice = indice->Siguiente;
	}
	return NULL;
}

// Funciones de validación
bool ValidarLetras(const char* Letra, int Vacio) {
	if (Vacio < 1) {
		MessageBox(NULL, "No se admiten espacios en blanco", "Info", MB_OK | MB_ICONERROR);
		return false;
	}
	else {
		for (int i = 0; Letra[i] != '\0'; ++i) {
			if (!isalpha(Letra[i]) && !isspace(Letra[i])) {
				// Si encuentra un carácter no válido, muestra un mensaje de error y devuelve false
				MessageBox(NULL, "Ingresa datos validos", "Error", MB_OK | MB_ICONERROR);
				return false;
			}
		}
	}


	return true;
}
bool ValidarNumeros(const char* cCEDULA, int CEDULA, const char* cCLAVE, int  CLAVE, int PASSWORD) {
	if (CEDULA > 8 || CEDULA < 7) {
		return false;
	}
	else if (CLAVE > 3) {
		return false;
	}
	else if (PASSWORD > 8 || PASSWORD < 1) {
		return false;
	}
	else {
		for (int i = 0; cCEDULA[i] != '\0'; ++i) {
			if (!isdigit(cCEDULA[i])) {
				return false;
			}
		}
		for (int i = 0; cCLAVE[i] != '\0'; ++i) {
			if (!isdigit(cCLAVE[i])) {
				return false;
			}
		}
	}
	return true;
}
bool ValidarTelefono(const char* cTEL, int TELEFONO) {
	for (int i = 0; cTEL[i] != '\0'; ++i) { 
		if (!isdigit(cTEL[i])) {  
			return false;
		}
	}
	if (TELEFONO==8 ||TELEFONO == 10||TELEFONO == 12) {
		return true;
	}	else {
		MessageBox(NULL, "Ingresa un Numero Telefonico valido", "Info", MB_OK | MB_ICONERROR);
		return false;
		
	}
	return true;
}
bool ValidarPrecio(const char* cPRECIO, int PRECIO) {
	if (PRECIO < 1) {
		MessageBox(NULL, "Ingresa un precio valido", "Info", MB_OK | MB_ICONERROR); 
		return false;
	}
	int Contador = 0;
	for (int i = 0; cPRECIO[i] != '\0'; ++i) {
		if (!isdigit(cPRECIO[i])&&cPRECIO[i]!=46) {
			MessageBox(NULL, "No se aceptan valores negativos o caracteres extraños", "Info", MB_OK | MB_ICONERROR);
			return false;
		}
		if(!isdigit(cPRECIO[i])) { 
			Contador++;
		}		
	}
	if (Contador!=0 && Contador != 1){
		MessageBox(NULL, "Ingresa un precio valido", "Info", MB_OK | MB_ICONERROR);
		return false;
	}
	return true;
}