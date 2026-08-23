#include "job.h"
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

static Job jobs[64]; // static restringe a visibilidade ao próprio arquivo
static int contador_jobs = 0;

int criar_job(pid_t pid, char *nome_task){
    if (contador_jobs >= 64){
        fprintf(stderr, "Fila de jobs cheia\n");
        return -1;
    }
    
    jobs[contador_jobs].job_id = contador_jobs + 1; // Só pra o id começar em 1 igual no enuciado
    jobs[contador_jobs].pid = pid;
    strcpy(jobs[contador_jobs].nome_task, nome_task);
    jobs[contador_jobs].status = 0; // Começa rodando

    int id = jobs[contador_jobs].job_id;
    contador_jobs = contador_jobs + 1;
    return id;
}

void listar_jobs_ativos(void){
    for (int i = 0; i < contador_jobs; i++){
        if (jobs[i].status == 0){
            printf("[%d] %d %s - running\n", jobs[i].job_id, jobs[i].pid, jobs[i].nome_task);
        }
    }
    return;
}

Job *encontrar_job(int job_id){
    for (int i = 0; i < contador_jobs; i++){
        if(jobs[i].job_id == job_id){
            return &jobs[i];
        }
    }
    fprintf(stderr, "Job não encontrado\n");
    return NULL;
}

void marcar_job_concluido(pid_t pid){
    for (int i = 0; i < contador_jobs; i++){
        if (jobs[i].pid == pid){
            jobs[i].status = 1;
            return;
        }
    }
}

void sigchld_handler(int sig) {
    (void)sig; // Diz pro sistema que eu sei que não tô usando a variável na função
    pid_t pid;
    int status;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) { // Qualquer filho, não bloqueia
        marcar_job_concluido(pid);
    }
}