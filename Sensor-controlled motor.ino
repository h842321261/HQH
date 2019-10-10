#include <mcp_can.h>
#include <SPI.h>
#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
  #define SERIAL SerialUSB
#else
  #define SERIAL Serial
#endif  //调用can总线库

                      //总机地址       留用        留用     目标位置高低八位
unsigned char stmp[8] = {0xff, 0x01, 0x00, 0x64, 0x00, 0x80, 0, 0}; //8段控制代码
                      //       功能码      加速度       目标速度

unsigned char buf[2]; //高低八位存放

int posIni0;
int posDif0;

int posIni1;
int posDif1;

MCP_CAN CAN(10);

void setup() {
  SERIAL.begin(9600); //串口波特率
  CAN.begin(CAN_1000KBPS);  //can总线速率（只支持1000kbps）
  
  posIni0 = analogRead(A0);  //初始化关节角度传感器
  posIni1 = analogRead(A1);
  
  CAN.sendMsgBuf(0x01, 0, 8, stmp); //执行电机归零
  CAN.sendMsgBuf(0x02, 0, 8, stmp);
}

void loop() {
  posDif0 = analogRead(A0) - posIni0;  //取差值
  spin(posDif0);
  posDif1 = analogRead(A1) - posIni1;  //取差值
  spin1(posDif1);
  delay(10);  //等待信号广播完成
}

void dec2bin(int num) {  //转化高低八位存放于buf中
  buf[0] = (num >> 8) & 0xFF;
  buf[1] = num & 0xFF;
}

void spin(int posDif) {
  if (abs(posDif) > 2) {  //设置传感器波动区间为±2
    dec2bin(posDif0 * 35);  //将1024（103电位器为1024，霍尔角度传感器为4096） 转化为36000
    
    stmp[6] = buf[1];  //更改目标位置
    stmp[7] = buf[0];
    
    CAN.sendMsgBuf(0x01, 0, 8, stmp);  //发送数据
  }
}  
  void spin1(int posDif) {
  if (abs(posDif) > 2) {  //设置传感器波动区间为±2
    dec2bin(posDif1 * 35);  //将1024（103电位器为1024，霍尔角度传感器为4096） 转化为36000
    
    stmp[6] = buf[1];  //更改目标位置
    stmp[7] = buf[0];
    
    CAN.sendMsgBuf(0x02, 0, 8, stmp);  //发送数据
  }
}
