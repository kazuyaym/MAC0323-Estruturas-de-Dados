/***************************************************************/
/**                                                           **/
/**   Marcos Kazuya Yamazaki                     7577622      **/
/**   Exercício-Programa 02                                   **/
/**                                                           **/
/***************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*--------------  Definindo Estruturas  --------------*/

typedef struct ST_cel celula;
typedef struct ST_head cabeca;
typedef struct ST_sol solucao;

struct ST_cel{
	celula *left,*right,*up, *down;
	cabeca *col;
	int valor, linha;
};

struct ST_head{
	celula pai;
	int clausula, quantidade;
	cabeca *prev, *next;
};

struct ST_sol{
	int *expre; solucao *prox;
};

/*----------------  Variveis Globais  ----------------*/

static int numSolucao = 0, /* numeros totais de soluções encontradas */
    novoCaso = 0, /* -c */
	imprimeNum = 0, /* -n */
	imprimeSol = 0, /* -N */
	nVariaveis = 0,
	nExpressoes = 0,
	pilha[1024],
	ind = 0;
solucao *pSol = NULL, /* Inicio da lista das soluções */
        *fSol = NULL; /* Final da lista das soluções */
cabeca dance; /* Cabeça das cabeças */

/**-----------------------------------------------------------**/
/**                                                           **/
/**		Guarda solucoes numa lista ligada                     **/
/**                                                           **/
/**-----------------------------------------------------------**/

void guardaSolucao()
{
	/*int i;
	for(i = 0; i < ind; i++) printf("%i ", pilha[i]);
	printf("\n");*/

	solucao *auxS = NULL;
	int i = 0;

	numSolucao++;
	auxS = malloc(sizeof(solucao));
	auxS -> expre = (int *) calloc(ind, sizeof(int));
	auxS -> prox = NULL;

	if(!pSol) pSol = (fSol = auxS);
	else fSol -> prox = auxS;
	fSol = auxS;
	for(i = 0; i < ind; i++){
		if(!novoCaso){
            if(pilha[i] > 0) auxS -> expre[i] = 1;
		}
		else auxS -> expre[i] = pilha[i];
	}
}

/**-----------------------------------------------------------**/
/**                                                           **/
/**			No problema da cobertura generalizada             **/
/** 	confere se a solução encontrada é valida              **/
/**                                                           **/
/**-----------------------------------------------------------**/

void verificaSolucao()
{
	cabeca *aux;
	int i;

	for(i = 0, aux = dance.prev; aux -> quantidade >= 1; i++, aux = aux -> prev);
	if(i == nExpressoes) guardaSolucao();
}

/**-----------------------------------------------------------**/
/**                                                           **/
/**		Recursao!                                             **/
/**                                                           **/
/**-----------------------------------------------------------**/

void procuraSolucao(cabeca *cab)
{
	celula *cel, *i, *j;

	if(cab == &dance){
		guardaSolucao();
		return;
	}
	if(cab -> clausula > nVariaveis){
		verificaSolucao();
		return;
	}

	for(cel = cab -> pai.down; cel != &(cab -> pai); cel = cel -> down){

		cel -> col -> next -> prev = cel -> col -> prev;
		cel -> col -> prev -> next = cel -> col -> next;

		pilha[ind] = cel -> linha; ind++;
		for(i = cel -> right; i != cel && i -> valor == 1; i = i -> right){
			i -> col -> next -> prev = i -> col -> prev;
			i -> col -> prev -> next = i -> col -> next;
		}
		for(i = cel -> down; i != cel; i = i -> down){ /*para baixo*/
			for(j = i -> right; j != i; j = j -> right){
				/*para os lado tira ele*/
				j -> up -> down = j -> down;
				j -> down -> up = j -> up;
				j -> col -> quantidade--;
			}
		}

		procuraSolucao(cab -> next);
		ind--;
		cel -> col -> next -> prev = cel -> col;
		cel -> col -> prev -> next = cel -> col;

		for(i = cel -> right; i != cel && i -> valor == 1; i = i -> right){
			i -> col -> next -> prev = i -> col;
			i -> col -> prev -> next = i -> col;
		}
		for(i = cel -> down; i != cel; i = i -> down){
			for(j = i -> right; j != i; j = j -> right){
				j -> up -> down = j;
				j -> down -> up = j;
				j -> col -> quantidade++;
			}
		}
	}
}

/**-----------------------------------------------------------**/
/**                                                           **/
/**        Adiciona uma nova coluna a metriz                  **/
/**-----------------------------------------------------------**/

void adicionaColuna(cabeca *H, int qnt)
{
	cabeca *aux;

	while(qnt-- > 0){
		aux = malloc(sizeof(cabeca));

		aux -> pai.linha = (aux -> pai.valor = (aux -> quantidade = 0));
		aux -> pai.left = (aux -> pai.right = &(aux -> pai));
		aux -> pai.up = (aux -> pai.down = &(aux -> pai));
		aux -> pai.col = NULL;
		aux -> next = H;
		aux -> prev = H -> prev;
		H -> prev = aux;
		aux -> prev -> next = aux;
		aux -> clausula = aux -> prev -> clausula + 1;
	}
}

