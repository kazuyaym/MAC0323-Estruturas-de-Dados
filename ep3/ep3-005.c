/***************************************************************/
/**                                                           **/
/**   Marcos Kazuya Yamazaki                     7577622      **/
/**   Exercício-Programa 03                                   **/
/**                                                           **/
/***************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wchar.h>

#define TRUE 1
#define FALSE 0
#define lgNmax 30

/* Cada linha do texto deve conter no maximo 128 caracteres */
/* texto[-livro-][-linha-][-caracter-]                      */
static char ***texto;
static int numLivros = 0, /* Numero de livros, ou arquivos que o programa esta guardando */
	   *linhaTexto, /* Numero de linhas que cada texto possui */
		   l = 1; /* Tamanho dos contextos */

typedef struct STnode *pont;
typedef struct STcontexto *contex;
typedef struct STpilha *pilha;

struct STnode {
	char pal[32];
	int **ocor, numOcor;
	
	pont *next;
	int sz;
};


struct STpilha{
	int contextos; 
	/* contexto: numeros de contextos que a celula possui  */
	/* 0 para vazio e negativo para o conjunto universo */
	
	contex iniContex,
	       finContex;
	pilha ant, pos;
	/*   celula duplamente ligada, isso facilitara nas rotacoes,   */
	/*   nas trocas e nas manipilacoes da pilha                    */
};


struct STcontexto{
	int text, /* O texto na qual o contexto pertence */
	    inf, sup; /* Intervalo do contexto, as linhas inferior e superior */	
	contex prox;
};

static pont head; /* Cabeca da skip list */
static pilha topoPilha; /* Aponta para a ultima celula da pilha */
static int N, lgN, 
	tamPilha; /* Tamanho da pilha atual */


int comparaString(char *a, char *b){
	/*   Se a for menor que b: devolve -1    */
	/*   Se a for maior que b: devolve 1   */
	/*   Se a for igual a b:   devolve 0    */
	int i = 0;
	while(1){
		/*printf(".. %c:%i %c:%i\n", a[i],a[i], b[i], b[i]);*/
		if(a[i] == 0 &&  b[i] == 0) return 0;
		/* Se entrou neste caso, quer dizer que as duas palavras sao iguais */
		
		/*  Como todas as palavras das celulas estao com letra minuscula       */ 
		/*  ela compara uma por um, caso repetivo compara e proxima e assim    */
		/*  por diante, ate umas das palavras acabar ou ela ser 'menor'        */
		if(a[i] < b[i]) return -1;
		if(a[i] > b[i]) return 1;
		i++;
	}
}

pont newCel(char *b, int k){
	/******************************************/
	/*                                        */
	/*   Cria nova celula da skip list        */
	/*   char *b: palavra que sera guardada   */
	/*   int k: nivel maximodos apontadores   */
	/*                                        */
	/******************************************/
	
	int i = 0;
	pont x = malloc(sizeof *x);
	x->next = malloc(k*sizeof(pont));
	
	/* Copia caracter por caracter, a palavra que será guardada */
	do x -> pal[i] = b[i++]; while(b[i-1] != 0);
	
	x->sz = k; /* Nivel de apontadores da celula */
	/* Atribui NULL para os apontadores iniciais */
	for (i = 0; i < k; i++) x->next[i] = NULL;
	return x;                         
}                                   

void skipListInit(){
	/******************************************/
	/*                                        */
	/*   Funcao que inicializa a Skip List    */
	/*                                        */
	/******************************************/
	
	char c[1];
	c[0] = 0;
	N = 0; lgN = 0; 
	head = newCel(c, lgNmax); 
}

pont searchR(pont t, char *b, int k){	
	/********************************************/
	/*                                          */
	/*   Dada uma string, procura a palavra     */
	/*   na skip list, e devolve a celula que   */
	/*   a contém ou devolve NULO se nao tiver  */
	/*                                          */
	/********************************************/
	int j;
	
	if (t->next[k] == NULL) { 
		if (k == 0) return NULL;
		return searchR(t, b, k-1); 
	}
	j = comparaString(t -> next[k] -> pal, b);
	if (!j){ /* j == 0, encontru palavra igual! */
		return t -> next[k];
	}
	if (j == 1){ /* como j == 1, a proximo nesse nivel eh maior */
		if (k == 0) return NULL;
		return searchR(t, b, k-1); 
	}
	return searchR(t->next[k], b, k);
}

