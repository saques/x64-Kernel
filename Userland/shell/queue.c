#include <queue.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct Q_node
{
     int64_t tile;
     struct Q_node * next;

} Q_node;

typedef struct Queue
{
  Q_node * first;
  Q_node * last;
  int64_t size;
} Queue;

Queue_p new_queue(void)
{
	Queue_p aux = malloc(sizeof(Queue));
	aux->size = 0;
	aux->first = 0;
	aux->last = 0;
  return aux;
}

int64_t enque(Queue_p q, int64_t t)
{
     Q_node * node = malloc(sizeof(Q_node));
     if (!q || !t){
	  return -1;
     }

     if (!node) {
	  return -1;
     }
     node->next = NULL;
     node->tile = t;
     if (!q->first) {
	  q->first = node;
	  q->last = node;
     }
     else{
	  q->last->next = node;
	  q->last = node;
     }
	 (q->size)++;
     return 0;
}

int64_t deque(Queue_p q)
{
     Q_node * node;
     void * t;

     if (!q || !q->first){
	  return NULL;
     }
     node = q->first;
     if (node == q->last) {
	  q->first = NULL;
	  q->last = NULL;
     }
     else {
	  q->first = q->first->next;
     }
     t = node->tile;
     free(node);
	 (q->size)--;
     return t;
}
void q_destroy(Queue_p q)
{
	Q_node * qn = q->first, * aux;
	while(qn!=NULL) {
		aux = qn;
		qn = qn->next;
		free(aux);
	}
	free(q);
}
int8_t q_is_empty(Queue_p q)
{
     return q->first == NULL;
}

int64_t q_size(Queue_p q)
{
	return q->size;
}
