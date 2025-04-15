#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <cstdint>

class Scheduler {
public:
    static const uint8_t MAX_TASKS = 10;
    static const uint32_t NO_TASK_ID = 0;

    Scheduler();
    void SCH_Init();
    void SCH_Update();
    uint32_t SCH_Add_Task(void (*pFunction)(), uint32_t delay, uint32_t period);
    uint8_t SCH_Delete_Task(uint32_t taskID);
    void SCH_Dispatch_Tasks();

private:
    struct Task {
        void (*pTask)(void) = nullptr;
        uint32_t delay = 0;
        uint32_t period = 0;
        uint8_t runMe = 0;
        uint32_t taskID = 0;
    };

    Task tasks[MAX_TASKS];
    uint32_t newTaskID = 0;

    uint32_t Get_New_Task_ID();
};

// Global instance and C-style API
extern Scheduler scheduler;

void SCH_Init();
void SCH_Update();
uint32_t SCH_Add_Task(void (*pFunction)(), uint32_t delay, uint32_t period);
uint8_t SCH_Delete_Task(uint32_t taskID);
void SCH_Dispatch_Tasks();

#endif // SCHEDULER_H_
