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
#include <time.h>


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
	int **ocor, numOcor, dicio;
	
	/* pal: onde será guardada a palavra                          */
	/* ocor[-texto-][-linha-] em que a palavra aparece            */
	/* numOcor: numero de linhas em que a palavra apareceu        */
	/* dicio: tem valor 0 se a palavra nao pertence ao dicionario */
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
static int N, lgN, tamPilha; /* Tamanho da pilha atual */


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
	do{
		x -> pal[i] = b[i];
		i++;
	}while(b[i-1] != 0);
	
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

void pegaPalavra(char *pal, int l, int b, int dic){
	/***************************************************/
	/*	                                               */
	/*       Pala cada palavra encontrada no arquivo   */
	/*   ela guardara na skip list, as respectivas     */
	/*   linhas e texto em que foi encotrando          */
	/*                                                 */
	/***************************************************/
	pont p;
	
	p = STsearch(pal);
	if(p){ /* a palavra ja existe na skip list, adicionar a nova ocorrencia se for diferente */
		if(!dic){
			if(!(p -> ocor[p -> numOcor-1][0] == b && p -> ocor[p -> numOcor-1][1] == l)){
				p -> numOcor++;
				p -> ocor = realloc(p -> ocor, p -> numOcor*sizeof(int*));
				p -> ocor[p -> numOcor-1] = malloc(2*sizeof(int));
				p -> ocor[p -> numOcor-1][0] = b; p -> ocor[p -> numOcor-1][1] = l;
			}
		}
		else p -> dicio = 1;
	}
	else if(!dic){ /* nao tem palavra na skip list, criar uma! */
		p = STinsert(pal);
		p -> dicio = 0;
		p -> numOcor = 1;
		p -> ocor = malloc(sizeof(int*));
		p -> ocor[0] = malloc(2*sizeof(int));
		p -> ocor[0][0] = b; p -> ocor[0][1] = l;
	}
}

int leString(char s[], int max){
	/*  Função para ler os comandos de interação com o programa */
    int i = 0;
    char letra;
	
	
    for (i = 0; i < (max - 1); i++) {
		/*letra = fgetc(stdin);*/
		
		scanf("%c", &letra);/*
		printf("%i %c\n", letra, letra);*/
		if (letra == '\n' || letra == 0) break;
		s[i] = letra;
		/*if(letra == 0 && i == 0) return -1;*/
    }
	s[i] = 0;
	if (letra == 0) return -1;
	return i;
}

int leLivro(char *livro, int dic){

	/*	linha: variavel que indicará a linha do texto a ser copiado */
	/*  letra: variavel que será auxiliará na copia do testo        */
	
	
	int i = 0,k = 0, linha = 1, caracter = 0;
	char letra, *palavra;
	FILE *file;
	
	if(dic == -1) livro = livro+2;

	file = fopen(livro, "r"); /* Abre o arquivo */
	if (file == NULL) {
		/* Imprime menssagem de erro, caso o arquivo nao exista */
		printf("\tArquivo %s not found.\n", livro);
		return 1;
	}

	if(!dic){
		if(numLivros == 0) texto = malloc(sizeof(char**)); /* Caso nao ha nenhum livro salvo */
		else texto = realloc (texto, (numLivros+1)*sizeof(char**)); /* Caso contrario, realoca a memoria */

		texto[numLivros] = malloc(125000 * sizeof(char*)); /* Para cada livro aloca memoria para cada linha */	
		for(i = 0; i < 125000; ++i) texto[numLivros][i] = malloc(128*sizeof(char));
	}
	
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
			if(dic != -1) palavra[caracter++] = letra + 'a' - 'A';
			else palavra[caracter++] = letra;
		}
		else if(dic == -1 && (letra == '.' || letra == '-' || letra == '+' ||  letra == '_'))
			palavra[caracter++] = letra;
		else if(caracter > 0){
			/* caso nao achou uma letra, a palavra acabou */
			palavra[caracter] = 0;
			if(!dic) pegaPalavra(palavra, linha, numLivros, dic);
			else if(dic == 1) pegaPalavra(palavra, 0, 0, dic);
			else if(dic == -1) leLivro(palavra, 0);
			caracter = 0; 
		}
		
		if(!dic){
			if (letra == '\n'){ /* Muda de linha */
				texto[numLivros][linha][k] = 0;
				linha++; k = 0;
			}
			else texto[numLivros][linha][k++] = letra;
		}
	}
	fclose(file); /* Fecha o arquivo */
	
	if(!dic){
		texto[numLivros] = realloc(texto[numLivros], (linha+1) * sizeof(char*));
		linhaTexto[numLivros] = linha; /* Guarda o numero de linha que o n-simo texto possui */

	
		i = 0;
		do{
			/* Na linha 0, é colocada o nome do arquivo */
			texto[numLivros][0][i] = livro[i];
			i++;
		}while(livro[i-1] != 0);
	
		/*for(i = 0; i < linha+1; i++) printf("%i: %s\n", i, texto[numLivros][i]);*/
		numLivros++; 
	}
	
	free(palavra);
	return 0;
}

