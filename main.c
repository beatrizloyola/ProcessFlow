#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "task.h"
#include "exec.h"

static void processar_linha (char *buf){
    // Limpar o \n do final do buffer
        int len = strcspn(buf, "\n");
        buf[len] = '\0';

        // Se o buffer ficar vazio dps da troca, pergunta de novo
        if (len == 0){
            return;
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

        // Exit
        if (strcmp(args[0], "exit") == 0){
            exit(0);

        // Task
        } else if (strcmp(args[0], "task") == 0){
            if (argc_line < 3){
                fprintf(stderr, "Não é possível criar a task\n");
                return;
            }
            // Nome, programa, ponteiro começando do programa, quantidade de tokens de args[2] até o NULL
            criar_task(args[1], args[2], &args[2], argc_line - 2);

        // Run
        } else if (strcmp(args[0], "run") == 0) {
            if (argc_line < 3){
                fprintf(stderr, "Quantidade de argumentos insuficiente\n");
                return;
                }
            // Run sequential
            if (strcmp(args[1], "sequential") == 0){
                for (int i = 2; i < argc_line; i++){
                    Task *task = encontrar_task(args[i]);
                    if (task == NULL){
                        continue;
                    } else {
                        spawn(task);
                    }
                }
            // Run parallel
            } if (strcmp(args[1], "parallel") == 0){
                pid_t pids[64];
                int n = 0;
                for (int i = 2; i < argc_line; i++){ // Cria todos os filhos sem esperar o anterior terminar
                    Task *task = encontrar_task(args[i]);
                    if (task == NULL){
                        continue;
                    } else {
                        pids[n++] = spawn_async(task);
                    }
                }
                for (int i = 0; i < n; i++){ // Espera cada um dos filhos
                    if (pids[i] > 0){
                        waitpid(pids[i], NULL, 0);
                    }
                }
            // Run pipe
            } if (strcmp(args[1], "pipe") == 0){
                Task *tasks[64];
                int n = 0;
                int deuErro = 0;
                for (int i = 2; i < argc_line; i++){
                    Task *task = encontrar_task(args[i]);
                    if (task == NULL){
                        // Se não encontrou a task, aborta o pipe
                        deuErro = 1;
                        break;
                    }
                    tasks[n++] = task;
                }
                if (!deuErro){ // Só chama a função se não tiver dado problema
                    run_pipe(tasks, n);
                }
            }
        } else if (strcmp(args[0], "input") == 0){
            if (argc_line < 3){
                fprintf(stderr, "Quantidade de argumentos insuficiente\n");
                return;
            }

            Task *task = encontrar_task(args[1]);
            if (task != NULL){
                task->input_file = strdup(args[2]);
            }
        } else if (strcmp(args[0], "output") == 0){
            if (argc_line < 3){
                fprintf(stderr, "Quantidade de argumentos insuficiente\n");
                return;
            }

            Task *task = encontrar_task(args[1]);
            if (task != NULL){
                task->output_file = strdup(args[2]);
            }
        } else if (strcmp(args[0], "append") == 0){
            if (argc_line < 3){
                fprintf(stderr, "Quantidade de argumentos insuficiente\n");
                return;
            }

            Task *task = encontrar_task(args[1]);
            if (task != NULL){
                task->append_file = strdup(args[2]);
            }
        } else if (strcmp(args[0], "workdir") == 0){
            if (argc_line < 2){
                fprintf(stderr, "Quantidade de argumentos insuficiente\n");
                return;
            }
            if (chdir(args[1]) == -1){ // Muda o diretório de trabalho do processo
                perror("workdir"); // Imprime mensagem de erro do sistema
            }
        }
}


int main(int argc, char *argv[]){ // argc é a quantidade de argumentos, argv é um array de string com os argumentos em si
    if (argc > 2){
        fprintf(stderr, "Erro: Argumentos demais!\n");
        return -1;
    } else if (argc == 2){ // Modo workflow
        FILE *f = fopen(argv[1], "r");
        if (f == NULL){
            fprintf(stderr, "Erro ao abrir o arquivo workflow\n");
            return 1;
        }
        char buf[4096];
        while (fgets(buf, sizeof(buf), f) != NULL){
            printf("%s", buf); // imprime a linha antes de processar
            processar_linha(buf);
        }
        fclose(f);
    } else { // Modo interativo
        char buf[4096] = "";
        while (1) {
            printf("processflow> ");
            fflush(stdout);
            if (fgets(buf, sizeof(buf), stdin) == NULL){
                printf("\n");
                break;
            }
            processar_linha(buf);
        }
    }
    return 0;
}