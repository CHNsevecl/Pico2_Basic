#pragma once

// ============================================================================
// UART 回显/测试模块
// ----------------------------------------------------------------------------
// 封装了 UART 的初始化、字符串发送，以及主服务循环（逐字节打印、
// 换行/超时整条回显、空闲时周期发送 "hallo"）。
// main 只需要调用 uart_echo_init() 和 uart_echo_service() 即可。
// ============================================================================

// ============= 可配置参数（修改后请重新编译） =============
// UART总线选择: uart0 或 uart1
#define UART_ID uart1
#define UART_BAUD_RATE 115200

// 当前默认使用 UART1: TX=8, RX=9
// 可根据需要修改为其他有效引脚
#define UART_TX_PIN 8
#define UART_RX_PIN 9
// =========================================================

#include <vector>

class UART {
public:
    // 初始化 UART（硬件配置 + 打印启动信息），内部会调用 uart_init()
    void uart_echo_send_byte(std::vector<uint8_t> byte, int len);

    void uart_echo_init();

    // 通过 UART 发送字符串（不包含结尾的 '\0'）
    void uart_echo_send_string(const char *str);

    // 主服务：处理一次接收/回显/超时/心跳（不含延时，建议主循环每 10ms 调用一次）
    void uart_echo_service();
};

