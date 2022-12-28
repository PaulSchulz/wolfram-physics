// Hypergraph
// Computationaly generate hypergraph

#include <stdio.h>
#include <stdlib.h>            // random()
#include <stdbool.h>           // true, false
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
    printf("  Size of Node:      %10ld bytes\n", sizeof(Node));
    printf("  Size of Link:      %10ld bytes\n", sizeof(Link));
    printf("  Max. Nodes:        %10d\n", MAXNODES);
    printf("  Max. Links:        %10d\n", MAXLINKS);
    printf("  Per node:\n");
    printf("    Max. Links: %d\n", MAXNODELINKS);
    printf("\n");
    printf("  maxx: %d\n", maxx);
    printf("  maxy: %d\n", maxy);
    printf("  maxz: %d\n", maxz);
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

}

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
    int i,j,k;

    int total_nodes = 0;

    int source_count = 0;
    int sink_count   = 0;
    int other_count  = 0;

    int source_count2 = 0;
    int sink_count2   = 0;
    int other_count2  = 0;

    printf("  Source Nodes2: %d\n", source_count2);
    printf("  Sink Nodes2:   %d\n", sink_count2);
    printf("  Other Nodes2:  %d\n", other_count2);
    printf("\n");

    for(k=0; k<maxz; k++) {
        for(j=0;j<maxy; j++) {
            for(i=0;i<maxx; i++) {

                total_nodes++;

                long id = node_id(i,j,k);
                Node* node = &nodes[id];
                Link* link = NULL;
                int nlinks = node->nlinks;
                int forward = 0;
                int backward = 0;

                int forward2 = 0;
                int backward2 = 0;

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
                    if(link->a == node) {
                        if(link->dir == FORWARD) {
                            forward++;
                        } else {
                            backward++;
                        }
                    } else {
                        if(link->dir == BACKWARD) {
                            forward++;
                        } else {
                            backward++;
                        }
                    }

                    if (is_forward_link(node, l) == true){
                        forward2++;
                        // printf("--> %d ", is_forward_link(node, l));
                    } else {
                        backward2++;
                        // printf("<-- %d ", is_forward_link(node, l));
                    }
                }
                // printf("%d\n", nlinks);

                if (nlinks == forward){
                    source_count++;
                } else if (nlinks == backward){
                    sink_count++;
                } else {
                    other_count++;
                }

                if (nlinks == 0) {
                    other_count++;
                } else if (nlinks == forward2) {
                    source_count2++;
                } else if (nlinks == backward2){
                    sink_count2++;
                } else {
                    other_count2++;
                }

            }
        }
    }

    printf("  Source Nodes: %d\n", source_count);
    printf("  Sink Nodes:   %d\n", sink_count);
    printf("  Other Nodes:  %d\n", other_count);
    printf("\n");
    printf("  Source Nodes2: %d\n", source_count2);
    printf("  Sink Nodes2:   %d\n", sink_count2);
    printf("  Other Nodes2:  %d\n", other_count2);
    printf("\n");
    printf("\n");

}

//////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[]) {
    printf("Available Resources\n");
    show_resources();
    printf("\n");

    printf("Create Initial State\n");
    create_initial_state();
    printf("\n");

//    printf("Creating Hypergraph\n");
//    create_hypergraph();
//    printf("\n");

    printf("Statistics\n");
    show_statistics();
    printf("\n");

//    display_graph();

    return 0;
}
