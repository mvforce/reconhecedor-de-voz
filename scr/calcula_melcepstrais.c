//--------------------------------------------------------------------------+

//autor carlosynoguti
//http://code.google.com/p/hmmtrain/

//  Os coef mel-cepstrais sao obtidos segundo o algoritmo de Mermelstein;
//  os passos a seguir sao:
//
//  - Calculo da FFT das am's pertencentes a janela de analise.
//  - Calculo do quadrado do modulo da FFT (equivale a energia).
//  - Filtragem do modulo acima pelo banco de filtros na escala mel,
//    obtendo-se a energia por filtro.
//  - Calculo do log da energia por filtro.
//  - Calculo da transformada inversa, obtendo-se os coef's mel-cepstrais
//    (esto e' implementado calculando-se a DCT: discrete cosine transform)
//
//  OBS: Lwindow: num de ams/janela; xw: am's janeladas
// +------------------------------------------------------------------------+
void calcmel(Tsinal *sinal)
{
//FILE *fp;
//fp=fopen("MEL.txt","w");

	sinal->ordem =12; // dimensao dos vetores acusticos (12 neste caso)

  // Declaracao das variaveis locais

  int f[26];             // freq chaves dos filtros mel para um banco de filtros fixo
  int i, j, k;               // var auxiliares para loops
  int Nj=18;             // numero de bandas do banco de filtros
  double b;
  float indice;       // var que armazena os novos valores das frequencias (freq. adaptadas) em float
  int *fc;            // freq chaves dos filtros mel para um banco de filtros variável
  double *epb;         // energia/banda e log da energia/banda
  int cont=0;

  f[0]  = 0 ;    f[1]  = 100;   f[2] = 200;     f[3] = 300;    f[4] = 400;
  f[5] = 500;    f[6]  = 600;   f[7] = 700;     f[8] = 800;    f[9] = 900;
  f[10] = 1000;  f[11] = 1149;  f[12] = 1320;   f[13] = 1516;  f[14] = 1741;
  f[15] = 2000;  f[16] = 2297;  f[17] = 2639;   f[18] = 3031;  f[19] = 3482;
  f[20] = 4000;  f[21] = 4595;  f[22] = 5278;   f[23] = 6063;  f[24] = 6964;
  f[25] = 8000;


 for(i=sinal->inicio;i<sinal->fim;i++)
 {
  b=0;
  indice=0;

  // Alocando memoria
  fc = malloc(sizeof(int)*(Nj+2));
  epb = malloc(sizeof(double)*(sinal->n_amostra/2));

  // Definicao das freq chaves que definem as freq centrais e as larguras de banda
  // dos filtros na escala mel.
       for (k=0;k<Nj+2;k++)
       {
        indice =(f[k]*512)/sinal->cabecalho->samplerate;
        fc[k] = floor(indice + 0.5 );
        if (k == (Nj+1)) fc[k] = 223;
        //printf("fc[%d] %d\n",k,fc[k]);
       }




  // Filtragem da FFT pelo banco de filtros na escala mel e calculo do log da
  // saida dos filtros
for(k=0; k<(sinal->n_amostra/2); k++)  epb[k] = 0.0;  // inicializacao

for(k=1; k<=Nj; k++) // k: filtro mel
{
 for(j=1; j<=(fc[k]-fc[k-1]); j++)      // energia da semi-banda inferior + freq central
 epb[fc[k]] += sinal->fft[i][fc[k-1]+j] * (j * 1.0/(fc[k]-fc[k-1]));
 for(j=1; j<(fc[k+1]-fc[k]); j++)       // energia da semi-banda superior

 epb[fc[k]] += sinal->fft[i][fc[k]+j] * (1.0 - j * 1.0/(fc[k+1]-fc[k]));


if(epb[fc[k]] > 0)
epb[fc[k]] = log10(epb[fc[k]]);  // log10 da epb (<math.h>)
else epb[fc[k]] = -100.0;

}

  // Calculo dos coef mel-cepstrais 1 a 12 (Mel_ceps[0] = ganho ==> inutil)
  for(j=1; j<=sinal->ordem; j++) // discrete cosine transform (DCT)
  {
         b = 0.0;               // var aux para calculo dos coef com precissao dupla
        for(k=1; k<=Nj; k++)  b += epb[fc[k]] * cos(j*(k-0.5)*(PI/Nj));
       sinal->mel[i][j-1] = (double) b;   // armazenamento com precissao simples
      // fprintf(fp,"%f;\n",sinal->mel[i][j-1]);

       sinal->X[cont]=sinal->mel[i][j-1];
       cont++;
  }

  // Desalocando ponteiros
 free(fc);
 free(epb);

}



//fclose(fp);

}   // fim da funcao "Calculo_Mel_Cepstrais()"


void deltamel(Tsinal *sinal)
{
int i,j,k;
int adjacente=sinal->ordem;
int n_quadros=sinal->fim-sinal->inicio;
int cont=0;

// Calculando parametros delta
  for (i=sinal->inicio;i<sinal->fim;i++)
  {
    for (j=-adjacente;j<=adjacente;j++)
      if (((i+j)>=0) && ((i+j)<n_quadros))
        for (k=0;k<sinal->ordem;k++)  sinal->delta[i][k] += -(double)j*sinal->mel[i+j][k];
  }

    for (i=sinal->inicio;i<sinal->fim;i++)
    for (j=0;j<sinal->ordem;j++)
    {
    sinal->delta[i][j] /= (2.0*(double)adjacente+1.0);
    //printf("%f\n", sinal->atributo.delta[i][j]);
     sinal->X[cont]=sinal->delta[i][j];
     cont++;
   }


}



void deltadeltamel(Tsinal *sinal)
{
int i,j,k;
int adjacente=sinal->ordem;
int n_quadros=sinal->fim-sinal->inicio;
int cont=0;

// Calculando parametros delta delta
  for (i=sinal->inicio;i<sinal->fim;i++)
  {
    for (j=-adjacente;j<=adjacente;j++)
      if (((i+j)>=0) && ((i+j)<n_quadros))
        for (k=0;k<sinal->ordem;k++)  sinal->deltadelta[i][k] += -(double)j*sinal->delta[i+j][k];
  }

    for (i=sinal->inicio;i<sinal->fim;i++)
    for (j=0;j<sinal->ordem;j++)
    sinal->deltadelta[i][j] /= (2.0*(double)adjacente+1.0);
    //printf("i %d %f\n",i, sinal->atributo.deltadelta[i][j]);
    sinal->X[cont]=sinal->deltadelta[i][j];
     cont++;
}
