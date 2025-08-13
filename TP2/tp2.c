#include <stdio.h>
#include <stdlib.h>
#include "racional.h"

#define MAX_SIZE 100

//le um valor n (tamanho do vetor) tal que 0 < n < 100, garante o numero dentro do intervalo
int le_tam_vetor(int tam) {

 int fim = 1;

 do {
      scanf("%d", &tam);
      if (tam > 0 && tam < 100)
        fim = 0;
 } while (fim);

 return tam;

}

//preenche o vetor com n numeros racionais lidos na entrada
void preenche_vetor(struct racional v[], int tam) { 

    int num, den;

    for (int i = 0; i < tam; i++) {

      scanf("%d %d", &num, &den);
      v[i] = cria_r(num, den);

    }

}

void ordena_vet(struct racional v[], int tam) {

    //verifica se ha denominador zero no vetor
    for (int j = 0; j < tam; j++) {
      if (v[j].den == 0)
        return;
    }

    //insertion sort
    for (int j = 1; j < tam; j++) { //talvez de algo aqui por causo do <=
      struct racional chave = v[j]; //acha a chave

      int i = j - 1;

      //passa os elementos maiores que a chave para frente
      while (i >= 0 && compara_r(v[i], chave) > 0) {
        v[i + 1] = v[i]; //move i para frente
        i--;
      }

      //coloca a chave na posição correta
      v[i + 1] = chave;

    }
    
}

// função para eliminar racionais inválidos de um vetor e passar pra posicao do NaN
void elimina_elemento(struct racional v[], int *tam) {

    for (int i = 0; i < (*tam); i++) {
      if (!valido_r(v[i])) {
        v[i] = v[(*tam) - 1];
        (*tam)--;
        i--;
    } 
  }
}

// Função para somar todos os racionais em um vetor usando soma_r
struct racional soma_total_racionais(struct racional v[], int tam) {

    struct racional soma = {0, 1}; //inicializa com zero
    struct racional temp;

    for (int i = 0; i < tam; i++) {
      if (!valido_r(v[i])) 
        return soma; // se o racional racional for inválido, retorna 0

      if (soma_r(soma, v[i], &temp))
        soma = temp; //atualiza
    }
    return soma;

}


int main () {

  struct racional v_racionais[MAX_SIZE]; 
  struct racional soma;
  int n = 0, i;

  n = le_tam_vetor(n);
  preenche_vetor(v_racionais, n);

  //imprime o vetor de racionais
  printf("VETOR = ");
  for (i = 0; i < n; i++) {
    imprime_r(v_racionais[i]); 
    printf(" "); 

  } printf("\n");


  //elimina os NaNs e imprime
  elimina_elemento(v_racionais, &n);

  printf("VETOR = ");
  for (i = 0; i < n; i++) {
    imprime_r(v_racionais[i]);
    printf(" "); 

  } printf("\n"); 

  //ordena o vetor e imprime
  ordena_vet(v_racionais, n);

  printf("VETOR = ");
  for (i = 0; i < n; i++) {
    imprime_r(v_racionais[i]);
    printf(" "); 
  
  } printf("\n"); 

  //soma os racionais do vetor e imprime
  soma = soma_total_racionais(v_racionais, n);
  
  printf("SOMA = ");
  imprime_r(soma);
  printf("\n");
 
  return (0);

}
