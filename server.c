/* 
    Funcionalidade: Um servidor iterativo TCP/IP 
    Autor: Barbara Reis
    Data da última modificação: 9/maio/2024 
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
    char buffer [ BUFSIZ + 1], *dados, *requisicao;
	struct sockaddr_in serveraddr, clientaddr;
	struct hostent *hp;
	char localhost [MAXHOSTNAME];
    int dado = 0; // inicializa o dado no servidor com o valor 0

    if (argc != 2) {
        puts("Uso correto: servidor <porta-serv>");
        exit(1);
    }

    /* system call que retorna o hostname na string localhost */
    gethostname (localhost, MAXHOSTNAME);
    // Abre o arquivo de log em modo de escrita
    // FILE *log_file = fopen("log.txt", "w");
    // if (log_file == NULL) {
    //     perror("Erro ao abrir o arquivo de log");
    //     exit(1);
    // }

    /* Redireciona a saída padrão (stdout) para o arquivo de log */
    //dup2(fileno(log_file), STDOUT_FILENO);

    /* chama o resolvedor DNS. Retorna uma struct contendo o end. IP do host  */
	if ((hp = gethostbyname(localhost)) == NULL){
		puts ("Nao consegui meu proprio IP\n");
		exit (1);
	}	

    /* prepara a estrutura de socket do servidor */
    serveraddr.sin_port = htons(atoi(argv[1])); // copia o end. IP que veio do DNS para a estrutura serveraddr 
	serveraddr.sin_family = AF_INET;  // define a familia de enderecos IP da internet
	bcopy ((char *) hp->h_addr, (char *) &serveraddr.sin_addr, hp->h_length); //copia o end. IP que veio do DNS para a estrutura serveraddr 
    		
    /* abre o socket do servidor. Retorna o descritor do socket */
	if ((server_socket = socket(AF_INET,SOCK_STREAM,0)) < 0){
        puts("Nao consegui abrir o socket\n");
		exit (1);
	}	
    
    /* associa o socket com o endereco do servidor. Informa a porta que o socket ira operar*/
	if (bind(server_socket, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0){
		puts("Nao consegui fazer o bind" );
		exit (1);
	}		

    /* prepara o servidor para receber conexoes */
    if(listen (server_socket, TAMFILA) < 0) {
        puts("Erro ao escutar conexões no servidor!\n");
        exit(1);
    }

    printf("Servidor inicializado com o dado: %d\n", dado);
   

	while (1){
		serveraddrsize = sizeof(serveraddr);
        printf("\nServidor aguardando conexao...\n");

        /* estabelece uma conexão solicitada pelo cliente */
		if ((client_socket = accept(server_socket, (struct sockaddr *) &clientaddr,&serveraddrsize))<0){
            puts("Servidor nao conseguiu estabelecer conexao com o cliente\n");
            exit(1);
		}	
        printf("Conexao estabelecida com sucesso.\n");

        /* verifica o tipo da requisicao do cliente. GET ou SET */
        if((nread = read(client_socket, buffer, BUFSIZ)) < 0){
            puts("Servidor conseguiu receber requisicao da proxy. Fechando a conexao..\n");
            close(server_socket); // precisa fechar o server socket?
            close(client_socket);
            exit(1);
        }

        requisicao = strtok(buffer, " "); 

        /* se a mensagem for do Cliente 1, envia o dado atual */
        if (strcmp(requisicao, "GET") == 0) {
            printf("\nRequisicao do tipo GET recebida.\n");
            printf("Cliente 1 consultando dado...\n");

            /* limpa o buffer antes de reutilizá-lo */
            memset(buffer, 0, sizeof(buffer));

            /* converte o dado atual para string para transmissao */
            sprintf(buffer, "%d", dado);
           
            printf("Servidor transmitindo o dado ----> %d\n", dado);

            /* transmite o dado atual para o cliente 1 */
            if(write(client_socket, buffer, strlen(buffer)) != strlen(buffer)){
                puts("Servidor nao conseguiu transmitir o dado atual para a proxy. Fechando a conexao..\n");
                close(server_socket); 
                close(client_socket);
                exit(1);
            }
            printf("Dado enviado para o cliente.\n");
        }

        /* se a mensagem for do Cliente 2, modifica o dado atual */
        if (strcmp(requisicao, "SET") == 0) {
            printf("\nRequisicao do tipo SET recebida.\n");

            /* armazena o dado enviado pelo cliente 2 */
            dados = strtok(NULL, " "); 
            dado = atoi(dados);
            printf("Cliente 2 modificando o dado do servidor para ----> %d\n", dado);
            printf("Dado modificado com sucesso.\n");
        }

      
        /* fecha a conexao com o cliente */
        close(client_socket);
        memset(buffer, 0, sizeof(buffer));
        
   }
   // Fecha o arquivo de log no final do programa
    //fclose(log_file);
   
   exit(0);
}