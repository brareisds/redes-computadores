/* 
    Funcionalidade: Salvar o log do sistema cliente-servidor TCP/IP
    Autor: Barbara Reis
    Data da última modificação: 11/maio/2024 
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void saveLog(const char *message);
void saveLog_with_data(const char *message, int dado);
int getTime(void);