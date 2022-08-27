// Hypergraph
// Computationaly generate hypergraph

#include <stdio.h>
#include <stdlib.h>            // random()
#define MAXNODES 10000000      // 10 timeslices
#define MAXLINKS MAXNODES * 8

#define MAXFROMNODES 8
#define MAXTONODES   8

int debug = 1;

typedef struct node {
    long fromnodes[MAXFROMNODES];
    int  nfromnodes;
    long tonodes[MAXTONODES];
    int  ntonodes;
} node;
node nodes[MAXNODES];

typedef struct link {
    long a;
    long b;
} link;
link spacetime[MAXLINKS];

long nextnode = 0;
long nextlink = 0;

// Error Codes
#define OK                  0
#define ERR_NULLNODE        1
#define ERR_TOMANYFROMNODES 2
#define ERR_TOMANYTONODES   3


long node_id(int x, int y, int z) {
    return (100*(100*(x%100) + (y%100)) + (z%100));
}

long node_id_p(int x, int y, int z) {
    return node_id(x,y,z);
}

long node_id_q(int x, int y, int z) {
    return node_id(x,y,z) + 100*100*100;
}

void show_resources(void) {
    printf("  Size of Node:      %10ld bytes\n", sizeof(node));
    printf("  Size of Link:      %10ld bytes\n", sizeof(link));
    printf("  Max. Nodes:        %10d\n", MAXNODES);
    printf("  Max. Links:        %10d\n", MAXLINKS);
    printf("  Per node:\n");
    printf("    Max. From Nodes: %d\n", MAXFROMNODES);
    printf("    Max. To Nodes:   %d\n", MAXTONODES);
    }

void add_node(void) {
    node node_a = nodes[nextnode];

    node_a.ntonodes = 0;
    node_a.nfromnodes = 0;
    nextnode++;
}

// Node/link indexing
int add_to_node(long a, long b) {
    node *a_node;


    a_node = &nodes[a];

    if(a_node->ntonodes < MAXTONODES){
        a_node->tonodes[a_node->ntonodes] = b;
        a_node->ntonodes++;
        return OK;
    } else {
        return ERR_TOMANYTONODES;
    }
}

int add_from_node(long b, long a) {
    node *b_node;
    b_node = &nodes[b];

    if(b_node->nfromnodes < MAXFROMNODES){
        b_node->fromnodes[b_node->nfromnodes] = b;
        b_node->nfromnodes++;
        return OK;
    } else {
        return ERR_TOMANYFROMNODES;
    }
}

void push_link(long a, long b) {
    spacetime[nextlink].a = a;
    spacetime[nextlink].b = b;
    nextlink++;

    add_to_node(a,b);
    add_from_node(b,a);
}

void create_initial_state(void) {
    int i,j,k;
    long p;

    for(i=0;i<100;i++) {
        for(j=0;j<100;j++) {
            for(k=0;k<100;k++) {
                // Allocate first two layers
                add_node();
                add_node();
            }
        }
    }
    for(i=0;i<100;i++) {
        for(j=0;j<100;j++) {
            for(k=0;k<100;k++) {
                p = node_id_p(i,j,k);
                push_link(p, node_id_q(i,  j,  k));
                push_link(p, node_id_q(i+1,j,  k));
                push_link(p, node_id_q(i,  j+1,k));
                push_link(p, node_id_q(i,  j,  k+1));
                push_link(p, node_id_q(i+1,j+1,k));
                push_link(p, node_id_q(i+1,j  ,k+1));
                push_link(p, node_id_q(i,  j+1,k+1));
                push_link(p, node_id_q(i+1,j+1,k+1));
            }
        }
    }
}

void show_node_details (long a) {
    node node_a = nodes[a];

    printf("Node: %10ld", a);
    printf("  from-nodes: %2d  to-nodes: %2d\n",
           node_a.ntonodes,
           node_a.nfromnodes
        );
}

void create_hypergraph(void) {
    long random_node;
    int i;

    for(i=0; i<10; i++){
        random_node = random() % nextnode;
        if(debug) show_node_details(random_node);
    }
}


void show_statistics(void) {
    printf("  Nodes: %10ld (%6.2f%%)\n", nextnode, 100.0 * nextnode / MAXNODES);
    printf("  Links: %10ld (%6.2f%%)\n", nextlink, 100.0 * nextlink / (MAXLINKS));
}

int main(int argc, char *argv[]) {
    printf("Available Resources\n");
    show_resources();
    printf("\n");

    printf("Create Initial State\n");
    create_initial_state();
    printf("\n");

    printf("Creating Hypergraph\n");
    create_hypergraph();
    printf("\n");

    printf("Statistics\n");
    show_statistics();
    printf("\n");

    return 0;
}
