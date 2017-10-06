/*
 * Compile me with:
 *   gcc -o tut tut.c $(pkg-config --cflags --libs gtk+-2.0 gmodule-2.0 cairo)
 */

#include <gtk/gtk.h>
#include <cairo.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <math.h>
#include <fftw3.h>
#include <sys/ioctl.h>
#include <sys/soundcard.h>



//------------------------------------ variaveis globais de interface utilizadas -------------------------------------//

GtkBuilder *builder;  // utilizado como descritor do arquivo de interface

 GtkWidget  *window;
 GtkWidget *drawingarea;

 GtkWidget *check_pre_enfase;
 GtkWidget *check_remove_dc;
 GtkWidget *check_janela_hamming;
 GtkWidget *check_mel;
 GtkWidget *check_delta;
 GtkWidget *check_delta_delta;
 GtkWidget *check_grade_eixo_x;
 GtkWidget *check_grade_eixo_y;
 GtkWidget *check_mostra_recorte;
 GtkWidget *check_limiar_silencio;
 GtkWidget *check_leitura;
 GtkAdjustment *ajuste_limiar_max;
 GtkAdjustment *ajuste_limiar_min;
 GtkWidget *filechooserbutton_amostra;
 GtkWidget *filechooserbutton_referencia;

//------------------------------------ variaveis globais utilizadas -------------------------------------//

GdkGC *estilo_linha_grade; // estilo utilizado na grade
GdkGC *estilo_fundo; // estilo utilizado no fundo da drawingarea
GdkGC *estilo_linha_referencia; // estilo utilizado na linha do grafico de referencia
GdkGC *estilo_linha_amostra;// estilo utilizado na linha do grafico de amostra

int Yamostra[24000] = {0};  //array com os valores do eixo Y amostra
int Yreferencia[24000] = {0};  //array com os valores do eixo Y  referencia
int ESPACO_GRADE=50;
int audiodev; // descritor para dispositivo de audio




 //------------------------------------ inclusao de arquivos proprios -------------------------------------//
//#include "definicoes_de_variaveis.h"
#include "definicoes_e_configuracoes.h"
#include "estruturas_prototipos_de_funcoes.h"
#include "def_cores.h"   // definicao das cores usadas nos graficos
#include "executar_dtw_clicked.c"



#include "transformada_rapida_fourier.c"
#include "processamento_sinal.c"
#include "calcula_melcepstrais.c"
#include "alinhamento_temporal_dinamico.c"

//----------------------------------------------------------------------------------------//



 //------------------------------------ definicao de funcoes -------------------------------------//
void desenha_grade();
void cria_estilos();
void cria_vinculo();
void desenha_grafico(GtkAdjustment *adj);