pont STsearch(char *b){
	return searchR(head, b, lgN); 
}

int randX(){
	/******************************************/
	/*                                        */
	/*   Gera um numero aleatorio, entre 1    */
	/*   e o numero maximo de apontadores     */
	/*                                        */
	/******************************************/
	
	int i, j, t = rand();
	for (i = 1, j = 2; i < lgNmax; i++, j += j)
		if (t > RAND_MAX/j) break;
	if (i > lgN) lgN = i;
	return i;
}

void insertR(pont t, pont x, int k){
	if ((t->next[k] == NULL) || comparaString(x -> pal, t -> next[k] -> pal) == -1) 
	{ 
		if (k < x->sz){ 
			x->next[k] = t->next[k]; 
			t->next[k] = x;
		}
        if (k == 0) return;
		insertR(t, x, k-1); return;
	}
	insertR(t->next[k], x, k);
}

pont STinsert(char *b){
	pont x = newCel(b, randX());
	insertR(head, x, lgN); N++;
	return  x;
}    

void pegaPalavra(char *pal, int l, int b){
	/***************************************************/
	/*	                                           */
	/*       Pala cada palavra encontrada no arquivo   */
	/*   ela guardara na skip list, as respectivas     */
	/*   linhas e texto em que foi encotrando          */
	/*                                                 */
	/***************************************************/
	pont p;
	
	p = STsearch(pal);
	if(p){ /* a palavra ja existe na skip list, adicionar a nova ocorrencia se for diferente */
		if(!(p -> ocor[p -> numOcor-1][0] == b && p -> ocor[p -> numOcor-1][1] == l)){
			p -> numOcor++;
			p -> ocor = realloc(p -> ocor, p -> numOcor*sizeof(int*));
			p -> ocor[p -> numOcor-1] = malloc(2*sizeof(int));
			p -> ocor[p -> numOcor-1][0] = b; p -> ocor[p -> numOcor-1][1] = l;
		}
	}
	else{ /* nao tem palavra na skip list, criar uma! */
		p = STinsert(pal);
		p -> numOcor = 1;
		p -> ocor = malloc(sizeof(int*));
		p -> ocor[0] = malloc(2*sizeof(int));
		p -> ocor[0][0] = b; p -> ocor[0][1] = l;
	}
}






int leString(char s[], int max){
    int i = 0;
    char letra;

    for (i = 0; i < (max - 1); i++) {
		letra = fgetc(stdin);
		if (letra == '\n') break;
		s[i] = letra;
    }
	s[i] = 0;
	return i;
}

int leLivro(char *livro){

	/*
	linha: variavel que indicará a linha do texto a ser copiado
	letra: variavel que será auxiliará na copia do testo
	*/
	
	int i = 0,k = 0, linha = 1, caracter = 0;
	char letra, *palavra;
	
	FILE *file = fopen(livro, "r"); /* Abre o arquivo */
	if (file == NULL) {
		/* Imprime menssagem de erro, caso o arquivo nao exista */
		printf("\tArquivo %s not found.\n", livro);
		return 1;
	}

	if(numLivros == 0) texto = malloc(sizeof(char**)); /* Caso nao ha nenhum livro salvo */
	else texto = realloc (texto, (numLivros+1)*sizeof(char**)); /* Caso contrario, realoca a memoria */

	texto[numLivros] = malloc(50000 * sizeof(char*)); /* Para cada livro aloca memoria para cada linha */
	for(i = 0; i < 50000; ++i) texto[numLivros][i] = malloc(128*sizeof(char));

	
	palavra = malloc(32*sizeof(char));
	/* A partir daqui, salva cada linha, caracter por caracter, ate o final do arquivo */
	while (feof(file) == FALSE) {
		letra = fgetc(file);
		
		
		if(letra >= 'a' && letra <=  'z'){
			/* encontra um caracter valido */
			palavra[caracter++] = letra;
		}
		else if(letra >= 'A' && letra <= 'Z'){
			/* encontra um caracter, porem em letra maiuscula, fazendo as adaptacoes */
			palavra[caracter++] = letra + 'a' - 'A';
		}
		else if(caracter > 0){
			/* caso nao achou uma letra, a palavra acabou */
			palavra[caracter] = 0;
			pegaPalavra(palavra, linha, numLivros);
			caracter = 0; 
		}
		
		if (letra == '\n'){ /* Muda de linha */
			texto[numLivros][linha][k] = 0;
			linha++; k = 0;
		}
		else texto[numLivros][linha][k++] = letra;
	}
	fclose(file); /* Fecha o arquivo */
	texto[numLivros] = realloc(texto[numLivros], (linha+1) * sizeof(char*));
	linhaTexto[numLivros] = linha; /* Guarda o numero de linha que o n-simo texto possui */
	
	i = 0;
	do{
		/* Na linha 0, é colocada o nome do arquivo */
		texto[numLivros][0][i] = livro[i];
		i++;
	}while(livro[i-1] != 0);

	
	
	/*for(i = 0; i < linha+1; i++) printf("%i: %s\n", i, texto[numLivros][i]);*/
	numLivros++; free(palavra);
	return 0;
}

