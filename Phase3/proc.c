#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/tty.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/delay.h>

MODULE_LICENSE("OS");
MODULE_AUTHOR("The Nigger");
MODULE_DESCRIPTION("A module which gives proc ");



int tasks[20000];



int global = 1;
int nigger (void* sag)
{
	allow_signal (SIGKILL);
	struct task_struct* task;
	while (global)
	{
		printk("------seek help--------------- checking processes memory allocations...\n");
        	for_each_process(task) {
                	if(task->mm != NULL)
				if (tasks[task->pid] != task->mm->total_vm) {
                        		printk (KERN_INFO "PROCESS: %s (pid = %d) Memory Allocated: %d\n",task->comm, task_pid_nr(task), tasks[task->pid] - task->mm->total_vm);
					printk (KERN_INFO "sad %d:", task->pid);			
					tasks[task->pid] = task->mm->total_vm;
				}
       		}
		ssleep(1);
	}
	return 0;
}

struct task_struct* task_glob;

static int proc(void)
{
	task_glob = kthread_create (&nigger, NULL, "Thread");
	task_glob = kthread_run (&nigger, NULL, "Thread");
        return 0;
}

static void clear_jews(void)
{
	printk(KERN_INFO "Stopping thread of procs");
	global = 0;
	kthread_stop (task_glob);
}

module_init(proc);
module_exit(clear_jews);


