/*
 * BMI270 测试主程序 —— Raspberry Pi Pico 2 (RP2350)
 *
 * 接线:
 *   BMI270 SDA  -> Pico GP16 (I2C0 SDA)
 *   BMI270 SCL  -> Pico GP17 (I2C0 SCL)
 *   BMI270 VDD  -> 3V3
 *   BMI270 GND  -> GND
 *
 * 驱动与输出封装在 include/BMI270.hpp + src/BMI270.cpp
 * 输出模式由 include/BMI270.hpp 顶部 BMI270_OUTPUT_MODE 决定:
 *   0 = 原始数据文本  1 = 滤波数据文本  2 = VOFA+ 姿态  3 = VOFA+ 加速度波形
 *
 * 用法: 主循环里调用 imu.print_state() 打印一帧, 间隔自定
 */

#include <cstdio>
#include <iostream>
#include "pico/stdlib.h"
#include "BMI270.hpp"

int main() {
    stdio_init_all();
    while (!stdio_usb_connected()) sleep_ms(100);

    BMI270 imu;
    if (!imu.begin()) {     // 默认 SDA=GP16, SCL=GP17, 自动探测 0x68/0x69
        printf("[fatal] BMI270 init failed! check wiring: SDA->GP16, SCL->GP17\n");
        while (true) tight_loop_contents();
    }
    std::cout << "[BMI270] init done. streaming accel/gyro/temp...\n";
    imu.print_header();     // 一次性启动信息 (VOFA+ 模式下自动静默)
    

    // 主循环: 打印一帧 + 自定间隔
    while (true) {
        imu.print_state();
        sleep_ms(10);
    }
}
