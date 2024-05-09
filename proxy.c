/* 
    Funcionalidade: Uma proxy TCP/IP 
    Autor: Barbara Reis
    Data da última modificação: 9/maio/2024 
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
    char buf[BUFSIZ+1], *host, requisicao[BUFSIZ+1];

    if(argc != 4) {
        puts("Uso correto: proxy <nome-servidor> <porta-servidor> <porta-proxy>");
        exit(1);
    }

    host = argv[1];
    FILE *log_file = fopen("log.txt", "w");
    if (log_file == NULL) {
        perror("Erro ao abrir o arquivo de log");
        exit(1);
    }

    /* chama o resolvedor DNS. Retorna uma struct contendo o end. IP do host do servidor */
    if((hp_serv = gethostbyname(host)) == NULL) {
        puts("O DNS não retornou o endereço IP do servidor!\n");
        exit(1);
    }

    /* prepara a estrutura de socket do servidor */
    bcopy((char *)hp_serv->h_addr, (char*)&serveraddr.sin_addr, hp_serv->h_length); // copia o end. IP que veio do DNS para a estrutura serveraddr
    serveraddr.sin_family = AF_INET; // define a familia de enderecos IP da internet
    serveraddr.sin_port = htons(atoi(argv[2])); // define a porta do servidor e a converte para o formato big-endian para ser interpretado pela rede

    /* chama o resolvedor DNS. Retorna uma struct contendo o end. IP agora do host da proxy*/
    if((hp_proxy = gethostbyname(host)) == NULL) {
        puts("O DNS não retornou o endereço IP da proxy!\n");
        exit(1);
    }

    /* prepara a estrutura de socket da proxy */
    bcopy((char *)hp_proxy->h_addr, (char*)&proxyaddr.sin_addr, hp_proxy->h_length); // copia o end. IP que veio do DNS para a estrutura proxyaddr 
    proxyaddr.sin_family = AF_INET; // define a familia de enderecos IP da internet
    proxyaddr.sin_port = htons(atoi(argv[3])); // define a porta da proxy e a converte para o formato big-endian para ser interpretado pela rede

    /* abre o socket da proxy */
    if((proxy_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        puts("Não consegui abrir o socket da proxy!\n");
        exit(1);
    }

    /* associa o socket com o endereco da proxy. Informa a porta que o socket ira operar */
    if(bind(proxy_socket, (struct sockaddr*)&proxyaddr, sizeof(proxyaddr)) < 0) {
        puts("Nao consegui fazer o bind da proxy!\n");
        exit(1);
    }

    /* prepara a proxy para receber conexoes */
    if(listen(proxy_socket, TAMFILA) < 0) {
        puts("Erro ao escutar conexoes na proxy!\n");
        exit(1);
    }
    printf("Proxy aguardando conexao..\n");

    

    while(1) {
        proxyaddrsize = sizeof(proxyaddr);

        /* estabelece uma conexão entre o Cliente 1 e a proxy */
        if((client_socket = accept(proxy_socket, (struct sockaddr*)&clientaddr, &proxyaddrsize)) < 0) {
            puts("Nao consegui aceitar a conexao!\n");
            exit(1);
        }

        /* abre o socket que ira se comunicar com o servidor */
        if((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            puts("Não consegui abrir o socket do servidor\n");
            exit(1);
        }

        /* conecta o socket do servidor ao endereco do mesmo. Estabele uma conexao entre a proxy e o servidor */
        if(connect(server_socket, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0) {
            puts("Não consegui conectar com o servidor!\n");
            exit(1);
        }

        /* limpa o buffer */
        memset(buf, 0, sizeof(buf));

        /* recebe a requisicao do Cliente 1. Nesse caso do tipo GET */
        if(nread = read(client_socket, buf, BUFSIZ) < 0){
            puts("Proxy nao conseguiu receber a requisicao do cliente 1. Fechando a conexão..\n");
            close(client_socket);
            close(server_socket);
            exit(1);
        }
        strcpy(requisicao, buf);

        /* transmite a requisicao feita pelo cliente para o servidor */
        if(write(server_socket, requisicao , strlen(requisicao)) != strlen(requisicao)){
            puts("Proxy nao conseguiu transmitir a requisicao do cliente 1 para o servidor. Fechando a conexão..\n");
            close(client_socket);
            close(server_socket);
            exit(1);
        }

        /* limpa o buffer antes de reutiliza-lo */
        memset(buf, 0, sizeof(buf));

        /* recebe os dados transmitidos pelo servidor */
        if(nread = read(server_socket, buf, BUFSIZ) < 0){
            puts("Proxy nao conseguiu receber os dados do servidor. Fechando a conexão..\n");
            close(client_socket);
            close(server_socket);
            exit(1);
        }
        
        printf("\nProxy recebeu do servidor o dado---> %d\n", atoi(buf));
        
        /* transmite os dados recebidos pelo servidor para o Cliente 1 */
        if(write(client_socket, buf, strlen(buf)) != strlen(buf)){
            puts("Proxy nao conseguiu transmitir os dados do servidor para o cliente 1. Fechando a conexão..\n");
            close(client_socket);
            close(server_socket);
            exit(1);
        }

        printf("Proxy enviando para o cliente 1 o dado---> %d\n", atoi(buf));

        close(client_socket);
        close(server_socket);
        
    }

    exit(0);
}
