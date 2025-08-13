//estado inicial
#define T_INICIO 0
#define T_FIM_DO_MUNDO 525600
#define N_TAMANHO_MUNDO 2000
#define N_HABILIDADES 10
#define N_HEROIS N_HABILIDADES * 5
#define N_BASES N_HEROIS / 5
#define N_MISSOES T_FIM_DO_MUNDO / 100
#define N_COMPOSTOS_V N_HABILIDADES * 3

//codigo dos eventos
#define CHEGA 1
#define ESPERA 2
#define DESISTE 3
#define AVISA 4
#define ENTRA 5
#define SAI 6
#define VIAJA 7
#define MORRE 8
#define MISSAO 9
#define FIM 10

//status dos herois
#define MORTO 0
#define VIVO 1

//estrutura de local
struct local {
    int x;
    int y;
};

//estrutura do heroi
struct heroi {
    int id;
    struct cjto_t *habilidades;
    int paciencia;
    int velocidade;
    int experiencia;
    int base;
    int status;
};

//estrutura da base
struct base {
    //id da base
    int id; 
    //numero maximo de herois na base
    int lotacao;
    //numero de missoes cumpridas pela base
    int conta_missoes; 
    //recorde de maior fila de espera da base
    int max_fila;
    //habilidades da base
    struct cjto_t *habilidades;
    //lista de herois presentes na base
    struct cjto_t *presentes; 
    //fila de espera da base
    struct fila_t *espera; 
    // coordenadas da base
    struct local coordenadas; 
};

//estrutura da missao
struct missao {
    int id;
    //habilidades necessarias para missao
    struct cjto_t *habilidades_req;
    struct local coordenadas;
    int tentativas;
   
};

//estrutura do mundo
struct mundo {
    //numero total de herois
    int n_herois; 
    //vetor com todos os herois
    struct heroi herois[N_HEROIS]; 

    int n_bases;
    struct base bases[N_BASES];

    int n_missoes;
    struct missao missoes[N_MISSOES];

    int n_habilidades;
    struct cjto_t *habilidades;

    int n_compostos_v;

    int n_tamanho_mundo;

    //tempo do mundo
    int relogio; 

    struct fprio_t *lef; 

    //numero de eventos tratados durante a execucao
    int eventos_tratados; 

    //numero de missoes cumpridas durante a execucao
    int missoes_cumpridas; 
};

//estrutura do evento
struct evento {
    //quando acontece o evento
    int tempo; 
    //tipo do evento
    int tipo; 
    //guarda dados do evento (base, heroi, etc)
    int info1; 
    int info2;  
};


int aleat( int min, int max);


int calcula_distancia(struct local a, struct local b);


struct evento *cria_evento(int tempo, int tipo, int info1, int info2);


struct evento *agenda_evento(struct mundo *m, int tempo, int tipo, int info1, int info2);


struct evento *destroi_evento(struct evento *ev);


struct mundo cria_mundo ();


struct heroi cria_heroi (int id);


struct base cria_base(struct mundo *m, int id);


struct missao cria_missao(struct mundo *m, int id);


void eventos_iniciais(struct mundo *m);


void uni_habilidades(struct mundo *m, struct base *b);


void chega(struct mundo *m, int tempo, int heroi, int base);


void espera(struct mundo *m, int tempo, int heroi, int base);


void desiste(struct mundo *m, int tempo, int heroi, int base);


void avisa(struct mundo *m, int tempo, int base);


void entra(struct mundo *m, int tempo, int heroi, int base);


void sai(struct mundo *m, int tempo, int heroi, int base);


void viaja(struct mundo *m, int tempo, int heroi, int base);


void morre(struct mundo *m, int tempo, int heroi, int missao);


void missao(struct mundo *m, int tempo, int missao);


void fim(struct mundo *m);



