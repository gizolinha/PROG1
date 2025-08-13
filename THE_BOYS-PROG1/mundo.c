#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include "mundo.h"
#include "fila.h"
#include "fprio.h"
#include "conjunto.h"

//gera um inteiro aleatorio entre min e max
int aleat(int min, int max) {

    return min + rand() % (max - min + 1);
}

//calcula distancia cartesiana entre dois pontos
int calcula_distancia(struct local a, struct local b) {

    double cd;

    cd = pow((b.x - a.x), 2) + pow((b.y -a.y), 2);

    return (int)sqrt(cd);
}

//cria e inicializa as estruturas de um novo evento
struct evento *cria_evento(int tempo, int tipo, int info1, int info2) {

    struct evento *novo_evento = malloc(sizeof(struct evento));

    //inicializa os campos do evento
    novo_evento->tempo = tempo;
    novo_evento->tipo = tipo;
    novo_evento->info1 = info1;
    novo_evento->info2 = info2;

    return novo_evento;

}

//cria um novo evento usando a cria_evento e insere ele na lef (fprio)
struct evento *agenda_evento(struct mundo *m, int tempo, int tipo, int info1, int info2) {

    //caso de m ou lef invalidos
    if(m == NULL || m->lef == NULL) {
        printf("erro, mundo ou lef invalidos (agenda_evento)"); 
        return NULL;
    }
    
    //cria um novo evento
    struct evento *ev = cria_evento(tempo, tipo, info1, info2);
    if(ev == NULL) {
        printf("erro, falha ao criar evento (agenda_evento)"); 
    }

    //insere ele na lef
    if(fprio_insere(m->lef, ev, tipo, tempo) == -1) {
        printf("erro, falha ao inserir evento na lef (agenda_evento)"); 
        destroi_evento(ev);
    }
    return ev;
}

//destroi e libera memoria de um evento
struct evento *destroi_evento(struct evento *ev) {

    if (ev != NULL) 
        free(ev);
    
    return NULL;
}

//cria e incializa as estruturas do mundo
struct mundo cria_mundo() {
    struct mundo m;

    //inicializa os dados do mundo
    m.n_herois = N_HEROIS;
    m.n_bases = N_BASES;
    m.n_missoes = N_MISSOES;
    m.n_habilidades = N_HABILIDADES;
    m.n_compostos_v = N_COMPOSTOS_V; 
    m.n_tamanho_mundo = N_TAMANHO_MUNDO;
    m.relogio = T_INICIO;
    m.eventos_tratados = 0;
    m.missoes_cumpridas = 0;

    //cria e inicializa vetor de herois usando a cria_heroi
    for(int i = 0; i < m.n_herois; i++)
        m.herois[i] = cria_heroi(i);

    //cria e inicializa o vetor de bases usando a cria_base
    for(int i = 0; i < m.n_bases; i++)
        m.bases[i] = cria_base(&m, i);
    
    //cria e inicializa o vetor de missoes usando a cria_missoes
    for(int i = 0; i < m.n_missoes; i++)
        m.missoes[i] = cria_missao(&m, i);

    //cria habilidades
    m.habilidades = cjto_cria(m.n_habilidades);
    //inicializa
    for(int i = 0; i < m.n_habilidades; i++)
        cjto_insere(m.habilidades, i);

    //cria a lista de eventos futuros usando a fprio_t e cria eventos inciais
    m.lef = fprio_cria();
    eventos_iniciais(&m);

    return m;
}

//destroi o mundo e todas as suas estruturas
void destroi_mundo(struct mundo *m) {

    //destroi conjunto de habilidades de cada heroi
    for(int i_h = 0; i_h < m->n_herois; i_h++)
        cjto_destroi(m->herois[i_h].habilidades); 

    //destroi base
    for(int i_b = 0; i_b < m->n_bases; i_b++) {
        //destroi presentes na base
        cjto_destroi(m->bases[i_b].presentes); 
        //destroi a fila de espera
        fila_destroi(m->bases[i_b].espera); 
        //destroi habilidades da base
        cjto_destroi(m->bases[i_b].habilidades); 
    }

    //destroi as habilidades de cada missao
    for(int i_m = 0; i_m < m->n_missoes; i_m++)
        cjto_destroi(m->missoes[i_m].habilidades_req);

    //destrois habilidades do mundo
    cjto_destroi(m->habilidades);
    //destroi lista de eventos futuros
    fprio_destroi(m->lef);
}


