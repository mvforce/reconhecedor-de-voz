void executar_dtw_clicked(GtkWidget *widget)
{
//Tsinal sinal_referencia = {0};  // inicia uma estrutura Tsinal zerada
//Tsinal sinal_amostra = {0}; // inicia uma estrutura Tsinal zerada

//Tsinal *sinal_referencia, *sinal_amostra;
Tsinal sinal_referencia;
//Tsinal *sinal_amostra;
//Tsinal limpadora = {0};

//sinal_referencia = malloc(sizeof(Tsinal));
//sinal_amostra = malloc(sizeof(Tsinal));

//*sinal_referencia =  limpadora;
//*sinal_amostra    =  limpadora;


printf("\n--------------------------  PROCESSANDO REFERENCIA  --------------------------\n");
//printf("%d\n\n",sizeof(Tsinal));
strcpy(sinal_referencia.nome,gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(filechooserbutton_referencia)));
processamento_sinal(&sinal_referencia);

//printf("Calculando cruzmentos por zero...\n");
//cruzamentos_zero(&sinal_referencia);
//printf("Media de cruzamentos por zero do sinal %d \n",sinal_referencia.media_cruzamentos);
if (GTK_TOGGLE_BUTTON (check_leitura)->active)
    {
//  leitura("../dados/referencia");
    }

//printf("\n--------------------------  PROCESSANDO AMOSTRA  --------------------------\n");
//strcpy(sinal_amostra->nome,"../dados/amostra");
//processamento_sinal(sinal_amostra);

//if (GTK_TOGGLE_BUTTON (check_leitura)->active)
    //{
  //leitura("../dados/amostra");
    //}

//printf("\n--------------------------  CALCULANDO DTW  --------------------------\n");


//dtw(sinal_referencia,sinal_amostra);


//free(sinal_referencia);
//free(sinal_amostra);




}
