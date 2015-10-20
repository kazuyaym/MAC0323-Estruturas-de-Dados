#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <limits.h>

/*----------------------------------------Struct----------------------------------------*/
/* struct que será a base da minha lista ligada */
struct no {
    char *palavra;
    struct no *next;
    int qtd;
};
typedef struct no node;


/*---------------------------------------Mergesort----------------------------------------*/

/* parte do mergesort que ordena em ordem alfabética */
node *merge_alfabetica (node *a, node* b){
    struct no cabeca; /* crio uma cabeça para organizar a lista que será formada */
    node *c = &cabeca; /* crio um apontador para referenciar a cabeça */
    while(a && b){ /* loop que continua até que a lista ligada a ou b termine */
        if( compara_palavra(a->palavra, b->palavra) < 0 ){ /* faço a comparação entre as palavras */
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

/* parte do mergesort que faz a divisão da lista */
node *lmergesort_alfabetica (node *c){
    node *a, *b; /*crio dois apontadores, um para marcar o inicio da lista e outro para percorrer a lista, respectivamente */
    if(!c->next) /* se a lista possui apenas uma célula, sai da função (condição do fim da recursão) */
        return c;
    a = c;
    b = c->next;
    while(b && b->next){ /* percorro até achar o meio da lista */
        c = c->next;
        b = b->next->next;
    }
    /* reparto a lista em duas */
    b = c->next;
    c->next = NULL;
    return merge_alfabetica(lmergesort_alfabetica(a), lmergesort_alfabetica(b)); /* continuo a recursão */
}



/* parte do mergesort que ordena em ordem numérica */
node *merge_numerico (node *a, node* b){
    struct no cabeca; /* crio uma cabeça para organizar a lista que será formada */
    node *c = &cabeca; /* crio um apontador para referenciar a cabeça */
    while(a && b){ /* loop que continua até que a lista ligada a ou b termine */
        if( a->qtd < b->qtd ){ /* faço a comparação entre as quantidades */
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

/* parte do mergesort que faz a divisão da lista */
node *lmergesort_numerico (node *c){
    node *a, *b; /*crio dois apontadores, um para marcar o inicio da lista e outro para percorrer a lista, respectivamente */
    if(!c->next) /* se a lista possui apenas uma célula, sai da função (condição do fim da recursão) */
        return c;
    a = c;
    b = c->next;
    while(b && b->next){ /* percorro até achar o meio da lista */
        c = c->next;
        b = b->next->next;
    }
    /* reparto a lista em duas */
    b = c->next;
    c->next = NULL;
    return merge_numerico(lmergesort_numerico(a), lmergesort_numerico(b)); /* continuo a recursão */
}





/*---------------------------------------Funções----------------------------------------*/

/* função que libera todos os apontadores de uma dada lista */
void libera_lista(node *t){
    node *x; /*crio um novo apontador para percorrer a lista */
    x = t; /* ele recebe o começo da lista */
    while(t){ /* enquanto t não for nulo, libero uma célula */
        t = t->next;
        free(x);
        x = t;
    }
    free(x); /*após liberar a lista, libero x */
}

/* função que verifica se há alguma pontuação (menos hífen) no começo, no meio e no final na palavra */
void verifica_palavra (char *a, int s){
    int i, j; /* crio contadores */
    for(i = 0; a[i] != '\0'; i++){ /* percorro toda a string */
        if(s == 1)
            a[i] = (char)tolower(a[i]);
        if(ispunct(a[i]) && a[i] != 45){ /* verifico se é uma pontuação e não é um hífen */
            for(j = i; a[j] != '\0'; j++) /* passo todos os valores uma posição para trás */
                a[j] = a[j+1];
            i--; /* diminuo o contador i pois a sua "próxima" posição voltou uma posição */
        }
    }
}

/* função que imprime a lista */
void imprime_lista (node *x, int i){
    node *p; /* crio um novo apontador para percorrer a lista */
    int j;
    if (i == 0) /* caso o parâmetro 'i', que é a opção de impressão máxima for 0, ou seja , não foi dado nenhum parâmetro, imprimo a lista toda */
        i = INT_MAX;
    /* percorro a lista e imprimo os valores da palavra */
    for (p = x->next, j = 0; p && j < i ; p = p->next, j++)
        printf ("%s\t ---\t %d\n", p->palavra, p->qtd);
}

/* função que cria novos nodes, passa o valor do texto e inclui na lista */
void insere_lista (char *x, node *p){
    node *nova; /* cria novo node para inserir na lista */
    /* verificação se possue espaço disponível para a criação de um novo node e um espaço para string no node*/
    if( (nova = malloc (sizeof (node))) == NULL){
        printf("Memória insuficiente!");
        return ;
    }
    if( (nova->palavra = malloc(20 * sizeof(char))) == NULL){
        printf("Memória insuficiente!");
        return ;
    }
    nova->qtd = 1; /* inicializo a quantidade com 0 */
    /* copio com o strcpy (pois sem ele, apenas atribuindo valores, não dá certo!! perdi 4 hrs para descobrir!!)a palavra para o node */
    nova->palavra = strcpy(nova->palavra, x);
    /* insiro o novo node no começo da lista */
    nova->next = p->next;
    p->next = nova;
}

/* função que compara duas strings, posso dizer que a única coisa que difere do strcmp é que esse trata as maiúculas diferente do strcmp */
int compara_palavra(char *a, char *b){
    char *c, *d; /*crio duas strings para receberem os valores das strings recebidas como parâmetros */
    int i; /* crio um contador */
    /*aloco espaço para as strings e passo os valores das strings recebidas como parâmetro */
    c = malloc(20 * sizeof(char));
    d = malloc(20 * sizeof(char));
    c = strcpy(c, a);
    d = strcpy(d, b);
    /* passo as duas strings para minúsculo */
    for (i = 0; c[i] != '\0'; i++)
        c[i] = (char)tolower(c[i]);
    for (i = 0; d[i] != '\0'; i++)
        d[i] = (char)tolower(d[i]);
    /* faço uma comparação mais tratada */
    if(strcmp(c, d) < 0)
        return -1;
    else
        if(strcmp(c, d) > 0)
            return 1;
        else{ /* único caso para prestar mais atenção é quando as strings são iguais dai levo em consideração o primeiro char das strins recebidas como parametro */
            if(strcmp(a, b) < 0)
                return -1;
            else
                return 1;
        }
}

/*função que contabiliza a frequência de cada palavra na lista ligada e remove as repetidas */
void contabiliza_strings(node *x){
    node *a, *p; /*crio dois apontadores para percorrerem a lista */

    for(a = x->next; a->next ; a = a->next){ /*laço que percorrerá toda a lista */
        if(strcmp(a->palavra, a->next->palavra) == 0){ /* condição que verifica se há mais de uma ocorrência de uma palavra */
            while(strcmp(a->palavra, a->next->palavra) == 0){ /*laço que continua a verificação para mais de duas ocorrências */
                a->qtd++; /*aumenta a quantidade */
                /* remove uma célula à mais */
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

/* função principal do programa */
int main (int argc, char *argv[]){
    char *texto, opcao; /* crio uma string para armazenar cada palavra do texto */
    node *head; /* crio um apontador para marcar o começo da lista */
    int qtd_impressos = 0, sensitive = 0;

    /* laço que verifica se há algum parâmetro enviado na chamada do programa */
    while((opcao = getopt(argc, argv, "n:I")) != -1){
        switch(opcao){
            case 'I': /* caso o insensitive case for ativado */
                sensitive = 1;
                break;
            case 'n': /* caso há um limite nas impressões */
                qtd_impressos = atoi(optarg);
                break;
        }
    }

    texto = malloc (20 * sizeof(char)); /* aloco espaço para o texto */
    head = malloc (sizeof(node)); /* aloco espaço para o node */
    head->qtd = 0; /* inicializo o valor da variável com 0 */
    head->next = NULL; /* o next do node aponta para o nulo */

    printf("\n\tEXERCICIO PROGRAMA 1 - MAC0323 ESTRUTURA DE DADOS\n\n\n");
    /* loop que lerá uma palavra por vez */
    while (scanf("%s", texto) != EOF){
        verifica_palavra(texto, sensitive); /* faço a verificação da pontuação */
        insere_lista(texto, head); /* insiro na lista */
    }

    head->next = lmergesort_alfabetica(head->next); /* faço o primeiro mergesort para ordenar em ordem alfabética */
    contabiliza_strings(head); /* faço a contagem das strings */
    head->next = lmergesort_numerico(head->next); /* faço o segundo mergesort para ordenar em ordem numérica */
    imprime_lista(head, qtd_impressos); /*imprimo a lista final */
    libera_lista(head);/* libero a lista */
    /* libero a minha célula usada como cabeça */
    free(head);

    return 0;
}
