#include <stdio.h>
#include <string.h>
#include "utilidades.h"

void limpar_buffer_de_teclado(){
    short int c; while ((c = getchar()) != '\n' && c != EOF);
}

void capitalizarPalavras(char *str) {
    int novaPalavra = 1; // Flag para indicar início de palavra
    
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == ' ') {
            novaPalavra = 1;
        } else {
            if (novaPalavra) { // Capitaliza a primeira letra da palavra
                if (str[i] >= 'a' && str[i] <= 'z')
                    str[i] = str[i] - 32;
                    
                novaPalavra = 0;
            } else // Minúsculas para o restante
                if (str[i] >= 'A' && str[i] <= 'Z') 
                    str[i] = str[i] + 32;
                
            
        }
    }
}

void substituir_espaco_por_underline(char *str){
    for (int i = 0; i < strlen(str); i++)
        if (str[i] == ' ')
            str[i] = '_';
}

void trocar_quebra_de_linha_por_terminador(char *str){
    str[strcspn(str, "\n")] = '\0';
}

FILE* achar_string_em_arquivo(char *nome, char *filename){
    FILE *f = fopen(filename, "r");
    if (!f) arquivo_nao_encontrado_exception(filename);
    
    char buffer[100];

    while (fscanf(f, " %63[^ ]", buffer) == 1)
        if (strcmp(buffer, nome) == 0) 
            return f;
    
    fclose(f);
    string_nao_encontrada_exception(nome, filename);
    return NULL;
}

int calc_bst(int bst, int ev, int iv){// todos os pokes são lv 100
    // esses (int) são pra fazer a função floor
    return 5 + (100 * (int) (((2 * (bst + iv)) + (int) (ev/4) + 5)/100));
}
int calc_hp(int bst, int ev, int iv){// todos os pokes são lv 100
    // esses (int) são pra fazer a função floor
    return (100 * (int) (((2 * (bst + iv)) + (int) (ev/4) + 100)/100)) + 100 + 10;
}