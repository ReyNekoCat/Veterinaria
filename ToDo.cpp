/*
 LOGIN
 - Cada m�dico puede ingresar con su clave de usuario y contrase�a /HECHO
 - Un usuario administrador que pueda crear m�s cuentas /FALTA VALIDACION (No es necesario)

 MENU [Disponible en todas las pantallas]
 - Informaci�n de doctor que se conect� al programa /HECHO
 - Manejo de Citas /HECHO
 - Agenda /HECHO
 - Salir /HECHO
 - Mostrar el nombre y foto del doctor en todas las pantallas /HECHO

 INFO [Debe poderse editar en cualquier momento]
 - Nombre completo del doctor /HECHO
 - C�dula profesional /HECHO
 - Foto (Mapa de bits) /HECHO
 - Clave de usuario /HECHO
 - Contrase�a /HECHO

 CITAS [Se deben poder agendar citas]

 - Fecha y hora
	> Debe tener un formato libre /HECHO
	> Evitar la creaci�n con una fecha pasada /HECHO
	> Evitar la modificaci�n de citas pasadas /HECHO

 - Nombre del cliente (Due�o de la mascota) /HECHO

 - Tel�fono de contacto. /HECHO
	> Solo n�meros y tener exactamente 8, 10 o 12 caracteres. /HECHO

 - Especie de la mascota (perro, gato, lombriz...)
	> Se debe seleccionar a trav�s de un ComboBox /HECHO

 - Nombre de la mascota /HECHO
	> Solo letras /HECHO

 - Motivo de la consulta /HECHO
	> Debe ser abierto /HECHO

 - Costo de la consulta /FALTA PROBAR
	> Solo n�meros y un s�lo punto /HECHO

 - Estatus (Pendiente, Efectuada, Cancelada, etc.)
	> Las citas pueden cancelarse o eliminarse antes de que sucedan /HECHO
		> Cancelada: Se puede seguir viendo con un estatus de "cancelada" /HECHO
		> Eliminada: Se libera la memoria y ya no se puede visualizar /HECHO

 AGENDA [Visualizador principal de la app]
 - Estar� en la ventana principal /HECHO
 - Visualizar las citas ordenadas de la m�s pr�xima a la m�s lejana /HECHO
 - Mostrar las citas de hoy en adelante (No mostrar las citas pasadas) /HECHO
 - Visualizar, modificar y eliminar cada cita (o redirigir a una secci�n para realizar estas acciones) /HECHO
 - En la vista de cada cita se debe mostrar toda la informaci�n, adem�s de la cantidad a pagar /HECHO
 - Se debe agregar una pantalla para consultar citas en un rango de fechas /HECHO

 ABC [Altas/Bajas/Cambios]
 - Las citas deben darse de alta, de baja o cambiarse en cualquier momento /HECHO
	> El proyecto debe tener las pantallas necesarias para realizar dichos cambios /HECHO
	> Todas estas operaciones deben ser realizadas sobre listas doblemente ligadas /HECHO
 - Las bajas deben eliminar los nodos de la lista ligada /HECHO
 - Citas ya pasadas en el tiempo, no se pueden modificar /HECHO
 - Las altas son a trav�s de la secci�n "Nueva cita" /HECHO
 - Las bajas y cambios se deben realizar a trav�s de la agenda /HECHO
 - Todas las entidades deben guardar la clave del usuario que est� registrando los datos /HECHO

 SALIR
 - Guardar los datos de las listas doblemente ligadas enarchivos binarios /HECHO
 - Al iniciar, leer los archivos binarios /HECHO
	 > Con la informaci�n guardada, poblar las listas ligadas correspondientes /HECHO

 ESTRUCTURA DE DATOS [Archivos Binarios]
 - Utilizar listas doblemente ligadas para el manejo de la informaci�n /HECHO
 - Todas las entidades se deben de guardar en archivos binarios /HECHO
 - Tener la l�gica correcta para guardar los datos capturados en el programaen archivos binarios /MHECHO
	 > La info se recupera desde este archivo para utilizarla en la aplicaci�n /HECHO
 - Uso de clases (propiedades y m�todos) para el manejo de listas ligadas /HECHO

 */