//cria e incializa individualmente o heroi
struct heroi cria_heroi(int id) {

    struct heroi h;

    //inicializa cada estrutura do heroi
    h.id = id;
    h.experiencia = 0;
    h.paciencia = aleat(1,100);
    h.velocidade = aleat(50,500);
    h.habilidades = cjto_aleat(aleat(1,3), N_HABILIDADES -1);
    //heroi inicializa sem base
    h.base = -1;
    h.status = VIVO;

    return h;
}

//cria e incializa individualmente a base
struct base cria_base(struct mundo *m, int id) {

    struct base b;

    b.id = id;
    b.lotacao = aleat(3, 10);
    b.conta_missoes = 0;
    b.max_fila = 0;
    b.habilidades = cjto_cria(N_HABILIDADES);
    b.presentes = cjto_cria(N_HEROIS - 1);
    b.espera = fila_cria();
    b.coordenadas.x = aleat(0, m->n_tamanho_mundo);
    b.coordenadas.y = aleat(0, m->n_tamanho_mundo);

    return b;
}

//cria e incializa individualmente a missao
struct missao cria_missao(struct mundo *m, int id) {

    struct missao mi;

    mi.id = id;
    mi.habilidades_req = cjto_aleat(aleat(6,10), N_HABILIDADES -1);
    mi.tentativas = 0;

    mi.coordenadas.x = aleat(0,m->n_tamanho_mundo);
    mi.coordenadas.y = aleat(0,m->n_tamanho_mundo);

    return mi; 
}

//crias os eventos iniciais e insere na lef
void eventos_iniciais(struct mundo *m) {

    int i, tempo, base;

    //cria evento para herois chegando em bases aleatórias
    for(i = 0; i < m->n_herois; i++) {
        base = aleat(0, m->n_bases -1);
        tempo = aleat(0, 4320); 
        agenda_evento(m, tempo, CHEGA, i, base); 
    }

    //cria evento para todas as missoes do mundo
    for(i = 0; i < m->n_missoes; i++) {
        tempo = aleat(0, T_FIM_DO_MUNDO);
        agenda_evento(m, tempo, MISSAO, i, -1);
    }

    //cria evento para o fim do mundo
    tempo = T_FIM_DO_MUNDO;
    agenda_evento(m, tempo, FIM, -1, -1);
}

//atualiza o conjunto uniao de habilidades dos herois presentes em determinada base
void uni_habilidades(struct mundo *m, struct base *b) {

    int heroi;

    //confere se mundo ou base sao nulos
    if (m == NULL || b == NULL)
        return;

    //cria um novo conjunto para uniao de habilidades
    struct cjto_t *hab_unidas = cjto_cria(N_HABILIDADES);
    if (hab_unidas == NULL)
        return;

    //percorre vetor de herois 
    for (int i = 0; i < N_HEROIS - 1; i++) {
        heroi = i;
        //confere se estao na base
        if(cjto_pertence(b->presentes, i)) {
            //se heroi eh valido
            if(heroi >= 0 && heroi < N_HEROIS) {
                //une as habilidades
                struct cjto_t *hab_heroi = m->herois[heroi].habilidades;
                struct cjto_t *nova_uniao = cjto_uniao(hab_unidas, hab_heroi);
                //da free no conjunto antigo
                cjto_destroi(hab_unidas);
                hab_unidas = nova_uniao;
            }
        }

    }
    //se existir outro coonjunto anterior na base destroi
    if (b->habilidades != NULL)
        cjto_destroi(b->habilidades);

    //novo conjunto de habilidades da base
    b->habilidades = hab_unidas;
}


