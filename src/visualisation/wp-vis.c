/* launcher.c */

#include <gtk/gtk.h>

#define APPLICATION_ID  "org.mawsonlakes.wp.wp-vis"

typedef struct _node {
    int xm;
    int ym;
    int e;
    int lx; // direction of link in x direction
    int ly; // direction of link in y direction
    int level; // Number of times note has been updated
    // Fields
    int n;  // Number of times particle visits node
} Node;

#define MAXCOLORS 18
int colors[MAXCOLORS][3] = {{255,0,0},   // red
                            {255,85,0},
                            {255,170,0},
                            {255,255,0},
                            {170,255,0},
                            {85,255,0},
                            {0,255,0},   // green
                            {0,255,85},
                            {0,255,170},
                            {0,255,255},
                            {0,170,255},
                            {0,85,255},
                            {0,0,255},   // blue
                            {85,0,255},
                            {170,0,255},
                            {255,0,255},
                            {255,0,170},
                            {255,0,85}};

int maxx = 50;
int maxy = 50;

Node *data;
int step = 0;

float xoffset = 20.0;
float yoffset = 60.0;
// float xseparation = 8.0;
// float yseparation = 8.0;
float xseparation = 16.0;
float yseparation = 16.0;
// float spotsize     = 1.0;
// float particlesize = 4.0;

float spotsize     = 6.0;
float particlesize = 8.0;


int data_index(int i, int j){
    // TODO: Can add boundary conditions here. eg. periodic boundary
    // identification.

    // Use correct definition of modulo, rather then remainder.
    i = i % maxx;
    if (i<0) i=i+maxx;

    j = j % maxy;
    if (j<0) j=j+maxy;

    return j*maxy + i;
}

Node*
data_init (void){
    int i,j;

    /* Intializes random number generator */
    time_t t;
    srand((unsigned) time(&t));

    Node* data;
    data = (Node *) malloc(maxx*maxy*sizeof(Node));

    for (j=0; j<maxy; j++){
        for (i=0; i<maxx; i++){
            data[data_index(i,j)].xm = 0;
            data[data_index(i,j)].ym = 0;
            data[data_index(i,j)].e = 0;
            data[data_index(i,j)].lx = (i+j)%2;
            data[data_index(i,j)].ly = (i+j)%2;
            data[data_index(i,j)].n = 0;
            data[data_index(i,j)].level = (i+j)%2;
        }
    }

    data[data_index(i/2, j/2)].e = 1;
    data[data_index(i/2, j/2)].n = 1;

    return data;
}

float
screenx (int x){
    return x*xseparation + xoffset;
}

float
screeny (int y){
    return y*yseparation + yoffset;
}

int
data_step_random (Node* data, int step){
    int i,j;

    i = rand() % maxx;
    j = rand() % maxy;

    int search = true;

    while (search) {
        if ((data[data_index(i,j)].lx == 0)
            && (data[data_index(i,j)].ly == 0)
            && (data[data_index(i-1,j)].lx == 1)
            && (data[data_index(i,j-1)].ly == 1)) {

            // min node - can be updated

            if ((data[data_index(i-1,j)].e == 1)
                || (data[data_index(i+1,j)].e == 1)
                || (data[data_index(i,j-1)].e == 1)
                || (data[data_index(i,j+1)].e == 1)) {

                data[data_index(i,j)].e = 1;
                data[data_index(i,j)].n = data[data_index(i,j)].n + 1;
                data[data_index(i-1,j)].e = 0;
                data[data_index(i+1,j)].e = 0;
                data[data_index(i,j-1)].e = 0;
                data[data_index(i,j+1)].e = 0;

            }
            // Change links
            data[data_index(i,j)].lx = 1;
            data[data_index(i,j)].ly = 1;
            data[data_index(i-1,j)].lx = 0;
            data[data_index(i,j-1)].ly = 0;
            data[data_index(i,j)].level += 2;

            search = false;

        } else {
            int d = rand() % 4;
            switch (d) {
            case 0:
                if (data[data_index(i,j)].lx == 1) i++;
                break;
            case 1:
                if (data[data_index(i,j)].ly == 1) j++;
                break;
            case 2:
                if (data[data_index(i-1,j)].lx == 0) i--;
                break;
            case 3:
                if (data[data_index(i,j-1)].ly == 0) j--;
                break;
            }
        }
    }

    step++;
    return step;
}

int
data_step_full (Node* data, int step) {
    int i,j;

    for (j==0; j<maxy; j++) {
        for (i==0; i<maxx; i++) {
            if ((data[data_index(i,j)].lx == 0)
                && (data[data_index(i,j)].ly == 0)
                && (data[data_index(i-1,j)].lx == 1)
                && (data[data_index(i,j-1)].ly == 1)) {
                // Change links
                data[data_index(i,j)].lx = 1;
                data[data_index(i,j)].ly = 1;
                data[data_index(i-1,j)].lx = 0;
                data[data_index(i,j-1)].ly = 0;
                data[data_index(i,j)].level += 2;

                step++;
            }
        }
    }

    return step;
}

