/***************************************************************/
/**                                                           **/
/**   Marcos Kazuya Yamazaki                     7577622      **/
/**   Exercício-Programa 04                                   **/
/**                                                           **/
/**                                                           ************************************************************/
/**                                                                                                                     **/
/**  		A estrutura nesse exercício programa foi um pouco modificada do código original, ao invés de                **/
/**		cada célula utilizar os ponteiros da própria definição de cada uma das estruturas como a Skip                   **/
/**		List, Arvore Binarias, Arvores Binarias Aleatorias e as Arvores Rubros Negras adicionei um ponteiro             **/
/**		auxiliar, que servirá para ler todas as células em ordem decrescente em aparições. Com isso,                    **/
/**		não precisaremos ocupar mais memoria do que já é gasto.                                                         **/
/**			Testado cada uma das estruturas com grandes volumes de textos, o algoritmo que teve a pior eficiência       **/
/**		foi a Skip List, já que para além de ter que fazer a procura, quando se faz a inserção também há                **/
/**		de fazer outra procura, mas para percorrer a lista em ordem crescente levamos tempo constante.                  **/
/**			Entre as arvores binarias, a aleatória é o que teve o segundo pior tempo entre os quatros estruturas,       **/
/**		o que pode levar mais tempo são as inserção na raiz, pois tem todas as rotações.                                **/
/**		Com tempos quase parecidos, a arvore binaria e as rubros negras tiveram os melhores desempenhos,                **/
/**		sendo as rubros negras um pouco mais rápida.                                                                    **/
/**                                                                                                                     **/
/*************************************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "Item.h"
#include "ST.h"

#define TRUE 1
#define FALSE 0

int diferenciar = 1, n = 0;

int imprime(){
	STsort(ITEMshow, n); 
	return 0;
}

int leArquivo(char *livro, int opcao1, int opcao2){
	
	int caracter = 0, comando = 0, pula = 0;
	Key v; Item item;
	char letra;
	char* palavra;
	FILE *file;
	
	if(opcao1 == 0) livro = livro + 2;
	
	file = fopen(livro, "r"); /* Abre o arquivo */
	if (file == NULL) {
		/* Imprime menssagem de erro, caso o arquivo nao exista */
		printf("\tArquivo %s not found.\n", livro);
		return 1;
	}
	palavra = malloc(128*sizeof(char));
	
	/* A partir daqui, salva cada linha, caracter por caracter, ate o final do arquivo */
	while (feof(file) == FALSE) {
		letra = fgetc(file);
		pula = 0;
		
		if(opcao1 == 0 && comando == 0){
			switch (letra){
				case '+': comando = 1; break;
				case '-': comando = 2; break;
				case 'p': comando = 3; imprime(); puts(""); break;
				default: 
					printf("Arquivo nao esta no formato correto.\n");
					return 1;
			}
			pula = 1;
		}
		
		if(!pula){
		
			if(letra >= 'a' && letra <=  'z'){
				/* encontra um caracter valido */
				palavra[caracter++] = letra;
			}
			
			else if(letra >= 'A' && letra <= 'Z'){
				/* encontra um caracter, porem em letra maiuscula, fazendo as adaptacoes */
				if(opcao1 == 0){
					palavra[caracter++] = letra;
				}
				else{
					if(diferenciar == 1) palavra[caracter++] = letra;
					else palavra[caracter++] = letra + 'a' - 'A';
				}
			}
			
			else if(opcao1 == 0 &&
					((letra == '.' || letra == '-' || letra == '+' ||  letra == '_') || 
					(letra >= '0' && letra <= '9')))
				palavra[caracter++] = letra;
				
			else if(caracter > 0){
				/* caso nao achou uma letra, a palavra acabou */
				palavra[caracter] = 0; 
				if(opcao1 == 0) leArquivo(palavra, 1, comando);
				else if(opcao2 == 1){ /* Adiciona a palavra */
					ITEMscan(&v, palavra);
					if (STsearch(v, 1) == NULLitem){
						key(item) = v; 
						STinsert(item); 
					}
				}
				else if(opcao2 == 2){ /* Decrementa palavra */
					ITEMscan(&v, palavra);
					if(STsearch(v, -1) == NULLitem){
						STdelete(v);	
					}
				}
				caracter = 0;
			}
			
			if (letra == '\n' && opcao1 == 0) comando = 0;
		
		}
		pula = 0;
	}
	fclose(file); /* Fecha o arquivo */
	free(palavra);
	return 0;
}


int main (int argc, char* argv[])
{
	int i,j,d = 1;
	float time1, time2;
	
	time1 = (float) clock()/(float)(CLOCKS_PER_SEC);
	printf("\n\t\tExercicio Programa 4\n");
	STinit();
	for(i = 1; i < argc; i++) if(argv[i][0] == '-')
		switch(argv[i][1]){
			case 'n':
				j = 2;
				while(argv[i][j+1] != '\0') j++;
				while(j > 1){
					n = n + (argv[i][j]-'0')*d;
					d *= 10; j--;
				}
			break;
			case 'I':  diferenciar = 0;  break;
			case 'f':  leArquivo(argv[i], 0, 0); break;
		}
	
	time2 = (float) clock()/(float)(CLOCKS_PER_SEC);
	printf("  (%.3f seconds)\n", (float) time2 - time1);
	freee();
	return 0;
}
