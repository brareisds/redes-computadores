/* 
    Funcionalidade: Um cliente-servidor TCP/IP 
    Autor: Barbara Reis
    Data da última modificação: 6/maio/2024 
*/


#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h> 
#include <netdb.h>
#include <stdlib.h>
#include <string.h>

#define QNT_MODIFICACOES 10


int main(int argc, char *argv[]){
    int sock_descr; 
    struct sockaddr_in serveraddr; 
    struct hostent *hp;
    char buffer[BUFSIZ + 1]; 
    char *host; 
    char *dados; 

    if(argc != 4){
        puts("Uso correto: cliente <nome-servidor> <porta> <dados>");
        exit(1);
    }

    host = argv [1];
    dados = argv[3];

    if((hp = gethostbyname(host)) == NULL){
        puts("O DNS não retornou o endereço IP do servidor!");
        exit(1);
    }


    /* copiar o end IP para o socket do cliente */
    bcopy((char *)hp->h_addr, (char*)&serveraddr.sin_addr, hp->h_length);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(atoi(argv[2])); /* Precisa converter somente a porta */

    /* Abre o socket */
    if((sock_descr = socket(AF_INET,SOCK_STREAM,0)) < 0){
        puts("Não consegui abrir o socket"); /* Difcil de acontecer */
        exit(1);
    }

    /* connect() is used on the client side, and assigns a free local port number to a socket */
    if(connect(sock_descr,(struct sockaddr*)&serveraddr,sizeof(serveraddr)) < 0){
        puts("Não consegui conectar ao servidor!");
        exit(1);
    }

    // Envia uma mensagem para a proxy para modificar o dado
    if(write(sock_descr,"SET" , strlen("SET")) != strlen("SET")){
        puts("Não consegui fazer a requisicao. Fechando a conexao..");
        close(sock_descr);
        exit(1);
    }
    printf("Modificando dado...\n");
    // Solicita e envia o novo valor do dado
    printf("Novo valor do dado: %d\n", atoi(dados));
    //scanf("%s", buffer);
    if(write(sock_descr,dados,strlen(dados)) != strlen(dados)){
        puts("Sou o cliente 2. Não consegui fazer a transmissao dos dados para o servidor. Fechando a conexao..");
        close(sock_descr);
        exit(1);
    }

    close(sock_descr);
    exit(0);
}




