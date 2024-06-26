/* 
    Funcionalidade: Salvar o log do sistema cliente-servidor TCP/IP
    Autor: Barbara Reis
    Data da ultima modificacao: 11/maio/2024 
*/

#ifndef LOG_FUNCTIONS_H
#define LOG_FUNCTIONS_H

#include "log_functions.h"

/* salva os logs que nao utilizam dado na string */
void saveLog(const char *message) {
    time_t mytime = time(NULL); // Obtem o tempo atual
    char timestamp[20]; // String para armazenar a data/hora formatada

    strftime(timestamp, 20, "%Y-%m-%d %H:%M:%S", localtime(&mytime)); // Formata a data/hora
    FILE *log_file = fopen("log.txt", "a"); 
    if (log_file == NULL) {
        perror("Erro ao abrir o arquivo de log do servidor.");
        exit(1);
    }

    fprintf(log_file, "%s %s", timestamp, message); // Escreve a data/hora e a mensagem no arquivo de log
    printf("%s %s", timestamp, message);
    fclose(log_file); 
}

/* salva os logs que possuem um dado na string */
void saveLog_with_data(const char *message, int dado) {
    time_t mytime = time(NULL); // Obtem o tempo atual
    char timestamp[20]; // String para armazenar a data/hora formatada

    strftime(timestamp, 20, "%Y-%m-%d %H:%M:%S", localtime(&mytime)); // Formata a data/hora
    FILE *log_file = fopen("log.txt", "a"); 
    if (log_file == NULL) {
        perror("Erro ao abrir o arquivo de log do servidor.");
        exit(1);
    }

    fprintf(log_file, "%s ", timestamp); 
    printf("%s ", timestamp);
    fprintf(log_file, message, dado); // Escreve a mensagem com dados no arquivo de log
    printf(message, dado); 
    fclose(log_file); 
}

#endif