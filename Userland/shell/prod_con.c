#include <sync_queue.h>
#include <stdbool.h>
#include <c_syscall.h>
#include <scanf.h>
#include <clib.h>
#include <stdbool.h>
#include <prodconsgui.h>
#define MIN_SPEED 10

static void * producer(uint64_t argc, uint8_t ** argv) {
	SQueue * pq = (SQueue *) argv[0];
	int * prod_speed = (int *) argv[1];
	guiprodcon * pcg = (guiprodcon *) argv[2];
	int i = 0;
	bool text = (bool) *argv[3];
	free(argv);
	while(1) {
		//Think
		sleep(*prod_speed);
		senque(pq, i, pcg, text);
		if (text) {
			printf("Producido %d. Ahora hay %d/%d items en la cola.\n", i, squeue_size(pq), squeue_max_size(pq));
		}
		i++;
	}
	exit();
}

static void * consumer(uint64_t argc, uint8_t ** argv) {
	SQueue * pq = (SQueue *) argv[0];
	int * cons_speed = (int *) argv[1];
	guiprodcon * pcg = (guiprodcon *) argv[2];
	int i = 0;
	bool text = (bool) *argv[3];
	free(argv);
	while(1) {
		//Think
		sleep(*cons_speed);
		i = sdeque(pq, pcg, text);
		if (text) {
			printf("Consumido %d. Ahora hay %d/%d items en la cola.\n", i, squeue_size(pq), squeue_max_size(pq));
		}
	}
	exit();
}

int64_t prod_con(int64_t argc, int64_t * argv[])
{
	int max_capacity, radius, * cons_speed, * prod_speed;
	bool text_mode = false,  * text_prod, * text_cons;
	if (argc < 2) {
		radius = 250;
	}
	else {
		sscanf(argv[1], "%d", &radius);
	}
	if (argc < 1) {
		/*
		printf("Por favor indique la capacidad de la cola.\n");
		wkexit();
		*/
		max_capacity = 5;
	}
	else {
		sscanf(argv[0], "%d", &max_capacity);
		if (max_capacity > 30) {
			text_mode =true;
		}
	}

	Args * arg1 = malloc(sizeof(Args));
	Args * arg2 = malloc(sizeof(Args));
	arg1->fg = 0;
	arg2->fg = 0;
	SQueue * sq = squeue_init(max_capacity);
	guiprodcon * pcg = 0;
	if (!text_mode)
		pcg= renderinit(max_capacity, radius);

	cons_speed = malloc(sizeof(int));
	prod_speed = malloc(sizeof(int));

	*cons_speed = 10;
	*prod_speed = 10;
	*text_prod = text_mode;
	*text_cons = text_mode;

	arg1->argc = 3;
	arg2->argc = 3;
	arg1->argv = malloc(4 * sizeof(void *));
	arg2->argv = malloc(4 * sizeof(void *));
	arg1->argv[0] = sq;
	arg2->argv[0] = sq;
	arg1->argv[1] = prod_speed;
	arg2->argv[1] = cons_speed;
	arg1->argv[2] = pcg;
	arg2->argv[2] = pcg;
	arg1->argv[3] = text_prod;
	arg2->argv[3] = text_cons;
	int prod_pid = fkexec(producer, "producer", arg1);
	int cons_pid = fkexec(consumer, "consumer", arg2);

	int8_t c, end = false, count = 0;
	while(!end) {
		c = getchar();
		if (count == 5) {
			if (!text_mode)
				squeue_render(sq, pcg);
			count = 0;
		}
		else {
			count++;
		}
		switch(c) {
			case 'q':
				kill(prod_pid, 0);
				kill(cons_pid, 0);
				squeue_destroy(sq);
				free(cons_speed);
				free(prod_speed);
				renderdestroy(pcg);
				end = true;
				break;
			case 'a':
				/* consumption speed up */
				if (*cons_speed > 0){
					(*cons_speed)--;
					if (text_mode){
						printf("La velocidad del consumidor aumento. Ahora es %d\n", *cons_speed );
					}
				}
				else {
					if (text_mode)
						printf("La velocidad del consumidor esta al maximo.\n");

				}
				break;
			case 'z':
				/* consumption speed down */
				if (*cons_speed < MIN_SPEED ){

					(*cons_speed)++;
					if (text_mode)
						printf("La velocidad del consumidor diminuyo. Ahora es %d\n", *cons_speed );

				}
				else {
					if (text_mode)
						printf("La velocidad del consumidor esta al minimo.\n");

				}
				break;
			case 's':
				/* production speed up */
				if (*prod_speed > 0  ){
					(*prod_speed)--;
					if (text_mode)
						printf("La velocidad del productor aumento. Ahora es %d\n", *prod_speed );

				}
				else {
					if (text_mode)
						printf("La velocidad del productor esta al maximo.\n");

				}
				break;
			case 'x':
				/* production speed down */
				if (*prod_speed < MIN_SPEED ){
					(*prod_speed)++;
					if (text_mode)
						printf("La velocidad del productor disminuyo. Ahora es %d\n", *prod_speed );

				}
				else {
					if (text_mode)
						printf("La velocidad del productor esta al minimo.\n");

				}
				break;
			default:
				break;
		}
	}
	wkexit();
}