void menssagemErro(){
	/* Imprima uma menssagem de erro, caso o comando digitado nao exista */
	printf("\tCommand does not exit!\n");
}

void add(pilha x, contex a, int k){
	/* Adiciona ao topo da pilha, um novo contexto */
	if(x -> iniContex == NULL) x -> iniContex = a;
	else x -> finContex -> prox = a;
	a -> prox = NULL;
	x -> contextos += k;
	x -> finContex = a;
}

void addPilha(pilha n){
	/* Empilha uma nova celula */
	if(!tamPilha){
		n -> ant = n;
		n -> pos = n;
	}
	else{
		n -> pos = topoPilha -> pos;
		n -> ant = topoPilha;
		topoPilha -> pos = (topoPilha -> pos -> ant = n);
	}
	topoPilha = n;
	tamPilha++;
}

void STempilha(pont b){
	int i, j;
	
	pilha aux = malloc(sizeof(struct STpilha));
	contex auxC;

	aux -> iniContex = NULL;
	
	aux -> contextos = 0;
	for(i = 0; i < (b -> numOcor); i++){ /* Trata cada linha */
		for(j = 1; j <= l; ++j){ /* em cada contexto */
			/* Verifica se tal contexto exite */
			if(b -> ocor[i][1] - l + j <= 0 ||
			   b -> ocor[i][1] + j - 1 > linhaTexto[b -> ocor[i][0]])
				continue;
			
			/* se sim */
			/*aux -> contextos++;   esta na funcao add */
			
			/*Cria a nova celula para guardar cada contexto*/
			auxC = malloc(sizeof(struct STcontexto));
			auxC -> prox = NULL;
			
			/*faz toda a manipulacao dos ponteiros*/
			add(aux, auxC, 1);

			auxC -> text = b -> ocor[i][0];
			auxC -> inf = b -> ocor[i][1] - l + j;
			auxC -> sup = b -> ocor[i][1] + j - 1;
			
			auxC = NULL;
		}
	}
	addPilha(aux);
}

void empilhaUniverso(){
	int i, j;
	contex auxC;
	
	pilha aux = malloc(sizeof(struct STpilha));
	aux -> iniContex = NULL;
	aux -> contextos = 0;

	for(i = 0; i < numLivros; i++){
		if(linhaTexto[i] != 0){
			for(j = 1; j <= linhaTexto[i] - l + 1; j++){

				/*Cria a nova celula para guardar contexto */
				auxC = malloc(sizeof(struct STcontexto));
				auxC -> prox = NULL;
				
				/*faz toda a manipulacao dos ponteiros*/
				add(aux, auxC, 1);
				auxC -> text = i;
				auxC -> inf = j;
				auxC -> sup = j + l - 1;
				
				auxC = NULL;
			}
		}
	}
	
	addPilha(aux);
}