/**-----------------------------------------------------------**/
/**                                                           **/
/**        Adiciona uma nova celula a matriz                  **/
/**-----------------------------------------------------------**/

void adicionaCelula(cabeca *H, celula *A, int v, int l)
{
	celula *aux = malloc(sizeof(celula));

	aux -> col = H;
	aux -> up = H -> pai.up;
	H -> pai.up = aux;
	aux -> down = &(H -> pai);
	aux -> up -> down = aux;

	if(A){
		aux -> left = A;
		aux -> right = A -> right;
		A -> right -> left = aux;
		A -> right = aux;
	}
	else{
		aux -> left = aux;
		aux -> right = aux;
	}

	aux -> valor = v;
	aux -> linha = l;
	aux -> col -> quantidade++;
}

/**-----------------------------------------------------------**/
/**                                                           **/
/**		Funcao que devolve um ponteiro, que aponta para uma   **/
/**	 celula anterior a que será adicionada                    **/
/**                                                           **/
/**-----------------------------------------------------------**/

celula* FUNCAO_0(int k)
{
	int v = 1, i;
	cabeca *aux;
	if(k < 0){v = -1; k = k*(-1);}

	for(i = 1, aux =dance.next; i < k; ++i, aux = aux -> next);
	if(v == 1) return aux -> pai.up -> left;
	else       return aux -> pai.down -> left;
}

cabeca* FUNCAO_1(int w)
{
	cabeca *aux;
	for(aux = dance.next; aux -> clausula != w; aux = aux -> next);
	return aux;
}

celula* FUNCAO_2(int k)
{
	cabeca *aux;
	if(k == 0) return NULL;

	for(aux = dance.next; aux -> clausula != k; aux = aux -> next);
	return aux -> pai.up;
}

/**-----------------------------------------------------------**/
/**                                                           **/
/**                                                           **/
/**-----------------------------------------------------------**/

int main (int argc, char* argv[])
{
	int kSat, i, j, k, w;
	char opcao, entrada[512];
	cabeca *aux;
	solucao *auxS = NULL;

    while((opcao = getopt(argc, argv, "nNC")) != -1){
        switch(opcao){
            case 'n':  imprimeNum = 1;  break;
            case 'N':  imprimeSol = 1;  break;
	        case 'C':  novoCaso   = 1; imprimeSol = 1; break;
        }
    }

	/* Cabeça das cabeças ! */
	dance.pai.linha = (dance.pai.valor =
				(dance.clausula = (dance.quantidade = 0)));
	dance.pai.left = (dance.pai.right = &dance.pai);
	dance.pai.up = (dance.pai.down = &dance.pai);
	dance.next = (dance.prev = &dance);
	dance.pai.col = NULL;

	if(!novoCaso) scanf("%i", &kSat);
	scanf("%i %i", &nVariaveis, &nExpressoes);

	adicionaColuna(&dance, nVariaveis);

	if(!novoCaso){
		for(i = 1, aux = dance.next; i <= nVariaveis; i++, aux = aux -> next){
			adicionaCelula(aux, NULL, 1, i*(-1)); adicionaCelula(aux, NULL, 1, i);
		}
		for(i = 0; i < nExpressoes; ++i){
			adicionaColuna(&dance, 1);
			for(j = 0; j < kSat; ++j){
				scanf("%i", &k);
				adicionaCelula(dance.prev, FUNCAO_0(k), 2, k);
			}
		}
		procuraSolucao(dance.next);
	}

	/*        -C nao consegui terminar a tempo =/       */
	else{
		for(i = 1; i <= nExpressoes; ++i){
			fflush(stdin);
			fgets(entrada, 512, stdin);
			j = 0; k = 0;
			do{
				sscanf(entrada,"%i",&w);
				adicionaCelula(FUNCAO_1(w), FUNCAO_2(k), 1, i);
				k = w;
				if(entrada[j+1] != '\0') entrada[j++] = '0';
				else break;
				if(entrada[j+1] != '\0') entrada[j++] = '0';
				else break;
			}while(1);
		}
		procuraSolucao(dance.next);
	}

	/*   Imprime soluções, ou mostra as soluções   */
	if(imprimeNum){
		printf("%i\n", numSolucao);
	}
	else if(imprimeSol){
		printf("%i\n", numSolucao);
		for(auxS = pSol; auxS; auxS = auxS -> prox){
			if(!novoCaso){
				for(i = 0; i<4 ; i++){
					printf("%i", auxS -> expre[i]);
				}
			}
			printf("\n");
		}
	}
	else{
		if(numSolucao > 0) printf("1\n");
		else printf("0\n");
	}
    return 0;
}
