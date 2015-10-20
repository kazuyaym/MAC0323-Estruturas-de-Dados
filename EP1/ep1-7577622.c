/***************************************************************/
/**                                                           **/
/**   Marcos Kazuya Yamazaki                     7577622      **/
/**   Exercício-Programa 01                                   **/
/**                                                           **/
/***************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char texto[655360];
/*  Vetor onde será guardada o texto de entrada */

/*  Celulas que guardada cada palavra do texto, nao ligando para as repetidas */
typedef struct palavra *pont;
pont iniSTP = NULL, fimSTP = NULL;

struct palavra{
	char *p;
	int qnt, M;
	pont next;
};

pont mergeOrdena(pont a, pont  b, int ind);
pont ordena(pont c, int ind);
int compara(pont a, pont b, int ind);

/*---------------------------------------------------------------------------------*/

int main (int argc, char* argv[])
{
	pont auxSTP, del = NULL;
	int dife = 1, i,j, tam = 0, k = 0, lin = -1;
	char c, *linha;

	/*   dife: Diferenciar letras maiusculas com minusculas          */
	/*           0 - falso	1 - verda                                */
	/*   i, j, k, tam: sao variaveis que auxiliarao com o tamanho    */
    /*   das string e entre outras coisas                            */
	/*   c, *linha: variaveis que auxiliarao para o comando inicial  */
	/*   como o -n** e o -I                                          */

	/*   Verifica os comando -n e -I    */
	for(i = 1; i < argc; i++){
		c = getopt(argc+i-1, argv, ":n:I:");
		if(c == 'n'){
			k = -1;
			for(j=0 ; argv[i][j] != '\0'; j++);
			linha = (char *) malloc((j-1)*sizeof(char));
			while(++k < j-2) linha[k] = argv[i][2+k];
			linha[k] = '\0';
			lin = atoi(linha);
			free(linha);
		}
		else if(c == 'I') dife = 0;
	}

	/*   Copia o texto que sera usado para um vetor   */
	fgets(texto, 655360, stdin);

	/*   Copia o texto do vetor para celulas onde cada      */
	/* 	palavra ficara guardada numa celula diferente       */
	/*   Excluindo pontuacao ou qualquer outro caracter     */
	/*  que nao seja uma letra                              */
	for(i = 0; ; ++i){
		/* Percorre o vetor ate encontrar um espaco ou uma pontuacao */
		if((texto[i] >= 65 && texto[i] <= 90) || (texto[i] >= 97 && texto[i] <= 122)) tam++;
		/* */
		else if(tam > 0){
			k = 0;
			/* Cria uma nova celula */
			auxSTP = (pont) malloc(sizeof *auxSTP);
			if(!auxSTP) return EXIT_FAILURE;
			auxSTP -> p = (char*) malloc((tam+1)*sizeof(char));
			if(!auxSTP -> p) return EXIT_FAILURE;

			auxSTP -> next = NULL;
			auxSTP -> qnt = 1;

			/*    Faz as devidas adaptacoes para futuras comparacoes, caso precise  */
            /*	diferenciar as letras maiusculas com as minusculas            		*/
			if(dife)
				if(texto[i-tam] <= 90)
					auxSTP -> M = 1;
				else auxSTP -> M = 0;
			else auxSTP -> M = 0;

			while(tam > 0)
				/* Copia caracter por caracter as palavras do vetor para o vetor da celula */
				auxSTP -> p[k++] = texto[i-(tam--)] <= 90 ?
					texto[i-(tam+1)]+32 :
					texto[i-(tam+1)];
			auxSTP -> p[k] = '\0';

			if(!iniSTP) iniSTP = auxSTP;
			else fimSTP -> next = auxSTP;
			fimSTP = auxSTP;
		}
		/* O texto acabou */
		if(texto[i] == '\0') break;
	}
	/* Ordena a lista em ordem alfabetica, ja comparando se existem palavras repetidas */
	iniSTP = ordena(iniSTP, 1);
	/* Ordena a lista em ordem decrescente de ocorrencias repetidas */
	iniSTP = ordena(iniSTP, 0);

	/*   Imprime as n linhas, caso foi dado o comando ou todas se nao.  */
	/*   E ja libera o espaco de cada celula depois de usava            */
	printf("\n");
	for(auxSTP = iniSTP; auxSTP; auxSTP = auxSTP -> next){
		free(del);
		if(auxSTP -> M) auxSTP -> p[0] -= 32;
		if(lin != 0) {
			printf("%s %i\n", auxSTP->p, auxSTP->qnt);
			lin--;
		}
		del = auxSTP;
	}
	free(del);
	return 0;
}