void operacaoConjuntos(int op){
	/*  Faz operacoes de uniao, intersecção e subtração dos dois ultimos conjuntos empilhado */
	/* op: 0 uniao      */
	/* op: 1 intersecao */
	/* op: 2 subtraçõa  */


	pilha s = topoPilha,
	      t = topoPilha -> ant,
	      n;
	contex sc = s -> iniContex,
	       tc = t -> iniContex,
	       aux1, aux2;
		
	if(!tamPilha) topoPilha = NULL;
	else{
		topoPilha -> pos -> ant = topoPilha -> ant -> ant;
		topoPilha -> ant -> ant -> pos = topoPilha -> pos;
		topoPilha = topoPilha -> ant -> ant;
	}     
	
	n = malloc(sizeof(struct STpilha));
	n -> contextos = 0;
	n -> iniContex = NULL;
	n -> finContex = NULL;
	
	/*printf("--%i--\n", op);*/
	
	while(!(!sc && !tc)){
		/*printf("# ");*/
		if(!sc){
			/*printf("4: NULL %i\n", tc -> inf);*/
			aux1 = tc; tc = tc -> prox;
			if(op == 0) add(n, aux1, 1);
			else if(op == 1) free(aux1);
			else if(op == 2) add(n, aux1, 1);
		}
		else if(!tc){
			/*printf("5: %i NULL\n", sc -> inf);*/
			aux1 = sc; sc = sc -> prox;
					
			if(op == 0) add(n, aux1, 1);
			else if(op == 1) free(aux1);
			else if(op == 2) free(aux1);
		}
		else if(sc -> text == tc -> text && sc -> inf == tc -> inf){
			/*printf("1: %i %i\n", sc -> inf, tc -> inf);*/
			aux1 = sc; aux2 = tc;
			sc = sc -> prox; tc = tc -> prox;
				
			if(op == 0) add(n, aux1, 1);
			else if(op == 1) add(n, aux1, 1);
			else if(op == 2) free(aux1);
			free(aux2);
		}
		else if(sc -> text < tc -> text || (sc -> inf < tc -> inf && sc -> text == tc -> text)){ 
			/*printf("2: %i %i\n", sc -> inf, tc -> inf);*/
			aux1 = sc; sc = sc -> prox;
					
			if(op == 0) add(n, aux1, 1);
			else if(op == 1) free(aux1);
			else if(op == 2) free(aux1);
		}
		else{ /* if(tc -> text < sc -> text || (tc -> inf < sc -> inf && tc -> text == sc -> text)) */
			/*printf("3: %i %i\n", sc -> inf, tc -> inf);*/
			aux1 = tc; tc = tc -> prox;
			
			if(op == 0) add(n, aux1, 1);
			else if(op == 1) free(aux1);
			else if(op == 2) add(n, aux1, 1);
		}
		aux2 = (aux1 = NULL);
	}
	
	free(s); free(t);
	addPilha(n);
}

void imprime(){
	int ultLinha = 0, ultTexto = -1, i;
	contex x;
	
	puts("");
	for(x = topoPilha -> iniContex; x; x = x -> prox){
		if(linhaTexto[x -> text] != 0){	
			if(x -> text != ultTexto){
				printf("%s:\n", texto[x -> text][0]);
				ultTexto = x -> text;
				ultLinha = 0;
			}
			for(i = x -> inf; i <= x -> sup; i++){
				if(i > ultLinha){
					printf("    %i: %s\n", i, texto[ultTexto][i]);
					ultLinha = i;
				}
			}
		}
	}
}

void imprimePalavras(){
	/* Imprime palavras que nao estao no dicionario */
	pont h;
	for(h = head -> next[0]; h; h = h -> next[0]){
		if(!h -> dicio) printf("\t%s\n", h -> pal);
	}
}


