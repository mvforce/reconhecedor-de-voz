void fft(Tsinal *sinal) //retorna um vetor com tamanho N de amostras double//
{
int i,j;

fftw_complex *dados_fft; // guarda os dados de saida da fft//
fftw_complex *in; // armazena os dados de entrada da fft //
fftw_plan p; //plano para execuçao da fft//

// faz a alocacao de memoria para as variaveis//
dados_fft = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * (sinal->n_amostra));
in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * (sinal->n_amostra));

//monta o plano com os dados//
p = fftw_plan_dft_1d(sinal->n_amostra,in,dados_fft, FFTW_FORWARD, FFTW_ESTIMATE );

for(i=sinal->inicio;i<sinal->fim;i++)
{
//grava na parte real da variavel in os valores do sinal //
// zera a parte imaginaria da variavel in pois nao existe//
for(j=0;j<sinal->n_amostra;j++)
{
in[j][0] = sinal->recorte[i][j];
in[j][1] = 0.0;
}


//executa a fft//
fftw_execute(p);

// Calculo do quadrado do modulo da FFT (equivale a energia).
for(j=0;j<(sinal->n_amostra/2);j++)
{
dados_fft[j][1]=fabs(dados_fft[j][1]); //modulo//
dados_fft[j][1]=pow(dados_fft[j][1],2); // eleva ao quadrado//
sinal->fft[i][j]=dados_fft[j][1];

}




}
fftw_free(in);
fftw_free(dados_fft);
fftw_destroy_plan(p);
}
