#include <stdio.h>
#include <stdlib.h>
#include "fprio.h"


// descreve uma fila de prioridades
struct fprio_t {
    struct fpnodo_t *primeiro;
    struct fpnodo_t *ultimo;
    int tam;
};

// descreve um nodo da fila de prioridades
struct fpnodo_t {  
    struct fpnodo_t *proximo;
    void *item;
    int prio;
    int tipo;
};


// Cria uma fila vazia.
// Retorno: ponteiro para a fila criada ou NULL se erro.
struct fprio_t *fprio_cria () {
    //aloca memoria para fila
    struct fprio_t *nova_fila = malloc(sizeof(struct fprio_t));
    if (!nova_fila)
        return NULL;

    //inicializa as estruturas da fila
    nova_fila->primeiro = NULL;
    nova_fila->ultimo = NULL;
    nova_fila->tam = 0;

    return nova_fila;
}

// Libera todas as estruturas de dados da fila, inclusive os itens.
// Retorno: NULL.
struct fprio_t *fprio_destroi (struct fprio_t *f) {
    //se a fila ja estiver destruida retorna
    if (f == NULL)
        return NULL;
    
    struct fpnodo_t *atual = f->primeiro;
    struct fpnodo_t *proximo;

    //libera os itens da lista
    while (atual != NULL) {
        proximo = atual->proximo;
        free(atual->item);
        free(atual);

        atual = proximo;
    }
    //libera estrutura da fila
    free(f);
    return NULL;
}

// Insere o item na fila, mantendo-a ordenada por prioridades crescentes.
// Itens com a mesma prioridade devem respeitar a politica FIFO (retirar
// na ordem em que inseriu).
// Inserir duas vezes o mesmo item (o mesmo ponteiro) é um erro.
// Retorno: número de itens na fila após a operação ou -1 se erro.
int fprio_insere (struct fprio_t *f, void *item, int tipo, int prio) {
    //fila ou item invalido
    if (!f || !item) 
        return -1;

    struct fpnodo_t *atual = f->primeiro;
    while (atual) {
        //item dupplicado
        if (atual->item == item)
            return -1;

        atual = atual->proximo;
    }

    struct fpnodo_t *novo_item = malloc(sizeof(struct fpnodo_t));
    //confere alocação
    if (!novo_item) 
        return -1;

    novo_item->item = item;
    novo_item->tipo = tipo;
    novo_item->prio = prio;
    novo_item->proximo = NULL;

    //insere no inicio se for prioridade maior
    if(!f->primeiro || f->primeiro->prio > prio) {
        novo_item->proximo = f->primeiro;
        f->primeiro = novo_item;
    }
    else {
        struct fpnodo_t *atual = f->primeiro;
        while (atual->proximo && (atual->proximo->prio < prio || (atual->proximo->prio == prio && atual->proximo->tipo != tipo)))
            atual = atual->proximo;

        novo_item->proximo = atual->proximo;
       atual->proximo = novo_item;
    }
    //aumenta o tamanho da fila
    f->tam++;
    return f->tam;
}

// Retira o primeiro item da fila e o devolve; o tipo e a prioridade
// do item são devolvidos nos parâmetros "tipo" e "prio".
// Retorno: ponteiro para o item retirado ou NULL se fila vazia ou erro.
void *fprio_retira (struct fprio_t *f, int *tipo, int *prio) {
    //confere se eh invalida ou vazia
    if (!f || f->tam == 0)
        return NULL;

    //se tipo ou prio for nulo
    if (!tipo|| !prio)
        return NULL;
    
    struct fpnodo_t *item_remover = f->primeiro;

    void *item = item_remover->item;

    *tipo = item_remover->tipo;
    *prio = item_remover->prio;

    f->primeiro = item_remover->proximo;
    free(item_remover);

    f->tam--;
    return item;
                                                                                    
}

// Informa o número de itens na fila.
// Retorno: N >= 0 ou -1 se erro.
int fprio_tamanho (struct fprio_t *f) {
    if (!f)
       return -1;
    
    return f->tam;
}

// Imprime o conteúdo da fila no formato "(tipo prio) (tipo prio) ..."
// Para cada item deve ser impresso seu tipo e sua prioridade, com um
// espaço entre valores, sem espaços antes ou depois e sem nova linha.
void fprio_imprime(struct fprio_t *f) {
    if (!f || !f->primeiro) 
        return;  

    struct fpnodo_t *atual = f->primeiro;
    int prim = 1;

    while (atual) {    
        if (!prim) 
            printf(" ");
            
        printf("(%d %d)", atual->tipo, atual->prio);
        prim = 0;
        atual = atual->proximo;
    }
}