void excluiTexto(char *excl){
	int j , i, imprimeT = 1;
	pilha p;
	contex c, del;
	
	for(i = 0; i < numLivros; i++){
		if(comparaString(excl, texto[i][0]) == 0){
			linhaTexto[i] = 0;
			
			for(j = 0, p = topoPilha; j < tamPilha; j++, p = p -> ant){
			/* Para cada conteudo que esta empilhado, deleta todos   */
			/* os conjuntos pertencentes ao texto excluido           */
				for(c = p -> iniContex; c -> prox; ){
				/* Verifica se o contexto pertence ao texto excluido */
				/* se sim deleta! */
					if(c -> prox -> text == i){
						p -> contextos--;
						del = c -> prox;
						c -> prox = c -> prox -> prox;
						free(del);	
					}
					else c = c -> prox;
				}
				/* Verifica se o primeiro dos contexto pertence */
				if(p -> iniContex -> text == i){
					p -> contextos--;
					del = p -> iniContex;
					p -> iniContex = del -> prox;
					free(del);
				}
			}
		}
	}
}

int main (int argc, char* argv[])
{
	int i = 1, variavelB = 0, terminaProg = 0, imprimeT = 1;
	char *comando = malloc(32 * sizeof(char));
	struct STnode* aux = NULL;
	float time1, time2;
	pilha del;
	
	tamPilha = 0; 
	skipListInit();
	linhaTexto = malloc(64*sizeof(int));

	for(i = 1; i < argc; i++){
		/*printf("%i: %c\n", i, opcao);*/
		if(argv[i][0] == '-'){
			switch(argv[i][1]){
				case 'f':  leLivro(argv[i], -1); break;
				case 'b':  variavelB = 1;    break;
			}
		}
		else leLivro(argv[i], 0);
    }
	
	do{
		if(variavelB == 0) printf("ep3 > ");
		terminaProg  = leString(comando, 32);
		if(terminaProg == -1) break;
		time1 = (float) clock()/(float)(CLOCKS_PER_SEC);
		if(comando[0] == '.'){
		
			comando++;
			switch(comando[0]){
				case 'p': /* imprimir o conjunto que esta no topo da pilha */
					if(comando[1] != 0) menssagemErro();
					else{
						if(tamPilha > 0) imprime();
						else printf("\tThe stack is empty\n");
					}
				break;
				case 'P':
					if(comando[1] == 'W' && comando[2] == '\0') imprimePalavras();
					else menssagemErro();
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
					leLivro(++comando, 0);
				break;
				case '-': 
					excluiTexto(++comando);
				break;
				case 'W': /* Le o dicionario */
					leLivro(++comando, 1);
				break;
				case 'w': break;
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
						if(tamPilha == 0) printf("\tThe stack is empty\n");
						else printf("\tThe set top of the stack contains %i contexts of %i lines\n", topoPilha -> contextos, l);
					}
					else menssagemErro();
				break;
				case 'q':
					if(comando[1] == 0){
						imprimeT = imprimeT ? 0 : 1;
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
					tamPilha -= 2;
					operacaoConjuntos(0);
					time2 = (float) clock()/(float)(CLOCKS_PER_SEC);
					if(1) printf("O tempo gasto foi de %.3f segundos\n", (float) time2 - time1);
				}
				else printf("\tThe stack must have at least two sets!\n");
			}
		}
		else if(comando[0] == '-'){
			if(comando[1] != 0) menssagemErro();
			else{ /* Faz a intersecção das dos dois ultimos conjuntos */
				if(tamPilha >= 2){
					tamPilha -= 2;
					operacaoConjuntos(1);
				}
				else printf("\tThe stack must have at least two sets!\n");
			}
		}
		else if(comando[0] == '\\'){
			if(comando[1] != 0) menssagemErro();
			else{ /* S e T sao os ultimos dois conjuntos da pilha, S\T */
				if(tamPilha >= 2){
					tamPilha -= 2;
					operacaoConjuntos(2);
				}
				else printf("\tThe stack must have at least two sets!\n");
			}
		}
		else if(comando[0] == 0) continue;
		else{ /* programa deve calcular o conjunto S dos contextos em que <palavra> ocorre. */
			aux = STsearch(comando);
			if(aux){
				STempilha(aux);
			}
			else{
				printf("\tWord is not found, nothing was pilled\n");
			}
		}
		time2 = (float) clock()/(float)(CLOCKS_PER_SEC);
		if(imprimeT) printf("  (%.3f seconds)\n", (float) time2 - time1);
	}while(1);
	return 0;
}
