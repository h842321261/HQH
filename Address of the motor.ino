#include <mcp_can.h>
#include <SPI.h>
#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
  #define SERIAL SerialUSB
#else
  #define SERIAL Serial
#endif

MCP_CAN CAN0(10);

                      //主机ID,功能码,留用, 加速度, 留用，目标速度,位置低高八位
unsigned char test[8] = {0xff, 0x01, 0x00, 0x19, 0x00, 0x80, 80, 70}; //测试

                      //主机ID,功能码,寄存器, 长度, 高低八位(模式，03为速度，00为位置), 留空，留空
unsigned char modeSetup[8] = {0xff, 0x06, 0x15, 0x01, 02, 00, 00, 00}; //由00位置模式改为03速度模式

                      //主机ID,功能码,寄存器, 长度, 数据低高八位, 留空，留空
unsigned char save[8] = {0xff, 0x06, 0x14, 0x01, 01, 00, 00, 00}; //参数保存位

void setup() {
  SERIAL.begin(115200); //串口波特率
  CAN0.begin(CAN_1000KBPS);
}

void loop() {
  CAN0.sendMsgBuf(0x01, 0, 8, modeSetup);       //改地址
  delay(1000);
  CAN0.sendMsgBuf(0x01, 0, 8, save);            //保存
  delay(1000);
  CAN0.sendMsgBuf(0x02, 0, 8, test);             //测试
  while(1);
}

//速度模式和位置模式的切换
