#include "global.h"
#include "thread.h"
#include "stdint.h"

static void release_prog_resource(struct task_struct * release_thread);
static uint32_t find_child(struct list_elem * pelem, int32_t ppid);
static uint32_t find_hanging_child(struct list_elem *pelem, int32_t ppid);
void sys_exit(int32_t status);
pid_t sys_wait(int32_t status);
static uint32_t init_adopt_a_child(struct list_elem * pelem, int32_t pid);
