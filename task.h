#ifndef TASK_H
#define TASK_H

typedef struct Task{
    char nome[256];
    char programa[256];
    char *argumentos[64];
    int qtd_argumentos;
} Task;

void criar_task(char *nome, char *programa, char **argumentos, int qtd_argumentos);
Task *encontrar_task(char *nome);

#endif