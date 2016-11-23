#include "can_queue.h"

void can_queue_init(CANQueue *queue) {
  pqueue_backed_init(&queue->pqueue, queue->queue_nodes, queue->msg_nodes);
}

bool can_queue_push(CANQueue *queue, const CANMessage *msg) {
  return pqueue_backed_push(&queue->pqueue, msg, msg->id);
}

bool can_queue_pop(CANQueue *queue, CANMessage *msg) {
  return pqueue_backed_pop(&queue->pqueue, msg);
}

size_t can_queue_size(CANQueue *queue) {
  return pqueue_backed_size(&queue->pqueue);
}