//----------------------------------------------------funcao principal ------------------------------------------------//
int main( int    argc,      char **argv )
{

gtk_init( &argc, &argv ); //inicia o GTK

//chama a funcao que vincula os objetos da interface a variaveis globais
cria_vinculo();

// Mostra todos os objetos contidos em window
gtk_widget_show( window );

// chama a funcao que cria os estilos dos graficos
cria_estilos(drawingarea);

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
void desenha_grade()
{
int width = drawingarea->allocation.width;  // aloca a largura
int height = drawingarea->allocation.height; // aloca a altura

int a;

// se estiver marcada a opcao de desenhar a grade VERTICAL no grafico
if (GTK_TOGGLE_BUTTON (check_grade_eixo_y)->active)
{
// linhas verticais da grade
for(a=1;a<width/ESPACO_GRADE;a++)
{
gdk_draw_line(drawingarea->window, estilo_linha_grade, a*ESPACO_GRADE, 0,a*ESPACO_GRADE,height);
}
}
// se estiver marcada a opcao de desenhar a grade HORIZONTAL no grafico
if (GTK_TOGGLE_BUTTON (check_grade_eixo_x)->active)
{
// linhas horizontais da grade
for(a=1;a<height/ESPACO_GRADE;a++)
{
	gdk_draw_line(drawingarea->window, estilo_linha_grade, 0,a*ESPACO_GRADE, width, a*ESPACO_GRADE);
}
}

}



void desenha_grafico(GtkAdjustment *adj)
{

int x = (int) adj->value;
// aloca as dimensoes do drawingarea
  int width = drawingarea->allocation.width; //largura
  int height = drawingarea->allocation.height; //altura

//inicia as variaveis utilizadas para plotagem do grafico

  // variaveis do grafico referencia
  int last_yref; // salva um valor atras do atual
  int yref; // valor atual

  // variaveis do grafico referencia
  int last_yamo; // salva um valor atras do atual
  int yamo;// valor atual

  int resolucao = 500; // quantidade de pontos a serem plotados
  int deslocamento=100; // a cada click quantos pontos de deslocamento

    gdk_window_clear (drawingarea->window); // limpa a area


	desenha_grade();   // chama a funcao que desenha a grade no grafico//

  int i=0;

// loop que plota os graficos na tela
for(i=x;i<x+resolucao;i++)
{
//  variaveis do grafico de referencia
  	yref=abs(Yreferencia[deslocamento+i]); //valor absoluto
	if (yref> 127) yref =  0+ 255 - yref; //cria a amplitude
    yref = height - yref * ( height / 256); // enquadra a onde na area do grafico

//  variaveis do grafico de amostra
    yamo=abs(Yamostra[deslocamento+i]); //valor absoluto
    if (yamo> 127) yamo= 0 + 255 - yamo; //cria a amplitude
    yamo = height - yamo * ( height / 256); // enquadra a onde na area do grafico

    if (i)
    {  // desenha a linha de referencia
      gdk_draw_line(drawingarea->window, estilo_linha_referencia, (i-1) , last_yref,(i), yref);

      // desenha a linha de amostra
      gdk_draw_line(drawingarea->window, estilo_linha_amostra,  (i-1) , last_yamo,(i), yamo);

       //  printf("X. %d  |  Y. %d  |  X %d  | Y %d\n", (i-2) , last_yref,(i), yref);
    }

 // salva os valores atuais como sendo os ultimos para a proxima plotagem
  last_yref = yref;
  last_yamo = yamo;
}


}


// funcao que cria os estilos utilizados nos graficos
void cria_estilos()
{
//------------------------------ estilo grade ---------------------------------//
estilo_linha_grade = gdk_gc_new(drawingarea->window); // cria um estilo como cor de fundo e estilo de linha
gdk_gc_set_rgb_fg_color(estilo_linha_grade,&blue);
gdk_gc_set_line_attributes(estilo_linha_grade,1,GDK_LINE_ON_OFF_DASH,GDK_CAP_NOT_LAST,GDK_JOIN_ROUND);

//------------------------------ estilo fundo ---------------------------------//
 gtk_widget_modify_bg(drawingarea,GTK_STATE_NORMAL,&black);


//------------------------------ estilo linha referencia ---------------------------------//
estilo_linha_referencia = gdk_gc_new(drawingarea->window); // cria um estilo como cor de fundo e estilo de linha
gdk_gc_set_rgb_fg_color(estilo_linha_referencia,&green);
gdk_gc_set_line_attributes(estilo_linha_referencia,1,GDK_LINE_SOLID,GDK_CAP_NOT_LAST,GDK_JOIN_ROUND);

//------------------------------ estilo linha amostra ---------------------------------//
estilo_linha_amostra = gdk_gc_new(drawingarea->window); // cria um estilo como cor de fundo e estilo de linha
gdk_gc_set_rgb_fg_color(estilo_linha_amostra,&pink);
gdk_gc_set_line_attributes(estilo_linha_amostra,1,GDK_LINE_SOLID,GDK_CAP_NOT_LAST,GDK_JOIN_ROUND);

}


//funcao que cria o vinculo do elemento glade a variavel global
void cria_vinculo()
{
builder = gtk_builder_new(); // cria um descritor para a ponte interface/codigo

gtk_builder_add_from_file( builder, "../interface/interface.glade", NULL );  // indica qual arquivo esta vinculado

 // armazena todos os widgets listados e vincula a uma variavel global

window = GTK_WIDGET( gtk_builder_get_object( builder, "window" ) );

drawingarea = GTK_WIDGET (gtk_builder_get_object (builder,"drawingarea"));

check_pre_enfase = GTK_WIDGET (gtk_builder_get_object (builder,"check_pre_enfase"));

check_remove_dc = GTK_WIDGET (gtk_builder_get_object (builder,"check_remove_dc"));

check_janela_hamming = GTK_WIDGET (gtk_builder_get_object (builder,"check_janela_hamming"));

check_mel = GTK_WIDGET (gtk_builder_get_object (builder,"check_mel"));

check_delta = GTK_WIDGET (gtk_builder_get_object (builder,"check_delta"));

check_delta_delta = GTK_WIDGET (gtk_builder_get_object (builder,"check_delta_delta"));

check_grade_eixo_x = GTK_WIDGET (gtk_builder_get_object (builder,"check_grade_eixo_x"));

check_grade_eixo_y = GTK_WIDGET (gtk_builder_get_object (builder,"check_grade_eixo_y"));

check_mostra_recorte= GTK_WIDGET (gtk_builder_get_object (builder,"check_mostra_recorte"));

check_limiar_silencio= GTK_WIDGET (gtk_builder_get_object (builder,"check_limiar_silencio"));

check_leitura = GTK_WIDGET (gtk_builder_get_object (builder,"check_leitura"));

ajuste_limiar_max =  GTK_ADJUSTMENT(gtk_builder_get_object (builder,"ajuste_limiar_max"));

ajuste_limiar_min =  GTK_ADJUSTMENT(gtk_builder_get_object (builder,"ajuste_limiar_min"));

filechooserbutton_referencia =  GTK_WIDGET(gtk_builder_get_object (builder,"filechooserbutton_referencia"));

filechooserbutton_amostra =  GTK_WIDGET(gtk_builder_get_object (builder,"filechooserbutton_amostra"));


//conecta todos os sinais "callbacks"
gtk_builder_connect_signals( builder, NULL );

}

