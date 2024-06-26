/* 
    Funcionalidade: Salvar o log do sistema cliente-servidor TCP/IP
    Autor: Barbara Reis
    Data da ultima modificacao: 11/maio/2024 
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* salva os logs que nao utilizam dado na string */
void saveLog(const char *message);
/* salva os logs que possuem um dado na string */
void saveLog_with_data(const char *message, int dado);
