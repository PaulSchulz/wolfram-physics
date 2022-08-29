//  Filename: wp-vis.c
//  Version: 3

#include <glib.h>
#include <glib/gstdio.h>

#include <gtk/gtk.h>
#include <stdio.h>
#include <time.h>

#include <math.h>  // cos()

#define APPLICATION_ID  "org.mawsonlakes.wp.wp-vis"

struct timespec tms;
int64_t start_time, end_time, begin_time;
int64_t start_steps, end_steps;

// Number of spatial dimensions
#define NLINKS 3
#define NFIELDS 1

// Dimer (domino) field (generalized from 2D)
typedef struct _field {
    int fa,fb,fc,fd;
    int f[2*NLINKS];
    int active;
    // Statistics
    int total_fa, total_fb, total_fc, total_fd;
    int total_f[2*NLINKS];
} Field;

// Scalar Field Wave Equation

typedef struct _node {
    int xm;
    int ym;
    int e;
    int lx; // direction of link in x direction
    int ly; // direction of link in y direction
    int lz; // direction of link in z direction
    int link[NLINKS];

    int level; // Number of times note has been updated

    // Particle
    int n;  // Number of times particle visits node

    // Field
    Field fields[NFIELDS];

} Node;

//////////////////////////////////////////////////////////////////////////////

int maxx = 500;
int maxy = 500;
int maxz = 1;

int level_max_max = 250;
int level_max = 1;

Node* data;
int step = 0;

int level_max_current = 0;

// Screen Text Layout
float xoffset_text = 16.0;
float yoffset_text = 20.0;

// Graphics
float xoffset     = 20.0;
float yoffset     = 20.0;

float xseparation = 2.0;
float yseparation = 2.0;
float xsep = 2.0;
float ysep = 2.0;
float spotsize     = 1.0;
float particlesize = 3.0;

// Statistics
int global_single = 0;
int global_count  = 0;

// Use correct definition of modulo, rather then remainder.
// Can add boundary conditions here. eg. periodic boundary identification.
int index_map (int i, int max){
    i = i % max;
    if (i<0) i=i+max;
    return i;
}

int data_index(int i, int j){
    return index_map(j,maxy)*maxy + index_map(i,maxx);
}

int data_index3(int i, int j, int k){
    return index_map(k,maxz)*maxy*maxx + index_map(j,maxy)*maxx + index_map(i,maxx);
}

Field* field_init (Field* field){
    // Field - Olde style
    if (true) {
        field->fa = 0;
        field->fb = 0;
        field->fc = 0;
        field->fd = 0;
        field->active = 0;
        // Field Statistics
        field->total_fa = 0;
        field->total_fb = 0;
        field->total_fc = 0;
        field->total_fd = 0;
    }

    // Field - New style, array based
    if(true){
        for(int i=0; i<NLINKS; i++){
            field->f[i] = 0;
            // Field Statistics
            field->total_f[i] = 0;
        }
    }

    return field;
}

Node* node_field_init (Node* node){

    for(int k=0; k<NFIELDS; k++) {
        Field* field = &node->fields[k];
        field = field_init(field);
    }
    return node;
}


