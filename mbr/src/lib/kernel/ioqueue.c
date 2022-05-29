#include "ioqueue.h"
#include "interrupt.h"
#include "thread.h"
#include "global.h"
#include "debug.h"

void ioqueue_init(struct ioqueue * ioq){
	lock_init(&ioq->lock);
	ioq->head = ioq->tail = 0;
	ioq->producer = ioq->consumer = NULL;
}
 
static int32_t next_pos(int32_t pos){
	return (pos + 1) % bufsize;
}

bool ioq_full(struct ioqueue * ioq){
	ASSERT(intr_get_status() == INTR_OFF);
	return next_pos(ioq->head) == ioq->tail;
}

bool ioq_empty(struct ioqueue * ioq){
	ASSERT(intr_get_status() == INTR_OFF);
	return ioq->head == ioq->tail;
}

static void wait(struct task_struct ** waiter){
	ASSERT(*waiter == NULL && waiter != NULL);
	*waiter = running_thread();
	thread_block(TASK_BLOCKED);
}

static void wakeup(struct task_struct ** waiter){
	ASSERT(*waiter != NULL);
	thread_unblock(*waiter);
	*waiter = NULL;
}

char ioq_getchar(struct ioqueue * ioq){
	//ASSERT(intr_get_status() == INTR_OFF);
	enum intr_status old_status = intr_get_status();
	intr_disable();
	while(ioq_empty(ioq)){
		
		lock_acquire(&ioq->lock);
		wait(&ioq->consumer);
		lock_release(&ioq->lock);
	}
	char ch = ioq->buf[ioq->tail];
	ioq->tail  = next_pos(ioq->tail);
	
	if(ioq->producer != NULL){
		wakeup(&ioq->consumer);
	}
	intr_set_status(old_status);
	return ch;
}

void ioq_putchar(struct ioqueue * ioq, char ch){
	//ASSERT(intr_get_status() == INTR_OFF);
	enum intr_status old_status = intr_get_status();
	intr_disable();
	while(ioq_full(ioq)){
		lock_acquire(&ioq->lock);
		wait(&ioq->producer);
		lock_release(&ioq->lock);
	}
	
	ioq->buf[ioq->head] = ch;	
	ioq->head = next_pos(ioq->head);
	
	if(ioq->consumer != NULL){
		wakeup(&ioq->consumer);
	}
	intr_set_status(old_status);
}
uint32_t ioq_length(struct ioqueue * ioq){
	uint32_t len = 0;
	if(ioq->head >= ioq->tail){
		len = ioq->head - ioq->tail;
	}else{
		len = bufsize - (ioq->tail - ioq->head);
	}
	return len;
}