//evento de chegada do heroi na base, decide se espera ou desiste
void chega(struct mundo *m, int tempo, int heroi, int base) {

    struct heroi h;
    struct base b;
    int presentes_b, lotacao_b, espera_b, fila_espera;

    //verifica se o heroi esta vivo
    if(m->herois[heroi].status == MORTO) 
        return; 

    //heroi e base do evento
    h = m->herois[heroi];
    b = m->bases[base];

    //quantos herois estao na base
    presentes_b = cjto_card(b.presentes);
    //tamanho da fila de espera
    fila_espera = fila_tamanho(b.espera);
    //lotação max da base
    lotacao_b = b.lotacao;

    //atualiza a base do heroi
    m->herois[heroi].base = base;

    //se ha vagas na base e a fila de espera esta vazia
    if ((presentes_b < lotacao_b) && (fila_tamanho(b.espera) == 0))
        espera_b = 1;
    else
        //decide se espera
        espera_b = h.paciencia > (10 * fila_espera);

    //se esperar cria evento espera
    if (espera_b == 1) {
        agenda_evento(m, tempo, ESPERA, heroi, base); 
        printf("%6d: CHEGA HEROI %2d BASE %d ", tempo, heroi, base);
        printf("(%2d/%2d) ESPERA \n", presentes_b, lotacao_b);
    }
    //se não cria evento desiste
    else {
        agenda_evento(m, tempo, DESISTE, heroi, base); 
        printf("%6d: CHEGA HEROI %2d BASE %d", tempo, heroi, base);
        printf("(%2d/%2d) DESISTE \n", presentes_b, lotacao_b);
    }    
}

//evento caso o heroi decida esperar na fila, cria o evento avisa
void espera(struct mundo *m, int tempo, int heroi, int base) {

    //alocar dinamicamente para evitar erro no free (destroi mundo)
    int *ptr_heroi = malloc(sizeof(int));
    *ptr_heroi = heroi;

    printf("%6d: ESPERA HEROI %2d BASE %d ", tempo, heroi, base);
    //quantos estao esperando na fila
    printf("(%2d)\n", fila_tamanho(m->bases[base].espera)); 

    //insere heroi na fila da base
    fila_insere(m->bases[base].espera, ptr_heroi);

    //cria evento avisa para o porteiro 
    agenda_evento(m, tempo, AVISA, -1, base);

}

//evento caso o heroi desista de esperar na fila, cria o evento viaja
void desiste(struct mundo *m, int tempo, int heroi, int base) {

    int prox_base;
    
    printf("%6d: DESISTE HEROI %2d BASE %d\n", tempo, heroi, base);

    //escolhe uma base aleatoria para o heroi viajar
    prox_base = aleat(0, m->n_bases -1);

    //cria evento viaja
    agenda_evento(m, tempo, VIAJA, heroi, prox_base);
}

 //evento que avisa o porteiro da base e caso haja vagas, admite o heroi e cria o eevnto entra
void avisa(struct mundo *m, int tempo, int base) { 

    int lotacao_b, presentes_b, tamanho_fila;
    //uso de um ponteiro para auxiliar e para evitar vazamento da alocação de heroi na espera
    int *aux = NULL;
    
    //base por referencia para guardar max_fila
    struct base *b = &m->bases[base];
    
    lotacao_b = b->lotacao;
    presentes_b = cjto_card(b->presentes);

    //recorde da fila da base
    tamanho_fila = fila_tamanho(b->espera);
    if (tamanho_fila > b->max_fila) {
        b->max_fila = tamanho_fila; 
    }

    printf("%6d: AVISA PORTEIRO BASE %d ", tempo, base);
    printf("(%2d/%2d) FILA [ ", presentes_b, lotacao_b);
    fila_imprime(b->espera);
    printf("]\n");

    //enquanto tiver vagas e a fila de espera nao estiver vazia
    while (presentes_b < lotacao_b && fila_tamanho(b->espera) != 0) {
        aux = fila_retira(b->espera);
        cjto_insere(b->presentes, *aux);
        presentes_b++;

        //cria evento ao entrar na base
        agenda_evento(m, tempo, ENTRA, *aux, base);
        printf("%6d: AVISA PORTEIRO BASE %d ADMITE %2d\n", tempo, base, *aux);

        if (aux) 
          free(aux);
    }
}

