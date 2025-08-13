#include <stdio.h>
#include <stdlib.h>
#include "racional.h"

//função para ordenar o vetor por insertion sort
void ordena_vetor(struct racional *v[], int tam) {
  
  int i;
  
  //verifica se ha denominador zero no vetor
  for (int j = 0; j < tam; j++) 
    if(v[j]->den == 0)
       return;

  //insertion sort
  for (int j = 1; j < tam; j++) {
    struct racional *chave;
  
    if(!(chave = malloc(sizeof(struct racional))))
      return;

    chave->num = v[j]->num;
    chave->den = v[j]->den;

    i = j - 1;

    //passa os elementos maiores que a chave para frente
    while (i >= 0 && compara_r(v[i], chave) > 0) {
    v[i + 1]->num = v[i]->num;
    v[i + 1]->den = v[i]->den;
  
    i--;
  }

  //coloca a chave na posição correta
  v[i + 1]->num = chave->num;
  v[i + 1]->den = chave->den;

  //libera memoria
  destroi_r(chave);
}

}

//elimina NaNs e passa o valor da ultima posição do vetor pro lugar do nan
void elimina_elemento(struct racional *v[], int *tam) {
  if (*tam == 0) return; //ve se o vetor esta vazio

  int ultima_pos = *tam - 1; 

  for (int i = 0; i < *tam; i++) {
      if (!valido_r(v[i])) {
          // substitui o elemento invalido pelo ultimo valido do vetor
          destroi_r(v[i]); 
          v[i] = v[ultima_pos]; // substitui pelo último elemento
          ultima_pos--; 
          (*tam)--; 
          i--; 
  }
}

}

//função para somar todos os racionais em um vetor 
struct racional *soma_total(struct racional *v[], int tam) {
  // aloca memoria para o resultado
  struct racional *result = malloc(sizeof(struct racional));
  if (!result) return NULL;

  // inicializa
  result->num = 0;
  result->den = 1;

  //racional temporário para armazenar cada soma parcial
  struct racional *temp = malloc(sizeof(struct racional));
  if (!temp) {
      free(result);
      return NULL;
  }

  for (int i = 0; i < tam; i++) {
      if (valido_r(v[i])) {
          if (!soma_r(result, v[i], temp)) {
              //libera a memória e retorna NULL
              free(temp);
              free(result);
              return NULL;
          }
          
          // copia o resultado temporário de volta para result
          result->num = temp->num;
          result->den = temp->den;
      }
  }

  free(temp); 
  return result;
}

void imprime_vetor(struct racional *v[], int tam) {
  
  for(int i = 0; i < tam; i++) {
        imprime_r(v[i]); 
  } 
  printf("\n");
  
}

void destroi_vetor(struct racional *v[], int tam) {
  
  for(int i = 0; i < tam; i++) {
    destroi_r(v[i]);
    v[i] = NULL;
  }
  
}

//le um valor n (tamanho do vetor) tal que 0 < n < 100, garante o numero dentro do intervalo
int le_tam_vetor(int tam) {

  int fim = 1;

  do {
        scanf("%d", &tam);
        if(tam > 0 && tam  < 100)
          fim = 0;
  } while (fim);

  return tam;

} 

//preenche o vetor com n numeros racionais lidos na entrada
void preenche_vetor(struct racional *v[], int tam) {

  int num, den;

  for(int i = 0; i < tam; i++) {
    scanf("%d", &num);
    scanf("%d", &den);
    
    v[i] = cria_r(num, den);
  }
   
}


int main () {
  
  struct racional **v;
  struct racional *soma_total_vet;
  int n = 0; //tamanho do vetor
 

  n = le_tam_vetor(n);

  if(!(v = malloc(n * sizeof(struct racional *))))
  return -1;

  preenche_vetor(v, n);
  
  //imprime o vetor de racionais
  printf("VETOR =");
  imprime_vetor(v, n);

  //elimina os Nans e imprime
  elimina_elemento(v, &n);
  printf("VETOR =");
  if(n == 0) //considerando os casos de vetor vazio por conta dos erros
    printf(" \n");
  else
      imprime_vetor(v, n);

  //ordena o vetor e imprime
  ordena_vetor(v, n);
  printf("VETOR =");
  if(n == 0) 
    printf(" \n");
  else
    imprime_vetor(v, n);

  //soma todos os elementos e imprime
  soma_total_vet = soma_total(v,n);
  printf("SOMA =");
  imprime_r(soma_total_vet); //imprime o ponteiro com resultado da soma
  printf("\n");
  

  //libera os racionais apontados pelo vetor e imprime
  destroi_vetor(v, n);
  printf("VETOR =");
  if(n == 0)
    printf(" \n");
  else
    imprime_vetor(v, n);

  //libera a o ponteiro da soma
  destroi_r(soma_total_vet);

  //libera o vetor alocado
  free(v);

  return 0;

}










