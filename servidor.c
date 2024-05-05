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
	struct sockaddr_in serv, cli;
	struct hostent *hp;
	char localhost [MAXHOSTNAME];
    struct in_addr **addr_list;
    int dado = 0;

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
    serv.sin_port = htons(atoi(argv[1]));

    // hp->h_addr = 127.0.1.1 (localhost)
    /* copia o end. IP que veio do dns para a estrutura socketaddr_in do socket serv*/
	bcopy ((char *) hp->h_addr, (char *) &serv.sin_addr, hp->h_length);
    
    //serv.sin_family = AF_INET;
	serv.sin_family = hp->h_addrtype;		

    /* open server socket. returns a file descriptor for the socket. 
    sock_desc = socket(AF_INET, transport, protocol)*/
	if ((s = socket(hp->h_addrtype,SOCK_STREAM,0)) < 0){
           puts ( "Nao consegui abrir o socket" );
		exit (1);
	}	

    /* bind() associates a socket with an address */
	if (bind(s, (struct sockaddr *) &serv, sizeof(serv)) < 0){
		puts ( "Nao consegui fazer o bind" );
		exit (1);
	}		
 
    /* listen() prepares it for incoming connections. */
	listen (s, TAMFILA);
	
	while (1){
		i = sizeof(serv);
        /* accept estabelece uma conexão solicitada pelo cliente*/
		if ((t=accept(s, (struct sockaddr *) &cli,&i))<0){
                   puts ( "Nao consegui estabelecer conexao" );
                   exit (1);
		}	

    
        //int nread;
        //nread = read(t, buf, BUFSIZ);
        //buf[nread] = '\0';
        printf("Sou o servidor, recebi a mensavgem----> %s\n", buf);
        write(t,buf, BUFSIZ);
        fflush(stdout);
        /* Limpa o buffer buf para a próxima iteração do loop */
        //buf[0] = '\0';

        /* Fecha o socket conectado ao cliente após o envio da menservgem de volta. */
        close(t);
   }
   exit(0);
}