Node*
data_init (void){
    int i,j;

    /* Intializes random number generator */
    time_t t;
    srand((unsigned) time(&t));

    // Allocate data array
    Node* data;
    data = (Node *) malloc(maxx*maxy*maxz*sizeof(Node));

    //for (k=0;k<maxy; k++){
    for (int k=0;k<1; k++){
        for (int j=0; j<maxy; j++){
            for (int i=0; i<maxx; i++){
                // Define pointers
                Node* node = &data[data_index(i,j)];

                node->xm = 0;
                node->ym = 0;
                node->e = 0;

                node->lx = (i+j)%2;
                node->ly = (i+j)%2;
                for(int l=0; l<NLINKS; l++){
                    node->link[l] = (i+j+k)%2;
                }

                node->level = (i+j+k)%2;

                // Particle
                node->n = 0;

                // Field initialization
                node = node_field_init(node);

            } // i loop
        } // j loop
    } // k loop

    // Activate one node in centre for all fields
    if (true) {
        Node* node = &data[data_index(maxx/2, maxy/2)];
        for(int k=0; k<NFIELDS; k++) {
            node->fields[k].active = 1;
        }
    }

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

//////////////////////////////////////////////////////////////////////////////
// Graphics Routines

//////////////////////////////////////////////////////////////////////////////
// Functions to update a node
// Node directions
//       b
//       ^
//       |
//  c <- o -> a
//       |
//       v
//       d

void node_make_active (
    Node* node,
    Node* node_a,
    Node* node_b,
    Node* node_c,
    Node* node_d
    ) {

}

void node_reset (Node* node) {

}

void node_propagation (
    Node* node,
    Node* node_a,
    Node* node_b,
    Node* node_c,
    Node* node_d
    ) {

}

void node_resolution (Node* node) {

}

//////////////////////////////////////////////////////////////////////////////
// Functions used to update field data
// Field directions
//       b
//       ^
//       |
//  c <- o -> a
//       |
//       v
//       d

void field_make_active (
    Field* field,
    Field* field_a,
    Field* field_b,
    Field* field_c,
    Field* field_d
    )
{
    // Activate node if required / disturbed
    if (field_a->fa == 1
        || field_a->fb == 1
        || field_a->fc == 1
        || field_a->fd == 1

        || field_b->fa == 1
        || field_b->fb == 1
        || field_b->fc == 1
        || field_b->fd == 1

        || field_c->fa == 1
        || field_c->fb == 1
        || field_c->fc == 1
        || field_c->fd == 1

        || field_d->fa == 1
        || field_d->fb == 1
        || field_d->fc == 1
        || field_d->fd == 1
        ) {
        if (field->active == 0) {
            field->active = 1;
        }
    }
}

void field_reset (Field* field) {
    field->fa = 0;
    field->fb = 0;
    field->fc = 0;
    field->fd = 0;
}

void field_propagation (
    Field* field,
    Field* field_a,
    Field* field_b,
    Field* field_c,
    Field* field_d
    ) {

    if (field_c->fa == 1) field->fa = 1;
    if (field_d->fb == 1) field->fb = 1;
    if (field_a->fc == 1) field->fc = 1;
    if (field_b->fd == 1) field->fd = 1;
}

void field_resolution (Field* field) {
    if ((field->fa == 1 && field->fc == 1)
        || (field->fb == 1 && field->fd == 1)) {
        // Annihilation
        field->fa = 0;
        field->fb = 0;
        field->fc = 0;
        field->fd = 0;
        field->active = field->active + 1;

    } else if (field->active != 0
               && field->fa == 0
               && field->fb == 0
               && field->fc == 0
               && field->fd == 0) {
        // Creation
        if (rand()%2 == 0) {
            field->fa = 1;
            field->fb = 0;
            field->fc = 1;
            field->fd = 0;
        } else {
            field->fa = 0;
            field->fb = 1;
            field->fc = 0;
            field->fd = 1;
        }
        field->active = field->active + 1;
    }

}

// Algorithm: Randomly pick a point in the array, then move down from this point
// until a (local) minimum is found which can be updated.
int
data_step_random (Node* data, int step){
    int i,j;

    // Pick random point
    i = rand() % maxx;
    j = rand() % maxy;

    int search = true;
     // Search for an updateable 'minimum' point.
    while (search) {

        // Define pointers
        //      b
        //      |
        // c <- . -> a
        //      |
        //      d

        Node* node = &data[data_index(i,j)];
        Node* node_a = &data[data_index(i+1,j)];
        Node* node_b = &data[data_index(i,j-1)];
        Node* node_c = &data[data_index(i-1,j)];
        Node* node_d = &data[data_index(i,j+1)];

        if ((node->lx == 0)
            && (node->ly == 0)
            && (node_c->lx == 1)
            && (node_b->ly == 1)) {

            // Minimum node found - can be updated
            // Finish search
            search = false;

            // Stop conditions
            if (node->level >= level_max){
                break;
            };

            // Update node
            step++;

            // Update fields

            // Field - Domino Tile
            //       fb
            //       ^
            //       |
            // fc <- o -> fa
            //       |
            //       v
            //       fd

            for(int k=0; k<NFIELDS; k++){
                Field* field = &node->fields[k];
                Field* field_a = &node_a->fields[k];
                Field* field_b = &node_b->fields[k];
                Field* field_c = &node_c->fields[k];
                Field* field_d = &node_d->fields[k];

                field_make_active(field, field_a, field_b, field_c, field_d);
                field_reset(field);
                field_propagation(field, field_a, field_b, field_c, field_d);
                field_resolution(field);

                // Statistics
                if (field->fa == 1) field->total_fa = field->total_fa + 1;
                if (field->fb == 1) field->total_fb = field->total_fb + 1;
                if (field->fc == 1) field->total_fc = field->total_fc + 1;
                if (field->fd == 1) field->total_fd = field->total_fd + 1;
            }

            // Update links
            node->lx = 1;
            node->ly = 1;
            node_c->lx = 0;
            node_b->ly = 0;
            node->level += 2;

        } else {
            // Randomly move to an adjacent node (downhill)
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

    // Global Statistics
    if(data[data_index(i,j)].level > level_max_current){
        level_max_current = data[data_index(i,j)].level;
    }

    return step;
}

// Iterate through all nodes once
// Note: Not currently used.
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

double angle = 0.0;

static void
draw_function (GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer user_data) {
    char text[256]; // Magic number!!

    cairo_set_source_rgb (cr, 0.0, 0.0, 0.0); /* black */
    cairo_paint (cr);

    // Scaling
    if (false) {
        cairo_translate(cr, 500.0, 500.0);
        cairo_scale(cr, 0.8, 0.8);
        angle += 0.002;
        cairo_rotate (cr, angle);

        cairo_translate(cr, -500.0, -500.0);
    }

    int nmin = 0;  // Number of minima
    int nmax = 0;  // Number of maxima
    int count = 0; // Total points
    int i,j;

    // Iterate over all nodes (i,j)
    for (j=0; j<maxy; j++) {
        for (i=0; i<maxx; i++) {
            count++;

            Node* node = &data[data_index(i,j)];
            Node* node_a = &data[data_index(i+1,j)];
            Node* node_b = &data[data_index(i,j-1)];
            Node* node_c = &data[data_index(i-1,j)];
            Node* node_d = &data[data_index(i,j+1)];

            // Circle for Nodes
            if (true) {
                cairo_set_source_rgb (cr, 0.2, 0.2, 0.2);
                cairo_set_line_width (cr, 1.0);
                cairo_arc (cr, screenx(i), screeny(j), 1.0, 0, 2 * M_PI);

                if ((i+j)%2 == 0 ){
                    cairo_fill(cr);
                } else {
                    cairo_stroke(cr);
                }
            }
        }
    }

    // Field overlay
    int nfield = 0;
    for (j=0; j<maxy; j++) {
        for (i=0; i<maxx; i++) {
            Node* node = &data[data_index(i,j)];

            // Field k=0;
            if (true
                && node->level == level_max_current
                ) {

                Field* field = &node->fields[nfield];

                // Block visualisation
                if (true) {
                    if (field->fa != 0){
                        cairo_set_source_rgb (cr, 1.0, 0.0, 0.0);
                                            cairo_set_line_width (cr, 1.0);
                        cairo_arc (cr, screenx(i), screeny(j), 1.0, 0, 2 * M_PI);
                        cairo_fill(cr);
                    }
                }

                if (true) {
                    if (field->fb != 0){
                        cairo_set_source_rgb (cr, 0.0, 1.0, 0.0);
                        cairo_set_line_width (cr, 1.0);
                        cairo_arc (cr, screenx(i), screeny(j), 1.0, 0, 2 * M_PI);
                        cairo_fill(cr);
                    }
                }

                if (true) {
                    if (field->fc != 0){
                        cairo_set_source_rgb (cr, 0.0, 0.0, 1.0);
                        cairo_set_line_width (cr, 1.0);
                        cairo_arc (cr, screenx(i), screeny(j), 1.0, 0, 2 * M_PI);
                        cairo_fill(cr);
                    }
                }

                if (true) {
                    if (field->fd != 0){
                        cairo_set_source_rgb (cr, 1.0, 1.0, 0.0);
                        cairo_set_line_width (cr, 1.0);
                        cairo_arc (cr, screenx(i), screeny(j), 1.0, 0, 2 * M_PI);
                        cairo_fill(cr);
                    }
                }

            }

        }
    }

    global_count  = 0;
    global_single = 0;

}

void
save_data (int level) {
    FILE *fileout;
    gchar *filename;

    filename = g_strdup_printf("arctic-data-%03d.txt", level);
    fileout = g_fopen(filename,"w");

    int nfield = 0;    // First field

    for(int j=0; j<maxy; j++){
        for(int i=0; i<maxx; i++){

            Node*  node = &data[data_index(i,j)];
            Field* field = &node->fields[nfield];g
                                                    char   nodechar = ' ';

            if (field->fa != 0 && field->fc != 0){
                nodechar = '-';
            } else if (field->fb != 0 && field->fd != 0){
                nodechar = '|';
            } else if (field->fa != 0) {
                nodechar = '>';
            } else if (field->fb != 0) {
                nodechar = '^';
            } else if (field->fc != 0) {
                nodechar = '<';
            } else if (field->fd != 0) {
                nodechar = 'v';
            };

            fprintf(fileout,"%c", nodechar);
        }

        fprintf(fileout,"\n");
    }

    printf("Saved data to %s",filename);
    fclose(fileout);
    g_free(filename);
}


// Global variable.
int  step_prev = 0;

static int
report_statistics (gpointer area) {
    // Area is a widget

    char text[256]; // Magic number!!

    int i,j;

    // Statistics
    int count    = 0;    // Total points
    int total    = 0;    // Active field nodes (at max level)
    int total_fa = 0;
    int total_fb = 0;
    int total_fc = 0;
    int total_fd = 0;
    int single   = 0;

    // Calculate number of 'singular' nodes across all fields.
    // eg. Nodes where the field only exists in one state,
    for (j=0; j<maxy; j++) {
        for (i=0; i<maxx; i++) {
            count++;

            Node* node = &data[data_index(i,j)];
            if (true
                && node->level == level_max_current
                ) {

                // Fields
                for(int k=0; k<NFIELDS; k++) {
                    Field* field = &node->fields[k];

                    if (field->fa == 1) total++;
                    if (field->fb == 1) total++;
                    if (field->fc == 1) total++;
                    if (field->fd == 1) total++;

                    total_fa += field->total_fa;
                    total_fb += field->total_fb;
                    total_fc += field->total_fc;
                    total_fd += field->total_fd;
                }

                if ((total_fa == 0
                     && total_fb == 0
                     && total_fc == 0
                     && total_fd != 0)

                    || (total_fa == 0
                       && total_fb == 0
                       && total_fc != 0
                       && total_fd == 0)

                    || (total_fa == 0
                       && total_fb != 0
                       && total_fc == 0
                       && total_fd == 0)

                    || (total_fa != 0
                       && total_fb == 0
                       && total_fc == 0
                       && total_fd == 0)
                    ) {
                    // Statistics
                    single++;
                }
            }

        }
    }

    printf("%8d/%-8d (%5.1f%%)",
           level_max_current - 1,
           level_max,
           100.0 * (level_max_current-1) / level_max
        );

    printf(" %8d/%-8d (%5.1f%%)",
           step,
           level_max * count / 2,
           100.0 * step / (level_max * count / 2)
        );

    snprintf(text, 256, "fields: %d", NFIELDS);
    printf(" %s\n", text);

    // Rates
    // start_time initially setup elseware
    if (clock_gettime(CLOCK_REALTIME,&tms)) {
        printf("*** ERROR Unable to get CLOCK_REALTIME\n");
    }
    /* seconds, multiplied with 1 million */
    end_time = tms.tv_sec * 1000000;
    /* Add full microseconds */
    end_time += tms.tv_nsec/1000;
    /* round up if necessary */
    if (tms.tv_nsec % 1000 >= 500) {
        ++end_time;
    }

    end_steps = step;

    snprintf(text, 256, "steps:  %8.2f st/s (avg: %8.2f st/s)",
             1000000.0 * (end_steps - start_steps) / (end_time - start_time),
             1000000.0 * end_steps / (end_time - begin_time)
        );
    printf("%s ", text);

    snprintf(text, 256, "frames: %8.2f f/s", 1000000.0 / (end_time - start_time));
    printf("%s\n", text);

    start_time = end_time;
    start_steps = end_steps;

    // Increase the level once all steps are done.
    if (true) {
        if ((step == level_max * count / 2) && (level_max < level_max_max)){
            save_data(level_max);

            printf("Increase level to: %d\n", level_max);
            level_max++;
        }
    };

    // Increase the level if no steps have been make (assume level is complete).
    if (false) {
        if (step_prev == step && level_max < level_max_max){
            level_max++;
        }
    }
    step_prev = step;

    printf("-------------------------------------------------------------------------\n");

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
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
    // Main Window
    GtkWidget  *win;
    GtkWidget  *area = gtk_drawing_area_new ();
    GtkBuilder *build;

    build = gtk_builder_new_from_resource("/org/mawsonlakes/wp/wp-vis/wp-vis.ui");
    win = GTK_WIDGET(gtk_builder_get_object(build, "win"));

    // Main: add drawable Area
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(area), draw_function, NULL, NULL);
    gtk_window_set_child(GTK_WINDOW(win), area);

    g_object_unref(build);

    gtk_window_set_application(GTK_WINDOW(win), GTK_APPLICATION(app));

    // Initialisation
    data = data_init();

    // Statistics - required prior to calculating rate stats
    // start_time initially setup elseware
    if (clock_gettime(CLOCK_REALTIME,&tms)) {
        printf("*** ERROR Unable to get CLOCK_REALTIME\n");
    }

    // seconds, multiplied with 1 million
    end_time = tms.tv_sec * 1000000;
    // Add full microseconds
    end_time += tms.tv_nsec/1000;
    // round up if necessary
    if (tms.tv_nsec % 1000 >= 500) {
        ++end_time;
    }
    start_time = end_time;
    begin_time = end_time;

    end_steps = 0;
    start_steps = end_steps;

    // Timer Event
    // g_timeout_add(1,  (GSourceFunc) time_handler, (gpointer) area);
    g_idle_add((GSourceFunc)time_handler, (gpointer)area);
    g_timeout_add(20, (GSourceFunc)time_handler2, (gpointer)area);
    // g_timeout_add(20, (GSourceFunc)time_handler2, (gpointer)statistics);
    g_timeout_add(200, (GSourceFunc)report_statistics, (gpointer)area);

    gtk_widget_show(win);
    // gtk_widget_show(monitor);
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
