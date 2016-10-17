#pragma once
// Generic priority queue
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct PQueueNode {
  void *data;
  uint16_t prio;
} PQueueNode;

typedef struct PQueue {
  PQueueNode *nodes;
  size_t max_nodes;
  size_t size;
} PQueue;

// Initialize and clear the priority queue.
// Note that the maximum size of the queue will be num_nodes - 1.
void pqueue_init(PQueue *queue, PQueueNode *nodes, size_t num_nodes);

// Push a node with the specified priority and data onto the queue.
bool pqueue_push(PQueue *queue, void *data, uint16_t prio);

// Pop the minimum node from the queue and return its data.
void *pqueue_pop(PQueue *queue);

// Returns the number of nodes currently in the queue.
size_t pqueue_size(PQueue *queue);
