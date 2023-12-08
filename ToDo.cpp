/*
 LOGIN
 - Cada médico puede ingresar con su clave de usuario y contraseña /HECHO
 - Un usuario administrador que pueda crear más cuentas /FALTA VALIDACION (No es necesario)

 MENU [Disponible en todas las pantallas]
 - Información de doctor que se conectó al programa /HECHO
 - Manejo de Citas /HECHO
 - Agenda /HECHO
 - Salir /HECHO
 - Mostrar el nombre y foto del doctor en todas las pantallas /HECHO

 INFO [Debe poderse editar en cualquier momento]
 - Nombre completo del doctor /HECHO
 - Cédula profesional /HECHO
 - Foto (Mapa de bits) /HECHO
 - Clave de usuario /HECHO
 - Contraseña /HECHO

 CITAS [Se deben poder agendar citas]

 - Fecha y hora
	> Debe tener un formato libre /HECHO
	> Evitar la creación con una fecha pasada /HECHO
	> Evitar la modificación de citas pasadas /HECHO

 - Nombre del cliente (Dueño de la mascota) /HECHO

 - Teléfono de contacto. /HECHO
	> Solo números y tener exactamente 8, 10 o 12 caracteres. /HECHO

 - Especie de la mascota (perro, gato, lombriz...)
	> Se debe seleccionar a través de un ComboBox /HECHO

 - Nombre de la mascota /HECHO
	> Solo letras /HECHO

 - Motivo de la consulta /HECHO
	> Debe ser abierto /HECHO

 - Costo de la consulta /FALTA PROBAR
	> Solo números y un sólo punto /HECHO

 - Estatus (Pendiente, Efectuada, Cancelada, etc.)
	> Las citas pueden cancelarse o eliminarse antes de que sucedan /HECHO
		> Cancelada: Se puede seguir viendo con un estatus de "cancelada" /HECHO
		> Eliminada: Se libera la memoria y ya no se puede visualizar /HECHO

 AGENDA [Visualizador principal de la app]
 - Estará en la ventana principal /HECHO
 - Visualizar las citas ordenadas de la más próxima a la más lejana /HECHO
 - Mostrar las citas de hoy en adelante (No mostrar las citas pasadas) /HECHO
 - Visualizar, modificar y eliminar cada cita (o redirigir a una sección para realizar estas acciones) /HECHO
 - En la vista de cada cita se debe mostrar toda la información, además de la cantidad a pagar /HECHO
 - Se debe agregar una pantalla para consultar citas en un rango de fechas /HECHO

 ABC [Altas/Bajas/Cambios]
 - Las citas deben darse de alta, de baja o cambiarse en cualquier momento /HECHO
	> El proyecto debe tener las pantallas necesarias para realizar dichos cambios /HECHO
	> Todas estas operaciones deben ser realizadas sobre listas doblemente ligadas /HECHO
 - Las bajas deben eliminar los nodos de la lista ligada /HECHO
 - Citas ya pasadas en el tiempo, no se pueden modificar /HECHO
 - Las altas son a través de la sección "Nueva cita" /HECHO
 - Las bajas y cambios se deben realizar a través de la agenda /HECHO
 - Todas las entidades deben guardar la clave del usuario que está registrando los datos /HECHO

 SALIR
 - Guardar los datos de las listas doblemente ligadas enarchivos binarios /HECHO
 - Al iniciar, leer los archivos binarios /HECHO
	 > Con la información guardada, poblar las listas ligadas correspondientes /HECHO

 ESTRUCTURA DE DATOS [Archivos Binarios]
 - Utilizar listas doblemente ligadas para el manejo de la información /HECHO
 - Todas las entidades se deben de guardar en archivos binarios /HECHO
 - Tener la lógica correcta para guardar los datos capturados en el programaen archivos binarios /MHECHO
	 > La info se recupera desde este archivo para utilizarla en la aplicación /HECHO
 - Uso de clases (propiedades y métodos) para el manejo de listas ligadas /HECHO

 */