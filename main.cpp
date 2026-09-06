#include <stdio.h>
#include "pico/stdlib.h"
#include "uart_echo.hpp"
#include <vector>

// main 只负责调用：
//  - UART 的初始化、收发/回显/超时/心跳等逻辑都在 uart_echo.cpp / uart_echo.hpp 中
int main() {
    // 初始化标准库（USB stdio 输出）
    stdio_init_all();

    while (!stdio_usb_connected()) {
        sleep_ms(10);
    }
    
    UART uart(uart0,115200, 0, 1); // 使用 UART0，波特率 115200，TX=0, RX=1

    // 初始化 UART 并打印启动信息
    uart.uart_echo_init();

    // 主循环：每 10ms 调用一次 UART 服务
    while (true) {
        // uart.uart_echo_send_byte({0x01, 0x02, 0x03}, 3); // Example usage of uart_send_byte
        std::vector<uint8_t> received = uart.uart_echo_receive_byte(1, 10); // Example usage of uart_receive_byte
        if (!received.empty()) {
            for (uint8_t byte : received) {
                uart.uart_echo_send_byte({byte}, 1); // Echo back the received byte
            }
        }
        sleep_ms(10);
    }

    return 0;
}
