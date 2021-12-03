/*!
 * @file  setTimeAndSqw.ino
 * @brief  设置时间并开始计时，设置sqw引脚输出
 * @details  读取时间和sqw引脚模式
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license  The MIT License (MIT)
 * @author  [qsjhyy](yihuan.huang@dfrobot.com)
 * @version  V1.0
 * @date  2021-11-18
 * @url  https://github.com/DFRobot/DFRobot_DS1307
 */
#include <DFRobot_DS1307.h>


/* 构造函数 */
DFRobot_DS1307 DS1307;

void setup()
{
  Serial.begin(115200);

  // 初始化传感器
  while( !(DS1307.begin()) ){
    Serial.println("Communication with device failed, please check connection");
    delay(3000);
  }
  Serial.println("Begin ok!");

  /**
   *  根据选择设置的时间类型, 传入对应范围的数值, 设置时间
   *  type 要获取的时间类型, eTimeType_t:
   *    eSEC, eMIN, eHR, eDOW, eDATE, eMTH, eYR
   *  typeTime 根据选择的时间类型, 写入对应范围的数值
   *    设置 eSEC 类型时, 范围为: 00-59
   *    设置 eMIN 类型时, 范围为: 00-59
   *    设置 eHR 类型时, 范围为: 00-23
   *    设置 eDOW 类型时, 范围为: 01-07
   *    设置 eDATE 类型时, 范围为: 01-31
   *    设置 eMTH 类型时, 范围为: 01-12
   *    设置 eYR 类型时, 范围为: 2000-2099
   *  注意: 超出范围的将导致设置错误
   */
  DS1307.setTypeTime(DS1307.eYR, 2000);

  /**
   *  从rtc模块获取单个类型的时间并转换为uint16_t
   *  type 要获取的时间类型, eTimeType_t:
   *    eSEC, eMIN, eHR, eDOW, eDATE, eMTH, eYR
   *  根据选择的时间类型, 返回对应范围的数值uint16_t
   *    读取 eSEC 类型时, 范围为: 00-59
   *    读取 eMIN 类型时, 范围为: 00-59
   *    读取 eHR 类型时, 范围为: 00-23
   *    读取 eDOW 类型时, 范围为: 01-07
   *    读取 eDATE 类型时, 范围为: 01-31
   *    读取 eMTH 类型时, 范围为: 01-12
   *    读取 eYR 类型时, 范围为: 2000-2099
   */
  Serial.print("get type time: ");
  Serial.println(DS1307.getTypeTime(DS1307.eYR));

  /**
   *  停止RTC计时功能
   *  this bit is part of the seconds byte
   */
  DS1307.stop();
  /**
   *  根据给的数组, 设置所有时间
   *  setTimeBuff 按如下格式编辑的数组(类型为uint16_t)
   *    setTimeBuff[0]为 eSEC 类型, 范围为: 00-59
   *    setTimeBuff[1]为 eMIN 类型, 范围为: 00-59
   *    setTimeBuff[2]为 eHR 类型, 范围为: 00-23
   *    setTimeBuff[3]为 eDOW 类型, 范围为: 01-07
   *    setTimeBuff[4]为 eDATE 类型, 范围为: 01-31
   *    setTimeBuff[5]为 eMTH 类型, 范围为: 01-12
   *    setTimeBuff[6]为 eYR 类型, 范围为: 2000-2099
   *  注意: 超出范围的将导致设置错误
   */
  uint16_t setTimeBuff[7] = {5, 1, 7, 6, 9, 9, 2021};
  DS1307.setTime(setTimeBuff);
  /**
   *  启动RTC计时功能
   *  this bit is part of the seconds byte
   */
  DS1307.start();

  /**
   *  control the operation of the SQW/OUT pin
   *  mode SQW Pin 输出模式:
   *    eSquareWave_LOW, eSquareWave_HIGH, eSquareWave_1Hz, 
   *    eSquareWave_4kHz, eSquareWave_8kHz, eSquareWave_32kHz
   */
  DS1307.setSqwPinMode(DS1307.eSquareWave_1Hz);

}

void loop()
{
  /**
   *  获取 SQW/OUT pin 当前输出模式
   *  eSqwPinMode_t:
   *    eSquareWave_LOW, eSquareWave_HIGH, eSquareWave_1Hz, 
   *    eSquareWave_4kHz, eSquareWave_8kHz, eSquareWave_32kHz
   */
  if(DS1307.eSquareWave_1Hz == DS1307.getSqwPinMode()){
    Serial.print("SQW/OUT pin: 1Hz | ");
  }

  /**
   *  从rtc模块获取时间并转换为uint16_t
   *  getTimeBuff 用于缓存获取的时间的数组, uint16_t *
   *    getTimeBuff[0]为 eSEC 类型, 范围为: 00-59
   *    getTimeBuff[1]为 eMIN 类型, 范围为: 00-59
   *    getTimeBuff[2]为 eHR 类型, 范围为: 00-23
   *    getTimeBuff[3]为 eDOW 类型, 范围为: 01-07
   *    getTimeBuff[4]为 eDATE 类型, 范围为: 01-31
   *    getTimeBuff[5]为 eMTH 类型, 范围为: 01-12
   *    getTimeBuff[6]为 eYR 类型, 范围为: 2000-2099
   */
  uint16_t getTimeBuff[7] = {0};
  DS1307.getTime(getTimeBuff);
  char outputarr[128];
  sprintf(outputarr, "time: %d/%d/%d-%d %d:%d:%d\r\n",
            getTimeBuff[6],
            getTimeBuff[5],
            getTimeBuff[4],
            getTimeBuff[3],
            getTimeBuff[2],
            getTimeBuff[1],
            getTimeBuff[0]
            );
  Serial.print(outputarr);

 delay(1000);
}
