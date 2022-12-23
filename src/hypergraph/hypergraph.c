// Hypergraph
// Computationaly generate hypergraph

#include <stdio.h>
#include <stdlib.h>            // random()
#define MAXNODES 10000000      // 10 timeslices
#define MAXLINKS MAXNODES*8

#define MAXNODELINKS     8

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
// Lattice structure
long node_id(int x, int y, int z) {
    return (100*(100*(x%100) + (y%100)) + (z%100));
}

void show_resources(void) {
    printf("  Size of Node:      %10ld bytes\n", sizeof(Node));
    printf("  Size of Link:      %10ld bytes\n", sizeof(Link));
    printf("  Max. Nodes:        %10d\n", MAXNODES);
    printf("  Max. Links:        %10d\n", MAXLINKS);
    printf("  Per node:\n");
    printf("    Max. Links: %d\n", MAXNODELINKS);
}

long add_node(long node) {
    Node* node_a = &(nodes[node]);

    // Initialize node data
    node_a->nlinks = 0;  // No links
}

// Node/link indexing
int add_link_to_nodes(long a, long b, long l) {
    Node* node_a;
    Node* node_b;
    Link* link_ab;

    node_a  = &nodes[a];
    node_b  = &nodes[b];
    link_ab = &structure[l];

    // TODO: Refactor into a 'add_link_to_node(a,l)'
    // Add link reference to node a
    if(node_a->nlinks < MAXNODELINKS){
        node_a->link[node_a->nlinks] = link_ab;
        node_b->nlinks++;
        return OK;
    } else {
        return ERR_TOMANYLINKS;
    }

    // Add link reference to node b
    if(node_b->nlinks < MAXNODELINKS){
        node_b->link[node_b->nlinks] = link_ab;
        node_b->nlinks++;
        return OK;
    } else {
        return ERR_TOMANYLINKS;
    }

    link_ab->a = node_a;
    link_ab->b = node_b;
    link_ab->dir = FORWARD;

}

/*
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
*/

/*
     void show_node_details (long a) {
         node node_a = nodes[a];

         printf("Node: %10ld", a);
         printf("  from-nodes: %2d  to-nodes: %2d\n",
                node_a.ntonodes,
                node_a.nfromnodes
             );
             }
*/

/*
  void create_hypergraph(void) {
  long random_node;
  int i;

  for(i=0; i<10; i++){
  random_node = random() % nextnode;
  if(debug) show_node_details(random_node);
  }
  }
*/

/*
  void show_statistics(void) {
  printf("  Nodes: %10ld (%6.2f%%)\n", nextnode, 100.0 * nextnode / MAXNODES);
  printf("  Links: %10ld (%6.2f%%)\n", nextlink, 100.0 * nextlink / (MAXLINKS));
  }
*/

//////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[]) {
    printf("Available Resources\n");
    show_resources();
    printf("\n");

//    printf("Create Initial State\n");
//    create_initial_state();
//    printf("\n");

//    printf("Creating Hypergraph\n");
//    create_hypergraph();
//    printf("\n");

//    printf("Statistics\n");
//    show_statistics();
//    printf("\n");

    return 0;
}
