#include <stdio.h>
void limpar_buffer_de_teclado(){
    short int c; while ((c = getchar()) != '\n' && c != EOF);
}

void capitalizarPalavras(char *str) {
    int novaPalavra = 1; // Flag para indicar início de palavra
    
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == ' ') {
            novaPalavra = 1;
        } else {
            if (novaPalavra) {
                // Capitaliza a primeira letra da palavra
                if (str[i] >= 'a' && str[i] <= 'z') {
                    str[i] = str[i] - 32;
                }
                novaPalavra = 0;
            } else {
                // Minúsculas para o restante
                if (str[i] >= 'A' && str[i] <= 'Z') {
                    str[i] = str[i] + 32;
                }
            }
        }
    }
}
void trocar_quebra_de_linha_por_terminador(char *str){
    str[strcspn(str, "\n")] = '\0';
}