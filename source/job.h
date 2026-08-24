#ifndef JOB_H
#define JOB_H

#include <sys/types.h>

typedef struct Job {
    int job_id;
    pid_t pid;
    char nome_task[256];
    int status; // 0 = rodando, 1 = cabou
} Job;

int criar_job(pid_t pid, char *nome_task);
Job *encontrar_job(int job_id);
void listar_jobs_ativos(void);
void marcar_job_concluido(pid_t pid);
void sigchld_handler(int sig);

#endif