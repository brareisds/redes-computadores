/* 
    Funcionalidade: Um cliente-proxy TCP/IP 
    Autor: Barbara Reis
    Data da última modificação: 11/maio/2024 
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h> 
#include <netdb.h> 
#include <stdlib.h>
#include <string.h>

#include "log_functions.h"

int main(int argc, char *argv[]){
    int sock_descr, nread; 
    struct sockaddr_in proxyaddr;
    struct hostent *hp; 
    char buffer[BUFSIZ + 1];
    char *host;

    if(argc != 3){
        puts("Uso correto: client1 <nome-proxy> <porta-proxy>");
        exit(1);
    }

    host = argv [1];

    /* chama o resolvedor DNS. Retorna uma struct contendo o end. IP do host */
    if((hp = gethostbyname(host)) == NULL){
        puts("O DNS não retornou o endereço IP da proxy!\n");
        exit(1);
    }

    /* prepara a estrutura de socket da proxy */
    bcopy((char *)hp->h_addr, (char*)&proxyaddr.sin_addr, hp->h_length); // copia o end. IP que veio do DNS para a estrutura proxyaddr
    proxyaddr.sin_family = AF_INET; // define a familia de enderecos IP da internet
    proxyaddr.sin_port = htons(atoi(argv[2])); // define a porta da proxy e a converte para o formato big-endian para ser interpretado pela rede

    /* abre o socket da proxy */
    if((sock_descr = socket(AF_INET,SOCK_STREAM,0)) < 0){
        puts("Não consegui abrir o socket da proxy\n"); 
        exit(1);
    }

    saveLog("Cliente 1: Consultando dado...\n");

    /* conecta o socket aberto ao endereco da proxy */
    if(connect(sock_descr,(struct sockaddr*)&proxyaddr,sizeof(proxyaddr)) < 0){
        puts("Não consegui conectar com a proxy!\n");
        exit(1);
    }
    
    /* limpa o buffer */
    memset(buffer, 0, sizeof(buffer));

    /* envia uma mensagem de requisicao para o servidor para consultar o dado */
    if(write(sock_descr,  "GET" , strlen("GET")) != strlen("GET")){
        puts("Cliente 1 nao conseguiu fazer a requisicao. Fechando a conexao..\n");
        close(sock_descr);
        exit(1);
    }
    
    /* recebe os dados da proxy */
    if((nread = read(sock_descr,buffer,BUFSIZ)) < 0){
        printf("Cliente 1 nao conseguiu fazer a consulta. Fechando a conexao..\n");
        close(sock_descr);
        exit(1);
    }

    saveLog_with_data("Cliente 1: Recebeu da proxy o dado---> %d\n\n", atoi(buffer));

    close(sock_descr);
    
    exit(0);
}