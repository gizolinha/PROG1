#include <stdio.h>
#include <stdlib.h>
#include "fila.h"


// Cria uma fila vazia.
// Retorno: ponteiro para a fila criada ou NULL se erro.
struct fila_t *fila_cria () {
    //aloca memoria para nova fila
    struct fila_t *nova_fila = malloc(sizeof(struct fila_t));
    if(!nova_fila)
        return NULL;

    //inicializa estruturas da fila
    nova_fila->prim = NULL;
    nova_fila->fim = NULL;
    nova_fila->num = 0;

    return nova_fila;
}

// Libera todas as estruturas de dados da fila, inclusive os itens.
// Retorno: NULL.
struct fila_t *fila_destroi (struct fila_t *f) {
    //se a fila ja estiver vazia
    if (f == NULL)
        return NULL;

    struct fila_nodo_t *atual = f->prim;
    struct fila_nodo_t *prox;

    //libera itens da lista
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

// Insere o item na fila
// Inserir duas vezes o mesmo item (o mesmo ponteiro) é um erro.
// Retorno: número de itens na fila após a operação ou -1 se erro.
int fila_insere (struct fila_t *f, void *item) {
    //verifica se a fila ou item sao invalidos
    if (!f || !item)
        return -1;

    //verifica se o item ja existe
    struct fila_nodo_t *atual = f->prim;
    while (atual) {
        if (atual->item == item)
            return -1;
        atual = atual->prox;
    }

    //cria novo item
    struct fila_nodo_t *novo_item = malloc(sizeof(struct fila_nodo_t));
    if (!novo_item)
        return -1;

    novo_item->item = item;
    novo_item->prox = NULL;

    //se a fila estiver vazia, insere no inicio
    if (!f->prim)
        f->prim = novo_item;
    else {
        //caso contrario, encontra o ultimo nodo da fila e insere
        struct fila_nodo_t *atual = f->prim;
        //enquanto tiver prox roda o laço
        while (atual->prox) 
            atual = atual->prox;

        atual->prox = novo_item;
    }
    //aumenta o tamanho da fila e retorna novo tamanho
    f->num++;
    return f->num;
}

// Retira o primeiro item da fila e o devolve
// Retorno: ponteiro para o item retirado ou NULL se fila vazia ou erro.
void *fila_retira (struct fila_t *f) {
    //se lista vazia ou invalida
    if (!f || f->num == 0)
        return NULL;

    struct fila_nodo_t *item_remover = f->prim;

    void *item = item_remover->item;

    //atualiza o novo primeiro item da fila pos remocao
    f->prim = item_remover->prox;
    free(item_remover);

    f->num--;
    return item;
}

// Informa o número de itens na fila.
// Retorno: N >= 0 ou -1 se erro.
int fila_tamanho (struct fila_t *f) {
    if (!f)
        return -1;

    return f->num;
}

// Imprime o conteúdo da fila 
void fila_imprime (struct fila_t *f) {
    if (!f || !f->prim)
        return;
    
    struct fila_nodo_t *atual = f->prim;

    while (atual) {
        // Desreferenciando para imprimir o valor inteiro
        printf("%d ", *(int *)atual->item);  
        atual = atual->prox;
    }
}
