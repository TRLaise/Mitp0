#include "utils.h"

t_log *logger;

int iniciar_servidor(char *puerto, t_log *un_log, char *msj_server)
{
	int socket_servidor;

	struct addrinfo hints, *servinfo;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;

	hints.ai_flags = AI_PASSIVE;	hints.ai_socktype = SOCK_STREAM;

	getaddrinfo(NULL, puerto, &hints, &servinfo);

 	// Creamos el socket de escucha del servidor
 	socket_servidor = socket(servinfo->ai_family,
                          servinfo->ai_socktype,
                          servinfo->ai_protocol);
	if (socket_servidor == -1) {
        perror("Error en crear socket de escucha");
        freeaddrinfo(servinfo);
        return -1;
    }

    // Asociar el socket a un puerto
    if (bind(socket_servidor, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {
        perror("Error en bind");
        close(socket_servidor);
        freeaddrinfo(servinfo);
        return -1;
    }

    // Escuchar las conexiones entrantes
    if (listen(socket_servidor, SOMAXCONN) == -1) {
        perror("Error en listen");
        close(socket_servidor);
        freeaddrinfo(servinfo);
        return -1;
    }

	freeaddrinfo(servinfo);
	log_info(un_log, "SERVER: %s", msj_server);

	return socket_servidor;
}

int esperar_cliente(int socket_servidor, t_log *un_log, char *mensaje)
{
	// Aceptamos un nuevo cliente
	int socket_cliente = accept(socket_servidor, NULL, NULL);

	if (socket_cliente == -1)
	{
		perror("Error al aceptar la conexión");
		return -1;
	}
	log_info(un_log, "%s fd:%d", mensaje, socket_cliente);
	return socket_cliente;
}

int recibir_operacion(int socket_cliente)
{
	int cod_op;
    ssize_t bytes = recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL);
	if (bytes > 0)
		return cod_op;
    else if (bytes == 0)
    {
        perror("El cliente cerro la conexion");
    }
	close(socket_cliente);
	return -1;

}

void *recibir_buffer(int *size, int socket_cliente)
{
	void *buffer;

	recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, MSG_WAITALL);

	return buffer;
}

void recibir_mensaje(int socket_cliente)
{
	int size;
	char *buffer = recibir_buffer(&size, socket_cliente);
	log_info(logger, "Me llego el mensaje %s", buffer);
	free(buffer);
}

t_list *recibir_paquete(int socket_cliente)
{
	int size;
	int desplazamiento = 0;
	void *buffer;
	t_list *valores = list_create();
	int tamanio;

	buffer = recibir_buffer(&size, socket_cliente);
	while (desplazamiento < size)
	{
		memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
		desplazamiento += sizeof(int);
		char *valor = malloc(tamanio);
		memcpy(valor, buffer + desplazamiento, tamanio);
		desplazamiento += tamanio;
		list_add(valores, valor);
	}
	free(buffer);
	return valores;
}
