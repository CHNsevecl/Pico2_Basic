#include "QD4310.hpp"

/*!
 * \brief 初始化QD4310
 */
void QD4310::QD4310_Init() {
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    gpio_set_dir(UART_RX_PIN, GPIO_IN); // 确保是输入模式
    gpio_pull_up(UART_RX_PIN);  
}

void QD4310::QD4310_SendCommand(const std::vector<uint8_t>& data) {
    uart_write_blocking(UART_ID, data.data(), data.size());
    
}


/*!
 * \brief 控制QD4310
 * \param addr 地址
 * \param Control_mode 控制模式，详见 #QD4310_ControlMode_t
 * \param Control_quantity 控制量   
 * 
 */
void QD4310::QD4310_Contol(uint8_t addr ,uint8_t Control_mode ,uint16_t Control_quantity) {
    std::vector<uint8_t> command;
    command.reserve(5);
    command.push_back(addr); //校验码占位
    command.push_back(Control_mode); //控制模式
    command.push_back(Control_quantity & 0xFF); //命令字
    command.push_back(Control_quantity >> 8);
    command.push_back(CRC8(command)); //地址
    QD4310_SendCommand(command);

    if(Control_mode == QD4310_MODE_REPORT){
        std::vector<uint8_t> datas = QD4310_ReceiveData(addr);
        if(datas.size() == 10){
            uint8_t CRC8_Byte = datas[9];
            datas.pop_back();
            if(CRC8(datas) == CRC8_Byte){
                feedback.status = datas[1];
                feedback.elc_current = double((int16_t)(datas[3] | (datas[4] << 8)))/32767*10.0;
                feedback.speed = (uint16_t)(datas[5] | (datas[6] << 8));
                feedback.angle = (double)(uint16_t)(datas[7] | (datas[8] << 8)) / 65535.0 * 360.0;
            }
        }
        else{
            std::cout << "接收数据错误" << std::endl;
        }
    }
    sleep_ms(1); //等待电机响应
}


std::vector<uint8_t> QD4310::QD4310_ReceiveData(uint8_t start, uint32_t timeout_ms){
    std::vector<uint8_t> data;
    data.reserve(10);
    bool start_flag = false;
    uint32_t timeout_us = timeout_ms * 1000;
    
    while (data.size() < 10) {
        uint8_t byte;  // 先声明变量
        
        // 等待一个字节（最多timeout_us微秒）
        if (!uart_is_readable_within_us(UART_ID, timeout_us)) {
            std::cout << "超时" << std::endl;
            break;  // 超时
        }
        
        
        byte = uart_getc(UART_ID);  // 只读取一次！
        
        if (!start_flag && byte == start) {
            start_flag = true;
            data.push_back(byte);
        }
        else if (start_flag) {
            data.push_back(byte);
        }
    }
    
    return data;
}


/**
 * @brief 计算CRC-8校验码（多项式 x^8 + x^2 + x + 1）
 * @param data 数据向量，约定 data[0] 为CRC占位字节（不参与计算）
 * @return CRC-8校验码
 * @note 计算结果应存入 data[0]
 */
uint8_t QD4310::CRC8(const std::vector<uint8_t>& data) {
    uint8_t crc = 0x00;
    for (int i = 0; i < data.size(); ++i) {
        crc ^= data[i];
        for (int j = 0; j < 8; ++j) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ 0x07; // 多项式 x^8 + x^2 + x + 1
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}

uint16_t QD4310::rad (double angle){
    return static_cast<uint16_t>(angle /(2*PI)*65535);
}