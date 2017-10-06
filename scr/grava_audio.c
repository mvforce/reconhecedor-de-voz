int gravar(char *palavra, int tipo)
{

	if(abre_dev(ESCRITA)==0) configura_dev(); //abre o dispositivo no modo gravacao e aplica os parametros//

    int numLeft;
    unsigned char *ptr;
    int num;
    unsigned char buffer[TAMANHO_TOTAL];
    FILE *fp;

   fp=fopen(palavra,"wb");

    printf("\nGRAVANDO %s ...\n",palavra);
   usleep(100);
    numLeft = TAMANHO_TOTAL;
    ptr = buffer;
    while( numLeft >= BLOCO )
    {

        if ( (num = read (audiodev, ptr, BLOCO)) < 0 )
        {
            perror (AUDIO);
            exit (-1);
        }
        else
        {
 	fwrite(ptr,BLOCO,1,fp);
	ptr += num;
    numLeft -= num;
        }
    }
fclose(fp);
close( audiodev ); //fecha e libera o dispositivo//


//------------------------------------  realiza o loop que armazena os dados byte a byte do array  Y referencia ---------------------------------//
if(tipo==REFERENCIA)
{
printf("Armazendo o audio em array...\n");
int i;
signed char  sbyte;
 fp=fopen("../dados/referencia","r");
 for(i=0;i<TAMANHO_TOTAL;i++)
{
fread(&sbyte, 1, 1, fp); // lê byte a byte//
Yreferencia[i]=(int)sbyte;
}
fclose(fp);
}
//---------------------------------------------------------------------------------------------------------------------------------//


//------------------------------------  realiza o loop que armazena os dados byte a byte do array  Y amostra---------------------------------//
if(tipo==AMOSTRA)
{
printf("Armazendo o audio em array...\n");
int i;
signed char  sbyte;
 fp=fopen("../dados/amostra","r");
 for(i=0;i<TAMANHO_TOTAL;i++)
{
fread(&sbyte, 1, 1, fp); // lê byte a byte//
Yamostra[i]=(int)sbyte;
}
fclose(fp);
}
//---------------------------------------------------------------------------------------------------------------------------------//


return(0);

}



