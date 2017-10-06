
#include <stdio.h>
#include <stdlib.h>

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




void leitura_arquivo(char *nome);

int main(int argc, char **argv)
{
leitura_arquivo("eu.wav");

	return 0;
}




void leitura_arquivo(char *nome)
{
FILE *fp;
fp=fopen(nome,"rb");
if(!fp)
{
	printf("Erro ao tentar abrir o arquivo:\n%s\n",nome);
	exit(1);
}
Twav *cabecalho;

cabecalho =(Twav *) malloc(sizeof(Twav));

fread(cabecalho,sizeof(Twav),1,fp);

//printf("arquivo: %s\n\n",nome);

//printf("chunkid -> %.4s\n",cabecalho->chunkid);
//printf("chunksize -> %d\n",cabecalho->chunksize);
//printf("format -> %.4s\n",cabecalho->format);
//printf("-----------------------\n");

//printf("subchunk1id -> %.4s\n",cabecalho->subchunk1id);
//printf("subchunk1size -> %d\n",cabecalho->subchunk1size);
//printf("audioformat -> %d\n",cabecalho->audioformat);
//printf("numchannels -> %d\n",cabecalho->numchannels);
//printf("samplerate -> %d\n",cabecalho->samplerate);
//printf("byterate -> %d\n",cabecalho->byterate);
//printf("blockalign -> %d\n",cabecalho->blockalign);
//printf("bitspersample -> %d\n",cabecalho->bitspersample);
//printf("-----------------------\n");

//printf("subchunk2id -> %.4s\n",cabecalho->subchunk2id);
//printf("subchunk2size -> %d\n",cabecalho->subchunk2size);



unsigned char *data;
data = (unsigned char *) malloc(cabecalho->subchunk2size);

fread(data,cabecalho->subchunk2size,1,fp);

int i;
signed char byte;
int count=0;

for(i=0;i<cabecalho->subchunk2size;i++)
{

if(count==99)
{
byte = (signed char)data[i];
printf("%d %d\n",i,byte);
count=0;
}
else
{
count++;
}


}

free(cabecalho);
free(data);
fclose(fp);
}



