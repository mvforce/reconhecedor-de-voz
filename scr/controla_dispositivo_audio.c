int abre_dev(int modo)
{
if(modo==ESCRITA)
{
 /* Abre dispositivo de audio */
    audiodev = open (AUDIO, O_RDONLY, 0);
    if (audiodev == -1)
    {
        perror (AUDIO);
        exit (-1);
    }
}
if(modo==LEITURA)
{
/* Abre dispositivo de audio */
    audiodev = open (AUDIO, O_WRONLY, 0);
    if (audiodev == -1)
    {
        perror (AUDIO);
        exit (-1);
    }
}
return(0);
}

void configura_dev()
{
int samplesize;
int channels;
int sample_rate;

 /* configura amostras de 8 bits. */
    samplesize = BITS;
    ioctl(audiodev, SNDCTL_DSP_SAMPLESIZE, &samplesize);
    if (samplesize != BITS)
    {
        perror("erro ao definir tamanho das amostras");
        exit(-1);
    }
 /* configura o numero de canais */
  channels=CANAIS;
  ioctl(audiodev, SNDCTL_DSP_CHANNELS, &channels);
 if (channels != CANAIS)
    {
        perror("erro ao definir numero de canais");
        exit(-1);
    }
 /* configura a taxa de amostragem */
  sample_rate = TX_AMOSTRAGEM;
  if (ioctl (audiodev, SNDCTL_DSP_SPEED, &sample_rate) == -1)
    {
      perror ("SNDCTL_DSP_SPEED");
      exit (-1);
    }
}