void menssagemErro(){
	/* Imprima uma menssagem de erro, caso o comando digitado nao exista */
	printf("\tCommand does not exit!\n");
}

void STempilha(pont b){
	int i, j;
	printf("\n\n\n\n\nK\n\n\n\n");
	pilha aux = malloc(sizeof(struct STpilha));
	contex auxC;
	printf("\n\n\n\nMORRE\n\n\n\n");
	/*aux -> tl = malloc((aux -> tam)*sizeof(int*));*/
	/*aux -> contextos = b -> numOcor;*/
	aux -> iniContex = NULL;
	
	aux -> contextos = 0;
	for(i = 0; i < (b -> numOcor); i++){ /* Trata cada linha */
		for(j = 1; j <= l; ++j){ /* em cada contexto */
			/* Verifica se tal contexto exite */
			if(b -> ocor[i][1] - l + j <= 0 ||
			   b -> ocor[i][1] + j - 1 > linhaTexto[b -> ocor[i][0]])
				continue;
			printf("LINHAS ACHADAS: %i %i %i\n", b -> ocor[i][1] - l + j, b -> ocor[i][1] + j - 1, linhaTexto[b -> ocor[i][0]]);
			/* se sim */
			aux -> contextos++;
			
			/*Cria a nova celula para guardar cada contexto*/
			auxC = malloc(sizeof(struct STcontexto));
			auxC -> prox = NULL;
			
			/*faz toda a manipulacao dos ponteiros*/
			if(aux -> iniContex == NULL){
				aux -> iniContex = auxC;
			}
			else{
				aux -> finContex -> prox = auxC;
			}
			
			aux -> finContex = auxC;
			auxC -> text = b -> ocor[i][0];
			auxC -> inf = b -> ocor[i][1] - l + j;
			auxC -> sup = b -> ocor[i][1] + j - 1;
			
			auxC = NULL;
		}
	}
	if(!tamPilha){
		aux -> ant = aux;
		aux -> pos = aux;
		topoPilha = aux;
	}
	else{
		aux -> pos = topoPilha -> pos;
		aux -> ant = topoPilha;
		topoPilha -> pos = (topoPilha -> pos -> ant = aux);
		topoPilha = aux;
	}
	tamPilha++;
}

void empilhaUniverso(){
	int i;
	pilha aux = malloc(sizeof(struct STpilha));
	contex auxC;
	aux -> contextos = 0;
	
	for(i = 0; i < numLivros; i++){
		if(linhaTexto[i] != 0){
			aux -> contextos--;
			/*Cria a nova celula para guardar contexto */
			auxC = malloc(sizeof(struct STcontexto));
			auxC -> prox = NULL;
			
			/*faz toda a manipulacao dos ponteiros*/
			if(aux -> iniContex == NULL) aux -> iniContex = auxC;
			else aux -> finContex -> prox = auxC;
			
			aux -> finContex = auxC;
			auxC -> text = i;
			auxC -> inf = 0;
			auxC -> sup = linhaTexto[i];
			
			auxC = NULL;
		}
	}
	/*CODIGO REPETIDOOOO!!!*/
	if(!tamPilha){
		aux -> ant = aux;
		aux -> pos = aux;
		topoPilha = aux;
	}
	else{
		aux -> pos = topoPilha -> pos;
		aux -> ant = topoPilha;
		topoPilha -> pos = (topoPilha -> pos -> ant = aux);
		topoPilha = aux;
	}
	tamPilha++;
}


