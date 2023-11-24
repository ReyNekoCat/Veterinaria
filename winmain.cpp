#include <Windows.h>
#include <iostream>
#include <oleauto.h> // Librería para administrar fechas
#include <commctrl.h>
#include <windowsx.h>
#include "resource.h"

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


bool ValidarLetras(const char* Letra, int Vacio) {
	if (Vacio < 1) {
		MessageBox(NULL, "No se admiten espacios en blanco", "Info", MB_OK | MB_ICONERROR); 
		return false;
	}else {
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
	if (CEDULA>8||CEDULA<7) {		
		return false;
	}else if (CLAVE != 3) {	
		return false;
	}else if(PASSWORD > 8 || PASSWORD < 1){
		return false;
	}else {
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
HINSTANCE hInst;  // Instancia actual
int ActiveVet = 000; // Veterinario actual (Bruh)

LRESULT CALLBACK LoginCallback(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK AgendaCallback(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK CitasCallback(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK PerfilCallback(HWND, UINT, WPARAM, LPARAM);
BOOL Menu(INT, HWND);

VETERINARIO* crearVet(char*, int, int, char*, char*);
NODOVET* nuevoNodoVet(VETERINARIO*);
void agregarVetFinal(VETERINARIO*);
NODOVET* buscarPorClave(int);
CITA* crearCita(HWND, int);
NODOCITA* nuevoNodoCita(CITA*);
void agregarCita(CITA*);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, PSTR cmdLine, INT cShow) {

	//Inicialización
	HWND hWindow = CreateDialog(hInst, MAKEINTRESOURCE(DLG_LOGIN), NULL, LoginCallback);
	MSG Msg;
	ZeroMemory(&Msg, sizeof(Msg));
	LISTAVET.Origen = NULL;
	LISTAVET.Fin = NULL;
	LISTACITA.Origen = NULL;
	LISTACITA.Fin = NULL;
	agregarVetFinal(crearVet((char*)"Administrador", 0000000, 001, (char*)"X", (char*)"Admin"));

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

	HWND hCalendar = GetDlgItem(hwnd, CALENDAR_AGENDA);
	SYSTEMTIME* fechaCalendar = { 0 };
	DateTime_GetSystemtime(hCalendar, fechaCalendar);

	switch (msg) {
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
	case WM_COMMAND: {
		int ID = LOWORD(wParam);
		if (Menu(ID, hwnd))
			return FALSE;
		if (LOWORD(wParam) == BTN_CREAR && HIWORD(wParam) == BN_CLICKED) {
			
			HWND hName = GetDlgItem(hwnd, EDIT_NOMBRE);
			int iTextLength = GetWindowTextLength(hName);
			char name[100];
			GetWindowText(hName, name, iTextLength+1);
			
			if (iTextLength < 1) {
				MessageBox(NULL, "Ingresa datos", "Error", MB_OK);
			}
			else {
				//Guardar la info

				MessageBox(NULL, name, "Error", MB_OK);
			}

		}
		switch (ID) {
			// Casos de Citas
			case BTN_CREAR: {
				agregarCita(crearCita(hwnd, ActiveVet));
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
			case BTN_PERFIL_CREAR:{ 				  
				HWND hVET_NOMBRE = GetDlgItem(hwnd, EDIT_VET_NOMBRE);   
				int Length_NOMBRE = GetWindowTextLength(hVET_NOMBRE); 
				char wVET_NOMBRE[100];
				GetDlgItemText(hwnd, EDIT_VET_NOMBRE , wVET_NOMBRE, 100);
				GetWindowText(hVET_NOMBRE, wVET_NOMBRE, Length_NOMBRE + 1);	
				//validacion de cedula
				HWND hCEDULA = GetDlgItem(hwnd, EDIT_CEDULA);
				int Length_CEDULA = GetWindowTextLength(hCEDULA);
				char wCEDULA[100];
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
				}else if (!ValidarNumeros(wCEDULA, Length_CEDULA, wCLAVE, LengthCLAVE, LengthPASS)) {
					MessageBox(NULL, "Asegurate de ingresar correctamente los datos", "Aviso", MB_OK | MB_ICONERROR); 
				}
				else {
					///Algoritmo para el guardado de datos///*
					MessageBox(NULL, "Tus datos han sido ingresados correctamente", "Bienvenido!!!", MB_OK | MB_ICONINFORMATION); 
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
				char wCEDULA[100];
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
					///Algoritmo para el guardado de datos///*
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
			HWND window1 = CreateDialog(hInst, MAKEINTRESOURCE(DLG_PERFIL_MODIFICAR), NULL, PerfilCallback);
			ShowWindow(window1, SW_SHOW);
		}break;
		case MENU_PERFIL_CREAR: {
			DestroyWindow(window0);
			HWND window1 = CreateDialog(hInst, MAKEINTRESOURCE(DLG_PERFIL_CREAR), NULL, PerfilCallback);
			ShowWindow(window1, SW_SHOW);
		}break;
		case MENU_SALIR: {
			int result = MessageBox(window0, "¿Desea cerrar el programa?", "Advertencia", 1);
			if (result != 1)
				break;
			PostQuitMessage(0);
		}break;
		default: return FALSE;
	}return TRUE;
}

VETERINARIO* crearVet(char* nombre, int cedula, int clave, char* fotoRuta, char* password) {
	VETERINARIO* nuevo = new VETERINARIO;
	strcpy_s(nuevo->Nombre, 100, nombre);
	nuevo->Cedula = cedula;
	nuevo->Clave = clave;
	strcpy_s(nuevo->FotoRuta, 200, fotoRuta);
	strcpy_s(nuevo->Password, 20, password);
	return nuevo;
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

	char Nombre[100], NombreMascota[30], Motivo[500], Especie[20], Estatus[20];
	int Telefono = 0; float Costo = 0; 
	double* hora = 0;
	double* dia = 0;
	int	fecha = 0;
	SYSTEMTIME* diaCitas = { 0 };
	SYSTEMTIME* horaCitas = { 0 };

	GetDlgItemText(hwnd, EDIT_NOMBRE, Nombre, GetWindowTextLength(hName));
	GetDlgItemText(hwnd, EDIT_MASCOTA, NombreMascota, GetWindowTextLength(hMascota));
	GetDlgItemText(hwnd, EDIT_MOTIVO, Motivo, GetWindowTextLength(hMotivo));
	GetDlgItemInt(hwnd, EDIT_TEL, NULL, NULL);
	ComboBox_GetText(hEspecie, Especie, GetWindowTextLength(hEspecie));
	ComboBox_GetText(hEspecie, Estatus, GetWindowTextLength(hStatus));
	DateTime_GetSystemtime(hDia, diaCitas);
	DateTime_GetSystemtime(hHora, horaCitas); // Marca error
	fecha = SystemTimeToVariantTime(diaCitas, dia) + SystemTimeToVariantTime(horaCitas, hora); //WIP

	// Ingreso a la lista
	CITA* nuevo = new CITA;
	nuevo->ClaveVet = claveVet;
	nuevo->Fecha = fecha; // fecha sería el VariantTime completo
	strcpy_s(nuevo->NombreCliente, 100, Nombre);	
	nuevo->Telefono = Telefono;
	strcpy_s(nuevo->Especie, 20, Especie);
	strcpy_s(nuevo->NombreMascota, 30, NombreMascota);
	strcpy_s(nuevo->Motivo, 500, Motivo);
	strcpy_s(nuevo->Estatus, 20, Estatus);
	nuevo->Costo = 200; //WIP
	return nuevo;
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
