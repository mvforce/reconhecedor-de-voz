

void dtw(Tsinal *sinal_referencia, Tsinal *sinal_amostra)
{
//calcula o tamanho dos arrays //
int tamanho_amostra = (sinal_amostra->fim - (sinal_amostra->inicio)) * sinal_amostra->ordem;
int tamanho_modelo =  (sinal_referencia->fim - (sinal_referencia->inicio)) * sinal_referencia->ordem;


printf("tamanho modelo %d\ntamanho amostra %d\n",tamanho_modelo,tamanho_amostra);

int i,j,a,b;
int iteracao;


double **D;
double **DA;
double **MOV;
double **W;
double somaw;
double distancia;

D =   malloc(sizeof(double)*tamanho_amostra);
DA =  malloc(sizeof(double)*tamanho_amostra);
MOV = malloc(sizeof(double)*tamanho_amostra);
W =   malloc(sizeof(double)*tamanho_amostra);


for(i=0;i<tamanho_amostra;i++)
{
D[i] =   malloc(sizeof(double)*tamanho_modelo);
DA[i] =  malloc(sizeof(double)*tamanho_modelo);
MOV[i] = malloc(sizeof(double)*tamanho_modelo);
W[i] =   malloc(sizeof(double)*tamanho_modelo);
}


//  zera variaveis//
somaw=0.0;
distancia=0.0;
for(i=0;i<tamanho_amostra;i++)
for(j=0;j<tamanho_modelo;j++)
{

D[i][j]=0.0;
DA[i][j]=0.0;
MOV[i][j]=0.0;
W[i][j]=0.0;
}


//printf("Calculando o array de Distancia\n");

//---------------------------------  Calculando o array de Distancia ---------------------------------//
for(i=0;i<tamanho_amostra;i++)
for(j=0;j<tamanho_modelo;j++)
{
D[i][j]=fabs(sinal_referencia->X[i] - sinal_amostra->X[j]);
}


//--------------------------------- Calculando  de DISTANCIA ACUMULADA ---------------------------------//


//calcula a primeira linha e primeira coluna em um loop a parte//
DA[0][0]=1;

j=0;
for(i=1;i<tamanho_amostra;i++)
{
DA[i][j]=DA[i-1][j]+D[i][j];
MOV[i][j]=3;

}

i=0;
for(j=1;j<tamanho_modelo;j++)
{
DA[i][j]=DA[i][j-1]+D[i][j];
MOV[i][j]=1;

}

//variaveis para auxiliar no loop. //
iteracao=0;
a=1;
b=1;

//---- Calcula o resto da matriz ----//
for(i=1;i<tamanho_amostra;i++)
for(j=1;j<tamanho_modelo;j++)
{
// caso menor seja esquerda//
if(DA[i-1][j]<DA[i][j-1]  && DA[i-1][j] < DA[i-1][j-1] )
{
MOV[i][j]=3;
DA[i][j]=DA[i-1][j]+D[i][j];


}
//caso seja abaixo//
else if(DA[i][j-1]<DA[i-1][j-1] && DA[i][j-1]<DA[i-1][j])
{
MOV[i][j]=1;
DA[i][j]=DA[i][j-1]+D[i][j];

}
// caso seja diagonal//
else
{
MOV[i][j]=2;
DA[i][j]=DA[i-1][j-1]+D[i][j];

}

} // fim do loop de calculo //



// ---------------------------------   Calculando Best Path   ---------------------------------//
        int IntPosLinAtual =tamanho_amostra-1;
        int IntPosColAtual = tamanho_modelo-1;
        int IntPosLinFinal = 0;
        int IntPosColFinal = 0;
        int IntValorAtual  =  1;
        double X1, X2, X3;
  //Zerando a Matriz de movimento para o calculo //
        for (i=0; i<tamanho_amostra;i++)
            for (j=0; j<tamanho_modelo; j++)
            {
                MOV[i][j] = 0;
            }


        W[IntPosLinAtual][IntPosColAtual] = 1;
        //printf("DA[%d][%d] %d----------\n",IntPosLinAtual,IntPosColAtual, DA[IntPosLinAtual][IntPosColAtual]);
        distancia+= DA[IntPosLinAtual][IntPosColAtual];
        somaw+=1;

        while ((IntPosLinAtual != IntPosLinFinal) && (IntPosColAtual != IntPosColFinal))
        {

            //Esquerda
            X1 = MOV[IntPosLinAtual-1][IntPosColAtual];
            //Direita
            X2 = MOV[IntPosLinAtual-1][IntPosColAtual-1];
            //Baixo
            X3 = MOV[IntPosLinAtual][IntPosColAtual-1];
            if ((X1<X2) && (X1<X3)){
                IntPosColAtual+=1;
            }
            else if ((X3<X1) && (X3<X2)){
                IntPosLinAtual-=1;
            }
            else {
                IntPosColAtual-=1;
                IntPosLinAtual-=1;
            }
            W[IntPosLinAtual][IntPosColAtual] = 1;
          //  printf("DA[%d][%d] %d\n",IntPosLinAtual+1,IntPosColAtual+1, DA[IntPosLinAtual+1][IntPosColAtual+1]);
            distancia+= DA[IntPosLinAtual+1][IntPosColAtual+1];
            somaw+=1;
                  }
        W[IntPosLinFinal][IntPosColFinal] = 1;
        distancia+= DA[IntPosLinAtual+1][IntPosColAtual+1];
        somaw+=1;
       // printf("DA[%d][%d] %d\n",IntPosLinFinal+1,IntPosLinFinal, DA[IntPosLinFinal+1][IntPosColFinal+1]);




//// ---------------------------------   EXIBE AS MATRIZES RESULTANTES   ---------------------------------//


//// ----------- exibe tabela  D -----------//
//printf(" ---------------------- D -------------------------\n");
//for(j=tamanho_modelo-1;j>=0;j--)
//{
//printf("\n");
//for(i=0;i<tamanho_amostra;i++)
//{
//printf("%2f ",D[i][j]);
//}
//}


//// ----------- exibe tabela DA-----------//
//printf("\n ---------------------- DA -------------------------\n");
//for(j=tamanho_modelo-1;j>=0;j--)
//{
//printf("\n");
//for(i=0;i<tamanho_amostra;i++)
//{
//printf("%2f ",DA[i][j]);
//}
//}



//// ----------- exibe tabela MOV-----------//
//printf("\n ---------------------- MOV -------------------------\n");
//for(j=tamanho_modelo-1;j>=0;j--)
//{
//printf("\n");
//for(i=0;i<tamanho_amostra;i++)
//{
//printf("%2f ",MOV[i][j]);
//}
//}


//// ----------- exibe tabela W  best path-----------//
//printf("\n ---------------------- W best path -------------------------\n");
//for(j=tamanho_modelo-1;j>=0;j--)
//{
//printf("\n");
//for(i=0;i<tamanho_amostra;i++)
//{
//printf("%2f ",W[i][j]);
//}
//}
double div=somaw/distancia;
printf("\nDistancia: %f\nSomaw: %f\nfator: %f \n",distancia,somaw, 100*div);

free(D);
free(DA);
free(MOV);
free(W);


printf("\n");
}
