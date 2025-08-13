
//GRR20231947 Giovanna Joecy Sona de Oliveira

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "fila.h"
#include "fprio.h"
#include "mundo.h"
#include "conjunto.h"


// programa principal
int main () {

  //iniciar o mundo
  struct mundo m;
  struct evento *ev;
  int fim_simulacao;
  int tempo_atual = 0; 
  int tipo_atual = 0;

  srand(time(NULL));

  m = cria_mundo();

  fim_simulacao = 0;
  //enquanto fim da simulacao for 0 e ainda tiverem eventos na lef
  while (!fim_simulacao && (ev = fprio_retira(m.lef, &tipo_atual, &tempo_atual))) {
      m.relogio = ev->tempo;
      //contadora de eventos da simulacao
      m.eventos_tratados += 1;
      switch (ev->tipo) {

          case CHEGA:
              chega(&m, ev->tempo, ev->info1, ev->info2);
              break;
          case ESPERA:
              espera(&m, ev->tempo, ev->info1, ev->info2);
              break;
          case DESISTE:
              desiste(&m, ev->tempo, ev->info1, ev->info2);
              break;
          case VIAJA:
              viaja(&m, ev->tempo, ev->info1, ev->info2);
              break;
          case MISSAO:
              missao(&m, ev->tempo, ev->info1);
              break;
          case AVISA:
              avisa(&m, ev->tempo, ev->info2);
              break;
          case ENTRA:
              entra(&m, ev->tempo, ev->info1, ev->info2);
              break;
          case SAI:
              sai(&m, ev->tempo, ev->info1, ev->info2);
              break;       
          case MORRE:
              morre(&m, ev->tempo, ev->info1, ev->info2);
              break;    
          case FIM:
              fim(&m);
              fim_simulacao = 1;
              break;         
      }
      destroi_evento(ev);
  }
  return 0;
}

