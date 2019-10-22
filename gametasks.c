/* ENCE260 embedded systems project
 * Group 108
 * Drogo Shi, Lucas Payne
 * 
 * Header file for the game tasks module.
 */

#include "gametasks.h"
#include "pacer.h"

void create_task(Task *task, void (*func) (Task *), int period, void *data)
{
    task->func = func;
    task->period = period;
    task->data = data;
    task->data2 = 0;
    task->lifetime = 0;
    task->on = 1;
}

void reset_tasks (Task *tasks, int num_tasks)
{
    for (int i = 0; i < num_tasks; i++) {
        tasks[i].lifetime = 0;
    }
}

int task_multiplex_step (Task *tasks, int num_tasks, int step)
{
    int any_task_done = 0;
    for (int i = 0; i < num_tasks; i++) {
        if (tasks[i].on && (step % tasks[i].period == 0)) {
            tasks[i].func(&tasks[i]);
            tasks[i].lifetime ++;
            any_task_done = 1;
        }
    }

    pacer_wait ();
    
    return any_task_done;
}
