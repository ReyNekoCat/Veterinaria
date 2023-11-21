/*
 LOGIN
 - Cada m�dico puede con su clave de usuario y contrase�a
 - Un usuario administrador que pueda crear m�s cuentas

 MENU [Disponible en todas las pantallas]
 - Informaci�n de doctor que se conect� al programa
 - Manejo de Citas
 - Agenda
 - Salir
 - Mostrar el nombre y foto del doctor en todas las pantallas

 INFO [Debe poderse editar en cualquier momento]
 - Nombre completo del doctor
 - C�dula profesional
 - Foto (Mapa de bits)
 - Clave de usuario
 - Contrase�a

 CITAS [Se deben poder agendar citas]

 - Fecha y hora
	> Debe tener un formato libre
	> Evitar la creaci�n con una fecha pasada
	> Evitar la modificaci�n de citas pasadas

 - Nombre del cliente (Due�o de la mascota)

 - Tel�fono de contacto.
	> Solo n�meros y tener exactamente 8, 10 o 12 caracteres.

 - Especie de la mascota (perro, gato, lombriz...)
	> Se debe seleccionar a trav�s de un ComboBox

 - Nombre de la mascota
	> Solo letras

 - Motivo de la consulta
	> Debe ser abierto

 - Costo de la consulta
	> Solo n�meros y un s�lo punto

 - Estatus (Pendiente, Efectuada, Cancelada, etc.)
	> Las citas pueden cancelarse o eliminarse antes de que sucedan
		> Cancelada: Se puede seguir viendo con un estatus de "cancelada"
		> Eliminada: Se libera la memoria y ya no se puede visualizar

 AGENDA [Visualizador principal de la app]
 - Estar� en la ventana principal
 - Visualizar las citas ordenadas de la m�s pr�xima a la m�s lejana
 - Mostrar las citas de hoy en adelante (No mostrar las citas pasadas)
 - Visualizar, modificar y eliminar cada cita (o redirigir a una secci�n para realizar estas acciones)
 - En la vista de cada cita se debe mostrar toda la informaci�n, adem�s de la cantidad a pagar
 - Se debe agregar una pantalla para consultar citas en un rango de fechas

 ABC [Altas/Bajas/Cambios]
 - Las citas deben darse de alta, de baja o cambiarse en cualquier momento
	> El proyecto debe tener las pantallas necesarias para realizar dichos cambios
	> Todas estas operaciones deben ser realizadas sobre listas doblemente ligadas
 - Las bajas deben eliminar los nodos de la lista ligada
 - Citas ya pasadas en el tiempo, no se pueden modificar
 - Las altas son a trav�s de la secci�n "Nueva cita"
 - Las bajas y cambios se deben realizar a trav�s de la agenda
 - Todas las entidades deben guardar la clave del usuario que est� registrando los datos

 SALIR
 - Guardar los datos de las listas doblemente ligadas enarchivos binarios
 - Al iniciar, leer los archivos binarios
	 > Con la informaci�n guardada, poblar las listas ligadas correspondientes

 ESTRUCTURA DE DATOS [Archivos Binarios]
 - Utilizar listas doblemente ligadas para el manejo de la informaci�n
 - Todas las entidades se deben de guardar en archivos binarios
 - Tener la l�gica correcta para guardar los datos capturados en el programaen archivos binarios
	 > La info se recupera desde este archivo para utilizarla en la aplicaci�n
 - Uso de clases (propiedades y m�todos) para el manejo de listas ligadas

 */