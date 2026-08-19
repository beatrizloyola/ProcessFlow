#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(){

    pid_t pid, pid1;

    pid = fork(); /* Fork cria uma duplicata do processo original.
    Se der certo, o pai retorna o id do filho e o filho retorna 0.
    Se der errado, o pai retorna -1 e o filho não é criado. */

    if (pid < 0) { // Se der errado
        exit(1); // Encerra o programa

    } else if (pid == 0) { // Só o filho pode executar esse bloco
        pid1 = getpid(); // Pega o id do processo que está sendo executado no momento (no caso, o filho)
        printf("pid = %d\n", pid); // Id real do filho
        printf("pid1 = %d\n", pid1); // Id real do pai

    } else if (pid > 0) { // Só o pai pode executar esse bloco
        pid1 = getpid(); // Pega o id do processo que está sendo executado no momento (no caso, o pai)
        printf("pid = %d\n", pid); // Na visão do pai, o id do filho é sempre zero
        printf("pid1 = %d\n", pid1); // Id real do pai
        wait(NULL); // Espera o filho acabar
        printf("filho terminou!\n");
    }

    return 0; // Tanto o pai quanto o filho caem aqui e encerram
}

// Dica:
// Uma função em C que, quando recebe comandos reais, cria um filho e executa o comando
// Ao executar essa função X múltiplas vezes com um for, ela roda os comandos em paralelo
// Colocar o wait() depois do for 