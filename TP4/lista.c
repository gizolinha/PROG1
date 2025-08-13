#include <stdio.h>
#include <stdlib.h>
#include "lista.h" 
// Implementação com lista encadeada dupla não-circular

// define a estrutura interna do item
struct item_t {
  //valor do item
  int valor; 
  //ponteiros para os itens
  struct item_t *anterior; 
  struct item_t *proximo; 

} ;

// estrutura de uma lista
struct lista_t {
  //itens da lista
  struct item_t *primeiro; 
  struct item_t *ultimo; 
  int tam; 
} ;

// Cria uma lista vazia.
// Retorno: ponteiro p/ a lista ou NULL em erro.
struct lista_t *lista_cria () {
  //aloca memoria para a lista
  struct lista_t *nova_lista = malloc(sizeof(struct lista_t));
  if (!nova_lista)
      return NULL;

  //inicializa as estruturas da lista
  nova_lista->primeiro = NULL;
  nova_lista->ultimo = NULL;
  nova_lista->tam = 0;

  //retorna ponteiro para a nova lista
  return nova_lista;  
}

// Remove todos os itens da lista e libera a memória.
// Retorno: NULL.
struct lista_t *lista_destroi (struct lista_t *lst){
  //se a lista ja estiver destruida retorna
  if (lst == NULL)
    return NULL;

  struct item_t *atual = lst->primeiro;
  struct item_t *proximo;

  //libera os itens da lista 
  while (atual != NULL) {
    //o atual aponta para o prox item
    proximo = atual->proximo; 
    //libera o atual
    free(atual); 
    //vai para o prox item da lista
    atual = proximo;

  }
  //libera estrutura da lista
  free(lst); 
  return NULL;
}

// Insere o item na lista na posição indicada;
// se a posição for além do fim da lista ou for -1, insere no fim.
// Retorno: número de itens na lista após a operação ou -1 em erro.
int lista_insere (struct lista_t *lst, int item, int pos) {
  //confere se a lista existe
  if (!lst)
    return -1;

  //aloca memoria para o novo item a ser inserido
  struct item_t *novo_item = malloc(sizeof(struct item_t));
  if (!novo_item)
    return -1;

  //atribui o valor do item a ser inserido no ponteiro novo_item
  novo_item->valor = item;
  //nao apontam pra nada pois item nao foi inserido ainda
  novo_item->anterior = NULL;
  novo_item->proximo = NULL;

  //caso em que a lista está vazia
  if (lst->tam == 0) {
    //como tem apenas 1 item, os ponteiros apontam pra ele
    lst->primeiro = novo_item;
    lst->ultimo = novo_item;
  }

  //caso inserir na primeira posição
  else if (pos == 0) {
    novo_item->proximo = lst->primeiro;
    lst->primeiro->anterior = novo_item;
    lst->primeiro = novo_item; //atualiza o ponteiro para o novo inicio
  }

  //inserir no final ou posição alem do fim da lista
  else { 
      if (pos == -1 || pos >= lst->tam) { 
        novo_item->anterior = lst->ultimo;
        lst->ultimo->proximo = novo_item;
        lst->ultimo = novo_item;
      }
      //inserir em uma posição específica
      else {
        //cria ponteiro que começa apontando para o primeiro item
        struct item_t *atual = lst->primeiro; 
        //percorre ate a posição desejada
        for (int i = 0; i < pos; i++) //NAO SEI SE ESSE &&& A CERTO
          atual = atual->proximo;

        novo_item->proximo = atual;
        novo_item->anterior = atual->anterior;

        if(atual->anterior) //se atual aponta para um item anterior
          atual->anterior->proximo = novo_item;

        atual->anterior = novo_item;
        //garantia caso tam = 0 mas pos > 0
        if (pos == 0)
        lst->primeiro = novo_item;
      }
  }

  lst->tam++; //atualiza o tamanho da lista 
  return lst->tam; // retorna quantos itens há na lista
}

