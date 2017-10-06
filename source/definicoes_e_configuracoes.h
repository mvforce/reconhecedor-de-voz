// Este arquivo contem algumas definicoes e configuracoes
// que sao utilizadas no programa


// --------------------------  configuracoes do audio  utilizado  --------------------- //
//#define AUDIO "/dev/dsp"
//#define BITS 8 // 8 bits
//#define CANAIS 1 //mono//
//#define TX_AMOSTRAGEM  8000 // frequencia 8 Khz//

// ----------------------------------  configuracoes sobre o arquivo resultante ----------------//
//#define BLOCO (BITS*CANAIS) // tamanho do bloco a ser lido
//#define TEMPO 3 //tempo total do arquivo em segundos//
//#define TAMANHO_TOTAL (TX_AMOSTRAGEM * TEMPO)// tamnaho total do arquivo //
//#define JAN (TAMANHO_TOTAL / N_AMOSTRAS) // quantidade de janelas 300 janelas de 10 mili //

//#define N_AMOSTRAS ( TX_AMOSTRAGEM / 100 ) // quantidade de amostras por janela - 80 //
//#define N_AMOSTRAS ( TX_AMOSTRAGEM / 20 ) // quantidade de amostras por janela - 400 //

//// --------------------------------------- modos utilizados no programa ------------------------//
//#define LEITURA 0 //modo leitura//
//#define ESCRITA 1 //modo escrita //

//#define REFERENCIA 1
//#define AMOSTRA 0

//// -----------------------------------  fatores determinados utilizados no programa ---------------//

//#define fator_L_min 0.985//fator que define L_min ( limiar minimo de silencio)
//#define fator_L_max 1.01// fator que define L_max ( limiar maximo de silencio)
//#define TAM_SILENCIO (4 * N_AMOSTRAS) //tamanho do silencio  4  janelas


//  ---------------------------- valor de PI ----------------------------------------------------------//
double PI=3.141592653589793238462643; //valor de PI //

