#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"

// 任务1：每秒打印一次
void vTask1(void *pvParameters) {
    while (1) {
        printf("Task 1 is running\n");
        vTaskDelay(pdMS_TO_TICKS(1000)); // 延时1秒
    }
}

// 任务2：每2秒打印一次
void vTask2(void *pvParameters) {
    int taskID = *(int *)pvParameters; // 获取任务ID
    while (1) {
        printf("Task %d is running\n", taskID);
        vTaskDelay(pdMS_TO_TICKS(2000)); // 延时2秒
    }
}

int main() {
    stdio_init_all(); // 初始化标准输入输出（用于 printf）
    int id2 = 3;
    
    // 创建任务1，优先级为 1
    xTaskCreate(vTask1, "Task 1", 256, NULL, 1, NULL);
    // 创建任务2，优先级为 1
    xTaskCreate(vTask2, "Task 2", 256, &id2, 1, NULL);
    
    // 启动 FreeRTOS 调度器（程序将不再返回）
    vTaskStartScheduler();
    
    // 如果调度器意外退出，进入死循环
    while (1) {
        tight_loop_contents();
    }
}