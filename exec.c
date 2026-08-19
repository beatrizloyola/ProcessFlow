#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include "task.h"
#include "exec.h"

void spawn(Task *t){
    pid_t pid, pid1;

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