// Hypergraph
// Computationaly generate hypergraph

#include <stdio.h>
#include <stdlib.h>            // random()
#include <stdbool.h>           // true, false
#include <time.h>              // Uses time() to generate random seed
#define MAXNODES 10000000      // 10 timeslices
#define MAXLINKS MAXNODES*8

// TODO: Replace node links with an list, rather than an array.
#define MAXNODELINKS     8

// Direction of link
#define FORWARD   1
#define BACKWARD -1

// Error Codes
#define OK                  0
#define ERR_NULLNODE        1
#define ERR_TOMANYLINKS     2

int debug = 1;

typedef struct _Node {
    struct _Link* link[MAXNODELINKS];
    int    nlinks;
    float  data;  // Put node data here
} Node;

typedef struct _Link {
    struct _Node* a;
    struct _Node* b;
    int dir; // Link direction
} Link;

// Node Array
Node nodes[MAXNODES];

// Link Array
Link structure[MAXLINKS];

//////////////////////////////////////////////////////////////////////////////
// State configuration
int maxx = 100;
int maxy = 100;
int maxz = 100;
int maxl = 3;

//////////////////////////////////////////////////////////////////////////////
// Lattice structure
long node_id(int x, int y, int z) {
    return (maxz*(maxy*(x%maxx) + (y%maxy)) + (z%maxz));
}

// Three 'distinct' links per node
long link_id(int x, int y, int z, int l) {
    return (maxl*(maxz*(maxy*(x%maxx) + (y%maxy)) + (z%maxz)) + (l%maxl));
}

void show_resources(void) {
    printf("Available Resources\n");
    printf("  Size of Node:      %10ld bytes\n", sizeof(Node));
    printf("  Size of Link:      %10ld bytes\n", sizeof(Link));
    printf("  Max. Nodes:        %10d\n", MAXNODES);
    printf("  Max. Links:        %10d\n", MAXLINKS);
    printf("  Per node:\n");
    printf("    Max. Links: %d\n", MAXNODELINKS);
    printf("\n");
}

void show_configuration(void) {
    printf("Configuration\n");
    printf("  maxx: %d\n", maxx);
    printf("  maxy: %d\n", maxy);
    printf("  maxz: %d\n", maxz);
    printf("\n");
}

long add_node(long node) {
    Node* node_a = &(nodes[node]);

    // Initialize node data
    node_a->nlinks = 0;  // No links
}

// Node/link indexing
int add_link_to_nodes(long a, long b, long l, int dir) {
    Node* node_a;
    Node* node_b;
    Link* link_ab;

    node_a  = &nodes[a];
    node_b  = &nodes[b];
    link_ab = &structure[l];

    // TODO: Refactor into a 'add_link_to_node(a,l)'
    int nlinks;

    // Add link reference to node a
    nlinks = node_a->nlinks;
    if (nlinks < MAXNODELINKS){
        node_a->link[nlinks] = link_ab;
        node_a->nlinks++;
    } else {
        return ERR_TOMANYLINKS;
    }

    // Add link reference to node b
    nlinks = node_b->nlinks;
    if(nlinks < MAXNODELINKS){
        node_b->link[nlinks] = link_ab;
        node_b->nlinks++;
    } else {
        return ERR_TOMANYLINKS;
        }

    link_ab->a = node_a;
    link_ab->b = node_b;
    link_ab->dir = dir;
}

