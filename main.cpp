#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/gpio.h"

// ============= 可配置参数 =============
// UART总线选择: uart0 或 uart1
#define UART_ID uart1
#define UART_BAUD_RATE 115200

// 当前默认使用 UART1: TX=4, RX=5
// 可根据需要修改为其他有效引脚
#define UART_TX_PIN 8
#define UART_RX_PIN 9

// =====================================

#define BUFFER_SIZE 256
char rx_buffer[BUFFER_SIZE];
uint rx_index = 0;
uint32_t last_rx_time_ms = 0;
const uint32_t RX_TIMEOUT_MS = 500; // 500ms无新数据则处理消息
const uint32_t HALLO_INTERVAL_US = 1000000;

// 初始化UART
void uart_init_custom() {
    // 初始化UART接口
    uart_init(UART_ID, UART_BAUD_RATE);
    
    // 设置TX和RX引脚
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    
    // 禁用流控
    uart_set_hw_flow(UART_ID, false, false);
    
    // 设置数据格式: 8位数据, 1位停止位, 无奇偶校验
    uart_set_format(UART_ID, 8, 1, UART_PARITY_NONE);
    
    printf("[UART 初始化完成]\n");
    printf("[配置: UART%d, 波特率=%d, TX引脚=%d, RX引脚=%d]\n", 
           UART_ID == uart0 ? 0 : 1, UART_BAUD_RATE, UART_TX_PIN, UART_RX_PIN);
}

// 发送字符串
void uart_send_string(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        uart_putc(UART_ID, str[i]);
    }
}

// 主程序
int main() {
    // 初始化标准库
    stdio_init_all();
    
    printf("\n=== UART 测试程序 ===\n");

    printf("UART%d TX=%d RX=%d BAUD=%d\n", UART_ID == uart0 ? 0 : 1, UART_TX_PIN, UART_RX_PIN, UART_BAUD_RATE);

    uart_init_custom();
    printf("=== UART 检测程序已启动 ===\n");
    printf("收到任意字节都会先打印，再在换行或超时后整条回显。\n");
    printf("没有收到数据时，每 1 秒发送一次 hallo。\n\n");

    uint32_t last_send_time_us = time_us_32();
    uint32_t last_report_time_us = time_us_32();

    while (true) {
        uint32_t now_us = time_us_32();
        uint32_t now_ms = now_us / 1000;

        while (uart_is_readable(UART_ID)) {
            char c = uart_getc(UART_ID);
            last_rx_time_ms = now_ms;

            printf("[RX 0x%02X '%c']\n", (unsigned char)c, (c >= 32 && c <= 126) ? c : '.');

            if (c == '\n' || c == '\r') {
                if (rx_index > 0) {
                    rx_buffer[rx_index] = '\0';
                    printf("[收到完整消息]: %s\n", rx_buffer);
                    uart_send_string(rx_buffer);
                    uart_send_string("\r\n");
                    rx_index = 0;
                    last_send_time_us = now_us;
                }
            } else if (rx_index < BUFFER_SIZE - 1) {
                rx_buffer[rx_index++] = c;
            } else {
                rx_index = 0;
                printf("[RX 缓冲区溢出，已清空]\n");
            }
        }

        if (rx_index > 0 && (now_ms - last_rx_time_ms) > RX_TIMEOUT_MS) {
            rx_buffer[rx_index] = '\0';
            printf("[收到超时消息]: %s\n", rx_buffer);
            uart_send_string(rx_buffer);
            uart_send_string("\r\n");
            rx_index = 0;
            last_send_time_us = now_us;
        }

        if (rx_index == 0 && (now_us - last_send_time_us) >= HALLO_INTERVAL_US) {
            uart_send_string("hallo\r\n");
            printf("[发送]: hallo\n");
            last_send_time_us = now_us;
        }

        if ((now_us - last_report_time_us) >= 5000000) {
            printf("[状态] RX空闲中\n");
            last_report_time_us = now_us;
        }

        sleep_ms(10);
    }
    
    return 0;
}
    

