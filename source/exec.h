#ifndef EXEC_H
#define EXEC_H

#include "task.h"

void spawn(Task *t);
pid_t spawn_async(Task *t);
void run_pipe(Task *tasks[], int n);

#endif