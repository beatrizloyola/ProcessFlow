#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include "task.h"
#include "exec.h"

void spawn(Task *t){
    pid_t pid;

    pid = fork(); /* Fork cria uma duplicata do processo original.
    Se der certo, o pai retorna o id do filho e o filho retorna 0.
    Se der errado, o pai retorna -1 e o filho não é criado. */

    if (pid < 0) { // Se der errado
        fprintf(stderr, "Erro no fork\n");
        return;
    } else if (pid == 0) { // Só o filho pode executar esse bloco
        if(execvp(t->programa, t->argumentos) == -1){ // Susbtitui a imagem do processo filho pelo programa
            fprintf(stderr, "Erro ao executar o programa\n");
            exit(1); // Mata o filho pra evitar processo zumbi
        } 
    } else if (pid > 0) { // Só o pai pode executar esse bloco
        waitpid(pid, NULL, 0); // Espera o filho acabar
    }

    return;
}

pid_t spawn_async(Task *t){
    pid_t pid;

    pid = fork(); /* Fork cria uma duplicata do processo original.
    Se der certo, o pai retorna o id do filho e o filho retorna 0.
    Se der errado, o pai retorna -1 e o filho não é criado. */

    if (pid < 0) { // Se der errado
        fprintf(stderr, "Erro no fork\n");
        return -1;
    } else if (pid == 0) { // Só o filho pode executar esse bloco
        if(execvp(t->programa, t->argumentos) == -1){ // Susbtitui a imagem do processo filho pelo programa
            fprintf(stderr, "Erro ao executar o programa\n");
            exit(1); // Mata o filho pra evitar processo zumbi
        } 
    }
    return pid;
}

void run_pipe(Task *tasks[], int n){
    int pipes[n-1][2]; // Uma pipe por par, 0 e 1 para escrita/leitura

    // Cria todos os pipes antes de qualquer fork (se não os filhos não iam ter acesso aos pipes certos)
    for (int i = 0; i < n -1; i++){
        if (pipe(pipes[i]) == -1){
            fprintf(stderr, "Erro ao criar o pipe\n");
            return;
        }
    }

    // Array pra guardar os pids de todo mundo
    pid_t pids[n];

    // Fork de cada processo
    for (int i = 0; i < n; i++){
        pids[i] = fork();

        // Filho
        if (pids[i] == 0){
            if (i > 0){
                dup2(pipes[i-1][0], STDIN_FILENO); // Primeiro, lê
            }
            
            if (i < n-1){
                dup2(pipes[i][1], STDOUT_FILENO); // Segundo, escreve
            }

            for (int j = 0; j < n - 1; j++){ // Fecha as pipes
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            if(execvp(tasks[i]->programa, tasks[i]->argumentos) == -1){ // Susbtitui a imagem do processo filho pelo programa
                fprintf(stderr, "Erro ao executar o programa\n");
                exit(1); // Mata o filho pra evitar processo zumbi
            }
        }        
    }

    // Pai
    for (int j = 0; j < n - 1; j++){ // Fecha as pipes
        close(pipes[j][0]);
        close(pipes[j][1]);
    }

    for (int i = 0; i < n; i++){ // Espera pelos filhos
        if (pids[i] > 0){
            waitpid(pids[i], NULL, 0);
        }
    }
}