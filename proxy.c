/* 
    Funcionalidade: Uma proxy TCP/IP 
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

#define TAMFILA 5

int main(int argc, char *argv[]) {
    int proxy_socket, server_socket, client_socket, proxyaddrsize, nread;
    struct sockaddr_in clientaddr, serveraddr, proxyaddr;
    struct hostent *hp_serv, *hp_proxy;
    char buf[BUFSIZ+1];
    char *host;

    if(argc != 4) {
        puts("Uso correto: proxy <nome-servidor> <porta-proxy> <porta-servidor>");
        exit(1);
    }

    host = argv[1];

    if((hp_proxy = gethostbyname(host)) == NULL) {
        puts("O DNS não retornou o endereço IP da proxy!");
        exit(1);
    }

    bcopy((char *)hp_proxy->h_addr, (char*)&proxyaddr.sin_addr, hp_proxy->h_length);
    proxyaddr.sin_family = AF_INET;
    proxyaddr.sin_port = htons(atoi(argv[2]));

    if((proxy_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        puts("Não conseguir abrir o socket!");
        exit(1);
    }

    if(bind(proxy_socket, (struct sockaddr*)&proxyaddr, sizeof(proxyaddr)) < 0) {
        puts("Nao consegui fazer o bind da proxy!");
        exit(1);
    }

    if(listen(proxy_socket, TAMFILA) < 0) {
        puts("Erro ao escutar conexões na proxy!");
        exit(1);
    }

    if((hp_serv = gethostbyname(host)) == NULL) {
        puts("O DNS não retornou o endereço IP do servidor!");
        exit(1);
    }

    bcopy((char *)hp_serv->h_addr, (char*)&serveraddr.sin_addr, hp_serv->h_length);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(atoi(argv[3]));

    while(1) {
        proxyaddrsize = sizeof(proxyaddr);

        if((client_socket = accept(proxy_socket, (struct sockaddr*)&clientaddr, &proxyaddrsize)) < 0) {
            puts("Nao consegui aceitar a conexao!");
            exit(1);
        }

        if((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            puts("Não consegui abrir o socket do servidor");
            exit(1);
        }

        if(connect(server_socket, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0) {
            puts("Não consegui conectar com o servidor!");
            exit(1);
        }

        if(nread = read(client_socket, buf, BUFSIZ) < 0){
            printf("Sou a proxy. Não consegui receber os dados do cliente 1. Fechando a conexão..\n");
            close(client_socket);
            close(server_socket);
            exit(1);
        }

        

        write(server_socket,  "GET" , strlen("GET"));
        
        if(nread = read(server_socket, buf, BUFSIZ) < 0){
            printf("Não consegui receber os dados. Fechando a conexão..\n");
            close(client_socket);
            close(server_socket);
            exit(1);
        }
        
        //printf("buf: %s\n", buf);
        printf("Sou a proxy, recebi do servidor a mensagem: %d\n", atoi(buf));
        write(client_socket, buf, strlen(buf));

        
        fflush(stdout);

        close(client_socket);
        close(server_socket);
    }

    exit(0);
}