void operacaoConjuntos(int op){
	pilha s = topoPilha,
	      t = topoPilha -> ant;
	      n;
	contex sc = s -> iniContex,
	       tc = t -> iniContex;
	       aux1, aux2;
	       
	/* Tratar o universo!!!!! */       
	
	n = malloc(sizeof(struct STpilha))
	aux -> contextos = 0;
	
	while(sc && tc){
		if(sc -> text == tc -> text && sc -> inf == tc -> inf){
			aux1 = sc; aux2 = tc;
			sc = sc -> prox; tc = tc -> prox;
				
			if(op == 0) add(aux1);
			else if(op == 1) add(aux1);
			else if(op == 2) free(aux1);
			free(aux2);
		}
		else if(sc -> text < tc -> text || (sc -> inf < tc -> inf && sc -> text == tc -> text)){ 
			aux1 = sc; sc -> prox;
					
			if(op == 0) add(aux1);
			else if(op == 1) free(aux1);
			else if(op == 2) add(aux1);
		}
		else{ 
			aux1 = tc; tc -> prox;
			if(op == 0) add(aux1);
			else if(op == 1) free(aux1);
			else if(op == 2) free(aux1);
		}
		aux2 = (aux1 = NULL);
	}
	
	/*while(sc && tc){
		if(sc -> text == tc -> text){
			if(sc -> inf == tc -> inf){
				aux1 = sc; aux2 = tc;
				sc = sc -> prox; tc = tc -> prox;
				
				if(op == 0) add(aux1);
				else if(op == 1) add(aux1);
				else if(op == 2) free(aux1);
				free(aux2);
			}
			else{
				if(sc -> inf < tc -> inf){ 
					aux1 = sc; sc -> prox;
					
					if(op == 0) add(aux1);
					else if(op == 1) free(aux1);
					else if(op == 2) add(aux1);
				}
				else{ 
					aux1 = tc; tc -> prox;
					if(op == 0) add(aux1);
					else if(op == 1) free(aux1);
					else if(op == 2) free(aux1);
				}
			}
			aux2 = (aux1 = NULL);
		}
		else{
			if(sc -> text < tc -> text){
				aux1 = sc; sc -> prox;
				
				if(op == 0) add(aux1);
				else if(op == 1) free(aux1);
				else if(op == 2) add(aux1);
			}
			else{
				aux1 = tc; tc -> prox;
				if(op == 0) add(aux1);
				else if(op == 1) free(aux1);
				else if(op == 2) free(aux1);
			}
		}
	}*/
}

