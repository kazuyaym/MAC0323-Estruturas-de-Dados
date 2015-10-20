#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <limits.h>

/*----------------------------------------Struct----------------------------------------*/
/* struct que ser� a base da minha lista ligada */
struct no {
    char *palavra;
    struct no *next;
    int qtd;
};
typedef struct no node;


/*---------------------------------------Mergesort----------------------------------------*/

/* parte do mergesort que ordena em ordem alfab�tica */
node *merge_alfabetica (node *a, node* b){
    struct no cabeca; /* crio uma cabe�a para organizar a lista que ser� formada */
    node *c = &cabeca; /* crio um apontador para referenciar a cabe�a */
    while(a && b){ /* loop que continua at� que a lista ligada a ou b termine */
        if( compara_palavra(a->palavra, b->palavra) < 0 ){ /* fa�o a compara��o entre as palavras */
            /* organizo a lista */
            c->next = a;
            c = a;
            a = a->next;
        }
        else{
            c->next = b;
            c = b;
            b = b->next;
        }
    }
    c->next = ( a == NULL ) ? b : a; /* dado que uma das listas acabou, uno o fim da outra a lista ordenada */
    return cabeca.next;
}

/* parte do mergesort que faz a divis�o da lista */
node *lmergesort_alfabetica (node *c){
    node *a, *b; /*crio dois apontadores, um para marcar o inicio da lista e outro para percorrer a lista, respectivamente */
    if(!c->next) /* se a lista possui apenas uma c�lula, sai da fun��o (condi��o do fim da recurs�o) */
        return c;
    a = c;
    b = c->next;
    while(b && b->next){ /* percorro at� achar o meio da lista */
        c = c->next;
        b = b->next->next;
    }
    /* reparto a lista em duas */
    b = c->next;
    c->next = NULL;
    return merge_alfabetica(lmergesort_alfabetica(a), lmergesort_alfabetica(b)); /* continuo a recurs�o */
}



/* parte do mergesort que ordena em ordem num�rica */
node *merge_numerico (node *a, node* b){
    struct no cabeca; /* crio uma cabe�a para organizar a lista que ser� formada */
    node *c = &cabeca; /* crio um apontador para referenciar a cabe�a */
    while(a && b){ /* loop que continua at� que a lista ligada a ou b termine */
        if( a->qtd < b->qtd ){ /* fa�o a compara��o entre as quantidades */
            /* organizo a lista */
            c->next = b;
            c = b;
            b = b->next;
        }
        else{
            c->next = a;
            c = a;
            a = a->next;
        }
    }
    c->next = ( a == NULL ) ? b : a; /* dado que uma das listas acabou, uno o fim da outra a lista ordenada */
    return cabeca.next;
}

/* parte do mergesort que faz a divis�o da lista */
node *lmergesort_numerico (node *c){
    node *a, *b; /*crio dois apontadores, um para marcar o inicio da lista e outro para percorrer a lista, respectivamente */
    if(!c->next) /* se a lista possui apenas uma c�lula, sai da fun��o (condi��o do fim da recurs�o) */
        return c;
    a = c;
    b = c->next;
    while(b && b->next){ /* percorro at� achar o meio da lista */
        c = c->next;
        b = b->next->next;
    }
    /* reparto a lista em duas */
    b = c->next;
    c->next = NULL;
    return merge_numerico(lmergesort_numerico(a), lmergesort_numerico(b)); /* continuo a recurs�o */
}





/*---------------------------------------Fun��es----------------------------------------*/

/* fun��o que libera todos os apontadores de uma dada lista */
void libera_lista(node *t){
    node *x; /*crio um novo apontador para percorrer a lista */
    x = t; /* ele recebe o come�o da lista */
    while(t){ /* enquanto t n�o for nulo, libero uma c�lula */
        t = t->next;
        free(x);
        x = t;
    }
    free(x); /*ap�s liberar a lista, libero x */
}

/* fun��o que verifica se h� alguma pontua��o (menos h�fen) no come�o, no meio e no final na palavra */
void verifica_palavra (char *a, int s){
    int i, j; /* crio contadores */
    for(i = 0; a[i] != '\0'; i++){ /* percorro toda a string */
        if(s == 1)
            a[i] = (char)tolower(a[i]);
        if(ispunct(a[i]) && a[i] != 45){ /* verifico se � uma pontua��o e n�o � um h�fen */
            for(j = i; a[j] != '\0'; j++) /* passo todos os valores uma posi��o para tr�s */
                a[j] = a[j+1];
            i--; /* diminuo o contador i pois a sua "pr�xima" posi��o voltou uma posi��o */
        }
    }
}

/* fun��o que imprime a lista */
void imprime_lista (node *x, int i){
    node *p; /* crio um novo apontador para percorrer a lista */
    int j;
    if (i == 0) /* caso o par�metro 'i', que � a op��o de impress�o m�xima for 0, ou seja , n�o foi dado nenhum par�metro, imprimo a lista toda */
        i = INT_MAX;
    /* percorro a lista e imprimo os valores da palavra */
    for (p = x->next, j = 0; p && j < i ; p = p->next, j++)
        printf ("%s\t ---\t %d\n", p->palavra, p->qtd);
}

