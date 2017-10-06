//------------------------------------------------------------------------------//
// Funcao que realiza as chamadas para o processamento do sinal
//  recebe como parametro o caminho para o arquivo a ser aberto
// como o processamento_sinal(Tsinal *sinal) já é o endereco, nas proximas
// vezes uso apenas funcao(sinal)

void processamento_sinal(Tsinal *sinal)
{
FILE *fp; // descritor para o arquivo
int i,j,k; // variaveis auxiliares


//strcpy(sinal->nome,arquivo); // para o caminho do arquivo para a estrutura
fp=fopen(sinal->nome,"rb"); // abre o arquivo

sinal->cabecalho =(Twav *) malloc(sizeof(Twav));
fread(sinal->cabecalho,sizeof(Twav),1,fp);

//unsigned char *data;
//data = (unsigned char *) malloc(sinal->cabecalho->subchunk2size);

//aloca os bytes do arquivo
sinal->bytes = (signed char *)malloc(sizeof(signed char) * (sinal->cabecalho->subchunk2size));

fread(sinal->bytes,(sinal->cabecalho->subchunk2size),1,fp);

//for(i=0;i<sinal->cabecalho->subchunk2size;i++) sinal->bytes[i] = (signed char)data[i];

i=0;
fclose(fp);
//free(data);


// -- calcula numero de janelas e amostras por janela -- //
sinal->n_amostra  = sinal->cabecalho->samplerate / 100;  // janelas de 10 milissegundos

sinal->n_janela = sinal ->cabecalho->subchunk2size / sinal->n_amostra;

//if((sinal ->comprimento % sinal->n_amostra) !=0) sinal->n_janela++;

// -- aloca o memoria para a struct de sinal -- //
//sinal->n_janela = malloc(sizeof(int));
//sinal->n_amostra =malloc(sizeof(int));
//sinal->ordem =    malloc(sizeof(int));
//sinal->inicio =   malloc(sizeof(int));
//sinal->fim =      malloc(sizeof(int));
//sinal->Lmax =     malloc(sizeof(int));
//sinal->Lmin =     malloc(sizeof(int));
//sinal->media_cruzamentos = malloc(sizeof(int));;


sinal->janelado = (int **)malloc(sizeof(int) * sinal->n_janela);

sinal->energia = (double **)malloc(sizeof(double) * sinal->n_janela);

sinal->recorte = (int **)malloc(sizeof(int) * sinal->n_janela);

sinal->cruzamentos = (int *) malloc(sizeof(int) * sinal->n_janela);

sinal->trecho_valido = (int *) malloc(sizeof(int) * sinal->n_janela);

sinal->fft = (double **)malloc(sizeof(double) * sinal->n_janela);

sinal->mel = (double **)malloc(sizeof(double) * sinal->n_janela);

sinal->delta = (double **)malloc(sizeof(double) * sinal->n_janela);

sinal->deltadelta = (double **)malloc(sizeof(double) * sinal->n_janela);

sinal->X = (double *)malloc(sizeof(double) * sinal->cabecalho->subchunk2size);

for(i=0;i<sinal->n_janela;i++)
{
	sinal->janelado[i] = (int *)malloc(sizeof(int) * sinal->n_amostra);

	sinal->energia[i] = (double *)malloc(sizeof(double) * sinal->n_amostra);

	sinal->recorte[i] = (int *)malloc(sizeof(int) * sinal->n_amostra);

	sinal->fft[i] = (double *)malloc(sizeof(double) * sinal->n_amostra);

	sinal->mel[i] = (double *)malloc(sizeof(double) * sinal->n_amostra);

	sinal->delta[i] = (double *)malloc(sizeof(double) * sinal->n_amostra);

	sinal->deltadelta[i] = (double *)malloc(sizeof(double) * sinal->n_amostra);

}


printf("Alocando o sinal em janela...\n");
printf("Calculando energia do sinal->..\n");
// aloca as energias em janelas//
FILE *saida = fopen("ENERGIA.txt","w");
k=0;
for(i=0;i<sinal->n_janela;i++)
for(j=0;j<sinal->n_amostra;j++)
{
sinal->energia[i][j]=pow((int)sinal->bytes[k],2);

fprintf(saida,"%d\t%d\t%f\n",k,sinal->bytes[k],sinal->energia[i][j]);

sinal->janelado[i][j]=sinal->bytes[k];
sinal->recorte[i][j]=sinal->janelado[i][j];
k++;
}
fclose(saida);

// chama as funções que tratam o sinal de audio//

printf("Calculando cruzmentos por zero...\n");
cruzamentos_zero(sinal);
printf("Media de cruzamentos por zero do sinal %d \n",sinal->media_cruzamentos);

// se a opcao de usar o proprio limiar estiver desabilitada//
//if (!GTK_TOGGLE_BUTTON (check_limiar_silencio)->active)
//{
//printf("Calculando limiares de energia...\n");
//calcula_limiar(sinal);
//printf("LMAX %d- LMIN %d\n",sinal->Lmax,sinal->Lmin);

// aplica os valores obtidos ao adjustamnet//
//gtk_adjustment_set_value (ajuste_limiar_max,sinal->Lmax);
//gtk_adjustment_set_value (ajuste_limiar_min,sinal->Lmin);
//}
//else  // caso o controle esteja marcado ele repassa os valores para as variaveis
//{
//sinal->Lmax =  gtk_adjustment_get_value(ajuste_limiar_max);
//sinal->Lmin =  gtk_adjustment_get_value(ajuste_limiar_min);
//}

//printf("Detectado endpoints...\n");
//endpoint(sinal);
//printf("inicio:%d - fim %d\n\n",sinal->inicio,sinal->fim);

//printf("Recortando sinal->..\n");
//recorta_sinal(sinal);

//// se estiver marcada a opcao de pre enfase
//if (GTK_TOGGLE_BUTTON (check_pre_enfase)->active)
//{
//printf("Aplicando pre-enfase no sinal->..\n");
//pre_enfase(sinal);
//}

//// se estiver marcada a opcao de remove DC
//if (GTK_TOGGLE_BUTTON (check_remove_dc)->active)
//{
//printf("Removendo componente DC ...\n");
//remove_componente_dc(sinal);
//}

//// se estiver marcada a opcao de janela hamming
//if (GTK_TOGGLE_BUTTON (check_janela_hamming)->active)
//{
//printf("Aplicando janela de Hamming...\n");
//janela_hamming(sinal);
//}


//printf("Extraindo atributos do sinal\n");
//extrai_atributos(sinal);



}

