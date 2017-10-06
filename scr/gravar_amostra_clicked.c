void gravar_amostra_clicked(GtkWidget *widget)
{
if( gravar("../dados/amostra",AMOSTRA) == 0)
{
printf("\nAudio de amostra gravado com sucesso !");
}


}
