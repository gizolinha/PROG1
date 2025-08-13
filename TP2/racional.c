#include <stdio.h>
#include <stdlib.h>
#include "racional.h"

/* retorna um número aleatório entre min e max, inclusive. */
long aleat (long min, long max) {

    return min + rand() % (max - min + 1);
} 

/* Máximo Divisor Comum entre a e b      */
long mdc (long a, long b) {

    if (b == 0)
      return a;
    
    return mdc(b, a % b);
}

/* Mínimo Múltiplo Comum entre a e b */
long mmc (long a, long b) {

    return (a * b) / mdc(a, b);
}

/* Recebe um número racional e o simplifica.
 * Por exemplo, ao receber 10/8 deve retornar 5/4.
 * Se ambos numerador e denominador forem negativos, deve retornar um positivo.
 * Se o denominador for negativo, o sinal deve migrar para o numerador.
 * Se r for inválido, devolve-o sem simplificar. */
struct racional simplifica_r (struct racional r) {

    int divisor;

    //caso r seja valido
    if(valido_r(r) == 0)
      return r;
    
    //simplifica pelo mdc
    divisor = mdc(r.num, r.den);
    r.num /= divisor;
    r.den /= divisor;

    //ajuste de sinais
    if(r.den < 0) {
      r.num = r.num * (-1);
      r.den = r.den * (-1);
    }
    return r;
}

/* Cria um número racional com o numerador e denominador indicados. */
struct racional cria_r (long numerador, long denominador) {

    struct racional r;

    r.num = numerador;
    r.den = denominador;

    return simplifica_r(r);

}

/* Retorna 1 se o racional r for válido ou 0 se for inválido.
 * Um racional é inválido se seu denominador for zero */
int valido_r (struct racional r){

    if(r.den != 0)
      return 1;
    return 0;

}


/* Retorna um número racional aleatório na forma simplificada.
 * Deve ser sorteado o numerador e depois o denominador.
 * o racional gerado pode ser válido ou inválido.
 * O numerador e o denominador devem ser inteiros entre min e max. */
struct racional sorteia_r (long min, long max) {

    struct racional r;

    r.num = aleat(min, max);
    r.den = aleat(min, max);

    return simplifica_r(r);
}

/* Imprime um racional r, respeitando estas regras:
   - o racional deve ser impresso na forma simplificada;
   - não imprima espaços em branco e não mude de linha;
   - a saída deve ter o formato "num/den", a menos dos casos abaixo:
     - se o racional for inválido, imprime a mensagem "NaN" (Not a Number);
     - se o numerador for 0, imprime somente "0";
     - se o denominador for 1, imprime somente o numerador;
     - se o numerador e denominador forem iguais, imprime somente "1";
     - se o racional for negativo, o sinal "-" vem antes do numerador;
     - se numerador e denominador forem negativos, o racional é positivo. */
void imprime_r (struct racional r) {

    //confere se r eh valido, s enao for, imprime e reotorna
    if(valido_r(r) == 0) {
      printf("NaN");
      return;
    }

    //caso numerador igual a zero, imprime zero
    if (r.num == 0)
      printf("0");
    
    //caso do denominador igual a um, imprime apenas o numerador
    else if (r.den == 1)
      printf("%ld", r.num);
    
    //caos em que o numerador eh igual ao denominador, imprime apenas 1
    else if(r.num == r.den)
      printf("1");
    
    else 
    //imprime padrão
    printf("%ld/%ld", r.num, r.den);

    //como a questão do sinal foi resolvida na simplifica_r, nao foi colocado na impressao  
      
}

/* Compara dois racionais r1 e r2. Retorno: -2 se r1 ou r2 for inválido,
 * -1 se r1 < r2, 0 se r1 = r2 ou 1 se r1 > r2 */
int compara_r (struct racional r1, struct racional r2) {

    int den, num1, num2;

    if(!valido_r(r1) || !valido_r(r2))
      return -2;
    
    den = mmc(r1.den, r2.den);

    num1 = r1.num * (den / r1.den);
    num2 = r2.num * (den / r2.den);

    if (num1 < num2)
      return -1;

    if (num1 > num2)
      return 1;
    
    else
      return 0;

}
            
/* Retorna a soma dos racionais r1 e r2 no parametro *r3.
 * Retorna 1 se a operacao foi bem sucedida ou
 *         0 se r1 ou r2 for inválido ou se *r3 for nulo */
int soma_r (struct racional r1, struct racional r2, struct racional *r3) {

    if (r3 == NULL)
      return 0;

    if (!valido_r(r1) || !valido_r(r2))
      return 0;

    //soma de racionais, e armazena a estrutura apontada por r3
    r3->num = (r1.num * r2.den) + (r2.num * r1.den);
    r3->den = r1.den * r2.den;

    //simplifica, usa *r para passar o valor (não o ponteiro)
    *r3 = simplifica_r(*r3);
    return 1;

 }

/* Retorna a subtracao dos racionais r1 e r2 no parametro *r3.
 * Retorna 1 se a operacao foi bem sucedida ou
 *         0 se r1 ou r2 for inválido ou se *r3 for nulo */
int subtrai_r (struct racional r1, struct racional r2, struct racional *r3){

    if (r3 == NULL)
      return 0;

    if (!valido_r(r1) || !valido_r(r2))
      return 0;

    //subtração de racionais
    r3->num = (r1.num * r2.den) - (r2.num * r1.den);
    r3->den = r1.den * r2.den;

    //simplifica
    *r3 = simplifica_r(*r3);
    return 1;

}

/* Retorna a multiplicacao dos racionais r1 e r2 no parametro *r3.
 * Retorna 1 se a operacao foi bem sucedida ou
 *         0 se r1 ou r2 for inválido ou se *r3 for nulo */
int multiplica_r (struct racional r1, struct racional r2, struct racional *r3) {

    if (r3 == NULL)
      return 0;

    if (!valido_r(r1) || !valido_r(r2))
      return 0;

    //multiplicação de racionais
    r3->num = r1.num * r2.num;
    r3->den = r1.den * r2.den;

    //simplifica
    *r3 = simplifica_r(*r3);
    return 1;

}

/* Retorna a divisao dos racionais r1 e r2 no parametro *r3.
 * Retorna 1 se a operacao foi bem sucedida ou
 *         0 se r1 ou r2 for inválido ou se *r3 for nulo */
int divide_r (struct racional r1, struct racional r2, struct racional *r3) {

    if (r3 == NULL)
      return 0;

    if (!valido_r(r1) || !valido_r(r2))
      return 0;

    //divisao de racionais, multiplica pelo inverso
    r3->num = r1.num * r2.den;
    r3->den = r1.den * r2.num;

    //simplifica
    *r3 = simplifica_r(*r3);
    return 1;
}
