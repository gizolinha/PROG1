#include <stdio.h>
#include <stdlib.h>
#include "fprio.h"

//cria uma nova fila
struct fprio_t *fprio_cria () {
    //aloca memoria para fila
    struct fprio_t *nova_fila = malloc(sizeof(struct fprio_t));
    if (!nova_fila)
        return NULL;

    //inicializa as estruturas da fila
    nova_fila->prim = NULL;
    nova_fila->fim = NULL;
    nova_fila->num = 0;

    return nova_fila;
}

//libera todas as estruturas de dados da fila, inclusive os itens
struct fprio_t *fprio_destroi (struct fprio_t *f) {
    //se a fila ja estiver destruida retorna
    if (f == NULL)
        return NULL;
    
    struct fpnodo_t *atual = f->prim;
    struct fpnodo_t *prox;

    //libera os itens da lista
    while (atual != NULL) {
        prox = atual->prox;
        free(atual->item);
        free(atual);

        atual = prox;
    }
    //libera estrutura da fila
    free(f);
    return NULL;
}

// Insere o item na fila, mantendo-a ordenada por prioridades crescentes.
int fprio_insere (struct fprio_t *f, void *item, int tipo, int prio) {
    //fila ou item invalido
    if (!f || !item) 
        return -1;

    struct fpnodo_t *atual = f->prim;
    while (atual) {
        //item dupplicado
        if (atual->item == item)
            return -1;

        atual = atual->prox;
    }

    struct fpnodo_t *novo_item = malloc(sizeof(struct fpnodo_t));
    //confere alocação
    if (!novo_item) 
        return -1;

    novo_item->item = item;
    novo_item->tipo = tipo;
    novo_item->prio = prio;
    novo_item->prox = NULL;

    //insere no inicio se for prioridade maior
    if(!f->prim || f->prim->prio > prio) {
        novo_item->prox = f->prim;
        f->prim = novo_item;
    }
    else {
        struct fpnodo_t *atual = f->prim;
        while (atual->prox && (atual->prox->prio < prio || (atual->prox->prio == prio && atual->prox->tipo != tipo)))
            atual = atual->prox;

        novo_item->prox = atual->prox;
       atual->prox = novo_item;
    }
    //aumenta o tamanho da fila
    f->num++;
    return f->num;
}

// Retira o primeiro item da fila e o devolve; o tipo e a prioridade
void *fprio_retira (struct fprio_t *f, int *tipo, int *prio) {
    //confere se eh invalida ou vazia
    if (!f || f->num == 0)
        return NULL;

    //se tipo ou prio for nulo
    if (!tipo|| !prio)
        return NULL;
    
    struct fpnodo_t *item_remover = f->prim;

    void *item = item_remover->item;

    *tipo = item_remover->tipo;
    *prio = item_remover->prio;

    f->prim = item_remover->prox;
    free(item_remover);

    f->num--;
    return item;                                                                            
}

//informa o numero de itens na lista
int fprio_tamanho (struct fprio_t *f) {
    if (!f)
       return -1;
    
    return f->num;
}

// Imprime o conteúdo da fila no formato "(tipo prio) (tipo prio) ..."
void fprio_imprime(struct fprio_t *f) {
    if (!f || !f->prim) 
        return;  

    struct fpnodo_t *atual = f->prim;
    int primeiro = 1;

    while (atual) {    
        if (!primeiro) 
            printf(" ");
            
        printf("(%d %d)", atual->tipo, atual->prio);
        primeiro = 0;
        atual = atual->prox;
    }
}