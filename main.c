#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "task.h"
#include "exec.h"


int main(int argc, char *argv[]){ // argc é a quantidade de argumentos, argv é um array de string com os argumentos em si
    if (argc > 2){
        fprintf(stderr, "Erro: Argumentos demais!\n");
        return -1;
    } else if (argc == 2){
        // Modo workflow
        // argv[1] é o caminho do arquivo .pf
    } else {
        // Modo interativo
        char buf[4096] = "";
        while (1) {
            printf("processflow> ");
            fflush(stdout);
            if (fgets(buf, sizeof(buf), stdin) == NULL){
                printf("\n");
                break;
            }

            // Limpar o \n do final do buffer
            int len = strcspn(buf, "\n");
            buf[len] = '\0';

            // Se o buffer ficar vazio dps da troca, pergunta de novo
            if (len == 0){
                continue;
            }

            char *args[64]; // Lista pra guardar os argumentos
            int argc_line = 0; // Contador

            char *token = strtok(buf, " "); // Pega o primeiro token
            while (token != NULL){ // Enquanto o token não for nulo (não chegar no fim dos args)
                args[argc_line] = token; // Lugar da lista aponta pra pedaço do buffer
                argc_line = argc_line + 1; // Aumenta o contador
                token = strtok(NULL, " "); // Passa pro próximo token
            }
            args[argc_line] = NULL; // Acabou token = acabou a lista

            // Comandos
            if (strcmp(args[0], "exit") == 0){
                exit(0);

            } else if (strcmp(args[0], "task") == 0){
                if (argc_line < 3){
                    fprintf(stderr, "Não é possível criar a task\n");
                    continue;
                }
                // Nome, programa, ponteiro começando do programa, quantidade de tokens de args[2] até o NULL
                criar_task(args[1], args[2], &args[2], argc_line - 2);

            } else if (strcmp(args[0], "run") == 0) {
                if (argc_line < 3){
                    fprintf(stderr, "Quantidade de argumentos insuficiente\n");
                    continue;
                    }
                if (strcmp(args[1], "sequential") == 0){
                    Task *task = encontrar_task(args[2]);
                    if (task == NULL){
                        continue;
                    } else {
                        spawn(task);
                    }
                }
            }
        }
        
        return 0;
    }
}