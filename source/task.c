#include "task.h"
#include <stdio.h>
#include <string.h>

static Task tasks[64]; // static restringe a visibilidade ao próprio arquivo
static int contador_tasks = 0;

void criar_task(char *nome, char *programa, char **argumentos, int qtd_argumentos){
    if (contador_tasks >= 64){
        fprintf(stderr, "Fila de tasks cheia\n");
        return;
    }

    if (qtd_argumentos >= 64){
        fprintf(stderr, "Muitos argumentos\n");
        return;
    }

    strcpy(tasks[contador_tasks].nome, nome);
    strcpy(tasks[contador_tasks].programa, programa);
    for (int i = 0; i < qtd_argumentos; i++){
        tasks[contador_tasks].argumentos[i] = strdup(argumentos[i]);
    }
    tasks[contador_tasks].qtd_argumentos = qtd_argumentos;
    contador_tasks = contador_tasks + 1;

    return;
}

Task *encontrar_task(char *nome){
    for (int i = 0; i < contador_tasks; i++){
        if(strcmp(nome, tasks[i].nome) == 0){
            return &tasks[i];
        }
    }
    fprintf(stderr, "Tarefa não encontrada\n");
    return NULL;
}