//evento quando o heroi eh admitido na base, cria o evento sai
void entra(struct mundo *m, int tempo, int heroi, int base) {

    struct base b;
    struct heroi h;
    int permanencia_b, presentes_b, lotacao_b;

    //atualiza o conjunto de habilidades da base
    uni_habilidades(m, &m->bases[base]);

    //checa se o heroi esta vivo
    if(m->herois[heroi].status == MORTO) 
        return; 

    //base e heroi do evento
    b = m->bases[base];
    h = m->herois[heroi];

    presentes_b = cjto_card(b.presentes);
    lotacao_b = b.lotacao;

    //quanto tempo heroi ficara na base
    permanencia_b = 15 + (h.paciencia * aleat(1, 20));

    //cria evento sai
    agenda_evento(m, (tempo + permanencia_b), SAI, heroi, base);

    printf("%6d: ENTRA HEROI %2d BASE %d ", tempo, heroi, base);
    printf("(%2d/%2d) SAI %d\n", presentes_b, lotacao_b, (tempo + permanencia_b));
}

//heroi sai da base, cria o evento viaja e o evento avisa
void sai(struct mundo *m, int tempo, int heroi, int base) {

    struct base b;
    int prox_base;

    //checa se o heroi esta vivo
    if(m->herois[heroi].status == MORTO) 
        return;

    b = m->bases[base];

    //tira heroi da base atual
    cjto_retira(b.presentes, heroi);

    //atualiza o conjunto de habilidades da base
    uni_habilidades(m, &m->bases[base]);

    //prox base para que o heroi vai viajar e garante que eh diferente da base atual
    do {
        prox_base = aleat(0, m->n_bases-1);
    } while (prox_base == base);

    //cria evento viaja
    agenda_evento(m, tempo, VIAJA, heroi, prox_base);

    //cria evento avisa
    agenda_evento(m, tempo, AVISA, -1, base);

    printf("%6d: SAI HEROI %2d BASE %d", tempo, heroi, base);
    printf(" (%2d/%2d)\n", cjto_card(b.presentes), b.lotacao); 

}

//heroi viaja para outra base, cria o evento chega
void viaja(struct mundo *m, int tempo, int heroi, int base) {

    struct base b_atual, b_prox;
    struct heroi h;
    int duracao, distancia;

    //heroi do evento
    h = m->herois[heroi];
    //base atual do heroi
    b_atual = m->bases[h.base];
    //base que  heroi vai viajar
    b_prox = m->bases[base];          
    
    //distancia ate a prox base
    distancia = calcula_distancia(b_atual.coordenadas, b_prox.coordenadas);
    
    //duracao da viagem ate a prox base
    duracao = (int)(distancia / h.velocidade);

    //cria evento chega
    agenda_evento(m, tempo + duracao, CHEGA, heroi, base);

    printf("%6d: VIAJA HEROI %2d BASE %d BASE %d ", tempo, heroi, h.base, base);
    printf("DIST %d VEL %d CHEGA %d\n", distancia, h.velocidade, tempo + duracao);
}                   

//heroi morre em determinada missao, cria o evento avisa
void morre(struct mundo *m, int tempo, int heroi, int missao) {
    
    // verifica se o heroi eh valido
    if (heroi < 0 || heroi >= m->n_herois) {
        printf("ERRO: heroi %d invalido\n", heroi);
        return;
    }

    int base = m->herois[heroi].base;

    // verifica se a base eh valida
    if (base < 0 || base >= m->n_bases) {
        printf("ERRO: base %d do heroi %d invalidos\n", base, heroi);
        return;
    }

    // verifica se o conjunto de presentes voi inicializado
    if (m->bases[base].presentes == NULL) {
        printf("ERRO: Conjunto de presentes da base %d não foi incializado\n", base);
        return;
    }

    //remove o heroi da base em que ele estava
    cjto_retira(m->bases[base].presentes, heroi);

    //atualiza o conjunto de habilidades da base
    uni_habilidades(m, &m->bases[base]);

    //atualiza o status do heroi
    m->herois[heroi].status = MORTO;
    printf("%6d: MORRE HEROI %2d MISSAO %d\n", tempo, heroi, missao);

    //cria evento avisa
    agenda_evento(m, tempo, AVISA, -1, m->herois[heroi].base);

    //heroi fica sem base pq morreu
    m->herois[heroi].base = -1;  
}


//confere se a base tem as habilidades necessarias para a missao
int base_p_valida(struct base *b, struct missao *mi) {
    if(cjto_contem(b->habilidades, mi->habilidades_req))
        return true;

    return false;
}

