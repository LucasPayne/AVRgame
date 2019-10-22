/* ENCE260 embedded systems project
 * Group 108
 * Drogo Shi, Lucas Payne
 * 
 * Header file for the game tasks module.
 */

#ifndef GAME_TASKS_HEADER_DEFINED
#define GAME_TASKS_HEADER_DEFINED

typedef struct Task_s {
    void (*func) (struct Task_s *);
    void *data;
    void *data2;
    int period;
    int lifetime;
    int on;
} Task;

void create_task(Task *task, void (*func) (Task *), int period, void *data);

void reset_tasks (Task *tasks, int num_tasks);
int task_multiplex_step (Task *tasks, int num_tasks, int step);


#endif
