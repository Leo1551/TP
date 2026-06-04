#ifndef UTILIDADES_H
#define UTILIDADES_H

void limpar_buffer_de_teclado(void);
void capitalizarPalavras(char *str);
void trocar_quebra_de_linha_por_terminador(char *str);
FILE* achar_string_em_arquivo(char *nome, char *filename);
int calc_bst(int bst, int ev, int iv);
int calc_hp(int bst, int ev, int iv);
#endif // UTILIDADES_H
