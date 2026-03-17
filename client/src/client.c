#include "client.h"
#include <commons/config.h>
#include <readline/readline.h>

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = log_create("tp0.log", "PrimerLog" , true, LOG_LEVEL_INFO);

	log_info(logger, "Soy un log");
	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = config_create("/home/utnso/Desktop/tp0/client/cliente.config");

	valor = config_get_string_value(config,"CLAVE");
	ip = config_get_string_value(config,"IP");
	puerto = config_get_string_value(config,"PUERTO");

	log_info(logger, "Clave: %s", valor);

	//log_destroy(logger);
	//config_destroy(config);
	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'

	// Loggeamos el valor de config


	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);


	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor,conexion);

	// Armamos y enviamos el paquete
	paquete(conexion);

	log_info(logger, "Terminé de mandar todo. Espero 10 segundos antes de cerrar...");
    sleep(10);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* input;
	
    // Verificamos si la entrada no es NULL

	do{
		input = readline("Introduce un comando: ");
		printf("Ingresaste: %s\n", input);

        // Liberamos la memoria alocada por readline
	}while(strcmp(input,""));

	free(input);
	
   /* if (input != NULL) {
        // Imprimimos lo que el usuario introdujo
        printf("Ingresaste: %s\n", input);

        // Liberamos la memoria alocada por readline
        free(input);
    }
	*/

	// La primera te la dejo de yapa
	//leido = readline("> ");

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío


	// ¡No te olvides de liberar las lineas antes de regresar!

}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete;

	paquete = crear_paquete();
	crear_buffer(paquete);

	// Leemos y esta vez agregamos las lineas al paquete
	do
	{
		leido = readline("Introduce un comando: ");
		printf("Ingresaste: %s\n", leido);
		agregar_a_paquete(paquete,leido,strlen(leido)+1);

        // Liberamos la memoria alocada por readline
	}while(strcmp(leido,""));

	enviar_paquete(paquete,conexion);

	free(leido);
	eliminar_paquete(paquete);

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */

	  // Usamos tu función de utils para cerrar el socket
    liberar_conexion(conexion);

    // Liberamos el logger de las commons
    if (logger != NULL) {
        log_destroy(logger);
    }

    // Liberamos la configuración de las commons
    if (config != NULL) {
        config_destroy(config);
    }
}
