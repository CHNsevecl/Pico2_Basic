#include <stdio.h>
#include "pico/stdlib.h"
#include "src/F32C.hpp"
#include "stdio.h"

#define addr1 0x01
#define addr2 0x02
int main()
{
    stdio_init_all();
    
    F32C f32c1;
    f32c1.F32C_Init();

    // f32c1.Address_Set(addr2, addr1); // 设置地址为0x01
    // f32c1.Parameter_Save(addr1);

    f32c1.F32C_Enable(addr1);
    f32c1.F32C_Enable(addr2);
    f32c1.Speed_set(0x0000, addr2); // 设置速度为100
    f32c1.Speed_set(0x000A, addr1); // 设置速度为100

    f32c1.Control_Mode_Chose(0x0000, addr1);
    // f32c1.Control_Mode_Chose(0x0002, addr2);
    
    
    while (true) {
 
        // f32c1.Single_Angle_set(0x0001,addr1);
        f32c1.Data_Feedback(addr2, 0x04); // 获取地址为0x01的数据
        sleep_ms(1000);

        // f32c1.Single_Angle_set(0x07D0,addr1);
        // sleep_ms(1000);

    }
}
