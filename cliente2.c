/* 
    Funcionalidade: Um cliente TCP/IP 
    Autor: Barbara Reis
    Data da última modificação: 29/abril/2024 
    Cliente: Programa que o usuario inicializa 
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h> /* biblioteca socket eh generica -> nao so pra tcpip. Os itens da internet em especifico estao nessa lib*/
#include <netdb.h> /* Biblioteca para usar o DNS */
#include <stdlib.h>
#include <string.h>

/* argc: numero de argumentos da linha de comando*/
int main(int argc, char *argv[]){
    int sock_descr; /* pid do socket */
    int numbytesrecv; /* numero bytes recebidos */
    struct sockaddr_in sock_enderec; /* struct imbutida do tipo sockaddr_in*/
    struct hostent *hp; /* estrutura imbutida para poder usar o DNS -> retorna o end. IP dado o nome etc dentro da struct*/
    char buf[BUFSIZ+1]; /*BUFSIZ const do sistema recomendada para o sistema*/
    char *host; /* onde o servidor esta rodadndo*/
    char *dados; /*que serao transmitos*/

    if(argc != 4){
        /* ./cli babs-pc 1500 oioi */
        puts("Uso correto: cliente <nome-servidor> <porta> <dados>");
        exit(1);
    }

    host = argv [1];
    dados = argv[3];

    /*Chama o DNS para resolver o nome. Retorna dentro do HP as informacoes*/
    if((hp = gethostbyname(host)) == NULL){
        puts("O DNS não retornou o endereço IP do servidor!");
        exit(1);
    }

    printf("Host name: %s\n", hp->h_name);
   
    /* copiar o end IP para o socket do cliente */
    bcopy((char *)hp->h_addr, (char*)&sock_enderec.sin_addr, hp->h_length);

    /* Fala que esta falando da familia dos endereccos da internet TCP/IP*/
    sock_enderec.sin_family = AF_INET;
    /* Processadores da intel sao littleEndian entao percisa converter para BigEndian usando a funcao htons. Converte a porta para interio*/
    sock_enderec.sin_port = htons(atoi(argv[2])); /* Precisa converter somente a porta */

    /* Abre o socket */
    if((sock_descr = socket(AF_INET,SOCK_STREAM,0)) < 0){
        puts("Não consegui abrir o socket"); /* Difcil de acontecer */
        exit(1);
    }

    /* A partir daqui o socket esta aberto */

    /* Cliente especifica endereço do servidor */
    /* connect() is used on the client side, and assigns a free local port number to a socket */
    if(connect(sock_descr,(struct sockaddr*)&sock_enderec,sizeof(sock_enderec)) < 0){
        puts("Não consegui conectar ao servidor!");
        exit(1);
    }

    // Envia uma mensagem para a proxy para modificar o dado
    write(sock_descr,"SET" , strlen("SET"));
    
    printf("Modificando dado...\n");
    // Solicita e envia o novo valor do dado
    printf("Novo valor do dado: %d\n", atoi(dados));
    //scanf("%s", buf);
    write(sock_descr,dados,strlen(dados));

    close(sock_descr);
    exit(0);
}