// Retira o item da lista da posição indicada.
// se a posição for além do fim da lista ou for -1, retira do fim.
// Retorno: número de itens na lista após a operação ou -1 em erro.
int lista_retira (struct lista_t *lst, int *item, int pos) {
  if (!lst || lst->tam == 0 || !item)
    return -1;

  //ponteiro pro item a ser retirado
  struct item_t *item_retira;

  // se a posição for -1 ou maior ou igual ao tamanho, retira do fim
  if (pos == -1 || pos >= lst->tam - 1) { 
    item_retira = lst->ultimo;
    *item = item_retira->valor;

    //atualiza o novo ultimo itme pós remoção
    lst->ultimo = item_retira->anterior;

    //se tiver um novo ultimo item 
    if (lst->ultimo)
      lst->ultimo->proximo = NULL;
    else
      //se a lista ficar vazia
      lst->primeiro = NULL;
  }

  //retira de uma posição específica
  else {
    item_retira = lst->primeiro;

    for (int i = 0; i < pos; i++) 
      item_retira = item_retira->proximo;

    *item = item_retira->valor;

    //se houver um item anterior a item_retira
    if (item_retira->anterior)
      item_retira->anterior->proximo = item_retira->proximo;
    else
    //caso o item retirado seja o primeiro da lista 
      lst->primeiro = item_retira->proximo;
    
    //se houver um proximo item a item_retira
    if (item_retira->proximo)
      item_retira->proximo->anterior = item_retira->anterior;
    else
      //caso o item retirado seja o ultimo da lista
      lst->ultimo = item_retira->anterior;
  }
  //libera memoria do item removido
  free(item_retira);
  //diminui o tamanho da lista
  lst->tam--;
  return lst->tam;
}

// Informa o valor do item na posição indicada, sem retirá-lo.
// se a posição for além do fim da lista ou for -1, consulta do fim.
// Retorno: número de itens na lista ou -1 em erro.
int lista_consulta (struct lista_t *lst, int *item, int pos) {
  if(lst == NULL || item == NULL)
    return -1;

  //se a posicao for -1, consulta o fim da lista
  if (pos == -1) {
    if (lst->tam == 0) // se a lista estiver vazia
      return -1;

    *item = lst->ultimo->valor;
    return lst->tam;
  }

  //se a posição estiver fora d inytervalo
  if (pos < 0 || pos >= lst->tam )
    return -1;

  //percorre a lista ate a posição desejada
  struct item_t *atual = lst->primeiro;
  for (int i = 0; i < pos; i++)
      atual = atual->proximo;

  //acessa o valor e retorna o numero de itens
  *item = atual->valor;
  return lst->tam;

}



// Informa a posição da 1ª ocorrência do valor indicado na lista.
// Retorno: posição do valor ou -1 se não encontrar ou erro.
int lista_procura (struct lista_t *lst, int valor) {
  if (!lst)
    return -1;

  struct item_t *atual = lst->primeiro;
    //percorre a lista procurando o valor
  for (int pos = 0; atual != NULL; pos++) {
    if(atual->valor == valor)
      return pos;

    //vai pro prox valor
    atual = atual->proximo;
  } 
    //se não achar retorna -1
    return -1;
}

// Informa o tamanho da lista (o número de itens presentes nela).
// Retorno: número de itens na lista ou -1 em erro.
int lista_tamanho (struct lista_t *lst) {
  if (!lst)
    return -1;

  return lst->tam;
}

// Imprime o conteúdo da lista do inicio ao fim no formato "item item ...",
// com um espaço entre itens, sem espaços antes/depois, sem newline.
void lista_imprime (struct lista_t *lst) {
  //confere se eh invalida ou vazia
  if (!lst || lst->tam == 0)
    return;
  
  //cria ponteiro que começa apontando para o primeiro item
  struct item_t *atual = lst->primeiro;  

  while (atual) {
    printf("%d", atual->valor); //imprime o atual

    atual = atual->proximo;
    //se tiver proximo numero imprime espaço pro prox valor
    if (atual)
      printf(" ");
  }
}