#include "../Hardware/modbus/inc/modbus_port.h"

void Modbus_PortSend(uint8_t *buf,uint16_t len)
{

    rs485_send_it_data(buf,len);


}



// 十七、测试示例
// 
// 主站发送：
// 
// 01 03 00 00 00 02 CRC
// 
// 读取：
// 
// DeviceBlock[0]
// DeviceBlock[1]
// 
// 返回：
// 
// 01 03 04 00 01 00 64 CRC
