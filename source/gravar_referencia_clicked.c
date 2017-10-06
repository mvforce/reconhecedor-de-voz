void gravar_referencia_clicked(GtkWidget *widget)
{
if( gravar("../dados/referencia", REFERENCIA) == 0)
{
printf("\nAudio de referencia gravado com sucesso !");
}


}