//evento missao
void missao(struct mundo *m, int tempo, int missao) {
    //variavel para armazemar o id da base mais prox valida
    int base_apta_mp = -1;
    //variavel para base mais prox independente
    int base_mp = -1;
    //para garantir menor distancia segura 
    int menor_distancia_apta = ((N_TAMANHO_MUNDO + 1) * sqrt(2));
    int menor_distancia = ((N_TAMANHO_MUNDO + 1) * sqrt(2));
    int distancia;

    //contador de tentativas de missao
    m->missoes[missao].tentativas++;

    printf("%6d: MISSAO %d TENT %d HAB REQ: [ ", tempo, missao, m->missoes[missao].tentativas);
    cjto_imprime(m->missoes[missao].habilidades_req);
    printf(" ]\n");

    //calcula distancia da base apta e a base mais prox (mesmo sem ser apta)
    for(int i = 0; i < m->n_bases; i++) {
        distancia = calcula_distancia(m->missoes[missao].coordenadas, m->bases[i].coordenadas);

        //atualiza a base mais proxima (mesmo sem ser apta)
        if (distancia < menor_distancia) {
            menor_distancia = distancia;
            base_mp = i;
        }

        //atualiza base mais proxima e apta
        if (base_p_valida(&m->bases[i], &m->missoes[missao]) && distancia < menor_distancia_apta) {
            menor_distancia_apta = distancia;
            base_apta_mp = i;
        }
    }
    
    //se houver base apta
    if (base_apta_mp != -1) {
        int bmp = base_apta_mp;

        //imprime a distancia da base ate a missao e os herois presentes na base
        printf("%6d: MISSAO %d BASE %d DIST %d HEROIS [ ", tempo, missao, bmp, menor_distancia_apta);
        cjto_imprime(m->bases[bmp].presentes);
        printf(" ]\n");

        //imprime a conjunto uniao de todas as habilidades dos presentes na base
        printf("%6d: MISSAO %d UNIAO HAB DA BASE %d: [",tempo, missao, bmp);
        cjto_imprime(m->bases[bmp].habilidades);
        printf(" ]\n");

        m->missoes_cumpridas++;
        m->bases[bmp].conta_missoes++;
        printf("%6d: MISSAO %d CUMPRIDA BASE %d HABS: [ ", tempo, missao, bmp);
        cjto_imprime(m->bases[bmp].habilidades);
        printf(" ]\n");

        for(int i = 0; i < m->n_herois; i++) {
            //se o heroi estiver na base que realizou a missao ganha xp
            if(m->herois[i].base == bmp && cjto_pertence(m->bases[bmp].presentes, i))
                m->herois[i].experiencia++;
        }
    } //caso nao tenha bases aptas proximas disponiveis, caso do composto v
    else if (tempo % 2500 == 0 && m->n_compostos_v != 0 && base_mp != -1) {

        int bmp = base_mp;

        //imprime a distancia da base ate a missao e os herois presentes na base
        printf("%6d: MISSAO %d BASE %d  HEROIS [ ", tempo, missao, bmp);
        cjto_imprime(m->bases[bmp].presentes);
        printf(" ]\n");

        //imprime a conjunto uniao de todas as habilidades dos presentes na base
        printf("%6d: MISSAO %d UNIAO HAB DA BASE %d: [",tempo, missao, bmp);
        cjto_imprime(m->bases[bmp].habilidades);
        printf(" ]\n");


        //achar heroi mais experiente
        int heroi_maxxp = -1;
        int max_xp = -1;
        for( int i = 0; i < m->n_herois; i++) {
            if(m->herois[i].base == bmp  && cjto_pertence(m->bases[bmp].presentes, i) && m->herois[i].experiencia > max_xp) {
                max_xp = m->herois[i].experiencia;
                heroi_maxxp = i;
            }
        }

        if (heroi_maxxp != -1) {
            //decrementa quantidade de compostos v
            m->n_compostos_v--;
            m->missoes_cumpridas++;
            m->bases[bmp].conta_missoes++;

            //printf para melhor depuração
            printf("%6d: MISSAO %d COMPOSTO V USADO NA BASE %d PELO HEROI %d\n", tempo, missao, bmp, heroi_maxxp);
            //heroi morrre
            agenda_evento(m, tempo, MORRE, heroi_maxxp, missao);

            //incrementa xp dos herois que sobraram
            for(int i = 0; i < m->n_herois; i++) {
                //se o heroi estiver na base que realizou a missao ganha xp
                if(m->herois[i].base == bmp && cjto_pertence(m->bases[bmp].presentes, i))

                    m->herois[i].experiencia++;
            }   
        }
               
    }
    else {
        //se nao houver base apta e nao puder usar composto v marca como impossivel e adia em 24h
        printf("%6d: MISSAO %d IMPOSSIVEL\n", tempo, missao);
        agenda_evento(m, tempo + (24 * 60), MISSAO, missao, -1);
    }
}

