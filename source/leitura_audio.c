void leitura(char *palavra)
{

int   n=0;
int   num=0;
int tamanho;
int buffer[sizeof(BLOCO)];

FILE *fp;

if(abre_dev(LEITURA)==0) configura_dev(); //abre o dispositivo no modo leitura e aplica os parametros//


fp=fopen(palavra,"rb");


//calcula o tamanho do arquivo//
fseek (fp, 0, SEEK_END);
tamanho = ftell (fp);
fseek (fp, 0, SEEK_SET);

printf("LENDO: %s - %d bytes\n",palavra,tamanho);

while(n<(tamanho/BLOCO))
{
fread(buffer,BLOCO,1,fp);
if ( (num = write (audiodev, buffer, BLOCO)) < 0 )
{
perror (AUDIO);
exit (-1);
}
n++;
}

close( audiodev ); //fecha e libera o dispositivo//
fclose(fp);
}