/* fun��o que cria novos nodes, passa o valor do texto e inclui na lista */
void insere_lista (char *x, node *p){
    node *nova; /* cria novo node para inserir na lista */
    /* verifica��o se possue espa�o dispon�vel para a cria��o de um novo node e um espa�o para string no node*/
    if( (nova = malloc (sizeof (node))) == NULL){
        printf("Mem�ria insuficiente!");
        return ;
    }
    if( (nova->palavra = malloc(20 * sizeof(char))) == NULL){
        printf("Mem�ria insuficiente!");
        return ;
    }
    nova->qtd = 1; /* inicializo a quantidade com 0 */
    /* copio com o strcpy (pois sem ele, apenas atribuindo valores, n�o d� certo!! perdi 4 hrs para descobrir!!)a palavra para o node */
    nova->palavra = strcpy(nova->palavra, x);
    /* insiro o novo node no come�o da lista */
    nova->next = p->next;
    p->next = nova;
}

/* fun��o que compara duas strings, posso dizer que a �nica coisa que difere do strcmp � que esse trata as mai�culas diferente do strcmp */
int compara_palavra(char *a, char *b){
    char *c, *d; /*crio duas strings para receberem os valores das strings recebidas como par�metros */
    int i; /* crio um contador */
    /*aloco espa�o para as strings e passo os valores das strings recebidas como par�metro */
    c = malloc(20 * sizeof(char));
    d = malloc(20 * sizeof(char));
    c = strcpy(c, a);
    d = strcpy(d, b);
    /* passo as duas strings para min�sculo */
    for (i = 0; c[i] != '\0'; i++)
        c[i] = (char)tolower(c[i]);
    for (i = 0; d[i] != '\0'; i++)
        d[i] = (char)tolower(d[i]);
    /* fa�o uma compara��o mais tratada */
    if(strcmp(c, d) < 0)
        return -1;
    else
        if(strcmp(c, d) > 0)
            return 1;
        else{ /* �nico caso para prestar mais aten��o � quando as strings s�o iguais dai levo em considera��o o primeiro char das strins recebidas como parametro */
            if(strcmp(a, b) < 0)
                return -1;
            else
                return 1;
        }
}

/*fun��o que contabiliza a frequ�ncia de cada palavra na lista ligada e remove as repetidas */
void contabiliza_strings(node *x){
    node *a, *p; /*crio dois apontadores para percorrerem a lista */

    for(a = x->next; a->next ; a = a->next){ /*la�o que percorrer� toda a lista */
        if(strcmp(a->palavra, a->next->palavra) == 0){ /* condi��o que verifica se h� mais de uma ocorr�ncia de uma palavra */
            while(strcmp(a->palavra, a->next->palavra) == 0){ /*la�o que continua a verifica��o para mais de duas ocorr�ncias */
                a->qtd++; /*aumenta a quantidade */
                /* remove uma c�lula � mais */
                p = a->next;
                if(a->next->next)
                    a->next = a->next->next;
                else{
                    a->next = NULL;
                    return;
                }
                free(p);
            }
        }
    }
}





/*-----------------------------------------Main-----------------------------------------*/

/* fun��o principal do programa */
int main (int argc, char *argv[]){
    char *texto, opcao; /* crio uma string para armazenar cada palavra do texto */
    node *head; /* crio um apontador para marcar o come�o da lista */
    int qtd_impressos = 0, sensitive = 0;

    /* la�o que verifica se h� algum par�metro enviado na chamada do programa */
    while((opcao = getopt(argc, argv, "n:I")) != -1){
        switch(opcao){
            case 'I': /* caso o insensitive case for ativado */
                sensitive = 1;
                break;
            case 'n': /* caso h� um limite nas impress�es */
                qtd_impressos = atoi(optarg);
                break;
        }
    }

    texto = malloc (20 * sizeof(char)); /* aloco espa�o para o texto */
    head = malloc (sizeof(node)); /* aloco espa�o para o node */
    head->qtd = 0; /* inicializo o valor da vari�vel com 0 */
    head->next = NULL; /* o next do node aponta para o nulo */

    printf("\n\tEXERCICIO PROGRAMA 1 - MAC0323 ESTRUTURA DE DADOS\n\n\n");
    /* loop que ler� uma palavra por vez */
    while (scanf("%s", texto) != EOF){
        verifica_palavra(texto, sensitive); /* fa�o a verifica��o da pontua��o */
        insere_lista(texto, head); /* insiro na lista */
    }

    head->next = lmergesort_alfabetica(head->next); /* fa�o o primeiro mergesort para ordenar em ordem alfab�tica */
    contabiliza_strings(head); /* fa�o a contagem das strings */
    head->next = lmergesort_numerico(head->next); /* fa�o o segundo mergesort para ordenar em ordem num�rica */
    imprime_lista(head, qtd_impressos); /*imprimo a lista final */
    libera_lista(head);/* libero a lista */
    /* libero a minha c�lula usada como cabe�a */
    free(head);

    return 0;
}