// Cubic array 100x100x100
void create_initial_state(void) {
    printf("Create Initial State\n");

    int i,j,k;

    for(k=0; k<maxz; k++) {
        for(j=0; j<maxy; j++) {
            for(i=0; i<maxx; i++) {
                // Allocate nodes/initialize
                add_node(node_id(i,j,k));
            }
        }
    }

    for(k=0; k<maxz; k++) {
        for(j=0; j<maxy; j++) {
            for(i=0; i<maxx; i++) {
                long p = node_id(i,j,k);
                int dir = FORWARD;
                if ((i+j+k)%2 == 0){
                    dir = FORWARD;
                    // printf(".");
                } else {
                    dir = BACKWARD;
                    // printf("+");
                }
                add_link_to_nodes(p, node_id(i+1, j,   k  ), link_id(i,j,k,0), dir);
                add_link_to_nodes(p, node_id(i,   j+1, k  ), link_id(i,j,k,1), dir);
                add_link_to_nodes(p, node_id(i,   j,   k+1), link_id(i,j,k,2), dir);
            }
            // printf("\n");
        }
        // printf("---\n");
    }

    printf("\n");
}

//////////////////////////////////////////////////////////////////////////////
// Utilities
bool is_forward_link(Node* node, int link_number) {
    int is_forward = false;
    int dir = BACKWARD;
     Link* link = node->link[link_number];

     if(link->a == node) {
         if(link->dir == FORWARD) {
             dir = FORWARD;
         } else {
             dir = BACKWARD;
         }
     } else { // link->b == node
         if(link->dir == FORWARD) {
             dir = BACKWARD;
         } else {
             dir = FORWARD;
         }
     }

     if (dir == FORWARD) {
         is_forward = true;
     } else {
         is_forward = false;
     }

     return is_forward;
}

//////////////////////////////////////////////////////////////////////////////
// Dynamics
void random_init (void){
    time_t t;
    unsigned seed;
    seed = (unsigned) time (&t);
    seed = 0;
    srand(seed);
}

// Return a random node from 'nodes'
Node* random_node (void) {
    // TODO: Does this need to be calculated each time?
    int max_nodes = maxx*maxy*maxz;
    int r = rand()%max_nodes;
    return &nodes[r];
}

//////////////////////////////////////////////////////////////////////////////
// Display and Statistics
void display_graph(void) {
    int i,j,k;

    int total_nodes = 0;

    for(k=0; k<maxz; k++) {
        for(j=0;j<maxy; j++) {
            for(i=0;i<maxx; i++) {
                printf("%d",nodes[node_id(i,j,k)].nlinks);
                // printf(".");
            }
            printf("\n");
        }
        printf("---\n");
    }
}

void show_statistics(void) {
    printf("Statistics\n");

    int i,j,k;

    int total_nodes  = 0;

    int null_count   = 0;
    int source_count = 0;
    int sink_count   = 0;
    int other_count  = 0;

    for(k=0; k<maxz; k++) {
        for(j=0; j<maxy; j++) {
            for(i=0; i<maxx; i++) {

                total_nodes++;

                long id = node_id(i,j,k);
                Node* node = &nodes[id];
                Link* link = NULL;
                int nlinks = node->nlinks;
                int forward = 0;
                int backward = 0;

                for (int l=0; l<nlinks; l++) {
                    link = node->link[l];
                    // Error
                    if (link == NULL) {
                        printf("*** ERROR - Null link found\n");
                        printf("***   pos i,j,k: %d,%d,%d\n", i,j,k);
                        printf("***   nlinks:     %d\n", nlinks);
                        printf("***   link index: %d\n", l);
                        exit(1);
                    }

                    if (is_forward_link(node, l) == true){
                        forward++;
                        // printf(">");
                    } else {
                        backward++;
                        // printf("<");
                    }
                }
                // printf(" ");

                if (nlinks == 0) {
                    null_count++;
                } else if (nlinks == forward){
                    source_count++;
                } else  if (nlinks == backward){
                    sink_count++;
                } else {
                    other_count++;
                }
            }
        }
    }

    printf("  Total Nodes:  %10d\n", total_nodes);
    printf("  Source Nodes: %10d\n", source_count);
    printf("  Sink Nodes:   %10d\n", sink_count);
    printf("  Other Nodes:  %10d\n", other_count);
    printf("\n");
}

//////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[]) {

    show_resources();
    show_configuration();

    create_initial_state();

//    create_hypergraph();

    show_statistics();

//    display_graph();

    return 0;
}
