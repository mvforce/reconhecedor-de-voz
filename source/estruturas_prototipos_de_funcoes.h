// Este arquivo contem todas as declaracoes
// das estruturas de dados  e prototipos das funcoes
// que sao utilizadas no programa

typedef struct
{

//-------------------- RIFF chunk descriptor (16 bytes) --------------------//
unsigned char chunkid[4];
unsigned int  chunksize;
unsigned char format[4];
//-------------------- FMT sub-chunk (24 bytes) --------------------//
unsigned char subchunk1id[4];
unsigned int  subchunk1size;
short int     audioformat;
short int     numchannels;
unsigned int  samplerate;
unsigned int  byterate;
short int     blockalign;
short int     bitspersample;

//-------------------- DATA sub-chunk (4 + data bytes) --------------------//
unsigned char subchunk2id[4];
unsigned int  subchunk2size;

}Twav;


typedef struct
{

signed char *bytes;
int **janelado;
double **energia;
int **recorte;
int *cruzamentos;
int *trecho_valido;

double **fft;

double **mel;
double **delta;
double **deltadelta;
double *X;

int n_janela;
int n_amostra;
int ordem;
int inicio;
int fim;
int Lmax;
int Lmin;
char *nome;
int media_cruzamentos;

Twav *cabecalho;




//Tatributo atributo;
}Tsinal;



//prototipos das funcoes //
void getch(void); //função que simula a getch() original//
void tela_principal();
void treinamento();

int abre_dev(int modo);
void configura_dev();
int gravar(char *palavra,int tipo);
void leitura(char *palavra);

void processamento_sinal(Tsinal *sinal);
void cruzamentos_zero(Tsinal *sinal);
void calcula_limiar(Tsinal *sinal);
void endpoint(Tsinal *sinal);
void pre_enfase(Tsinal *sinal);
void remove_componente_dc(Tsinal *sinal);
void janela_hamming(Tsinal *sinal);
void recorta_sinal(Tsinal *sinal);
void extrai_atributos(Tsinal *sinal);
void fft(Tsinal *sinal);
void calcmel(Tsinal *sinal );
void deltamel(Tsinal *sinal);
void deltadeltamel(Tsinal *sinal);
int dicionario_adiciona(Tsinal *sinal);
void dtw(Tsinal *sinal_referencia,Tsinal *sinal_amostra);
