/*!
 * @file  useEEPROM.ino
 * @brief  本demo演示了, 如何使用传感器上的EEPORM
 * @details  可以通过其保存上次时间, 重新上电后用保存的时间设置DS1307
 * @n 也可向EEPROM写入(读取)一些字符串, 或者uint8_t的数组, 掉电保存
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
  // uint16_t setTimeBuff[7] = {5, 1, 7, 6, 9, 9, 2021};
  // DS1307.setTime(setTimeBuff);

  /**
   *  将当前时间存入EEPROM, 可用于掉电重启后, 将时间设置为最后一次保存的时间
   *  掉电时调用此接口, 重启时调用setTimeFromEEPROM()即可实现
   */
  // DS1307.saveTimeToEEPROM();

  /**
   *  将时间设置为最后一次保存的时间
   *    如果之前没调用过saveTimeToEEPROM(), 或者调用完后自行
   *    修改了EEPROM里面的内容, 则可能导致时间设置错误
   *  如果未使传感器掉电时复位主控板, 可能会使该接口误调用, 从而误更改时间
   * 注意: 初次使用请手动设置时间, 调用setTime()
   */
  DS1307.setTimeFromEEPROM();

  /**
   *  启动RTC计时功能
   *  this bit is part of the seconds byte
   */
  DS1307.start();

  Serial.println("--- EEPROM Read-Write Test---");
  uint8_t someData[] = "This is data from the eeprom!";   // data to write
  /**
   *  通过I2C总线写EEPROM
   *  addr  EEPROM数据地址 8bits
   *  pBuf 要写入数据的存放缓存
   *  size 要写入数据的长度
   *  用户可自由存储的数据大小为247个字节, 范围0-247
   *  最后8个字节是存储用户调用saveTimeToEEPROM()保存的时间数据
   */
  DS1307.writeEEPROM(0, someData, sizeof(someData));
  Serial.println("Written Done!");

  Serial.print("Read Data: ");
  uint8_t addr, data;
  for(addr=0; addr<sizeof(someData); addr++) 
  {
    data = 0;
    /**
     * 通过I2C总线读EEPROM
     * addr  EEPROM数据地址 8bits
     * pBuf 要读取数据的存放缓存
     * size 要读取数据的长度
     * 返回读取的长度, 返回0表示读取失败
     */
    DS1307.readEEPROM(addr, &data, sizeof(data));
    Serial.print((char)data);
  }
  Serial.println();
}

void loop()
{
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

  /**
   *  将当前时间存入EEPROM, 可用于掉电重启后, 将时间设置为最后一次保存的时间
   *  掉电时调用此接口, 重启时调用setTimeFromEEPROM()即可实现
   */
  DS1307.saveTimeToEEPROM();

 delay(1000);
}
