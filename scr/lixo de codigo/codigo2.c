/*
 * Compile me with:
 *   gcc -o tut tut.c $(pkg-config --cflags --libs gtk+-2.0 gmodule-2.0 cairo)
 */

#include <gtk/gtk.h>
#include <cairo.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>
#include <unistd.h>
#include <math.h>
#include <gtkdatabox.h>


 //------------------------------------ inclusao de arquivos proprios -------------------------------------//

#include "def_cores.h"   // definicao das cores usadas nos graficos

 //------------------------------------ variaveis globais utilizadas -------------------------------------//

GtkBuilder *builder;  // utilizado como descritor do arquivo de interface

GdkGC *estilo_linha_grade; // estilo utilizado na grade
GdkGC *estilo_fundo; // estilo utilizado no fundo da drawingarea
GdkGC *estilo_linha_referencia; // estilo utilizado na linha do grafico de referencia
GdkGC *estilo_linha_amostra;// estilo utilizado na linha do grafico de amostra

int TAMANHO_TOTAL = 8000;
int X[24000];  //array com os valores do eixo X
int Y[24000]; //array com os valores do eixo Y
int ESPACO_GRADE=50;
 //------------------------------------ definicao de funcoes -------------------------------------//
gint timeout_callback(gpointer data); //
void desenha_grade(GtkWidget *widget);
void Cria_Estilos(GtkWidget *widget);
void desenha_grafico_referencia(GtkAdjustment *adj);

//----------------------------------------------------funcao principal ------------------------------------------------//
int main( int    argc,      char **argv )
{


    GError     *error = NULL; // utilizada para tratar
     FILE *fp; // descritor para o arquivo que contem os bytes

    gtk_init( &argc, &argv ); //inicia o GTK

    builder = gtk_builder_new(); // cria um descritor para a ponte interface/codigo

   gtk_builder_add_from_file( builder, "../interface/interface.glade", NULL );  // indica qual arquivo esta vinculado

 // pega a janela principal e vincula a variavel window
      GtkWidget  *window = GTK_WIDGET( gtk_builder_get_object( builder, "window" ) );

 // pega a drawingarea e vincula a variavel drawingarea
   GtkWidget *drawingarea = GTK_WIDGET (gtk_builder_get_object (builder,"drawingarea"));

    gtk_builder_connect_signals( builder, NULL ); //conecta todos os sinais "callbacks"


//------------------------------------  realiza o loop que armazena os dados ---------------------------------//
int i;
 fp=fopen("../bin/saida.txt","r");
 for(i=0;i<TAMANHO_TOTAL;i++)
{
  fscanf(fp,"%d %d",&X[i],&Y[i]);
}
//---------------------------------------------------------------------------------------------------------------------------------//


// Mostra todos os objetos contidos em window
gtk_widget_show( window );

// chama a funcao que cria os estilos dos graficos
Cria_Estilos(drawingarea);

//funcao que desenha o grafico de referencia na posicao inicial
desenha_grade(drawingarea);

gtk_main(); // inicia o loop principal

    return( 0 );
}

// funcao que sai do loop principal de eventos, finalizando o programa
void sair(GtkObject *object, gpointer user_data)
{
g_object_unref( G_OBJECT( builder ) ); // funcao que destroi o descritor builder
gtk_main_quit(); // encerra o loop principal
}


//funcao responsavel por desenhar a grade no drawnigarea
void desenha_grade(GtkWidget *widget)  // recebe um drawnigarea
{
int width = widget->allocation.width;  // aloca a largura
int height = widget->allocation.height; // aloca a altura

int a;

for(a=1;a<width/ESPACO_GRADE;a++)
{
gdk_draw_line(widget->window, estilo_linha_grade, a*ESPACO_GRADE, 0,a*ESPACO_GRADE,height);
}

for(a=1;a<height/ESPACO_GRADE;a++)
{
	gdk_draw_line(widget->window, estilo_linha_grade, 0,a*ESPACO_GRADE, width, a*ESPACO_GRADE);
}

}



void desenha_grafico_referencia(GtkAdjustment *adj)
{

int x = (int) adj->value;
GtkWidget *widget = GTK_WIDGET (gtk_builder_get_object (builder,"drawingarea"));

  int width = widget->allocation.width;
  int height = widget->allocation.height;
  int i;
  int last_y;
  int y;
  int resolucao = 1024;
  int deslocamento = TAMANHO_TOTAL*0.01; // 1% do deslocamento // setado no glade

    gdk_window_clear (widget->window);
	desenha_grade(widget);

  for (i = 0; i != resolucao ; i++)
  {
	  y=abs(Y[deslocamento+i]);
     if (y> 127) y = 128 + 255 - y;
    y = height - y * ( height / 256);

    if (i)
    {
      gdk_draw_line(widget->window, estilo_linha_referencia, ((i-1) * width) / (resolucao-1), last_y,((i) * width) / (resolucao-1), y);
    }
  last_y = y;

}


}





void Cria_Estilos(GtkWidget *widget)
{
//------------------------------ estilo grade ---------------------------------//
estilo_linha_grade = gdk_gc_new(widget->window); // cria um estilo como cor de fundo e estilo de linha
gdk_gc_set_rgb_fg_color(estilo_linha_grade,&blue);

gdk_gc_set_line_attributes(estilo_linha_grade,1,GDK_LINE_ON_OFF_DASH,GDK_CAP_NOT_LAST,GDK_JOIN_ROUND);

//------------------------------ estilo fundo ---------------------------------//
 gtk_widget_modify_bg(widget,GTK_STATE_NORMAL,&black);


//------------------------------ estilo linha referencia ---------------------------------//
estilo_linha_referencia = gdk_gc_new(widget->window); // cria um estilo como cor de fundo e estilo de linha
gdk_gc_set_rgb_fg_color(estilo_linha_referencia,&green);
gdk_gc_set_line_attributes(estilo_linha_referencia,1,GDK_LINE_SOLID,GDK_CAP_NOT_LAST,GDK_JOIN_ROUND);

//------------------------------ estilo linha amostra ---------------------------------//


}

