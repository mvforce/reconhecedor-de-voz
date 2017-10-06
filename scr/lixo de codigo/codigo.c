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
gint timeout_callback(gpointer data);
 GtkBuilder *builder;
 void desenha_grade(GtkWidget *widget);

 //------------------------------------ cores -------------------------------------//
GdkColor red = {0, 65535, 0, 0};
  GdkColor green = {0, 0, 65535, 0};
  GdkColor blue = {0, 0, 0, 65535};
  GdkColor pink = {0, 65535, 0, 65535};
  GdkColor black = {0, 0, 0, 0};
GdkColor yellow = { 0, 0xFFFF, 0xFFFF, 0x0000 };
GdkColor gray =   { 0, 0x8888, 0x8888, 0x8888 };

//----------------------------------------------------------------------------------//
static int audiofd = -1;
#define samples_per_sec 8000
#define frames_per_sec 30
#define bufsiz (samples_per_sec/frames_per_sec)
static char inbuf[bufsiz];

int main( int    argc,      char **argv )
{

    GtkWidget  *window;
    GError     *error = NULL;

    /* Init GTK+ */
    gtk_init( &argc, &argv );

    /* Create new GtkBuilder object */
    builder = gtk_builder_new();
    /* Load UI from file. If error occurs, report it and quit application.
     * Replace "tut.glade" with your saved project. */
    if( ! gtk_builder_add_from_file( builder, "interface.glade", &error ) )
    {
        g_warning( "%s", error->message );
        g_free( error );
        return( 1 );
    }

    /* Get main window pointer from UI */
    window = GTK_WIDGET( gtk_builder_get_object( builder, "window" ) );
GtkWidget *drawingarea = GTK_WIDGET (gtk_builder_get_object (builder,"drawingarea"));
    /* Connect signals */
    gtk_builder_connect_signals( builder, NULL );


audiofd = open("/dev/audio", O_RDONLY);
  if (audiofd == -1) {
    g_error("%s: /dev/audio: %s\n", argv[0], g_strerror(errno));
  }

    /* Show window. All other widgets are automatically shown by GtkBuilder */
    gtk_widget_show( window );

   gtk_timeout_add(1000/frames_per_sec/1, timeout_callback, (gpointer)drawingarea);


    /* Start main loop */
    gtk_main();

    return( 0 );
}


void sair(GtkObject *object, gpointer user_data)

{
//Sai do loop principal de eventos, finalizando o programa

/* Destroy builder, since we don't need it anymore */
g_object_unref( G_OBJECT( builder ) );

gtk_main_quit();


}


double now() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec + tv.tv_usec * 0.000001;
}


gint timeout_callback(gpointer data) {
  GtkWidget *widget = (GtkWidget*)data;
  int width = widget->allocation.width;
  int height = widget->allocation.height;
  int ii;
  int yvalue;
  int last_yvalue;
  double beforeread, length;


  beforeread = now();
  if (read(audiofd, inbuf, bufsiz) != bufsiz) {
    /* XXX this aborts the program if a signal is received during read */
    g_error("reading audio data: %s\n", g_strerror(errno));
  }



  length = now() - beforeread;

gdk_window_clear (widget->window);
 desenha_grade(widget);



  for (ii = 0; ii != bufsiz; ii++)
  {
    yvalue = (unsigned char)inbuf[ii];
printf("%d %d\n",ii,inbuf[ii]);
    if (yvalue > 127) yvalue = 128 + 255 - yvalue;
    yvalue = height - yvalue * height / 256;
    if (ii)
    {
      gdk_draw_line(widget->window, widget->style->black_gc,
		    (ii-1) * width / (bufsiz-1), last_yvalue,
		    ii * width / (bufsiz-1), yvalue);
    }
    last_yvalue = yvalue;
  }

  return TRUE;
}

void desenha_grade(GtkWidget *widget)
{
int width = widget->allocation.width;
  int height = widget->allocation.height;
GdkGC *estilo;
estilo = gdk_gc_new(widget->window);

int a;
int espaco=50;
gdk_gc_set_rgb_fg_color(estilo,&blue);
gdk_gc_set_line_attributes(estilo,1,GDK_LINE_ON_OFF_DASH,GDK_CAP_NOT_LAST,GDK_JOIN_ROUND);


for(a=1;a<width/espaco;a++) gdk_draw_line(widget->window, estilo, a*espaco, 0,a*espaco,height);

for(a=1;a<height/espaco;a++) gdk_draw_line(widget->window, estilo, 0,a*espaco, width, a*espaco);

}


void desenha_grafico_amostra(GtkWidget *widget)
{



}
