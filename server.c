/* Um servidor TCP/IP Iterativo Iterativo ou Concorrente 
   Elias P. Duarte Jr. 
   Ultima Atualizacao: 17/08/22 */
/*https://en.wikipedia.org/wiki/Berkeley_sockets*/
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h> // gethostbyname(localhost))
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h> // add por mim -> Functions for manipulating numeric IP addresses.

#define TAMFILA      5
#define MAXHOSTNAME 30

int main (int argc, char *argv[]) {
	int s, t;
	int i;
    char buf [BUFSIZ + 1];
	struct sockaddr_in sa, isa;
	struct hostent *hp;
	char localhost [MAXHOSTNAME];
    struct in_addr **addr_list;
    int dado;

    if (argc != 2) {
        puts("Uso correto: servidor <porta>");
        exit(1);
    }

    /* returns the null-terminated hostname in the character array localhost, which has a length of len bytes*/
	/* int gethostname(char *name, size_t len); */
    gethostname (localhost, MAXHOSTNAME);
    printf("localhost: %s\n", localhost);


    
	if ((hp = gethostbyname(localhost)) == NULL){
		puts ("Nao consegui meu proprio IP");
		exit (1);
	}	

    // --------------------------------------------------------------------------------
    printf("Host name: %s\n", hp->h_name);
    printf("Aliases:\n");
    for (int i = 0; hp->h_aliases[i] != NULL; i++) {
        printf("\t%s\n", hp->h_aliases[i]);
    }
    
    printf("Address type: %d\n", hp->h_addrtype);
    printf("Address length: %d\n", hp->h_length);
    
    printf("Addresses:\n");
    /* inet_ntoa converte um endereço IP de formato binário para uma representação em string legível. */
    /* lista de enderecos PERGUNTA */
    for (int i = 0; hp->h_addr_list[i] != NULL; i++) {
        printf("%s\n", inet_ntoa(*(struct in_addr*)(hp->h_addr_list[i])));
    }
    printf("(char *) hp->h_addr: %s\n",(char *) hp->h_addr);


	// -----------------------------------------------------------------------------------

    // htons -> converte o numero da porta para o formato correto de rede (big-endian)

	/* atribui o numero da porta (devidamente convertido para big endian) ao membro da 
     * struct 'sockaddr_in' que representa o numero da porta  */
    sa.sin_port = htons(atoi(argv[1]));

    // hp->h_addr = 127.0.1.1 (localhost)
    /* copia o end. IP que veio do dns para a estrutura socketaddr_in do socket sa*/
	bcopy ((char *) hp->h_addr, (char *) &sa.sin_addr, hp->h_length);
    
    //sa.sin_family = AF_INET;
	sa.sin_family = hp->h_addrtype;		

    /* open server socket. returns a file descriptor for the socket. 
    sock_desc = socket(AF_INET, transport, protocol)*/
	if ((s = socket(hp->h_addrtype,SOCK_STREAM,0)) < 0){
           puts ( "Nao consegui abrir o socket" );
		exit (1);
	}	

    /* bind() associates a socket with an address */
	if (bind(s, (struct sockaddr *) &sa, sizeof(sa)) < 0){
		puts ( "Nao consegui fazer o bind" );
		exit (1);
	}		
    dado = 0;
    /* listen() prepares it for incoming connections. */
	listen (s, TAMFILA);

    
	while (1){
		i = sizeof(sa);
        /* accept estabelece uma conexão solicitada pelo cliente*/
		if ((t=accept(s, (struct sockaddr *) &isa,&i))<0){
            puts ( "Nao consegui estabelecer conexao" );
            exit (1);
		}	

        read(t, buf, BUFSIZ);
        // Se a mensagem for do Cliente 1, envia o dado atual
        if (strcmp(buf, "GET") == 0) {
            sprintf(buf, "%d", dado);
            printf("Sou o servidor, tenho a mensagem----> %d\n", dado);
            write(t,buf, BUFSIZ);
        }

        // Se a mensagem for do Cliente 2, modifica o dado
        if (strcmp(buf, "SET") == 0) {
            read(t, buf, BUFSIZ);
            //sprintf(buf, "%d", dado);
            dado = atoi(buf);
            printf("Dado modificado no servidor para: %d\n", dado);
        }
        close(t);
   }
   exit(0);
}