#ifndef Mission_hpp
#define Mission_hpp 

#include <stdio.h>
#include <iostream>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "BMI270.hpp"

inline SemaphoreHandle_t xPrintMutex = NULL;  // 定义互斥量句柄

void vTask1(void *pvParameters);
void vTask2(void *pvParameters);

#endif /* Mission_hpp */