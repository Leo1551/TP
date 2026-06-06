#include <stdio.h>
#include <string.h>
#include "utilidades.h"
#include "pokemon.h"

void limpar_buffer_de_teclado(){
    short int c; while ((c = getchar()) != '\n' && c != EOF);
}

char* show_status_condition(Condition condition){
    if (condition == OK)
        return "Sem status";
    if (condition == BURN)
        return "Burned";
    if (condition == FREEZE)
        return "Frozen";
    if (condition == PARALYZE)
        return "Paralyzed";
    if (condition == POISON)
        return "Poisoned";
    if (condition == BADLY_POISON)
        return "Badly Poisoned";
    if (condition == SLEEP)
        return "Sleeping";
    if (condition == CONFUSED)
        return "Confused";
    
    return "Não pode ser computado";    
}
char* int_type_to_string(int type){
    char tipo_convertido;
    char *tipo_search;
    sprintf(tipo_convertido, "%c", type);
    FILE *arq = achar_string_em_arquivo(&tipo_convertido, "arquivos/types.txt");

    fscanf(arq, "%s", tipo_search);

    return tipo_search;

}
char* int_category_to_string(int categoria){
    switch (categoria)
    {
        case 1:  return "Physical";
        case 2:  return "Special";
        case 3:  return "Status";
    }
    return "Categoria não computada";
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
char* other_conditions_to_string(MoveCondition *conditions){
    int tam = sizeof(conditions)/sizeof(MoveCondition);
    char buffer[1000];

    for (int i = 0; i < tam; i++){
        if (conditions[i] == NONE)
            return "None";
        else if (conditions[i] == IN_CHARGE)
            strcat(buffer, "Carregando move ");
        else if(conditions[i] == BLASTED)
            strcat(buffer, "Recarregando energias");
        else if(conditions[i] == FLYING)
            strcat(buffer, "Durante Fly");
        else if(conditions[i] == DIVE)
            strcat(buffer, "Usando Dive");
        else if(conditions[i] == DIG)
            strcat(buffer, "Usando Dig");
        else if(conditions[i] == BIND)
            strcat(buffer, "Sob efeito de Bind");
        else if(conditions[i] == MIST)
            strcat(buffer, "Sob efeito de Mist");
        else if(conditions[i] == BIDE)
            strcat(buffer, "Carregando Bide");
        else if(conditions[i] == SEED)
            strcat(buffer, "Sob efeito de Leech Seed");

        strcat(buffer, " ");
    }

    return buffer;
    
}