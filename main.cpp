#include <stdio.h>
#include "pico/stdlib.h"
#include "uart_echo.hpp"

// main 只负责调用：
//  - UART 的初始化、收发/回显/超时/心跳等逻辑都在 uart_echo.cpp / uart_echo.hpp 中
int main() {
    // 初始化标准库（USB stdio 输出）
    stdio_init_all();
    UART uart;

    // 初始化 UART 并打印启动信息
    uart.uart_echo_init();

    // 主循环：每 10ms 调用一次 UART 服务
    while (true) {
        uart.uart_echo_send_byte({0x01, 0x02, 0x03}, 3); // Example usage of uart_send_byte
        sleep_ms(10);
    }

    return 0;
}
