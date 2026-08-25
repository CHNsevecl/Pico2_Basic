/*
 * FreeRTOS
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://aws.amazon.com/freertos
 *
 */
#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/* ============================================================
 * 中断服务函数映射（将 FreeRTOS 的中断处理函数映射到 Pico SDK 的实现）
 * ============================================================ */
#define vPortSVCHandler         isr_svcall      // SVC 系统调用处理
#define xPortPendSVHandler      isr_pendsv      // PendSV 任务切换处理
#define xPortSysTickHandler     isr_systick     // SysTick 系统时钟中断处理


/* ============================================================
 * 调度器相关配置
 * ============================================================ */
#define configUSE_PREEMPTION                    1   // 1=抢占式调度（高优先级可打断低优先级），0=协作式调度（任务主动让出）
#define configUSE_TIME_SLICING                  1   // 1=启用时间片轮转（同优先级任务轮流执行），0=禁用
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 0   // 1=启用硬件优化任务选择（提高效率），0=使用通用算法
#define configUSE_TICKLESS_IDLE                 0   // 1=启用 Tickless 低功耗模式（空闲时停止时钟），0=禁用
#define configCPU_CLOCK_HZ                      150000000   // CPU 主频（RP2350 = 150MHz），用于配置系统时钟
#define configTICK_RATE_HZ                      1000        // 系统时钟节拍频率（1000Hz = 1ms/滴答）
#define configMAX_PRIORITIES                    32          // 最大任务优先级（0~31，数值越大优先级越高）
#define configMINIMAL_STACK_SIZE                256         // 空闲任务的最小堆栈大小（单位：字，256字 = 1024字节）
#define configMAX_TASK_NAME_LEN                 16          // 任务名称的最大长度（字节）
#define configUSE_16_BIT_TICKS                  0           // 1=使用16位 Tick 计数（旧芯片），0=使用32位（现代芯片）
#define configIDLE_SHOULD_YIELD                 1           // 1=空闲任务主动让出 CPU（同优先级时），0=不主动让出
#define configUSE_TASK_NOTIFICATIONS            1           // 1=启用任务通知（轻量级任务间通信），0=禁用
#define configTASK_NOTIFICATION_ARRAY_ENTRIES   3           // 每个任务的通知数组大小（支持多个通知）
#define configUSE_PASSIVE_IDLE_HOOK             0           // 1=启用被动空闲钩子（SMP 多核专用），0=禁用


/* ============================================================
 * 同步与互斥相关配置
 * ============================================================ */
#define configUSE_MUTEXES                       1   // 1=启用互斥量（支持优先级继承），0=禁用
#define configUSE_RECURSIVE_MUTEXES             1   // 1=启用递归互斥量（同一任务可多次获取），0=禁用
#define configUSE_COUNTING_SEMAPHORES           1   // 1=启用计数信号量（管理多个资源），0=禁用
#define configQUEUE_REGISTRY_SIZE               10  // 可注册的消息队列数量（调试时可查看队列状态）
#define configUSE_QUEUE_SETS                    0   // 1=启用队列集（一个任务等待多个队列），0=禁用
#define configUSE_NEWLIB_REENTRANT              0   // 1=启用 Newlib 重入（多任务环境下 C 库安全），0=禁用（Pico SDK 不使用）
#define configENABLE_BACKWARD_COMPATIBILITY     0   // 1=启用旧版本 API 兼容，0=禁用（使用新 API）
#define configNUM_THREAD_LOCAL_STORAGE_POINTERS 5   // 每个任务的线程本地存储指针数量（用于存储任务私有数据）


/* ============================================================
 * 系统基础类型定义
 * ============================================================ */
#define configSTACK_DEPTH_TYPE                  uint16_t    // 堆栈深度变量的类型（uint16_t = 最大 65535 字）
#define configMESSAGE_BUFFER_LENGTH_TYPE        size_t      // 消息缓冲区长度变量的类型（size_t = 指针大小）


/* ============================================================
 * 内存分配相关配置
 * ============================================================ */
#define configSUPPORT_STATIC_ALLOCATION         0   // 1=支持静态内存分配（编译时分配），0=不支持
#define configSUPPORT_DYNAMIC_ALLOCATION        1   // 1=支持动态内存分配（运行时分配），0=不支持
#undef configAPPLICATION_ALLOCATED_HEAP
#define configAPPLICATION_ALLOCATED_HEAP        0   // 1=用户自定义堆空间（需要用户提供 ucHeap），0=使用 FreeRTOS 内部堆
#define configTOTAL_HEAP_SIZE                   (128*1024)  // FreeRTOS 总堆大小（128KB），用于动态内存分配


/* ============================================================
 * 钩子函数相关配置（在特定事件发生时回调用户函数）
 * ============================================================ */
#define configUSE_IDLE_HOOK                     0   // 1=启用空闲任务钩子（每次空闲循环调用），0=禁用
#define configUSE_TICK_HOOK                     0   // 1=启用时钟节拍钩子（每次中断调用），0=禁用
#define configCHECK_FOR_STACK_OVERFLOW          0   // 0=不检测堆栈溢出，1=运行时检测（慢），2=运行时检测（快）
#define configUSE_MALLOC_FAILED_HOOK            0   // 1=启用内存分配失败钩子，0=禁用
#define configUSE_DAEMON_TASK_STARTUP_HOOK      0   // 1=启用定时器服务任务启动钩子，0=禁用


