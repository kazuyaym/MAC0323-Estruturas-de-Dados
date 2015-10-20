/***************************************************************/
/**                                                           **/
/**   Marcos Kazuya Yamazaki                     7577622      **/
/**   Exercício-Programa 02                                   **/
/**                                                           **/
/***************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*-------------------------------------------------------------*/
/*--                                                         --*/
/*--    Dentro das celulas que serao usadas nesse problema   --*/
/*-- terão 5 ponteiros, 4 representando cada um lado         --*/
/*-- (direita, esquerda, acima, abaxo), e o outro apontará   --*/
/*-- para a cabeça de cada coluna principal.                 --*/
/*-- Dois inteiros: "valor", que normalmente reprentara a    --*/
/*-- a coluna ou a linha, quando for a cabeça, ou valor 0    --*/
/*-- (zero) quando ela estiver no interior da matriz e       --*/
/*-- "qnt" que tera valor >= 0 quando a celula pertencer     --*/
/*-- a cabeça das colunas, e ela representará a quantidade   --*/
/*-- de celulas em cada coluna                               --*/
/*--                                                         --*/
/*-------------------------------------------------------------*/

typedef struct ST_cel celula;
struct ST_cel{celula *L,*R,*U,*D,*head; int valor, qnt;};

typedef struct ST_sol solucao;
struct ST_sol{int *expre; solucao *prox;};

/*-------------------------------------------------------------*/
/*--                                                         --*/
/*--    Funcao que cria uma celula e insere ela na matriz,   --*/
/*-- porem suponhe-se que ela estará no inferior direito     --*/
/*-- Parametros de entrada:                                  --*/
/*-- celula *x, celula *y: coordenadas x e y da matriz onde  --*/
/*--                       será inserida a celula            --*/
/*-- int v: valor que sera guardado na celula                --*/
/*--                                                         --*/
/*-------------------------------------------------------------*/

void colocaCelula(celula *x, celula *y, int v){
	celula *aux;
	aux = malloc(sizeof (celula));
	aux -> valor = v; aux -> qnt = 0;
	aux -> head = x;
	aux -> head -> qnt++;
	aux -> U = x -> U; aux -> L = y -> L; aux -> D = x; aux -> R = y;
	y -> L = (x -> U = (y -> L -> R = (x -> U -> D = aux)));
}

void colocaCelulaClausula(celula *cel,int x, int y){
	celula *aux, *aux2;

	if(!x) aux = cel -> L;
	else for(aux = cel -> R; aux -> valor != x; aux = aux -> R);
	if(!y) aux2 = cel -> U;
	else for(aux2 = cel -> D; aux2 -> valor != y; aux2 = aux2 -> D);
	colocaCelula(aux, aux2, 0);
}

void criaPonteirosDancantes(celula *cel, int i,int j,int k)
{
	int cont = 1;
	celula *aux, *aux2, *kazu;
	while(i-- > 0){
		aux = malloc(sizeof (celula));
		aux -> R = cel;
		aux -> L = cel -> L;
		cel -> L = (cel -> L -> R = (aux -> U = (aux -> D = aux)));

		aux -> head = cel;
		aux -> valor = aux -> L -> valor + 1;
		aux -> qnt = 0;
	}
	while(j-- > 0){
		aux = malloc(sizeof (celula));
		aux -> D = cel;
		aux -> U = cel -> U;
		cel -> U = (cel -> U -> D = (aux -> L = (aux -> R = aux)));

		aux -> head = cel;
		aux -> qnt = 0;
		if(!k){
			aux -> valor = (cont%2 == 1) ?
				   (cont+1)/(-2) : cont/2;
			cont++;
		}
		else if(k == 2) aux -> valor = aux -> U -> valor + 1;
	}
	if(!k){ /* k == 0 */
		for(aux = cel -> R, aux2 = cel -> D;
			aux != cel; aux = aux -> R, aux2 = aux2 -> D -> D){
			colocaCelula(aux, aux2, 0); colocaCelula(aux, aux2 -> D, 0);
		}
	}
	/*  for(kazu = cel -> L -> L, cont = 1; cont < 57; cont++, kazu = kazu -> L) printf("%i\n",kazu -> valor);  */
}

void escondeLinha(celula *del){
	celula *aux = del;
	do{
		aux -> U -> D = aux -> D; aux -> D -> U = aux -> U;
		(aux -> head -> qnt)--;
		aux = aux -> R;
	} while(aux != del);
}

