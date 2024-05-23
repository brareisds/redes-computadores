# Trabalho Prático: Proxy TCP

## Implementação

A implementação do sistema cliente-servidor TCP/IP foi feita em linguagem C, utilizando sockets para comunicação entre os diferentes componentes. O sistema é composto por quatro programas principais: a Proxy, o Cliente 1, o Servidor e o Cliente 2, cada um desempenhando um papel específico no fluxo de comunicação. Além disso, foi criado um arquivo log_functions.c que contém funções para salvar logs das operações realizadas pelos programas e um makefile encarregado de compilar esses arquivos para gerar os executáveis.

## Funcionamento do Sistema

- **Servidor**: Mantém uma variável que é um número inteiro. Espera por conexões dos clientes para atualizar o valor dessa variável.

- **Proxy**: Recebe as conexões dos clientes e encaminha para o servidor. Recebe os dados do servidor e os retorna ao cliente. Mantém um log das operações realizadas.

- **Clientes**: Dois clientes foram desenvolvidos: um que se conecta à proxy para obter o dado do servidor e outro que se conecta diretamente ao servidor para modificar o dado.

## Estrutura do Projeto

- `servidor.c`: Código fonte do servidor.
- `proxy.c`: Código fonte da proxy.
- `cliente_leitura.c`: Código fonte do cliente para leitura do dado.
- `cliente_escrita.c`: Código fonte do cliente para escrita do dado.
- `log_testes`: Log dos testes realizados.

## Como Executar

1. Compile o código fonte do servidor e da proxy.
2. Inicie o servidor informando sua porta para o serviço.
3. Inicie a proxy informando sua porta para o serviço.
4. Compile e execute os clientes conforme necessário informado a porta do servidor para o cliente 1 ou a porta da proxy para o cliente 2.

       make
       ./server 12345
       ./proxy localhost 12345 54321
       ./client1 localhost 54321
       ./client2 localhost 12345 42