/* ============================================================
 * 运行时统计相关配置
 * ============================================================ */
#define configGENERATE_RUN_TIME_STATS           0   // 1=生成运行时统计信息，0=禁用
#define configUSE_TRACE_FACILITY                0   // 1=启用跟踪工具（如 TaskTracer），0=禁用
#define configUSE_STATS_FORMATTING_FUNCTIONS    0   // 1=启用格式化统计函数（vTaskList），0=禁用


/* ============================================================
 * 协程相关配置（FreeRTOS 轻量级协程，已基本弃用）
 * ============================================================ */
#define configUSE_CO_ROUTINES                   0   // 1=启用协程，0=禁用
#define configMAX_CO_ROUTINE_PRIORITIES         1   // 协程最大优先级


/* ============================================================
 * 软件定时器相关配置
 * ============================================================ */
#define configUSE_TIMERS                        1   // 1=启用软件定时器，0=禁用
#define configTIMER_TASK_PRIORITY               3   // 定时器服务任务的优先级（正常应设置较高）
#define configTIMER_QUEUE_LENGTH                10  // 定时器命令队列的长度
#define configTIMER_TASK_STACK_DEPTH            256 // 定时器服务任务的堆栈大小


/* ============================================================
 * 断言配置（用于调试时捕获异常）
 * ============================================================ */
#define configASSERT( x )                       // 可定义为 `while(!(x)) {}` 或自定义断言


/* ============================================================
 * API 函数包含开关（决定哪些 API 被编译进固件）
 * ============================================================ */
#define INCLUDE_vTaskPrioritySet                1   // 包含 vTaskPrioritySet（设置任务优先级）
#define INCLUDE_uxTaskPriorityGet               1   // 包含 uxTaskPriorityGet（获取任务优先级）
#define INCLUDE_vTaskDelete                     1   // 包含 vTaskDelete（删除任务）
#define INCLUDE_vTaskSuspend                    1   // 包含 vTaskSuspend（挂起任务）
#define INCLUDE_xResumeFromISR                  1   // 包含 xTaskResumeFromISR（中断中恢复任务）
#define INCLUDE_vTaskDelayUntil                 1   // 包含 vTaskDelayUntil（绝对时间延时）
#define INCLUDE_vTaskDelay                      1   // 包含 vTaskDelay（相对时间延时）
#define INCLUDE_xTaskGetSchedulerState          1   // 包含 xTaskGetSchedulerState（获取调度器状态）
#define INCLUDE_xTaskGetCurrentTaskHandle       1   // 包含 xTaskGetCurrentTaskHandle（获取当前任务句柄）
#define INCLUDE_uxTaskGetStackHighWaterMark     0   // 包含 uxTaskGetStackHighWaterMark（查询堆栈剩余空间）
#define INCLUDE_xTaskGetIdleTaskHandle          0   // 包含 xTaskGetIdleTaskHandle（获取空闲任务句柄）
#define INCLUDE_eTaskGetState                   0   // 包含 eTaskGetState（获取任务状态）
#define INCLUDE_xEventGroupSetBitFromISR        1   // 包含 xEventGroupSetBitFromISR（中断中设置事件位）
#define INCLUDE_xTimerPendFunctionCall          1   // 包含 xTimerPendFunctionCall（定时器中调用函数）
#define INCLUDE_xTaskAbortDelay                 0   // 包含 xTaskAbortDelay（中止任务延时）
#define INCLUDE_xTaskGetHandle                  0   // 包含 xTaskGetHandle（根据名称查找任务句柄）
#define INCLUDE_xTaskResumeFromISR              1   // 包含 xTaskResumeFromISR（中断中恢复任务）


/* ============================================================
 * RP2350 专用配置（ARM Cortex-M33 双核）
 * ============================================================ */
#if PICO_RP2350

/* -------- 安全和硬件特性配置 -------- */
#define configENABLE_MPU                        0   // 1=启用内存保护单元（MPU），0=禁用（当前移植暂不支持）
#define configENABLE_TRUSTZONE                  0   // 1=启用 TrustZone（安全区/非安全区），0=禁用（当前移植暂不支持）
#define configRUN_FREERTOS_SECURE_ONLY          1   // 1=任务只运行在安全区，0=任务可运行在非安全区
#define configENABLE_FPU                        1   // 1=启用浮点单元（FPU），0=禁用（启用后任务切换会保存 FPU 寄存器）
#define configMAX_SYSCALL_INTERRUPT_PRIORITY    16  // 最高可被 FreeRTOS 管理的硬件中断优先级（数值越小优先级越高）

/* -------- 多核（SMP）配置 -------- */
#define configNUMBER_OF_CORES                   2   // CPU 核心数（RP2350 = 2 核）
#define configNUM_CORES                         configNUMBER_OF_CORES   // 同义，用于兼容
#define configTICK_CORE                         0   // 系统时钟中断运行在哪个核心（0 = Core 0）
#define configRUN_MULTIPLE_PRIORITIES           1   // 1=多个核心可同时运行不同优先级的任务，0=只有空闲任务可运行在其他核心
#define configUSE_CORE_AFFINITY                 1   // 1=启用核心亲和性（可绑定任务到指定核心），0=禁用

#endif /* PICO_RP2350 */
/* ============================================================ */

#endif /* FREERTOS_CONFIG_H */