// funcao que calcula os cruzamentos por zero do sinal de audio
// recebe como parametro uma estrutura Tsinal e retorna o mesmo tipo
void cruzamentos_zero(Tsinal *sinal)
{
int i,j;
int total=0;

//compara os segmentos em busca de cruzamentos por zero//
for(i=0;i<sinal->n_janela;i++)
for(j=0;j<sinal->n_amostra;j++)
{
if(sinal->janelado[i][j]<0) // usa o segmento_puro e não energia.. pois energia é ao quadrado, sempre positiva//
{
while(sinal->janelado[i][j] < 0) j++;
sinal->cruzamentos[i]++;
}

else if(sinal->janelado[i][j] > 0)
{
while(sinal->janelado[i][j] > 0) j++;
sinal->cruzamentos[i]++;
}

}
// faz um loop no total de janelas para gerar um total de cruzamentos
for(i=0;i<sinal->n_janela;i++)
{
	total += sinal->cruzamentos[i];
}
// calcula a media aritimetica dos cruzamentos
sinal->media_cruzamentos = total/sinal->n_janela;


}


//------------------------------------------------------------------------------//
// Calcula os limiares maximo e minimo de silencio para detecção de endpoints//
// nivel de ruido medio do ambiemte
void calcula_limiar(Tsinal *sinal)
{
//---- inicializa as variaveis globais --- //

int JAN_SILENCIO = 4;

int TAM_SILENCIO = JAN_SILENCIO * sinal->n_amostra;


int i,j;
double media=0;
double soma_quad_desvios=0;
double desvio_padrao=0;
double desvios[TAM_SILENCIO];
double quad_desvios[TAM_SILENCIO];


//calcula a media//
media=0.0;
for(i=0;i<JAN_SILENCIO;i++) // utilizado 4, pois assumi que o tamanho medio de silencio é de 40 mili, logo 4 janelas de 10 mili
for(j=0;j<sinal->n_amostra;j++) media=sinal->energia[i][j] + media;

media=media / TAM_SILENCIO;

//calcula os desvios//
for(i=0;i<JAN_SILENCIO;i++)  // utilizado 4, pois assumi que o tamanho medio de silencio é de 40 mili, logo 4 janelas de 10 mili
for(j=0;j<sinal->n_amostra;j++) desvios[i]=sinal->energia[i][j]-media;

// calcula quadrado dos desvios//
for(i=0;i<=TAM_SILENCIO;i++) quad_desvios[i]=desvios[i]*desvios[i];

// calcula variancia//
soma_quad_desvios=0.0;
for(i=0;i<=TAM_SILENCIO;i++) soma_quad_desvios=soma_quad_desvios+quad_desvios[i];

soma_quad_desvios= soma_quad_desvios / TAM_SILENCIO;
desvio_padrao=sqrt(soma_quad_desvios);

sinal->Lmin=(int)(media-desvio_padrao);
sinal->Lmax=(int)(media+desvio_padrao);

//sinal->Lmin = sinal->Lmin * fator_L_min;
//sinal->Lmax = sinal->Lmax * fator_L_max;

}