int main (int argc, char* argv[])
{
	int i = 0, j = 0;
	char *comando = malloc(32 * sizeof(char));
	struct STnode* aux = NULL;
	pilha del, kaz;
	
	tamPilha = 0; 
	skipListInit();
	linhaTexto = malloc(64*sizeof(int));
	
	do{
		printf("ep3 > ");
		leString(comando, 32);

		if(comando[0] == '.'){
			comando++;
			switch(comando[0]){
				case 'p': /* imprimir o conjunto que esta no topo da pilha */
					if(comando[1] != 0) menssagemErro();
					else{

					}
				break;
				case 'D': /* deletar o conjunto que esta no topo da pilha */
					if(comando[1] != 0) menssagemErro();
					else{
						if(tamPilha == 0){
							 printf("\tThe stack is already empty\n");
							 continue;
						}
						if(tamPilha == 1){
							free(topoPilha);
							topoPilha = NULL;
							tamPilha = 0;
						}
						else{
							del = topoPilha;
							topoPilha = topoPilha -> ant;
							topoPilha -> pos = topoPilha -> pos -> pos;
							topoPilha -> pos -> ant = topoPilha;
							tamPilha--;
							free(del);
							del = NULL;
						}
					}
				break;
				case '+':
					/* Comando que adiciona um novo livro */
					/*linhaTexto = realloc(linhaTexto, (numLivros+1)*sizeof(int));*/
					leLivro(++comando);
				break;
				case '-': 
					/* nesse programa também vamos supor que um texto só será excluido quando a lista estiver vazia */
					comando++;
				break;
				case 'S':
					if(comando[1] != 0) menssagemErro();
					else{ /* Trocar a ordem dos dois elementos no topo da pilha: { S, T, ..., n} -> { T, S, ..., n}	*/
						topoPilha -> ant -> pos = topoPilha -> pos;
						topoPilha -> pos -> ant = topoPilha -> ant;
						topoPilha -> pos = topoPilha -> ant;
						topoPilha -> ant = topoPilha -> ant -> ant;
						topoPilha -> ant -> pos = topoPilha;
						topoPilha -> pos -> ant = topoPilha;
						topoPilha = topoPilha -> pos;
					}
				break;
				case 'R':
					if(comando[1] != 0) menssagemErro();
					else{ /* Rotacao da pilha a partir do topo: {1,2,3..,n} -> {2,3,..,n,1} */
						topoPilha = topoPilha -> ant;
					}
				break;
				case 'N':
					if(comando[1] == 'P') /* Para saber quantos elementos estao na pilha */
						printf("\tThe stack has %i elements\n", tamPilha);
					else if(comando[1] == 'T'){

					}
					else menssagemErro();
				break;
				case 'q':
					if(comando[1] == 0){

					}
					/* Comando de saido do programa */
					else if(comando[1] == 'u' && comando[2] == 'i' && comando[3] == 't'){
						puts("\tPrograma finalizado!");
						return 0;
					}
					else menssagemErro();
				break;
				case 'U': /* Conjunto Universo */
					if(comando[1] != 0) menssagemErro();
					else{
						empilhaUniverso();
					}
				break;
				default:
					/* Neste programa o conjunto de linhas consecutivas varia de 1 a 9 */
					if(comando[0] > '0' && comando[0] <= '9' && comando[1] == 0){
						if(tamPilha == 0) l = comando[0] - '0';
						else printf("\tThe stack is not empty!\n");
					}
					else menssagemErro();
			}
		}
		else if(comando[0] == '+'){ 
			if(comando[1] != 0) menssagemErro();
			else{ /* Faz a uniao das dos dois ultimos conjuntos */
				if(tamPilha >= 2){
					operacaoConjuntos(1);
					tamPilha--;
				}
				else printf("\tThe stack must have at least two sets!\n");
			}
		}
		else if(comando[0] == '-'){
			if(comando[1] != 0) menssagemErro();
			else{ /* Faz a intersecção das dos dois ultimos conjuntos */
				if(tamPilha >= 2){
					operacaoConjuntos(1);
					tamPilha--;
				}
				else printf("\tThe stack must have at least two sets!\n");
			}
		}
		else if(comando[0] == '\\'){
			if(comando[1] != 0) menssagemErro();
			else{ /* S e T sao os ultimos dois conjuntos da pilha, S\T */
				if(tamPilha >= 2){
					operacaoConjuntos(2);
					tamPilha--;
				}
				else printf("\tThe stack must have at least two sets!\n");
			}
		}
		else if(comando[0] == 0) continue;
		else{ /* programa deve calcular o conjunto S dos contextos em que <palavra> ocorre. */
			aux = STsearch(comando);
			if(aux){
				STempilha(aux);
				for(i = 0; i < aux -> numOcor; ++i) 
					printf("%i: %s\n", aux -> ocor[i][1], texto[aux -> ocor[i][0]][aux -> ocor[i][1]]);
				puts("");
			}
			else{
				printf("\tWord is not found, nothing was pilled\n");
			}
		}
		/*imprime();*/
		if(topoPilha){
			/*printf("contexto: %i\n", topoPilha -> tam);
			for(i = 0; i < topoPilha -> tam; ++i) {
				printf("%i %i\n", topoPilha -> tl[i][0], topoPilha -> tl[i][1]);
			}
			puts("");*/
			for(i = 0; i < numLivros; i++){
				printf("texto %i tem %i linhas\n", i, linhaTexto[i]);
			}
			puts("");
			
			for(i = 0, kaz = topoPilha; i < tamPilha; ++i, kaz = kaz -> ant) {
				printf("%i : ", kaz -> contextos);
			}
			puts("");
		}
	}while(1);
	
	return 0;
}
