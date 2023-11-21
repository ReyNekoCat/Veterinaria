/*
 LOGIN
 - Cada médico puede con su clave de usuario y contraseña
 - Un usuario administrador que pueda crear más cuentas

 MENU [Disponible en todas las pantallas]
 - Información de doctor que se conectó al programa
 - Manejo de Citas
 - Agenda
 - Salir
 - Mostrar el nombre y foto del doctor en todas las pantallas

 INFO [Debe poderse editar en cualquier momento]
 - Nombre completo del doctor
 - Cédula profesional
 - Foto (Mapa de bits)
 - Clave de usuario
 - Contraseña

 CITAS [Se deben poder agendar citas]

 - Fecha y hora
	> Debe tener un formato libre
	> Evitar la creación con una fecha pasada
	> Evitar la modificación de citas pasadas

 - Nombre del cliente (Dueño de la mascota)

 - Teléfono de contacto.
	> Solo números y tener exactamente 8, 10 o 12 caracteres.

 - Especie de la mascota (perro, gato, lombriz...)
	> Se debe seleccionar a través de un ComboBox

 - Nombre de la mascota
	> Solo letras

 - Motivo de la consulta
	> Debe ser abierto

 - Costo de la consulta
	> Solo números y un sólo punto

 - Estatus (Pendiente, Efectuada, Cancelada, etc.)
	> Las citas pueden cancelarse o eliminarse antes de que sucedan
		> Cancelada: Se puede seguir viendo con un estatus de "cancelada"
		> Eliminada: Se libera la memoria y ya no se puede visualizar

 AGENDA [Visualizador principal de la app]
 - Estará en la ventana principal
 - Visualizar las citas ordenadas de la más próxima a la más lejana
 - Mostrar las citas de hoy en adelante (No mostrar las citas pasadas)
 - Visualizar, modificar y eliminar cada cita (o redirigir a una sección para realizar estas acciones)
 - En la vista de cada cita se debe mostrar toda la información, además de la cantidad a pagar
 - Se debe agregar una pantalla para consultar citas en un rango de fechas

 ABC [Altas/Bajas/Cambios]
 - Las citas deben darse de alta, de baja o cambiarse en cualquier momento
	> El proyecto debe tener las pantallas necesarias para realizar dichos cambios
	> Todas estas operaciones deben ser realizadas sobre listas doblemente ligadas
 - Las bajas deben eliminar los nodos de la lista ligada
 - Citas ya pasadas en el tiempo, no se pueden modificar
 - Las altas son a través de la sección "Nueva cita"
 - Las bajas y cambios se deben realizar a través de la agenda
 - Todas las entidades deben guardar la clave del usuario que está registrando los datos

 SALIR
 - Guardar los datos de las listas doblemente ligadas enarchivos binarios
 - Al iniciar, leer los archivos binarios
	 > Con la información guardada, poblar las listas ligadas correspondientes

 ESTRUCTURA DE DATOS [Archivos Binarios]
 - Utilizar listas doblemente ligadas para el manejo de la información
 - Todas las entidades se deben de guardar en archivos binarios
 - Tener la lógica correcta para guardar los datos capturados en el programaen archivos binarios
	 > La info se recupera desde este archivo para utilizarla en la aplicación
 - Uso de clases (propiedades y métodos) para el manejo de listas ligadas

 */