//------------------------------------------------------------------------------//
// Calcula os endpoints do sinal usando as energias //
void endpoint(Tsinal *sinal)
{
	int i,j,jfim;
	int pontos=0;
    int ultimo_trecho=0;

sinal->inicio=0;
sinal->fim=sinal->n_janela;

for(i=4;i<sinal->n_janela;i++)  // pois o TAM_SILENCIO  4 * sinal->n_janela
{
pontos=0;
if(sinal->cruzamentos[i] > (sinal->media_cruzamentos * 0.3) + sinal->media_cruzamentos) // 30%
{
for(j=0;j<sinal->n_amostra-2;j++)
{
if(sinal->energia[i][j] < sinal->Lmin || sinal->energia[i][j] > sinal->Lmax)
{
	pontos++;
    if(pontos > (sinal->n_amostra * 0.05)) // se o numero de pontos for maior que 5% da janela
    {
		j=sinal->n_amostra; // encerra o loop
	    sinal->trecho_valido[i]=1; // assume como techo valido
	}
}

}
}
}

for(i=0;i<sinal->n_janela-1;i++)
if(sinal->trecho_valido[i]   > 0 &&  // para determinar o inicio e o fim  ele pega apenas segmentos
   sinal->trecho_valido[i+1] > 0 )  // continuos de audio
{
printf("%d ",i);
if(sinal->inicio == 0) sinal->inicio=i;
sinal->fim=i;
}
printf("\n");

}


//---------------------------------------------------------------------------
// Funcao que aplica uma pre enfase do sinal de voz//
void pre_enfase(Tsinal *sinal)
{
double CP=0.95;
int i,j;

for(i=sinal->inicio;i<sinal->fim;i++)
for(j=1;j<sinal->n_amostra;j++)  sinal->recorte[i][j] = sinal->janelado[i][j] - (CP * sinal->janelado[i][j-1]);

}



//---------------------------------------------------------------------------
// Funcao que remove a componente DC de um sinal de voz
void remove_componente_dc(Tsinal *sinal)
{
  double dc = 9.5;
  int i,j;

for(i=sinal->inicio;i<sinal->fim;i++)
for(j=0;j<sinal->n_amostra;j++)   dc += sinal->recorte[i][j];

dc /= (double)sinal->n_amostra;

for(i=sinal->inicio;i<sinal->fim;i++)
for(j=0;j<sinal->n_amostra;j++)  sinal->recorte[i][j] -= dc;

}


//------------------------------------------------------------------------------
// Aplica uma janela de Hamming ao sinal
void janela_hamming(Tsinal *sinal)
{
  double a; // var aux para o calculo da janela
  int i,j; // contador

  // Calculando parametros
for(i=sinal->inicio;i<sinal->fim;i++)
for(j=0;j<sinal->n_amostra;j++)
  {
    a = (2 * PI * j) / (sinal->n_amostra - 1);
    sinal->recorte[i][j] *= (0.54 - 0.46 * cos(a));
  }

}



//---------------------------------------------------------------------------
// Funcao que grava os atributos de um sinal de voz//
void recorta_sinal(Tsinal *sinal)
{
int i,j,margem;

//int k=0;
//FILE *arq_recorte;
//arq_recorte=fopen(sinal->nome,"wb");
//signed char sbyte;

//trecho da funcao que aplica 10 % de margem//

margem=(sinal->fim-sinal->inicio) * 0.1;
if((sinal->inicio-margem) >= 0)          sinal->inicio = sinal->inicio-margem;
if((sinal->fim+margem) <= sinal->n_janela)           sinal->fim = sinal->fim+margem;

//for(i=sinal->inicio;i<sinal->fim;i++)
//for(j=0;j<sinal->n_amostra;j++)
//{
//fwrite(&sinal->bytes[k], 1, 1, arq_recorte);
//sbyte = (signed char)sinal->recorte[i][j]; //coloca o sinal janelado para ficar audivel
//fwrite(&sbyte, 1, 1, arq_recorte);
//}
//fclose(arq_recorte);

}


//---------------------------------------------------------------------------
// Funcao chama rotina para extrair os atributos de um sinal de voz//
void extrai_atributos(Tsinal *sinal)
{

printf("Calculando a transformada Rapida de Fourier(FFTW3) no sinal->..\n");
fft(sinal);

printf("Calculando os coeficientes Mel Cepstrais...\n");
calcmel(sinal);

// se estiver marcada a opcao CALCULO DELTA
if (GTK_TOGGLE_BUTTON (check_delta)->active)
{
printf("Calculando os coeficientes Delta Mel Cepstrais...\n");
deltamel(sinal); //derivada primeira//
}

// se estiver marcada a opcao CALCULO DELTA DELTA
if (GTK_TOGGLE_BUTTON (check_delta_delta)->active)
{
printf("Calculando os coeficientes Delta Delta Mel Cepstrais...\n");
deltadeltamel(sinal); //derivada segunda//
}

}

