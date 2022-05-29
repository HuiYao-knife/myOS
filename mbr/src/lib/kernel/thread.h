#ifndef	__THREAD_H
#define	__THREAD_H
#include "stdint.h"
#include "list.h"
#include "memory.h"
#define	MAX_FILES_OPEN_PER_PROC	8
typedef	void thread_func(void*);
typedef int16_t pid_t;

enum	task_status{
	TASK_RUNNING,
	TASK_READY,
	TASK_BLOCKED,
	TASK_WAITING,
	TASK_HANGING,
	TASK_DIED
};

struct intr_stack{
	uint32_t vec_no;
	uint32_t edi;
	uint32_t esi;
	uint32_t ebp;
	uint32_t esp_dummy;
	uint32_t ebx;
	uint32_t edx;
	uint32_t ecx;
	uint32_t eax;
	uint32_t gs;
	uint32_t fs;
	uint32_t es;
	uint32_t ds;
	
	uint32_t err_code;
	void (*eip) (void);
	uint32_t cs;
	uint32_t eflags;
	void * esp;
	uint32_t ss;
};

struct thread_stack {
	uint32_t ebp;
	uint32_t ebx;
	uint32_t edi;
	uint32_t esi;
	void (*eip) (thread_func * func, void * func_arg);

	void (*unused_retaddr);
	thread_func * function;
	void * func_arg;
};

struct task_struct{
	uint32_t  *self_kstack;
	pid_t	pid;
	enum task_status status;
	uint8_t priority;
	char name[16];
	
	uint8_t ticks;
	uint32_t elapsed_ticks;
	int32_t fd_table[MAX_FILES_OPEN_PER_PROC];
	struct list_elem general_tag;
	struct list_elem all_list_tag;
	uint32_t * pgdir;
	struct virtual_addr userprog_vaddr;
	struct mem_block_desc u_block_descs[DESC_CNT];
	uint32_t cwd_inode_nr;
	int16_t parent_pid; 
	int8_t exit_status;
	uint32_t stack_magic;
};

extern struct list thread_ready_list;
extern struct list thread_all_list;
void schedule();
struct task_struct * running_thread();
void thread_create(struct task_struct * pthread, thread_func function, void * func_arg);
static void kernel_thread(thread_func * function, void * func_arg);
void init_thread(struct task_struct * pthread, char * name, int prio);
struct task_struct * thread_start(char * name, int prio, thread_func function, void * func_arg);
static void make_main_thread(void);
void thread_unblock(struct task_struct* pthread);
void thread_block(enum task_status stat);
static pid_t allocate_pid(void);
void idle(void * arg);
void thread_yield(void);
void init(void);
pid_t fork_pid(void);
static void pad_print(char *buf, int32_t buf_len, void * ptr, char format);
void sys_ps(void);
static bool elem2thread_info(struct list_elem * pelem, int arg);
static void pid_pool_init(void);
static pid_t allocate_pid(void);
void release_pid(pid_t pid);
static uint32_t pid_check(struct list_elem * pelem, int32_t pid);
void thread_exit(struct task_struct * thread_over, uint32_t need_schedule);
#endif