/*---------------------------------------------------------------------------------*/

pont mergeOrdena(pont a, pont  b, int ind)
{
	/*  Algoritmo adaptado do mergeSort, com um caso a mais,   */
	/*  que é no caso das palavras serem iguais, que ao inves  */
	/*  de ordena-lo esse retira a celula repetiva, 'rouba'    */
	/*  a contagem e depois libera ela                         */

    struct palavra head; pont c = &head, d;
	int ve;
    while (a && b){
		ve = compara(a, b, ind);
        if(ve == 1){
            c->next = a;
            c=a;
            a=a->next;
        }
        else if(ve == 0){
            c->next=b;
            c=b;
            b=b->next;
        }
		else{
			/*   Neste caso é quando há a ocorrencia de palavra repetida  */
			d = a;
			b->qnt += a->qnt;
			a=a->next;
			free(d);
		}
	}
    c->next = !a ? b : a;
    return head.next;
}

/*---------------------------------------------------------------------------------*/


pont ordena(pont c, int ind)
{
	/*                Algoritmo identico ao merge                 */
	/*   pont c: recebe a celula na qual vai ordenar              */
	/*   int ind: indice, para diferenciar, se compara por ordem  */
	/*  alfabetica ou pelo numero de ocorrencias                  */                
	
    pont a, b;
    if(!c->next) return c;
    a = c;
    b = c->next;
    while(b && b->next){
        c = c->next; b=b->next->next;
    }
    b = c-> next;
    c->next = NULL;
    return mergeOrdena(ordena(a, ind), ordena(b, ind), ind);
}

/*---------------------------------------------------------------------------------*/

int compara(pont a, pont b, int ind)
{
	/*   Se a for menor devolver 1 caso contrario devolver 0         */
	/*   ind: 1 para nomes e 0 para quantidade de palavra repetida   */

	int i = 0, naoDife = 0;
	/*   naoDife: para comparar letras Maiusculas com Minusculas     */
	/*   0: quer dizer que a letra inicial nao faz diferenca se      */
	/*	 se for maiuscula ou minuscula ou sao de mesma natureza      */
	/*   1 ou 2: Diferenciar M com m                                 */

	if(ind){
		if(a->M == b->M) naoDife = 0;
		else if(a->M > b->M) naoDife = 1;
		else if(a->M < b->M) naoDife = 2;

		while(1){
			if(a->p[i] == '\0' &&  b->p[i] == '\0') {
				/* Se entrou neste caso, quer dizer que as duas palavras sao  */
				/* iguais nao importando com a letras Maiuscula ou Minuscula  */
				if(!naoDife) return 2;
				else if (naoDife == 1) return 1;
				else return 0;
			}
			/*  Como todas as palavras das celulas estao com letra minuscula       */ 
			/*  ela compara uma por um, caso repetivo compara e proxima e assim    */
			/*  por diante, ate umas das palavras acabar ou ela ser 'menor'        */
			if(a->p[i] < b->p[i] || a->p[i] == '\0') return 1;
			else if(a->p[i] > b->p[i] || b->p[i] == '\0') return 0;
			else i++;
		}
	}
	else{
		/* Compara a quantidade de palavras repetidas */
		if(a->qnt >= b->qnt) return 1;
		else return 0;
	}
}
