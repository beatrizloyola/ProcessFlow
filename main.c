#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]){ // argc é a quantidade de argumentos, argv é um array de string com os argumentos em si
    if (argc > 2){
        printf("Erro: Argumentos demais!\n");
        return -1;
    } else if (argc == 2){
        // Modo workflow
        // argv[1] é o caminho do arquivo pf
    } else {
        // Modo interativo
    }
}