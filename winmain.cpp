#include <Windows.h>
#include <iostream>
#include <oleauto.h> // Librería para administrar fechas
#include <commctrl.h>
#include <windowsx.h>
#include <fstream> 
#include <string>
#include <atlstr.h>  
#include <sstream>  
#include <iomanip> 
#include <format>  
#include "resource.h"
using namespace std;

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
	int Telefono;
	double Fecha;
	double varHora;
	double varDia;
	double Costo;
	char FormatHora[15];
	char FormatFecha[30];
	char NombreCliente[100];	
	char Especie[20];
	char NombreMascota[30];
	char Motivo[500];
	char Estatus[20];
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
struct TEMP_CITAS {
	NODOCITA* Origen;
	NODOCITA* Fin;
}TEMP_LISTACITA;

HINSTANCE hInst;     // Instancia actual
int ActiveVet = 0;   // Veterinario actual (Bruh)

// Variables de redirección
char RGetHora[30]  = { NULL };
char RGetFecha[20] = { NULL };
SYSTEMTIME RSysDia = { 0 };

// Declaración de funciones
LRESULT CALLBACK LoginCallback		(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK AgendaDiaCallback	(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK AgendaRangoCallback(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK CitasCrearCallback	(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK CitasModCallback	(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK PerfilModCallback	(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK PerfilCrearCallback(HWND, UINT, WPARAM, LPARAM);
BOOL Menu(INT, HWND);

VETERINARIO* crearVet(char*, int, int, char*, char*);
void modVet(char*, int, int, char*, char*);
NODOVET* nuevoNodoVet(VETERINARIO*);
void agregarVetFinal(VETERINARIO*);
NODOVET* buscarPorClave(int);
CITA* crearCita(HWND, int);
CITA* crearCitaDirecto(int, int, double, double, double, double, char*, char*, char*, char*, char*, char*, char*);
void agregarCitaFinal(CITA*);
void modCita(HWND, int);
bool deleteCita(HWND, int);
NODOCITA* nuevoNodoCita(CITA*);
NODOCITA* nuevoNodoCitaEXTRA(NODOCITA*);
void agregarCita(CITA*);
NODOCITA* buscarCitaPorDia(int, double);
NODOCITA* buscarCitaPorFecha(int, double);
NODOCITA* buscarCitaPorFormatHora(int, char*, double);
NODOCITA* buscarCitaPorFormatFecha(int, char*);
void crearTempListaCitaPorDia(int, double);
void crearTempListaCitaPorFechas(int, double, double);
void agregarCitaFinalTemp(CITA*);

char* formatoHora(LPSYSTEMTIME, char*);
char* formatoFecha(LPSYSTEMTIME, char*);

bool ValidarLetras(const char*, int);
bool ValidarNumeros(const char*, int, const char*, int, int);
bool ValidarTelefono(const char*, int);
bool ValidarPrecio(const char*, int);

void GuardarVETBIN(void); 
void GuardarCITABIN(void);
bool CargarVETBIN(VETERINARIOS&);
bool CargarCITABIN(CITAS&);

// Función principal/Callbacks/Menu
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, PSTR cmdLine, INT cShow) {

	//Inicialización
	HWND hWindow = CreateDialog(hInst, MAKEINTRESOURCE(DLG_LOGIN), NULL, LoginCallback);
	MSG Msg;
	ZeroMemory(&Msg, sizeof(Msg));

	if (CargarVETBIN(LISTAVET)) {

	}
	else {
		agregarVetFinal(crearVet((char*)"Administrador", 1234567, 001, (char*)"X", (char*)"1"));
	}


	if (!CargarCITABIN(LISTACITA)) {
		LISTACITA.Origen = NULL;
		LISTACITA.Fin = NULL;
	}
	
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

						//MessageBox(hwnd, Busqueda->Dato->Password, wPassword, MB_OK);

						if (strcmp(Busqueda->Dato->Password, wPassword) == 0){
							ActiveVet = Busqueda->Dato->Clave;
							HWND window = CreateDialog(hInst, MAKEINTRESOURCE(DLG_AGENDA_PORFECHA), NULL, AgendaDiaCallback);
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
LRESULT CALLBACK AgendaDiaCallback(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {
		case WM_INITDIALOG: { 
			NODOVET* busqueda = buscarPorClave(ActiveVet); 
			SetDlgItemText(hwnd, EDIT_VET_AGENDA, busqueda->Dato->Nombre); 
			SetDlgItemText(hwnd, EDIT_DIRECCION, busqueda->Dato->FotoRuta); 

			//Abrir imagen desde la ruta guardada
			OPENFILENAME ofn; 
			ZeroMemory(&ofn, sizeof(ofn)); 
			SetDlgItemText(hwnd, EDIT_DIRECCION, busqueda->Dato->FotoRuta); 
			HBITMAP imagen = (HBITMAP)LoadImage(hInst, busqueda->Dato->FotoRuta, IMAGE_BITMAP, 75, 75, LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_LOADFROMFILE); 
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
				case CB_AGENDA_HORA: {
					switch HIWORD(wParam) {
						case CBN_DROPDOWN: {
							//Creación de la Lista de Citas Temporal
							HWND hDTPdia = GetDlgItem(hwnd, DTP_MEJOR_QUE_MONTHCALENDAR);
							HWND hCBhora = GetDlgItem(hwnd, CB_AGENDA_HORA);
							ComboBox_ResetContent(hCBhora);
							SYSTEMTIME SysDia = { 0 };          double VarDia;
							SYSTEMTIME SysfechaActual = { 0 };  double fechaActual = 0;
							DateTime_GetSystemtime(hDTPdia, &SysDia);
							GetLocalTime(&SysfechaActual);
							SystemTimeToVariantTime(&SysDia, &VarDia);
							crearTempListaCitaPorDia(ActiveVet, VarDia);
							SystemTimeToVariantTime(&SysfechaActual, &fechaActual);
							NODOCITA* TempCita = TEMP_LISTACITA.Origen;

							//Seteo del ComboBox
							while (TempCita != NULL) {
								if (TempCita->Dato->Fecha > fechaActual) {
									ComboBox_AddString(hCBhora, (LPARAM)TempCita->Dato->FormatHora);
								}
								TempCita = TempCita->Siguiente;
							}
						}break;
					}break;
				}break;
				case BTN_ACEPTAR_HORA: {
					char GetHora[20];
					HWND hCBhora = GetDlgItem(hwnd, CB_AGENDA_HORA);
					HWND hDTPdia = GetDlgItem(hwnd, DTP_MEJOR_QUE_MONTHCALENDAR);
					SYSTEMTIME SysDia = { 0 }; double VarDia;
					DateTime_GetSystemtime(hDTPdia, &SysDia);
					SystemTimeToVariantTime(&SysDia, &VarDia);
					ComboBox_GetText(hCBhora, GetHora, 20);

					if (buscarCitaPorFormatHora(ActiveVet, GetHora, (int)VarDia) != 0 || buscarCitaPorFormatHora(ActiveVet, GetHora, (int)VarDia) != NULL) {
						SetDlgItemText(hwnd, EDIT_NOMBRE, buscarCitaPorFormatHora(ActiveVet, GetHora, (int)VarDia)->Dato->NombreCliente);
						SetDlgItemInt(hwnd, EDIT_TEL, buscarCitaPorFormatHora(ActiveVet, GetHora, (int)VarDia)->Dato->Telefono, NULL);
						SetDlgItemText(hwnd, EDIT_MASCOTA, buscarCitaPorFormatHora(ActiveVet, GetHora, (int)VarDia)->Dato->NombreMascota);
						SetDlgItemText(hwnd, CB_ESPECIE, buscarCitaPorFormatHora(ActiveVet, GetHora, (int)VarDia)->Dato->Especie);
						SetDlgItemText(hwnd, CB_ESTATUS, buscarCitaPorFormatHora(ActiveVet, GetHora, (int)VarDia)->Dato->Estatus);
						SetDlgItemText(hwnd, EDIT_MOTIVO, buscarCitaPorFormatHora(ActiveVet, GetHora, (int)VarDia)->Dato->Motivo);
						SetDlgItemText(hwnd, EDIT_PRECIO, std::to_string(buscarCitaPorFormatHora(ActiveVet, GetHora, (int)VarDia)->Dato->Costo).c_str());
					}

				}break;
				case BTN_AGENDA_MODIFICAR: {
					HWND hCBhora = GetDlgItem(hwnd, CB_AGENDA_HORA);
					HWND hDTPdia = GetDlgItem(hwnd, DTP_MEJOR_QUE_MONTHCALENDAR);
					DateTime_GetSystemtime(hDTPdia, &RSysDia);
					ComboBox_GetText(hCBhora, RGetHora, 20);
					HWND window1 = CreateDialog(hInst, MAKEINTRESOURCE(DLG_CITAS_MODIFICAR), NULL, CitasModCallback);
					ShowWindow(window1, SW_SHOW);
					DestroyWindow(hwnd);
				}break;
				case BTN_AGENDA_ELIMINAR: {
					HWND hCBhora = GetDlgItem(hwnd, CB_AGENDA_HORA);
					HWND hDTPdia = GetDlgItem(hwnd, DTP_MEJOR_QUE_MONTHCALENDAR);
					DateTime_GetSystemtime(hDTPdia, &RSysDia);
					ComboBox_GetText(hCBhora, RGetHora, 20);
					HWND window1 = CreateDialog(hInst, MAKEINTRESOURCE(DLG_CITAS_ELIMINAR), NULL, CitasModCallback);
					ShowWindow(window1, SW_SHOW);
					DestroyWindow(hwnd);
				}break;
			}
		}break;
	}
	return FALSE;
}
LRESULT CALLBACK AgendaRangoCallback(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

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
				case CB_RANGO: {
					switch HIWORD(wParam) {
						case CBN_DROPDOWN: {
							HWND hCBhora = GetDlgItem(hwnd, CB_RANGO);
							ComboBox_ResetContent(hCBhora);

							HWND hDTPdia1	= GetDlgItem(hwnd, DTP_RANGO_DIA1);
							HWND hDTPhora1  = GetDlgItem(hwnd, DTP_RANGO_HORA1);
							HWND hDTPdia2   = GetDlgItem(hwnd, DTP_RANGO_DIA2);
							HWND hDTPhora2  = GetDlgItem(hwnd, DTP_RANGO_HORA2);
							SYSTEMTIME Dia1  = { 0 };		double dia1;
							SYSTEMTIME Hora1 = { 0 };	    double hora1;
							SYSTEMTIME Dia2  = { 0 };	    double dia2;
							SYSTEMTIME Hora2 = { 0 };       double hora2;
							DateTime_GetSystemtime(hDTPdia1,  &Dia1);
							DateTime_GetSystemtime(hDTPhora1, &Hora1);
							DateTime_GetSystemtime(hDTPdia2,  &Dia2);
							DateTime_GetSystemtime(hDTPhora2, &Hora2);
							SystemTimeToVariantTime(&Dia1,  &dia1);
							SystemTimeToVariantTime(&Hora1, &hora1);
							SystemTimeToVariantTime(&Dia2,  &dia2);
							SystemTimeToVariantTime(&Hora2, &hora2);
							double fecha1, fecha2;
							fecha1 = ((int)dia1) + (hora1 - ((int)hora1));
							fecha2 = ((int)dia2) + (hora2 - ((int)hora2));

							crearTempListaCitaPorFechas(ActiveVet, fecha1, fecha2);
							NODOCITA* TempCita = TEMP_LISTACITA.Origen;
							char buff[200]; SYSTEMTIME SysFechaCita = { 0 };

							while (TempCita != NULL) {
								VariantTimeToSystemTime(TempCita->Dato->Fecha, &SysFechaCita);
								formatoFecha(&SysFechaCita, buff);
								ComboBox_AddString(hCBhora, (LPARAM)buff);
								TempCita = TempCita->Siguiente;
							}
						}break;
					}					
				}break;
				case BTN_ACEPTAR_HORA: {
					char GetFormatFecha[30];
					HWND hCBhora = GetDlgItem(hwnd, CB_RANGO);
					ComboBox_GetText(hCBhora, GetFormatFecha, 30);

					if (buscarCitaPorFormatFecha(ActiveVet, GetFormatFecha) != 0 || buscarCitaPorFormatFecha(ActiveVet, GetFormatFecha) != NULL) {
						SetDlgItemText(hwnd, EDIT_NOMBRE, buscarCitaPorFormatFecha(ActiveVet, GetFormatFecha)->Dato->NombreCliente);
						SetDlgItemInt(hwnd, EDIT_TEL, buscarCitaPorFormatFecha(ActiveVet, GetFormatFecha)->Dato->Telefono, NULL);
						SetDlgItemText(hwnd, EDIT_MASCOTA, buscarCitaPorFormatFecha(ActiveVet, GetFormatFecha)->Dato->NombreMascota);
						SetDlgItemText(hwnd, CB_ESPECIE, buscarCitaPorFormatFecha(ActiveVet, GetFormatFecha)->Dato->Especie);
						SetDlgItemText(hwnd, CB_ESTATUS, buscarCitaPorFormatFecha(ActiveVet, GetFormatFecha)->Dato->Estatus);
						SetDlgItemText(hwnd, EDIT_MOTIVO, buscarCitaPorFormatFecha(ActiveVet, GetFormatFecha)->Dato->Motivo);
						SetDlgItemText(hwnd, EDIT_PRECIO, std::to_string(buscarCitaPorFormatFecha(ActiveVet, GetFormatFecha)->Dato->Costo).c_str());
					}

				}break;
				case BTN_AGENDA_MODIFICAR: {
					HWND hCBfecha = GetDlgItem(hwnd, CB_RANGO);
					ComboBox_GetText(hCBfecha, RGetFecha, 30);
					HWND window1 = CreateDialog(hInst, MAKEINTRESOURCE(DLG_CITAS_MODIFICAR), NULL, CitasModCallback);
					ShowWindow(window1, SW_SHOW);
					DestroyWindow(hwnd);
				}break;
				case BTN_AGENDA_ELIMINAR: {
					HWND hCBfecha = GetDlgItem(hwnd, CB_RANGO);
					ComboBox_GetText(hCBfecha, RGetFecha, 30);
					HWND window1 = CreateDialog(hInst, MAKEINTRESOURCE(DLG_CITAS_MODIFICAR), NULL, CitasModCallback);
					ShowWindow(window1, SW_SHOW);
					DestroyWindow(hwnd);
				}break;
			}
		}break;
	}
	return FALSE;
}
LRESULT CALLBACK CitasCrearCallback(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

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

		//Combobox de especie
		ifstream archivo("especies.txt");
		ifstream TXT("Estatus.txt");
		if (archivo.is_open()) {
			string especie;
			HWND hCB_ESPECIE = GetDlgItem(hwnd, CB_ESPECIE);
			// Lee el archivo línea por línea y agrega cada especie al ComboBox
			while (getline(archivo, especie)) {
				SendMessage(hCB_ESPECIE, CB_ADDSTRING, NULL, (LPARAM)especie.c_str());

			}
		}
		if (TXT.is_open()) {
			string Estatus;
			HWND hCB_Estatus = GetDlgItem(hwnd, CB_ESTATUS);
			// Lee el archivo línea por línea y agrega cada especie al ComboBox
			while (getline(TXT, Estatus)) {
				SendMessage(hCB_Estatus, CB_ADDSTRING, NULL, (LPARAM)Estatus.c_str());

			}
		}
		// Cierra el archivo
		archivo.close();
		TXT.close();
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
				//Validar Fecha y Hora
				HWND hDia = GetDlgItem(hwnd, DTP_CREAR_FECHA);
				HWND hHora = GetDlgItem(hwnd, DTP_CREAR_HORA);
				double hora = 0;  double fecha = 0;
				double dia = 0;   double fechaActual = 0;			
				SYSTEMTIME diaCitas = { 0 }; 
				SYSTEMTIME horaCitas = { 0 };
				SYSTEMTIME SysfechaActual = { 0 };
				DateTime_GetSystemtime(hDia, &diaCitas);
				DateTime_GetSystemtime(hHora, &horaCitas);
				GetLocalTime(&SysfechaActual);
				SystemTimeToVariantTime(&diaCitas, &dia);
				SystemTimeToVariantTime(&horaCitas, &hora);
				SystemTimeToVariantTime(&SysfechaActual, &fechaActual);
				fecha = ((int)dia) + (hora - ((int)hora));	

				if ((buscarCitaPorFecha(ActiveVet, fecha) != NULL) || (fecha < fechaActual)) {
					MessageBox(NULL, "La fecha no está disponible (Ocupada o pasada)", "Error al crear", MB_OK | MB_ICONERROR);
					break;
				}					
				else if (!ValidarLetras(wVET_NOMBRE, Length_NOMBRE)|| !ValidarLetras(wCITA_MASCOTA, Length_MASCOTA)|| !ValidarTelefono(wTEL, LengthTEL)||!ValidarPrecio(wPRECIO, LengthPRECIO))
					break;
				else {
					// Guardado de datos nuevos
					agregarCita(crearCita(hwnd, ActiveVet));
					MessageBox(NULL, "La cita ha sido agregada correctamente", "Creación exitosa", MB_OK | MB_ICONINFORMATION); 
				}
			}break;
			
		}
	}break;
	}
	return FALSE;
}
LRESULT CALLBACK CitasModCallback(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {
	case WM_INITDIALOG: {
		NODOVET* busqueda = buscarPorClave(ActiveVet);
		SetDlgItemText(hwnd, EDIT_VET_CITA, busqueda->Dato->Nombre);
		SetDlgItemText(hwnd, EDIT_DIRECCION, busqueda->Dato->FotoRuta);

		

		//Abrir imagen desde la ruta guardada
		OPENFILENAME ofn;
		ZeroMemory(&ofn, sizeof(ofn));
		SetDlgItemText(hwnd, EDIT_DIRECCION, busqueda->Dato->FotoRuta);
		HBITMAP imagen = (HBITMAP)LoadImage(hInst, busqueda->Dato->FotoRuta, IMAGE_BITMAP, 75, 75, LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_LOADFROMFILE);
		if (imagen != NULL)
			SendMessage(GetDlgItem(hwnd, PC_CITA_FOTO), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)imagen);

		//Setear DTP y CB si se redireccionó
		HWND hDTPdia = GetDlgItem(hwnd, DTP_MODIFICAR_FECHA);
		HWND hCBhora = GetDlgItem(hwnd, CB_MODIFICAR_HORA);
		if (RGetHora[0] != NULL && RSysDia.wYear != 0) {
			DateTime_SetSystemtime(hDTPdia, GDT_VALID, &RSysDia);
			ComboBox_SetText(hCBhora, RGetHora);
			RGetHora[0] = NULL;
			RSysDia.wYear = 0;
		}
		else if (RGetFecha[0] != NULL) {
			NODOCITA* RFecha = buscarCitaPorFormatFecha(ActiveVet, RGetFecha);
			SYSTEMTIME SysDiaBuff = { 0 };
			VariantTimeToSystemTime(RFecha->Dato->varDia, &SysDiaBuff);
			DateTime_SetSystemtime(hDTPdia, GDT_VALID, &SysDiaBuff);
			ComboBox_SetText(hCBhora, RFecha->Dato->FormatHora);
			RGetFecha[0] = NULL;
		}
		//Combobox de especie
		ifstream archivo("especies.txt");
		ifstream TXT("Estatus.txt");  
		if (archivo.is_open()) {
			string especie;
			HWND hCB_ESPECIE = GetDlgItem(hwnd, CB_ESPECIE);
			// Lee el archivo línea por línea y agrega cada especie al ComboBox
			while (getline(archivo, especie)) {
				SendMessage(hCB_ESPECIE, CB_ADDSTRING, NULL, (LPARAM)especie.c_str()); 

			}			 
		}	
		if (TXT.is_open()) {  
			string Estatus; 
			HWND hCB_Estatus = GetDlgItem(hwnd, CB_ESTATUS); 
			// Lee el archivo línea por línea y agrega cada especie al ComboBox
			while (getline(TXT, Estatus)) {  
				SendMessage(hCB_Estatus, CB_ADDSTRING, NULL, (LPARAM)Estatus.c_str());  

			}		
		}
		// Cierra el archivo
		archivo.close();
		TXT.close();
	}break;
	case WM_COMMAND: {
		int ID = LOWORD(wParam);
		if (Menu(ID, hwnd))
			return FALSE;
		switch (ID) {
			// Casos de Citas
			case CB_MODIFICAR_HORA: {
				switch HIWORD(wParam) {
				case CBN_DROPDOWN: {
					//Creación de la Lista de Citas Temporal
					HWND hDTPdia = GetDlgItem(hwnd, DTP_MODIFICAR_FECHA);
					HWND hCBhora = GetDlgItem(hwnd, CB_MODIFICAR_HORA);
					ComboBox_ResetContent(hCBhora);
					SYSTEMTIME SysDia = { 0 };          double VarDia;
					SYSTEMTIME SysfechaActual = { 0 };  double fechaActual = 0;
					DateTime_GetSystemtime(hDTPdia, &SysDia);
					GetLocalTime(&SysfechaActual);
					SystemTimeToVariantTime(&SysDia, &VarDia);
					crearTempListaCitaPorDia(ActiveVet, VarDia);
					SystemTimeToVariantTime(&SysfechaActual, &fechaActual);
					NODOCITA* TempCita = TEMP_LISTACITA.Origen;

					//Seteo del ComboBox
					while (TempCita != NULL) {
						if (TempCita->Dato->Fecha > fechaActual) {
							ComboBox_AddString(hCBhora, (LPARAM)TempCita->Dato->FormatHora);
						}
						TempCita = TempCita->Siguiente;
					}
				}break;
				}break;
			}break;
			case BTN_ACEPTAR_HORA: {
				char GetHora[20];
				HWND hCBhora = GetDlgItem(hwnd, CB_MODIFICAR_HORA);
				HWND hDTPdia = GetDlgItem(hwnd, DTP_MODIFICAR_FECHA);
				SYSTEMTIME SysDia = { 0 }; double VarDia;
				DateTime_GetSystemtime(hDTPdia, &SysDia);
				SystemTimeToVariantTime(&SysDia, &VarDia);
				ComboBox_GetText(hCBhora, GetHora, 20);

				if (buscarCitaPorFormatHora(ActiveVet, GetHora, (int)VarDia) != 0 || buscarCitaPorFormatHora(ActiveVet, GetHora, (int)VarDia) != NULL) {
					SetDlgItemText(hwnd, EDIT_NOMBRE, buscarCitaPorFormatHora(ActiveVet, GetHora, (int)VarDia)->Dato->NombreCliente);
					SetDlgItemInt(hwnd, EDIT_TEL, buscarCitaPorFormatHora(ActiveVet, GetHora, (int)VarDia)->Dato->Telefono, NULL);
					SetDlgItemText(hwnd, EDIT_MASCOTA, buscarCitaPorFormatHora(ActiveVet, GetHora, (int)VarDia)->Dato->NombreMascota);
					SetDlgItemText(hwnd, CB_ESPECIE, buscarCitaPorFormatHora(ActiveVet, GetHora, (int)VarDia)->Dato->Especie);
					SetDlgItemText(hwnd, CB_ESTATUS, buscarCitaPorFormatHora(ActiveVet, GetHora, (int)VarDia)->Dato->Estatus);
					SetDlgItemText(hwnd, EDIT_MOTIVO, buscarCitaPorFormatHora(ActiveVet, GetHora, (int)VarDia)->Dato->Motivo);
					SetDlgItemText(hwnd, EDIT_PRECIO, std::to_string(buscarCitaPorFormatHora(ActiveVet, GetHora, (int)VarDia)->Dato->Costo).c_str());
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
			case BTN_ELIMINAR: {
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
					// Eliminación de datos
					deleteCita(hwnd, ActiveVet);
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
				HWND hPASSWORD = GetDlgItem(hwnd, EDIT_PERFIL_PASSWORD);
				int LengthPASS = GetWindowTextLength(hPASSWORD);
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
					GetWindowText(hPASSWORD, wPASSWORD, GetWindowTextLength(hPASSWORD) + 1);
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
			HWND hPASSWORD = GetDlgItem(hwnd, EDIT_PERFIL_PASSWORD_CREAR);
			int LengthPASS = GetWindowTextLength(hPASSWORD);

			if (buscarPorClave(GetDlgItemInt(hwnd, EDIT_CLAVE_CREAR, NULL, NULL)) != NULL) {
				MessageBox(NULL, "La clave ingresada no está disponible", "Aviso", MB_OK | MB_ICONERROR);
			} else if (!ValidarLetras(wVET_NOMBRE, Length_NOMBRE)) {
				break;
			}
			else if (!ValidarNumeros(wCEDULA, Length_CEDULA, wCLAVE, LengthCLAVE, LengthPASS)) {
				MessageBox(NULL, "Asegurate de ingresar correctamente los datos", "Aviso", MB_OK | MB_ICONERROR);
			}
			else {
				///Algoritmo para el guardado de datos///
				HWND hRUTA = GetDlgItem(hwnd, EDIT_DIRECCION_CREAR);
				char wRUTA[200], wPASSWORD[20];
				GetDlgItemText(hwnd, EDIT_DIRECCION_CREAR, wRUTA, 200);
				GetWindowText(hRUTA, wRUTA, GetWindowTextLength(hRUTA) + 1);
				GetDlgItemText(hwnd, EDIT_PERFIL_PASSWORD_CREAR, wPASSWORD, 20);
				GetWindowText(hPASSWORD, wPASSWORD, GetWindowTextLength(hPASSWORD) + 1);
				agregarVetFinal(crearVet(wVET_NOMBRE, GetDlgItemInt(hwnd, EDIT_CEDULA_CREAR, NULL, NULL), GetDlgItemInt(hwnd, EDIT_CLAVE_CREAR, NULL, NULL), wRUTA, wPASSWORD));
				MessageBox(NULL, "Los datos del nuevo veterinario se han guardado", "Perfil creado con exito", MB_OK | MB_ICONINFORMATION);		
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
			HWND window1 = CreateDialog(hInst, MAKEINTRESOURCE(DLG_AGENDA_PORFECHA), NULL, AgendaDiaCallback);
			ShowWindow(window1, SW_SHOW);
		}break;
		case MENU_AGENDA_PORRANGO: {
			DestroyWindow(window0);
			HWND window1 = CreateDialog(hInst, MAKEINTRESOURCE(DLG_AGENDA_PORRANGO), NULL, AgendaRangoCallback);
			ShowWindow(window1, SW_SHOW);
		}break;
		case MENU_CITAS_NUEVA: {
			DestroyWindow(window0);
			HWND window1 = CreateDialog(hInst, MAKEINTRESOURCE(DLG_CITAS_NUEVA), NULL, CitasCrearCallback);
			ShowWindow(window1, SW_SHOW);
		}break;
		case MENU_CITAS_MODIFICAR: {
			DestroyWindow(window0);
			HWND window1 = CreateDialog(hInst, MAKEINTRESOURCE(DLG_CITAS_MODIFICAR), NULL, CitasModCallback);
			ShowWindow(window1, SW_SHOW);
		}break;
		case MENU_CITAS_ELIMINAR: {
			DestroyWindow(window0);
			HWND window1 = CreateDialog(hInst, MAKEINTRESOURCE(DLG_CITAS_ELIMINAR), NULL, CitasModCallback);
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
			GuardarVETBIN();
			GuardarCITABIN();
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
	float Costo = 0; 
	double hora = 0;
	double dia = 0;
	double fecha = 0;
	SYSTEMTIME diaCitas = { 0 };
	SYSTEMTIME horaCitas = { 0 };
	SYSTEMTIME horaBuff = { 0 };
	SYSTEMTIME fechaBuff = { 0 };

	GetDlgItemText(hwnd, EDIT_NOMBRE, Nombre, GetWindowTextLength(hName)+1);
	GetDlgItemText(hwnd, EDIT_MASCOTA, NombreMascota, GetWindowTextLength(hMascota)+1);
	GetDlgItemText(hwnd, EDIT_MOTIVO, Motivo, GetWindowTextLength(hMotivo)+1);
	GetDlgItemText(hwnd, EDIT_PRECIO, Precio, GetWindowTextLength(hPrecio)+1);
	
	ComboBox_GetText(hEspecie, Especie, GetWindowTextLength(hEspecie)+1);
	ComboBox_GetText(hStatus, Estatus, GetWindowTextLength(hStatus)+1);
	DateTime_GetSystemtime(hDia, &diaCitas);
	DateTime_GetSystemtime(hHora, &horaCitas);
	SystemTimeToVariantTime(&diaCitas, &dia);
	SystemTimeToVariantTime(&horaCitas, &hora);
	fecha = ((int)dia) + (hora - ((int)hora));

	// Ingreso a la lista
	CITA* nuevo = new CITA;
	nuevo->ClaveVet = claveVet;
	nuevo->Fecha = fecha;
	nuevo->varDia = (int)dia;
	nuevo->varHora = (hora - (int)hora);
	VariantTimeToSystemTime((hora - (int)hora), &horaBuff);
	VariantTimeToSystemTime(fecha, &fechaBuff);
	formatoHora(&horaBuff, nuevo->FormatHora);
	formatoFecha(&fechaBuff, nuevo->FormatFecha);
	strcpy_s(nuevo->NombreCliente, 100, Nombre);	
	nuevo->Telefono = GetDlgItemInt(hwnd, EDIT_TEL, NULL, NULL);
	strcpy_s(nuevo->Especie, 20, Especie);
	strcpy_s(nuevo->NombreMascota, 30, NombreMascota);
	strcpy_s(nuevo->Motivo, 500, Motivo);
	strcpy_s(nuevo->Estatus, 20, Estatus);
	nuevo->Costo = atof(Precio); //Probar
	return nuevo;
}
CITA* crearCitaDirecto(int ClaveVet, int Telefono, double Fecha, double varHora, double varDia, double Costo, char* FormatHora, char* FormatFecha, char* NombreCliente, char* Especie, char* NombreMascota, char* Motivo, char* Estatus) {

//	fecha = ((int)dia) + (hora - ((int)hora));

	// Ingreso a la lista
	CITA* nuevo = new CITA;
	nuevo->ClaveVet = ClaveVet;
	nuevo->Telefono = Telefono;
	nuevo->Fecha = Fecha;
	nuevo->varHora = varHora;
	nuevo->varDia = varDia;
	nuevo->Costo = Costo;
	strcpy_s(nuevo->FormatHora, 15, FormatHora);
	strcpy_s(nuevo->FormatFecha, 30, FormatFecha);
	strcpy_s(nuevo->NombreCliente, 100, NombreCliente);
	strcpy_s(nuevo->Especie, 20, Especie);
	strcpy_s(nuevo->NombreMascota, 30, NombreMascota);
	strcpy_s(nuevo->Motivo, 500, Motivo);
	strcpy_s(nuevo->Estatus, 20, Estatus);
	return nuevo;
}
void agregarCitaFinal(CITA* dato) {
	NODOCITA* nodo = nuevoNodoCita(dato);
	if (LISTACITA.Origen == NULL) {
		LISTACITA.Origen = nodo;
		LISTACITA.Fin = nodo;
		nodo->Siguiente = NULL;
		nodo->Anterior = NULL;
	}
	else {
		LISTACITA.Fin->Siguiente = nodo;
		nodo->Anterior = LISTACITA.Fin;
		nodo->Siguiente = NULL;
		LISTACITA.Fin = nodo;
	}
}
void modCita(HWND hwnd, int claveVet) {
	HWND hDia = GetDlgItem(hwnd, DTP_MODIFICAR_FECHA);
	HWND hCBhora = GetDlgItem(hwnd, CB_MODIFICAR_HORA);
	HWND hName = GetDlgItem(hwnd, EDIT_NOMBRE);
	HWND hTel = GetDlgItem(hwnd, EDIT_TEL);
	HWND hMascota = GetDlgItem(hwnd, EDIT_MASCOTA);
	HWND hEspecie = GetDlgItem(hwnd, CB_ESPECIE);
	HWND hStatus = GetDlgItem(hwnd, CB_ESTATUS);
	HWND hPrecio = GetDlgItem(hwnd, EDIT_PRECIO);
	HWND hMotivo = GetDlgItem(hwnd, EDIT_MOTIVO);

	char FormatHora[15], Nombre[100], NombreMascota[30], Motivo[500], Especie[20], Estatus[20], Precio[10];
	float Costo = 0;
	double hora = 0;
	double dia = 0;
	double fecha = 0;
	SYSTEMTIME diaCitas = { 0 };
	SYSTEMTIME horaCitas = { 0 };

	GetDlgItemText(hwnd, CB_MODIFICAR_HORA, FormatHora, GetWindowTextLength(hCBhora) + 1);
	GetDlgItemText(hwnd, EDIT_NOMBRE, Nombre, GetWindowTextLength(hName) + 1);
	GetDlgItemText(hwnd, EDIT_MASCOTA, NombreMascota, GetWindowTextLength(hMascota) + 1);
	GetDlgItemText(hwnd, EDIT_MOTIVO, Motivo, GetWindowTextLength(hMotivo) + 1);
	GetDlgItemText(hwnd, EDIT_PRECIO, Precio, GetWindowTextLength(hPrecio) + 1);
	
	ComboBox_GetText(hEspecie, Especie, GetWindowTextLength(hEspecie) + 1);
	ComboBox_GetText(hStatus, Estatus, GetWindowTextLength(hStatus) + 1);
	DateTime_GetSystemtime(hDia, &diaCitas);
	SystemTimeToVariantTime(&diaCitas, &dia);

	NODOCITA* original = buscarCitaPorFormatHora(claveVet, FormatHora, (int)dia);

	// Ingreso a la lista
	strcpy_s(original->Dato->NombreCliente, 100, Nombre);
	original->Dato->Telefono = GetDlgItemInt(hwnd, EDIT_TEL, NULL, NULL);
	strcpy_s(original->Dato->Especie, 20, Especie);
	strcpy_s(original->Dato->NombreMascota, 30, NombreMascota);
	strcpy_s(original->Dato->Motivo, 500, Motivo);
	strcpy_s(original->Dato->Estatus, 20, Estatus);
	original->Dato->Costo = atof(Precio);
}
bool deleteCita(HWND hwnd, int claveVet) {
	HWND hDia = GetDlgItem(hwnd, DTP_MODIFICAR_FECHA);
	HWND hCBhora = GetDlgItem(hwnd, CB_MODIFICAR_HORA);
	char FormatHora[15];
	double hora = 0, dia = 0;
	SYSTEMTIME diaCitas = { 0 };
	SYSTEMTIME horaCitas = { 0 };
	GetDlgItemText(hwnd, CB_MODIFICAR_HORA, FormatHora, GetWindowTextLength(hCBhora) + 1);
	DateTime_GetSystemtime(hDia, &diaCitas);
	SystemTimeToVariantTime(&diaCitas, &dia);

	if (FormatHora[0] != NULL) {
		NODOCITA* nodo = buscarCitaPorFormatHora(claveVet, FormatHora, (int)dia);
		if(nodo != nullptr){
			// Extracción de la lista
			if (nodo->Dato == LISTACITA.Origen->Dato && nodo->Dato == LISTACITA.Fin->Dato) {
				LISTACITA.Origen = NULL;
				LISTACITA.Fin = NULL;
				delete(nodo);
				MessageBox(NULL, "La cita ha sido eliminada correctamente", "Cita:", MB_OK | MB_ICONINFORMATION);
				return TRUE;
			}
			else if (nodo == LISTACITA.Origen) {
				LISTACITA.Origen = nodo->Siguiente;
				LISTACITA.Origen->Anterior = NULL;
				nodo->Siguiente = NULL;
				delete(nodo);
				MessageBox(NULL, "La cita ha sido eliminada correctamente", "Cita:", MB_OK | MB_ICONINFORMATION);
				return TRUE;
			}
			else if (nodo == LISTACITA.Fin) {
				LISTACITA.Fin = nodo->Anterior;
				LISTACITA.Fin->Siguiente = NULL;
				nodo->Anterior = NULL;
				delete(nodo);
				MessageBox(NULL, "La cita ha sido eliminada correctamente", "Cita:", MB_OK | MB_ICONINFORMATION);
				return TRUE;
			}
			else {
				nodo->Anterior->Siguiente = nodo->Siguiente;
				nodo->Siguiente->Anterior = nodo->Anterior;
				nodo->Anterior = NULL;
				nodo->Siguiente = NULL;
				delete(nodo);
				MessageBox(NULL, "La cita ha sido eliminada correctamente", "Cita:", MB_OK | MB_ICONINFORMATION);
				return TRUE;
			}
		}
		else{
			MessageBox(NULL, "La cita seleccionada ya no existe", "Error al eliminar", MB_OK | MB_ICONERROR);
			return FALSE;
		}	
	}
	else {
		MessageBox(NULL, "No se ha seleccionado ninguna cita", "Error al eliminar", MB_OK | MB_ICONERROR);
		return FALSE;
	}
}
NODOCITA* nuevoNodoCita(CITA* dato) {
	NODOCITA* nodo = new NODOCITA;
	nodo->Dato = dato;
	nodo->Siguiente = NULL;
	nodo->Anterior = NULL;
	return nodo;
}
NODOCITA* nuevoNodoCitaEXTRA(NODOCITA* nodo) {
	NODOCITA* NewNodo = new NODOCITA;
	NewNodo->Dato = nodo->Dato;
	NewNodo->Siguiente = nodo->Siguiente;
	NewNodo->Anterior = nodo->Anterior;
	return NewNodo;
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
		if (temp != NULL){
			nodo->Anterior = temp;
			nodo->Siguiente = temp->Siguiente;
			temp->Siguiente->Anterior = nodo;
			temp->Siguiente = nodo;
		}
	}
}
void crearTempListaCitaPorDia(int claveVet, double dia) {
	if(LISTACITA.Origen != nullptr){
		NODOCITA* indice = nuevoNodoCitaEXTRA(LISTACITA.Origen);
			TEMP_LISTACITA.Origen = NULL;
			TEMP_LISTACITA.Fin = NULL;
			while (indice != NULL) {
				if ((int)indice->Dato->Fecha == (int)dia) {
					agregarCitaFinalTemp(indice->Dato);
				}
				indice = indice->Siguiente;
			}
	}
}
void crearTempListaCitaPorFechas(int claveVet, double fecha1, double fecha2) {
	if (LISTACITA.Origen != nullptr) {
		NODOCITA* indice = nuevoNodoCitaEXTRA(LISTACITA.Origen);
		TEMP_LISTACITA.Origen = NULL;
		TEMP_LISTACITA.Fin = NULL;
		while (indice != NULL) {
			if (indice->Dato->Fecha > fecha1 && indice->Dato->Fecha < fecha2) {
				agregarCitaFinalTemp(indice->Dato);
			}
			indice = indice->Siguiente;
		}
	}
}
void agregarCitaFinalTemp(CITA* dato) {
	NODOCITA* nodo = nuevoNodoCita(dato);
	if (TEMP_LISTACITA.Origen == NULL) {
		TEMP_LISTACITA.Origen = nodo;
		TEMP_LISTACITA.Fin = nodo;
		nodo->Siguiente = NULL;
		nodo->Anterior = NULL;
	}
	else {
		TEMP_LISTACITA.Fin->Siguiente = nodo;
		nodo->Anterior = TEMP_LISTACITA.Fin;
		nodo->Siguiente = NULL;
		TEMP_LISTACITA.Fin = nodo;
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
NODOCITA* buscarCitaPorDia(int claveVet, double dia) {
	if (LISTACITA.Origen == NULL)
		return NULL;
	NODOCITA* indice = LISTACITA.Origen;

	while (indice != NULL) {
		if ((int)indice->Dato->Fecha == (int)dia) {
			return indice;
			break;
		}
		indice = indice->Siguiente;
	}
	return NULL;
}
NODOCITA* buscarCitaPorFecha(int claveVet, double fecha) {
	if (LISTACITA.Origen == NULL)
		return NULL;
	NODOCITA* indice = LISTACITA.Origen;

	while (indice != NULL) {
		if (indice->Dato->Fecha == fecha) {
			return indice;
			break;
		}
		indice = indice->Siguiente;
	}
	return NULL;
}
NODOCITA* buscarCitaPorFormatHora(int claveVet, char* hora, double dia) {
	if (LISTACITA.Origen == NULL)
		return NULL;
	NODOCITA* indice = LISTACITA.Origen;

	while (indice != NULL) {
		if (strcmp(indice->Dato->FormatHora, hora) == 0 && indice->Dato->varDia == (int)dia) {
			return indice;
			break;
		}
		indice = indice->Siguiente;
	}
	return NULL;
}
NODOCITA* buscarCitaPorFormatFecha(int claveVet, char* fecha) {
	if (LISTACITA.Origen == NULL)
		return NULL;
	NODOCITA* indice = LISTACITA.Origen;

	while (indice != NULL) {
		if (strcmp(indice->Dato->FormatFecha, fecha) == 0) {
			return indice;
			break;
		}
		indice = indice->Siguiente;
	}
	return NULL;
}

char* formatoHora(LPSYSTEMTIME Sys, char* buff) {

	std::string strMessage;

	CString cstrMessage;

	cstrMessage.Format(_T("%d-%02d-%02d %02d:%02d:%02d.%03d"),
		//Sys.wYear,
		//Sys.wMonth,
		//Sys.wDay,
		Sys->wHour,
		Sys->wMinute,
		Sys->wSecond,
		Sys->wMilliseconds);

	strMessage = CT2A(cstrMessage.GetString());
	std::cout << "System time = " << strMessage << std::endl;

	std::ostringstream ossMessage;

	/*  << Sys.wYear << "-"
		<< std::setw(2) << std::setfill('0') << Sys.wMonth << "-"
		<< std::setw(2) << std::setfill('0') << Sys.wDay << " "
		<< std::setw(3) << std::setfill('0') << Sys->wMilliseconds;*/ 

	ossMessage
		<< std::setw(2) << std::setfill('0') << Sys->wHour << ":"
		<< std::setw(2) << std::setfill('0') << Sys->wMinute << ":"
		<< std::setw(2) << std::setfill('0') << Sys->wSecond << ".";	

	strMessage = ossMessage.str();
	std::cout << "System time = " << strMessage << std::endl;

	/*Sys.wYear,
		Sys.wMonth,
		Sys.wDay,
		Sys->wMilliseconds*/

	sprintf_s(buff,
		100,
		"%02d:%02d:%02d",
		Sys->wHour,
		Sys->wMinute,
		Sys->wSecond);
	
	return buff;
}
char* formatoFecha(LPSYSTEMTIME Sys, char* buff) {

	std::string strMessage;

	CString cstrMessage;

	cstrMessage.Format(_T("%d-%02d-%02d %02d:%02d:%02d.%03d"),
		Sys->wYear,
		Sys->wMonth,
		Sys->wDay,
		Sys->wHour,
		Sys->wMinute,
		Sys->wSecond,
		Sys->wMilliseconds);

	strMessage = CT2A(cstrMessage.GetString());
	std::cout << "System time = " << strMessage << std::endl;

	std::ostringstream ossMessage;

	/*  */

	ossMessage
		<< Sys->wYear << "-"
		<< std::setw(2) << std::setfill('0') << Sys->wMonth << "-"
		<< std::setw(2) << std::setfill('0') << Sys->wDay << " "
		<< std::setw(2) << std::setfill('0') << Sys->wHour << ":"
		<< std::setw(2) << std::setfill('0') << Sys->wMinute << ":"
		<< std::setw(2) << std::setfill('0') << Sys->wSecond << "."
		<< std::setw(3) << std::setfill('0') << Sys->wMilliseconds;

	strMessage = ossMessage.str();
	std::cout << "System time = " << strMessage << std::endl;

	/**/

	sprintf_s(buff,
		100,
		"%d-%02d-%02d %02d:%02d:%02d",
		Sys->wYear,
		Sys->wMonth,
		Sys->wDay,
		Sys->wHour,
		Sys->wMinute,
		Sys->wSecond,
		Sys->wMilliseconds);

	return buff;
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

// Funciones para guardar/cargar archivos binarios
void GuardarVETBIN() {
	ofstream archivo("Info de veterinarios.bin", ios::binary | ios::out | ios::trunc);
	if (!archivo.is_open()) {
		MessageBox(NULL, "No se pudo abrir el archivo", "Error", MB_OK | MB_ICONERROR);
		return;
	}

	NODOVET* actual = LISTAVET.Origen;
	while (actual != NULL) {
		if (archivo.bad()) {
			MessageBox(NULL, "Ocurrió un error durante la escritura", "Error", MB_OK | MB_ICONERROR);
			return;
		}

		archivo.write(reinterpret_cast<char*>(&actual->Dato->Nombre), sizeof(actual->Dato->Nombre));
		archivo.write(reinterpret_cast<char*>(&actual->Dato->Cedula), sizeof(actual->Dato->Cedula));
		archivo.write(reinterpret_cast<char*>(&actual->Dato->Clave), sizeof(actual->Dato->Clave));
		archivo.write(reinterpret_cast<char*>(&actual->Dato->FotoRuta), sizeof(actual->Dato->FotoRuta));
		archivo.write(reinterpret_cast<char*>(&actual->Dato->Password), sizeof(actual->Dato->Password));

		actual = actual->Siguiente;
	}
	archivo.close();
}
void GuardarCITABIN() { 
	ofstream archivo("Info de citas.bin", ios::binary | ios::out | ios::trunc);
	if (!archivo.is_open()) {
		MessageBox(NULL, "No se pudo abrir el archivo", "Error", MB_OK | MB_ICONERROR);
		return;
	}
	NODOCITA* actual = LISTACITA.Origen;  
	while (actual != NULL) {
		if (archivo.bad()) {
			MessageBox(NULL, "Ocurrió un error durante la escritura", "Error", MB_OK | MB_ICONERROR);
			return;
		}

		archivo.write(reinterpret_cast<char*>(&actual->Dato->ClaveVet), sizeof(actual->Dato->ClaveVet));
		archivo.write(reinterpret_cast<char*>(&actual->Dato->Telefono), sizeof(actual->Dato->Telefono));
		archivo.write(reinterpret_cast<char*>(&actual->Dato->Fecha), sizeof(actual->Dato->Fecha));
		archivo.write(reinterpret_cast<char*>(&actual->Dato->varHora), sizeof(actual->Dato->varHora));
		archivo.write(reinterpret_cast<char*>(&actual->Dato->varDia), sizeof(actual->Dato->varDia));
		archivo.write(reinterpret_cast<char*>(&actual->Dato->Costo), sizeof(actual->Dato->Costo));
		archivo.write(reinterpret_cast<char*>(&actual->Dato->FormatHora), sizeof(actual->Dato->FormatHora));
		archivo.write(reinterpret_cast<char*>(&actual->Dato->FormatFecha), sizeof(actual->Dato->FormatFecha));
		archivo.write(reinterpret_cast<char*>(&actual->Dato->NombreCliente), sizeof(actual->Dato->NombreCliente));
		archivo.write(reinterpret_cast<char*>(&actual->Dato->Especie), sizeof(actual->Dato->Especie));
		archivo.write(reinterpret_cast<char*>(&actual->Dato->NombreMascota), sizeof(actual->Dato->NombreMascota));
		archivo.write(reinterpret_cast<char*>(&actual->Dato->Motivo), sizeof(actual->Dato->Motivo));
		archivo.write(reinterpret_cast<char*>(&actual->Dato->Estatus), sizeof(actual->Dato->Estatus)); 
		
		actual = actual->Siguiente;
	} 
	archivo.close(); 
} 
bool CargarVETBIN(VETERINARIOS& listaVeterinarios) {
	ifstream archivo("Info de veterinarios.bin", ios::binary);
	if (!archivo.is_open()) {
		MessageBox(NULL, "No se pudo abrir el archivo", "Error", MB_OK | MB_ICONERROR);
		return false;
	}

	archivo.seekg(0, ios::end);
	unsigned int bytes = archivo.tellg();
	archivo.seekg(0, ios::beg);

	unsigned int lectura = 0;
	while (lectura < bytes) {
		VETERINARIO temp;
		archivo.read(reinterpret_cast<char*>(&temp), sizeof(VETERINARIO));

		agregarVetFinal(crearVet(temp.Nombre, temp.Cedula, temp.Clave, temp.FotoRuta, temp.Password));

		lectura += sizeof(VETERINARIO);
	}

	archivo.close();
	return (lectura > 0);
}
bool CargarCITABIN(CITAS& listadeCitas) {
	ifstream archivo("Info de citas.bin", ios::binary);
	if (!archivo.is_open()) {
		MessageBox(NULL, "No se pudo abrir el archivo", "Error", MB_OK | MB_ICONERROR);
		return false;
	}

	archivo.seekg(0, ios::end);
	unsigned int bytes = archivo.tellg();
	archivo.seekg(0, ios::beg);

	unsigned int lectura = 0;
	while (lectura < bytes) {
		CITA temp;

		archivo.read(reinterpret_cast<char*>(&temp), sizeof(CITA)); 

		agregarCitaFinal(crearCitaDirecto(temp.ClaveVet, temp.Telefono, temp.Fecha, temp.varHora, temp.varDia, temp.Costo, temp.FormatHora , temp.FormatFecha, temp.NombreCliente, temp.Especie, temp.NombreMascota, temp.Motivo, temp.Estatus));

		lectura += sizeof(CITA); 

	}
	archivo.close();
	return (lectura > 0);
}