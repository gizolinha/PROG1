#include <stdio.h>
#include <stdlib.h>
#include "racional.h"

/* Maximo Divisor Comum entre a e b      */
/* calcula o mdc pelo metodo de Euclides */
long mdc (long a, long b) {

    if(b == 0)
      return a;
    
    return mdc(b, a % b);
}

/* Minimo Multiplo Comum entre a e b */
/* mmc = (a * b) / mdc (a, b)        */
long mmc (long a, long b) {

    return (a * b) / mdc(a, b);
}

/* Simplifica o número racional indicado no parâmetro.
 * Por exemplo, se o número for 10/8 muda para 5/4.
 * Retorna 1 em sucesso e 0 se r for inválido ou o ponteiro for nulo.
 * Se ambos numerador e denominador forem negativos, o resultado é positivo.
 * Se o denominador for negativo, o sinal deve migrar para o numerador. */
int simplifica_r (struct racional *r) {

  long int divisor;

  //verifica se o ponteiro eh nulo ou r eh invalido
  if(r == NULL || r->den == 0)
    return 0;

  //divide num e den pelo mdc
  divisor = mdc(r->num, r->den);
  r->num /= divisor;
  r->den /= divisor;
  
  /* Se ambos numerador e denominador forem negativos, deve retornar um positivo.
  * Se o denominador for negativo, o sinal deve migrar para o numerador. */
  if(r->den < 0) {
    r->num = r->num * (-1);
    r->den = r->den * (-1);
  }
  return 1;
}


/* Cria um número racional com o numerador e denominador indicados
 * e retorna um ponteiro que aponta para ele.
 * A memória para o número racional deve ser alocada dinamicamente
 * por esta função. Retorna NULL se não conseguiu alocar a memória. */
struct racional *cria_r (long numerador, long denominador) {

  struct racional *r; //retorna um ponteiro para struct

  if(!(r = malloc(sizeof(struct racional))))
    return NULL; 
  
  r->num = numerador;
  r->den = denominador;

  return r; //retorna o ponteiro
}


/* Libera a memória alocada para o racional apontado por r */
void destroi_r (struct racional *r) {

  if(r != NULL)
    free(r);

}

/* Retorna 1 se o racional r for válido ou 0 se for inválido. Um racional
 * é inválido se o denominador for zero ou se ele não tiver sido alocado. */
int valido_r (struct racional *r) {

  if(r == NULL || r->den == 0) 
    return 0;
  
  return 1;
}


/* Imprime um racional r, respeitando estas regras:
   - o racional deve estar na forma simplificada;
   - não use espacos em branco e não mude de linha;
   - o formato de saída deve ser "num/den", a menos dos casos abaixo;
     - se o ponteiro for nulo, imprime a mensagem "NULL";
     - se o racional for inválido, imprime a mensagem "NaN" (Not a Number);
     - se o numerador for 0, imprime somente "0";
     - se o denominador for 1, imprime somente o numerador;
     - se o numerador e denominador forem iguais, imprime somente "1";
     - se o racional for negativo, o sinal é impresso antes do número;
     - se numerador e denominador forem negativos, o racional é positivo. */
void imprime_r (struct racional *r) {

  //caso ponteiro nulo
  if(r == NULL) {
    printf(" NULL");
    return;
  }

  //caso racional invalido
  if(!valido_r(r)) {
    printf(" NaN");
    return;
  }

  //simplifica para imprimir outros casos
  simplifica_r(r);

  //numerador = 0
  if(r->num == 0)
    printf(" 0");
  
  //denominador = 1
  else if(r->den == 1)
      printf(" %ld", r->num);
  
  //numerador e denominador iguais
  else if(r->num == r->den)
      printf(" 1");
  
  //imprime normal
  else printf(" %ld/%ld", r->num, r->den);

}

/* Compara dois números racionais r1 e r2.
 * Retorna -2 se r1 ou r2 for inválido ou se o respectivo ponteiro for nulo.
 * Retorna -1 se r1 < r2; 0 se r1 = r2; 1 se r1 > r2.
 * Atenção: faça a comparação normalizando os denominadores pelo MMC.
 * Fazer a comparação baseado na divisão do numerador pelo denominador
 * pode gerar erro de arredondamento e falsear o resultado. */
int compara_r (struct racional *r1, struct racional *r2) {

   long int den, num1, num2;

  //ponteiros nulos
  if((r1 == NULL) || (r2 == NULL))
    return -2;

  if(!valido_r(r1) || !valido_r(r2))
    return -2;
  
  den = mmc(r1->den, r2->den);

  num1 = r1->num * (den / r1->den);
  num2 = r2->num * (den / r2->den);

  if(num1 < num2)
    return -1;
  
  if(num1 > num2)
    return 1;
  
  return 0;
}

/* Coloca em *r3 a soma simplificada dos racionais *r1 e *r2.
 * Retorna 1 em sucesso e 0 se r1 ou r2 for inválido ou um ponteiro for nulo. */
int soma_r (struct racional *r1, struct racional *r2, struct racional *r3) {

  if(r3 == NULL)
    return 0;

  if(!valido_r(r1) || !valido_r(r2))
    return 0;
  
  //soma de racionais
  r3->num = (r1->num * r2->den) + (r2->num * r1->den);
  r3->den = r1->den * r2->den;

  //simplifica o racional da subtracao em r3
  simplifica_r(r3);

  return 1;

}

/* Coloca em *r3 a diferença simplificada dos racionais *r1 e *r2.
 * Retorna 1 em sucesso e 0 se r1 ou r2 for inválido ou um ponteiro for nulo. */
int subtrai_r (struct racional *r1, struct racional *r2, struct racional *r3) {

  if(r3 == NULL)
    return 0;
  
  if(!valido_r(r1) || !valido_r(r2))
    return 0;
  
  //subtração de racionais
  r3->num = (r1->num * r2->den) - (r2->num * r1->den);
  r3->den = r1->den * r2->den;

  //simplifica o racional da subtracao em r3
  simplifica_r(r3);

  return 1;

}

/* Coloca em *r3 o produto simplificado dos racionais *r1 e *r2.
 * Retorna 1 em sucesso e 0 se r1 ou r2 for inválido ou um ponteiro for nulo. */
int multiplica_r (struct racional *r1, struct racional *r2, struct racional *r3){

  if(r3 == NULL)
    return 0;
  
  if(!valido_r(r1) || !valido_r(r2))
    return 0;
  
  r3->num = r1->num * r2->num;
  r3->den = r1->den * r2->den;

  simplifica_r(r3);

  return 1;

}

/* Coloca em *r3 a divisão simplificada do racional *r1 por *r2.
 * Retorna 1 em sucesso e 0 se r1 ou r2 for inválido ou um ponteiro for nulo. */
int divide_r (struct racional *r1, struct racional *r2, struct racional *r3) {

  if(r3 == NULL)
    return 0;
  
  if(!valido_r(r1) || !valido_r(r2))
    return 0;
  
  r3->num = r1->num * r2->den;
  r3->den = r2->num * r1->den;

  simplifica_r(r3);

  return 1;
  
}