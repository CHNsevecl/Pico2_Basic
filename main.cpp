#include <stdio.h>
#include <iostream>
#include "pico/stdlib.h"
#include "src/QD4310.hpp"
#include <math.h>


int main()
{
    stdio_init_all();

    QD4310 motor;
    motor.QD4310_Contol(0x01, QD4310_MODE_ENABLE, 0);
    

    motor.QD4310_Contol(0x01, QD4310_MODE_ANGLE, motor.rad(0.0));


    while (true) {
        motor.QD4310_Contol(0x01, QD4310_MODE_REPORT, 0);
        std::cout << std::dec << motor.feedback.angle << std::endl;
        std::cout << motor.feedback.elc_current << std::endl;
        // std::cout << "C++ 标准版本: " << __cplusplus << std::endl;
        sleep_ms(1000);
    }
}
