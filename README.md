# DFRobot_DS1307
* [中文版](./README_CN.md)

The DS1307 serial real-time clock (RTC) is a low-power, full binary-coded decimal (BCD) clock/calendar 
plus 56 bytes of NV SRAM. Address and data are transferred serially through an I2C, bidirectional bus. 
The clock/calendar provides seconds, minutes, hours, day, date, month, and year information. The end of 
the month date is automatically adjusted for months with fewer than 31 days, including corrections for leap 
year. The clock operates in either the 24-hour or 12-hour format with AM/PM indicator. The DS1307 has a 
built-in power-sense circuit that detects power failures and automatically switches to the backup supply. 
Timekeeping operation continues while the part operates from the backup supply.

![产品实物图](./resources/images/DS1307.png)


## Product Link (https://www.dfrobot.com/product-879.html)
    SKU: DFR0151


## Table of Contents

* [Summary](#summary)
* [Installation](#installation)
* [Methods](#methods)
* [Compatibility](#compatibility)
* [History](#history)
* [Credits](#credits)


## Summary

* Real-Time Clock (RTC) Counts Seconds, Minutes, Hours, Date of the Month, Month, Day of the week, and Year with Leap-Year Compensation Valid Up to 2100
* 56-Byte, Battery-Backed, General-Purpose RAMwith Unlimited Writes
* I2C Serial Interface
* Programmable Square-Wave Output Signal
* Automatic Power-Fail Detect and Switch Circuitry
* Consumes Less than 500nA in Battery-Backup Mode with Oscillator Running
* Optional Industrial Temperature Range:-40°C to +85°C 
* Available in 8-Pin Plastic DIP or SO
* Underwriters Laboratories (UL) Recognized


## Installation

To use this library, first download the library file, paste it into the \Arduino\libraries directory, 
then open the examples folder and run the demo in the folder.


## Methods

```C++

  /**
   * @fn begin
   * @brief 初始化函数
   * @return true 表示初始化成功; false 表示初始化失败
   */
  bool begin(void);

  /**
   * @fn getTime
   * @brief 从rtc模块获取时间并转换为uint16_t
   * @param rtc 用于缓存获取的时间的数组, uint16_t *
   * @return None
   */
  void getTime(uint16_t *rtc);

  /**
   * @fn getTypeTime
   * @brief 从rtc模块获取单个类型的时间并转换为uint16_t
   * @param type 要获取的时间类型, eTimeType_t:
   * @n eSEC, eMIN, eHR, eDOW, eDATE, eMTH, eYR
   * @return 根据选择的时间类型, 返回对应范围的数值uint16_t
   * @n 读取 eSEC 类型时, 范围为: 00-59
   * @n 读取 eMIN 类型时, 范围为: 00-59
   * @n 读取 eHR 类型时, 范围为: 00-23
   * @n 读取 eDOW 类型时, 范围为: 01-07
   * @n 读取 eDATE 类型时, 范围为: 01-31
   * @n 读取 eMTH 类型时, 范围为: 01-12
   * @n 读取 eYR 类型时, 范围为: 2000-2099
   */
  uint16_t getTypeTime(eTimeType_t type);

  /**
   * @fn setTime
   * @brief 根据给的数组, 设置所有时间
   * @param rtc 按如下格式编辑的数组(类型为uint16_t)
   * @n rtc[0]为 eSEC 类型, 范围为: 00-59
   * @n rtc[1]为 eMIN 类型, 范围为: 00-59
   * @n rtc[2]为 eHR 类型, 范围为: 00-23
   * @n rtc[3]为 eDOW 类型, 范围为: 01-07
   * @n rtc[4]为 eDATE 类型, 范围为: 01-31
   * @n rtc[5]为 eMTH 类型, 范围为: 01-12
   * @n rtc[6]为 eYR 类型, 范围为: 2000-2099
   * @n 注意: 超出范围的将导致设置错误
   * @return None
   */
  void setTime(uint16_t *rtc);

  /**
   * @fn setTypeTime
   * @brief 根据选择设置的时间类型, 传入对应范围的数值, 设置时间
   * @param type 要获取的时间类型, eTimeType_t:
   * @n eSEC, eMIN, eHR, eDOW, eDATE, eMTH, eYR
   * @param typeTime 根据选择的时间类型, 写入对应范围的数值
   * @n 设置 eSEC 类型时, 范围为: 00-59
   * @n 设置 eMIN 类型时, 范围为: 00-59
   * @n 设置 eHR 类型时, 范围为: 00-23
   * @n 设置 eDOW 类型时, 范围为: 01-07
   * @n 设置 eDATE 类型时, 范围为: 01-31
   * @n 设置 eMTH 类型时, 范围为: 01-12
   * @n 设置 eYR 类型时, 范围为: 2000-2099
   * @n 注意: 超出范围的将导致设置错误
   * @return None
   */
  void setTypeTime(eTimeType_t type, uint16_t typeTime);

  /**
   * @fn start
   * @brief 启动RTC计时功能
   * @n this bit is part of the seconds byte
   * @return None
   */
  void start(void);

  /**
   * @fn stop
   * @brief 停止RTC计时功能
   * @n this bit is part of the seconds byte
   * @return None
   */
  void stop(void);

  /**
   * @fn setSqwPinMode
   * @brief control the operation of the SQW/OUT pin
   * @param mode SQW Pin 输出模式:
   * @n eSquareWave_LOW, eSquareWave_HIGH, eSquareWave_1Hz, 
   * @n eSquareWave_4kHz, eSquareWave_8kHz, eSquareWave_32kHz
   * @return None
   */
  void setSqwPinMode(eSqwPinMode_t mode);

  /**
   * @fn getSqwPinMode
   * @brief 获取 SQW/OUT pin 当前输出模式
   * @return eSqwPinMode_t:
   * @n eSquareWave_LOW, eSquareWave_HIGH, eSquareWave_1Hz, 
   * @n eSquareWave_4kHz, eSquareWave_8kHz, eSquareWave_32kHz
   */
  eSqwPinMode_t getSqwPinMode(void);

  /**
   * @fn saveTimeToEEPROM
   * @brief 将当前时间存入EEPROM, 可用于掉电重启后, 将时间设置为最后一次保存的时间
   * @n 掉电时调用此接口, 重启时调用setTimeFromEEPROM()即可实现
   * @return None
   */
  void saveTimeToEEPROM(void);

  /**
   * @fn setTimeFromEEPROM
   * @brief 将时间设置为最后一次保存的时间
   * @n 如果之前没调用过saveTimeToEEPROM(), 或者调用完后自行
   * @n 修改了EEPROM里面的内容, 则可能导致时间设置错误
   * @note 如果未使传感器掉电时复位主控板, 可能会使该接口误调用, 从而误更改时间
   * @return None
   */
  void setTimeFromEEPROM(void);

  /**
   * @fn readEEPROM
   * @brief 通过I2C总线读EEPROM
   * @param addr  EEPROM数据地址 8bits
   * @param pBuf 要读取数据的存放缓存
   * @param size 要读取数据的长度
   * @return 返回读取的长度, 返回0表示读取失败
   */
  size_t readEEPROM(uint8_t addr, uint8_t* pBuf, size_t size);

  /**
   * @fn writeEEPROM
   * @brief 通过I2C总线写EEPROM
   * @param addr  EEPROM数据地址 8bits
   * @param pBuf 要写入数据的存放缓存
   * @param size 要写入数据的长度
   * @note 用户可自由存储的数据大小为247个字节, 范围0-247
   * @n 最后8个字节是存储用户调用saveTimeToEEPROM()保存的时间数据
   * @return None
   */
  void writeEEPROM(uint8_t addr, const uint8_t* pBuf, size_t size);

```


## Compatibility

MCU                | Work Well    | Work Wrong   | Untested    | Remarks
------------------ | :----------: | :----------: | :---------: | :----:
Arduino Uno        |      √       |              |             |
Arduino MEGA2560   |      √       |              |             |
Arduino Leonardo   |      √       |              |             |
FireBeetle-ESP8266 |      √       |              |             |
FireBeetle-ESP32   |      √       |              |             |
FireBeetle-M0      |      √       |              |             |
Micro:bit          |      √       |              |             |


## History

- 2021/12/02 - Version 1.0.0 released.


## Credits

Written by qsjhyy(yihuan.huang@dfrobot.com), 2021. (Welcome to our [website](https://www.dfrobot.com/))