void recolocaLinha(celula *rec){
	celula *aux = rec;
	do{
		aux -> U -> D = aux; aux -> D -> U = aux;
		(aux -> head -> qnt)++;
		aux = aux -> R;
	}while(aux != rec);
}

void  guardaSolucao(celula *cel, solucao *pSol){
	celula *aux = cel -> D;
	for(; aux != cel; aux = aux -> D) printf("%i ", aux -> valor);
	printf("\n");
}

void verificaSolucao(int cls, celula *cel, int cobertura, int *nSol, solucao *pSol){
	celula *aux = cel -> L;
	while(cls != 0 && (cobertura? (aux -> qnt == 1) : (aux -> qnt >= 1))){
		cls--; aux = aux -> L;
	}
	if(!cls){
		guardaSolucao(cel, pSol);
		*nSol++;
	}
	return;
}

void buscaProfKsat(celula *C, int k, int cls, celula *cel, int *nSol, solucao *pSol){
	celula *aux;

	aux = C -> valor < 0 ? C -> D : C -> U;
	escondeLinha(aux);

	if(!k){
		verificaSolucao(cls, cel, 0, nSol, pSol);
		recolocaLinha(aux);
		return;
	}
	buscaProfKsat(C -> D, k-1, cls, cel, nSol, pSol);
	buscaProfKsat(C -> D -> D, k-1, cls, cel, nSol, pSol);
	recolocaLinha(aux);
}

void buscaProfEspecial(){
	/* recursao que tira e coloca mais linhas ao mesmo tempo */
}

int main (int argc, char* argv[]){
	int kSat, nVariaveis, nExpressoes, i, j, k, nSol = 0,
		novoCaso = 0, imprimeNum = 0, imprimeSol = 0;
	char opcao;
	celula dance, *aux, *kazu, *kazu2;
	solucao *pSol = NULL;

    while((opcao = getopt(argc, argv, "nNC")) != -1){
        switch(opcao){
            case 'n':  imprimeNum = 1;  break;    /* Imprime o numero de solucoes */
            case 'N':  imprimeSol = 1;  break;    /* Imprime as solução e a qnt   */
			case 'C':  novoCaso   = 1;  break;    /* ~o~                          */
        }
    }

	/*  Criada a cabeça das cabeças, fazemos o ponteiro da cabeça das  */
    /* 	linhas quanto das colunas ligaram para ela mesmo               */
	dance.L = (dance.R = (dance.U = (dance.D = &dance)));
	dance.head = &(dance); dance.valor = (dance.qnt = 0);

	if(!novoCaso) scanf("%i", &kSat);
	scanf("%i %i", &nVariaveis, &nExpressoes);

	/*quantidade = (int*) malloc((nVariaveis+nExpressoes+1)*sizeof(int));
	for(i = 0; i <= (nVariaveis+nExpressoes); i++) quantidade[i] = 0;*/

	if(!novoCaso){
		criaPonteirosDancantes(&dance, nVariaveis, nVariaveis*2, 0);
		for(i = 0; i < nExpressoes; ++i){
			criaPonteirosDancantes(&dance, 1, 0, 1);
			for(j = 0; j < kSat; ++j){
				scanf("%i", &k);
				colocaCelulaClausula(&dance, 0, k);
			}
		}
		buscaProfKsat(dance.D, nVariaveis-1, nExpressoes, &dance, &nSol, pSol);
		buscaProfKsat(dance.D -> D, nVariaveis-1, nExpressoes, &dance, &nSol, pSol);
	}
	else{
		criaPonteirosDancantes(&dance, nVariaveis, nExpressoes, 2);
		for(i = 0; i < nExpressoes; ++i){
			/* PEGAR OS BAGUIOS ATE O ENTER! APRENDER COMO FAZER */
		}
		/* SUPONHE-SE QUE ATE AQUI A MATRIZ ESTEJA PRONTA */
		for(i = 0, aux = dance.D; i < nExpressoes; ++i, aux = aux -> D){
			buscaProfEspecial(aux, &dance);
		}
	}
	/*
	for(kazu = &(dance), i = 0; kazu != &(dance) || i == 0; i = 1, kazu = kazu -> D, printf("\n"))
		for(kazu2 = kazu, j = 0; kazu2 != kazu || j == 0; j = 1, kazu2 = kazu2 -> R)
			printf("%i:%i:%i ",kazu2 -> head -> valor, kazu2 -> valor, kazu2 -> qnt);
	*/		
	printf("%i\n\n", nSol);
    return 0;
}