static void
    draw_function (GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer user_data) {
    char text[256]; // Magic number!!

    cairo_set_source_rgb (cr, 0.0, 0.0, 0.0); /* white */
    cairo_paint (cr);

    int nmin = 0;  // Number of minima
    int nmax = 0;  // Number of maxima
    int count = 0; // Total points
    int i,j;

    for (j=0; j<maxy; j++) {
        for (i=0; i<maxx; i++) {
            count++;

            // Circle for particle data
            if (false) {
                if (data[data_index(i,j)].n != 0) {
                    cairo_arc (cr, screenx(i), screeny(j), spotsize*1.5, 0, 2 * M_PI);
                    cairo_set_source_rgb (cr, 1.0, 1.0, 0.0);
                    cairo_fill(cr);
                }
            }

            // Minima and Maxima graphics
            if (true) {
                if ((data[data_index(i,j)].lx == 0)
                    && (data[data_index(i,j)].ly == 0)
                    && (data[data_index(i-1,j)].lx == 1)
                    && (data[data_index(i,j-1)].ly == 1)) {

                    if (true) {
                        cairo_set_source_rgb (cr, 1.0, 1.0, 1.0);
                        cairo_set_line_width (cr, 1.0);
                        cairo_arc (cr, screenx(i), screeny(j), particlesize, 0, 2 * M_PI);
                        cairo_stroke (cr);
                    }
                    nmin++;
                } else if ((data[data_index(i,j)].lx == 1)
                           && (data[data_index(i,j)].ly == 1)
                           && (data[data_index(i-1,j)].lx == 0)
                           &&  (data[data_index(i,j-1)].ly == 0)) {

                    if (false) {
                        cairo_set_source_rgb (cr, 1.0, 1.0, 1.0); /* red - local maxima */
                        cairo_set_line_width (cr, 1.0);
                        cairo_arc (cr, screenx(i), screeny(j), particlesize, 0, 2 * M_PI);
                        cairo_stroke (cr);
                    }
                    nmax++;
                } else {
                    // cairo_set_source_rgb (cr, 0.0, 1.0, 0.0); /* green - saddle */
                }
            }

            // Color as level
            if (true) {
                int color = (data[data_index(i,j)].level)%MAXCOLORS;
                cairo_set_source_rgb (cr,
                                      colors[color][0]/255.0,
                                      colors[color][1]/255.0,
                                      colors[color][2]/255.0);
                cairo_arc (cr, screenx(i), screeny(j), 4.0, 0, 2 * M_PI);
                cairo_fill(cr);
            }

            if (data[data_index(i,j)].e != 0){
                cairo_set_source_rgb (cr, 1.0, 1.0, 1.0); /* blue */
                cairo_arc (cr, screenx(i), screeny(j), particlesize, 0, 2 * M_PI);
                cairo_stroke (cr);
            }

    }
    }

    cairo_set_source_rgb (cr, 1.0, 1.0, 1.0);
    cairo_move_to(cr, 20, 30);

    cairo_select_font_face(cr, "Courier",
                           CAIRO_FONT_SLANT_NORMAL,
                           CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 13);

    snprintf(text, 256, "steps: %9d | points: %5d | nmin: %5d | nmax: %5d | saddle: %5d",
             step, count, nmin, nmax, count - nmin - nmax);
    cairo_show_text(cr, text);
    //cairo_show_text(cr, "Most relationships seem so transitory");
}

gboolean time_handler(GtkWidget* widget) {
    //if (widget->window == NULL) return FALSE;

     step = data_step_random(data, step);
     // step = data_step_full(data, step);
     return TRUE;
}


gboolean time_handler2(GtkWidget* widget) {
    gtk_widget_queue_draw(widget);

    return TRUE;
}



static void
app_activate (GApplication *app, gpointer user_data) {
    // g_print ("You need a filename argument.\n");
    GtkWidget  *win;
    GtkWidget  *area = gtk_drawing_area_new ();;
    GtkBuilder *build;

    build = gtk_builder_new_from_resource("/org/mawsonlakes/wp/wp-vis/wp-vis.ui");
    win = GTK_WIDGET(gtk_builder_get_object(build, "win"));

    // gtk_window_set_default_size(GTK_WINDOW(win),600,600);

    // Add Drawable Area
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA (area), draw_function, NULL, NULL);
    gtk_window_set_child(GTK_WINDOW (win), area);

    gtk_window_set_application(GTK_WINDOW (win), GTK_APPLICATION (app));
    g_object_unref(build);

    // Initialisation
    data = data_init();

    // Timer Event
    g_timeout_add(1,  (GSourceFunc) time_handler, (gpointer) area);
    g_timeout_add(10, (GSourceFunc) time_handler2, (gpointer) area);

    gtk_widget_show (win);

}

int
main (int argc, char **argv) {
    GtkApplication *app;
    int stat;

    app = gtk_application_new (APPLICATION_ID, G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (app_activate), NULL);
    stat = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);
    return stat;
}
