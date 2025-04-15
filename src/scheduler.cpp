#include "scheduler.h"

// Global instance
Scheduler scheduler;

Scheduler::Scheduler() {
    SCH_Init();
}

void Scheduler::SCH_Init() {
    for (uint8_t i = 0; i < MAX_TASKS; ++i) {
        tasks[i] = Task();
    }
}

void Scheduler::SCH_Update() {
    if (tasks[0].pTask && tasks[0].runMe == 0) {
        if (tasks[0].delay > 0) {
            tasks[0].delay--;
        }
        if (tasks[0].delay == 0) {
            tasks[0].runMe = 1;
        }
    }
}

uint32_t Scheduler::SCH_Add_Task(void (*pFunction)(), uint32_t delay, uint32_t period) {
    uint8_t index = 0;
    uint32_t sumDelay = 0;
    uint32_t newDelay = 0;

    for (index = 0; index < MAX_TASKS; ++index) {
        sumDelay += tasks[index].delay;
        if (sumDelay > delay) {
            newDelay = delay - (sumDelay - tasks[index].delay);
            tasks[index].delay = sumDelay - delay;

            for (int i = MAX_TASKS - 1; i > index; --i) {
                tasks[i] = tasks[i - 1];
            }

            tasks[index].pTask = pFunction;
            tasks[index].delay = newDelay;
            tasks[index].period = period;
            tasks[index].runMe = (newDelay == 0) ? 1 : 0;
            tasks[index].taskID = Get_New_Task_ID();

            return tasks[index].taskID;
        } else if (!tasks[index].pTask) {
            tasks[index].pTask = pFunction;
            tasks[index].delay = delay - sumDelay;
            tasks[index].period = period;
            tasks[index].runMe = (tasks[index].delay == 0) ? 1 : 0;
            tasks[index].taskID = Get_New_Task_ID();

            return tasks[index].taskID;
        }
    }
    return NO_TASK_ID;
}

uint8_t Scheduler::SCH_Delete_Task(uint32_t taskID) {
    for (uint8_t i = 0; i < MAX_TASKS; ++i) {
        if (tasks[i].taskID == taskID) {
            if (i != 0 && i < MAX_TASKS - 1 && tasks[i + 1].pTask) {
                tasks[i + 1].delay += tasks[i].delay;
            }

            for (uint8_t j = i; j < MAX_TASKS - 1; ++j) {
                tasks[j] = tasks[j + 1];
            }
            tasks[MAX_TASKS - 1] = Task(); // Clear last task
            return 1;
        }
    }
    return 0;
}

void Scheduler::SCH_Dispatch_Tasks() {
    if (tasks[0].runMe > 0) {
        (*tasks[0].pTask)();
        tasks[0].runMe = 0;

        Task temp = tasks[0];
        SCH_Delete_Task(temp.taskID);

        if (temp.period != 0) {
            SCH_Add_Task(temp.pTask, temp.period, temp.period);
        }
    }
}

uint32_t Scheduler::Get_New_Task_ID() {
    newTaskID++;
    if (newTaskID == NO_TASK_ID) {
        newTaskID++;
    }
    return newTaskID;
}

// C-style API Wrappers
void SCH_Init() {
    scheduler.SCH_Init();
}

void SCH_Update() {
    scheduler.SCH_Update();
}

uint32_t SCH_Add_Task(void (*pFunction)(), uint32_t delay, uint32_t period) {
    return scheduler.SCH_Add_Task(pFunction, delay, period);
}

uint8_t SCH_Delete_Task(uint32_t taskID) {
    return scheduler.SCH_Delete_Task(taskID);
}

void SCH_Dispatch_Tasks() {
    scheduler.SCH_Dispatch_Tasks();
}