//acha missao com menor numero de tentavivas
int acha_min_tentativas(struct mundo *m) {

    //comeca com as min sendo missao 0
    int min = m->missoes[0].tentativas;

    for(int i = 1; i < m->n_missoes; i++) {
        if(m->missoes[i].tentativas < min)
            min = m->missoes[i].tentativas;
    }

    return min;
}

//acha missao com maior numero de tentativas
int acha_max_tentativas(struct mundo *m) {

    //comeca com as max sendo missao 0
    int max = m->missoes[0].tentativas;

    for(int i = 1; i < m->n_missoes; i++) {
        if(m->missoes[i].tentativas > max)
            max = m->missoes[i].tentativas;

    }
    return max;
}

//calcula media de tentativas
float acha_media_tentativas(struct mundo *m) {

    int soma = 0;

    for(int i = 0; i < m->n_missoes; i++)
        soma = soma + m->missoes[i].tentativas;

    return ((float)soma/m->n_missoes);
};


//encerra a simulacao e imprime as estatisticas do mundo
void fim(struct mundo *m) {

    struct heroi h;
    struct base b;

    printf("%6d: FIM\n", m->relogio);
    printf("\n");

    //imprime info dos herois
    for(int i = 0; i < m->n_herois; i++) {
        h = m->herois[i];
        //vetor para armazenar string do status do heroi
        char status[6];
        
        //usa strcpy para copiar a string pro vetor e alterar o texto caso heroi morra
        if(h.status == MORTO)
            strcpy(status, "MORTO");
        else
            strcpy(status, "VIVO");

        printf("HEROI %d %s PAC %d VEL %4d EXP %4d HABS [ ", i, status, h.paciencia, h.velocidade, h.experiencia);
        cjto_imprime(h.habilidades);
        printf(" ]\n");
        
    }
    printf("\n");

    //imprime info das bases
    for(int i = 0; i < m->n_bases; i++) {
        b = m->bases[i];

        printf("BASE %2d LOT %2d FILA MAX %2d MISSOES %d\n", i, b.lotacao, b.max_fila, b.conta_missoes);
        
    }
    printf("\n");

    //calcula estatisticas da simulação
    int total_ev = m->eventos_tratados;
    int missoes_sucedidas = m->missoes_cumpridas;
    int total_missoes = m->n_missoes;

    //porcetagem de sucesso em missoes
    float perc_missoes = (missoes_sucedidas / (float)total_missoes) * 100.0;
    //minimo de tentativas em missoes
    int min_tentativas = acha_min_tentativas(m);
    //max de tentativas em missoes
    int max_tentativas = acha_max_tentativas(m);
    //media de tentativas em missoes
    float media_tentativas = acha_media_tentativas(m);

    //taxa de mortalidade
    int total_herois = m->n_herois;
    int herois_mortos = 0;
    for(int i = 0; i < m->n_herois; i++) {
        if (m->herois[i].status == MORTO)
            herois_mortos++;
    }
    float taxa_mort = (herois_mortos / (float)total_herois) * 100.0;

    printf("EVENTOS TRATADOS: %d\n", total_ev);
    printf("MISSOES CUMPRIDAS: %d/%d (%.1f%%)\n", missoes_sucedidas, total_missoes, perc_missoes);
    printf("TENTATIVAS/MISSAO: MIN %d, MAX %d, MEDIA %.1f\n", min_tentativas, max_tentativas, media_tentativas);
    printf("TAXA MORTALIDADE: %.1f%%\n", taxa_mort);
    printf("\n");

    destroi_mundo(m);
}




