/* 
    Funcionalidade: Um servidor iterativo TCP/IP 
    Autor: Barbara Reis
    Data da última modificação: 6/maio/2024 
*/

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define TAMFILA      5
#define MAXHOSTNAME 30

int main (int argc, char *argv[]) {
	int server_socket, client_socket, serveraddrsize, nread;
    char buffer [ BUFSIZ + 1];
	struct sockaddr_in serveraddr, clientaddr;
	struct hostent *hp;
	char localhost [MAXHOSTNAME];
    int dado = 0; // inicializa o dado no servidor com o valor 0

    if (argc != 2) {
        puts("Uso correto: servidor <porta>");
        exit(1);
    }

    /* returns the null-terminated hostname in the character array localhost, which has a length of len bytes*/
    gethostname (localhost, MAXHOSTNAME);

	if ((hp = gethostbyname(localhost)) == NULL){
		puts ("Nao consegui meu proprio IP");
		exit (1);
	}	

    /* copia o end. IP que veio do dns para a estrutura socketaddr_in do socket serveraddr*/
	bcopy ((char *) hp->h_addr, (char *) &serveraddr.sin_addr, hp->h_length);
    serveraddr.sin_port = htons(atoi(argv[1]));
	serveraddr.sin_family = hp->h_addrtype;		

    /* open server socket. returns a file descriptor for the socket. */
	if ((server_socket = socket(hp->h_addrtype,SOCK_STREAM,0)) < 0){
        puts ( "Nao consegui abrir o socket" );
		exit (1);
	}	
    puts("Socket do servidor criado com sucesso!");

    /* bind() associates a socket with an address */
	if (bind(server_socket, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0){
		puts ( "Nao consegui fazer o bind" );
		exit (1);
	}		
   puts("Bind feito com sucesso!");

    /* listen() prepares it for incoming connections. */
	listen (server_socket, TAMFILA);
    puts("Servidor aguardando conexao..");

	while (1){
		serveraddrsize = sizeof(serveraddr);
        /* accept estabelece uma conexão solicitada pelo cliente*/
		if ((client_socket = accept(server_socket, (struct sockaddr *) &clientaddr,&serveraddrsize))<0){
            puts ( "Nao consegui estabelecer conexao" );
            exit (1);
		}	
        // Limpa o buffer
        memset(buffer, 0, sizeof(buffer));
        // verifica a requisicao do cliente
        nread = read(client_socket, buffer, BUFSIZ);
        buffer[nread + 1] = '\0';
        printf("buf: %s\n", buffer);

        // Se a mensagem for do Cliente 1, envia o dado atual
        if (strcmp(buffer, "GET") == 0) {
            printf("entrou no if do get\n");
            sprintf(buffer, "%d", dado);
            printf("Sou o servidor, tenho a mensagem----> %d\n", dado);
            printf("buffer %s\n", buffer);
            write(client_socket, buffer, BUFSIZ);
        }

        // Se a mensagem for do Cliente 2, modifica o dado
        if (strcmp(buffer, "SET") == 0) {
            printf("entrou no if do set\n");
            nread = read(client_socket, buffer, BUFSIZ);
            buffer[nread + 1] = '\0';
            //sprintf(buffer, "%d", dado);
            dado = atoi(buffer);
            printf("Dado modificado no servidor para: %d\n", dado);
        }

        fflush(stdout);
        buffer[0] = '\0';
        close(client_socket);
   }
   
   exit(0);
}