/* 
    Funcionalidade: Um cliente-proxy TCP/IP 
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

int main(int argc, char *argv[]){
    int sock_descr, nread; 
    struct sockaddr_in proxyaddr;
    struct hostent *hp; 
    char buffer[BUFSIZ + 1];
    char *host;
    char *dados;

    if(argc != 3){
        puts("Uso correto: cliente <nome-proxy> <porta>");
        exit(1);
    }

    host = argv [1];

    if((hp = gethostbyname(host)) == NULL){
        puts("O DNS não retornou o endereço IP da proxy!");
        exit(1);
    }

    bcopy((char *)hp->h_addr, (char*)&proxyaddr.sin_addr, hp->h_length);
    proxyaddr.sin_family = AF_INET;
    proxyaddr.sin_port = htons(atoi(argv[2])); 

    /* Abre o socket */
    if((sock_descr = socket(AF_INET,SOCK_STREAM,0)) < 0){
        puts("Não consegui abrir o socket"); /* Difcil de acontecer */
        exit(1);
    }

    /* connect() is used on the client side, and assigns a free local port number to a socket */
    if(connect(sock_descr,(struct sockaddr*)&proxyaddr,sizeof(proxyaddr)) < 0){
        puts("Não consegui conectar com a proxy!");
        exit(1);
    }
    
    /* Transmissao dos dados */
    printf("Cliente 1 solicitando dado...\n");
    if(write(sock_descr,  "GET" , strlen("GET")) != strlen("GET")){
        puts("Não consegui fazer a solicitacao. Fechando a conexao..");
        close(sock_descr);
        exit(1);
    }

    /* Recebe os dados. Comando que bloqueia */
    if(nread = read(sock_descr,buffer,BUFSIZ) < 0){
        printf("Sou o cliente 1 -> Não consegui receber os dados da proxy. Fechando a conexão..\n");
        close(sock_descr);
        exit(1);
    }
    //buffer[nread] = '\0';
    printf("Sou o cliente 1, recebi da proxy o dado: ---> %d\n",atoi(buffer));
    //printf(": %d\n", );
    fflush(stdout);
    buffer[0] = '\0';
    /* limpar lixo do bufferfer */

    close(sock_descr);
    exit(0);
}