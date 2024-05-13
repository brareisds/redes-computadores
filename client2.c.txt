/* 
    Funcionalidade: Um cliente-servidor TCP/IP 
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


int eh_inteiro(const char *dado) {
    char *endptr;
    strtol(dado, &endptr, 10);

    // Se o ponteiro de término for o fim da string, eh inteiro.
    if (*endptr == '\0') {
        return 1;
    }

    return 0; // Não é um número inteiro
}

int main(int argc, char *argv[]){
    int sock_descr; 
    struct sockaddr_in serveraddr; 
    struct hostent *hp; 
    char *host, *dados; 
    char mensagem[BUFSIZ + 1];

    if(argc != 4){
        puts("Uso correto: client2 <nome-servidor> <porta-servidor> <dado-inteiro>");
        exit(1);
    }

    host = argv [1];
    dados = argv[3];

    if(!eh_inteiro(dados)){
        puts("O dado fornecido nao eh um numero inteiro valido.\n");
        exit(1);
    }

    /* chama o resolvedor DNS. Retorna uma struct contendo o end. IP do host */
    if((hp = gethostbyname(host)) == NULL){
        puts("O DNS não retornou o endereço IP do servidor!\n");
        exit(1);
    }
    
    /* prepara a estrutura de socket do servidor */
    bcopy((char *)hp->h_addr, (char*)&serveraddr.sin_addr, hp->h_length); // copia o end. IP que veio do DNS para a estrutura serveraddr 
    serveraddr.sin_family = AF_INET;  // define a familia de enderecos IP da internet
    serveraddr.sin_port = htons(atoi(argv[2])); // define a porta do servidor e a converte para o formato big-endian para ser interpretado pela rede

    /* abre o socket do servidor */
    if((sock_descr = socket(AF_INET,SOCK_STREAM,0)) < 0){
        puts("Cliente 2 nao conseguiu abrir o socket\n"); 
        exit(1);
    }

    /* conecta o socket aberto ao endereco do servidor. Estabelece uma conexao com o servidor */
    if(connect(sock_descr,(struct sockaddr*)&serveraddr,sizeof(serveraddr)) < 0){
        puts("Cliente 2 não conseguiu conectar ao servidor!\n");
        exit(1);
    }

    /* formata a mensagem que sera enviada para o servidor com a requisicao (SET) e com o novo valor do dado */
    sprintf(mensagem, "SET %s", dados);

    saveLog_with_data("Cliente 2: Modificando o dado do servidor para --> %d\n", atoi(dados));
    printf("\n");

    /* transmite a mensagem para o servidor e verifica o sucesso da msg */
    if(write(sock_descr, mensagem, strlen(mensagem)) != strlen(mensagem)){
        puts("Cliente 2 não conseguiu transmitir dados para o servidor! Fechando a conexao..\n");
        close(sock_descr);
        exit(1);
    }

    memset(mensagem, 0, sizeof(mensagem));
    close(sock_descr);
  
    exit(0);
}




