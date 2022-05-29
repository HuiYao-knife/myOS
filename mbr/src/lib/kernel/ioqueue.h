#ifndef	__IOQUEUE_H
#define	__IOQUEUE_H
#include "stdint.h"
#include "thread.h"
#include "sync.h"
#define	bufsize	64

struct ioqueue{
	struct lock lock;
	struct task_struct * producer;
	struct task_struct * consumer;
	char buf[bufsize];
	int32_t head;
	int32_t tail;
};

char ioq_getchar(struct ioqueue * ioq);
void ioq_putchar(struct ioqueue * ioq, char ch);
static void wakeup(struct task_struct ** waiter);
static void wait(struct task_struct ** waiter);
bool ioq_empty(struct ioqueue * ioq);
bool ioq_full(struct ioqueue * ioq);
void ioqueue_init(struct ioqueue * ioq);
static int32_t next_pos(int32_t pos);
uint32_t ioq_length(struct ioqueue * ioq);
#endif
