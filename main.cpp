/*
 * BMI270 测试主程序 —— Raspberry Pi Pico 2 (RP2350)
 *
 * 接线:
 *   BMI270 SDA  -> Pico GP16 (I2C0 SDA)
 *   BMI270 SCL  -> Pico GP17 (I2C0 SCL)
 *   BMI270 VDD  -> 3V3
 *   BMI270 GND  -> GND
 *
 * 驱动封装在 include/BMI270.hpp + src/BMI270.cpp
 * 输出模式由 include/BMI270.hpp 顶部 BMI270_OUTPUT_MODE 决定:
 *   0 = 原始数据文本  1 = 滤波数据文本  2 = VOFA+ (FireWater 姿态)
 */

#include <cstdio>
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

#if BMI270_OUTPUT_MODE == 2
    // ---- VOFA+ 姿态模式: 每帧 "roll,pitch,yaw\n" (度) ----
    // VOFA+ 使用: 协议选 "FireWater" -> 添加 cube(3D) 控件
    //   -> cube 设置: 欧拉角模式, 角度单位, X←I0, Y←I1, Z←I2
    while (true) {
        imu.update_attitude(BMI270_ATT_DT);
        float roll, pitch, yaw;
        imu.get_euler(roll, pitch, yaw);
        printf("%.3f,%.3f,%.3f\n", roll, pitch, yaw);   // 帧以换行结尾
        sleep_ms((int)(BMI270_ATT_DT * 1000));
    }
#elif BMI270_OUTPUT_MODE == 3
    // ---- VOFA+ 加速度波形模式: 每帧 "ax,ay,az\n" (mg) ----
    // VOFA+ 使用: 协议选 "FireWater" -> 添加 waveform 控件
    //   -> waveform 通道选 I0(ax) / I1(ay) / I2(az)
    while (true) {
        float acc_mg[3], gyr_dps[3], temp_c;
        if (imu.read(acc_mg, gyr_dps, &temp_c)) {
            printf("%.1f,%.1f,%.1f\n", acc_mg[0], acc_mg[1], acc_mg[2]);
        }
        sleep_ms(10);
    }
#else
    // ---- RAW / 滤波模式: 文本输出 @10Hz ----
    printf("\n===== BMI270 Test on Pico 2 =====\n");
    printf("[ok] BMI270 found at 0x%02X, chip_id = 0x%02X\n", imu.addr(), imu.chip_id());
    printf("[ok] init done. streaming accel/gyro/temp @10Hz...\n\n");
    while (true) {
        float acc_mg[3], gyr_dps[3], temp_c;
        if (imu.read(acc_mg, gyr_dps, &temp_c)) {
            printf("acc(mg): %7.1f %7.1f %7.1f | gyr(dps): %8.2f %8.2f %8.2f | temp: %5.2f C\n",
                   acc_mg[0], acc_mg[1], acc_mg[2],
                   gyr_dps[0], gyr_dps[1], gyr_dps[2],
                   temp_c);
        } else {
            printf("[err] I2C read failed\n");
        }
        sleep_ms(100);
    }
#endif
}
