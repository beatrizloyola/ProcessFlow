#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

// Sequência de Collatz
// O final sempre vai dar um

// Filho calcula, pai mostra o resultado

int main() {
    int n;
    pid_t pid;

    printf("Enter n: ");
    scanf("%d", &n);

    pid = fork();

    if (pid == 0){ // Filho
        while (n > 1){
            printf("%d, ", n);
            if (n % 2 == 0){ // Par divide por 2
                n = n / 2;
            } else { // Ímpar multiplica por 3 e soma 1
                n = 3 * n + 1;
            }
        }
        printf("%d\n", n);
    } else if (pid > 0){ // Pai
        wait(NULL);
        printf("n = %d\n", n); // Continua o digitado, dados não são compartilhados entre processos (stacks diferentes)
    }

    return 0;
}