/*!
 * @file  DFRobot_DS1307.h
 * @brief  Define infrastructure of DFRobot_DS1307 class
 * @details  The clock/calendar provides seconds, minutes, hours, 
 * @n  day, date, month, and year information.
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license  The MIT License (MIT)
 * @author  [qsjhyy](yihuan.huang@dfrobot.com)
 * @version  V1.0
 * @date  2021-11-18
 * @url  https://github.com/DFRobot/DFRobot_DS1307
 */
#ifndef __DFRobot_DS1307_H__
#define __DFRobot_DS1307_H__

// include types & constants of Wiring core API
#include <Arduino.h>
#include <Wire.h>


// #define ENABLE_DBG   //!< 打开这个宏, 可以看到程序的详细运行过程
#ifdef ENABLE_DBG
  #define DBG(...) {Serial.print("[");Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
#else
  #define DBG(...)
#endif

// DS1307 I2C address
#define EEPROM_I2C_ADDR   uint8_t(0x50)   ///< EEPROM I2C address
#define DS1307_I2C_ADDR   uint8_t(0x68)   ///< DS1307 I2C address
#define DS1307_BASE_YR            2000    ///< 基础年份为2000年

// DS1307 register address
#define DS1307_SEC_REG    uint8_t(0x00)   ///< DS1307 Seconds register address
#define DS1307_MIN_REG    uint8_t(0x01)   ///< DS1307 Minutes register address
#define DS1307_HR_REG     uint8_t(0x02)   ///< DS1307 Hour register address
#define DS1307_DOW_REG    uint8_t(0x03)   ///< DS1307 DAY register address
#define DS1307_DATE_REG   uint8_t(0x04)   ///< DS1307 Date register address
#define DS1307_MTH_REG    uint8_t(0x05)   ///< DS1307 Month register address
#define DS1307_YR_REG     uint8_t(0x06)   ///< DS1307 Year register address
#define DS1307_CTL_REG    uint8_t(0x07)   ///< DS1307 Control register address
#define DS1307_RAM_REG    uint8_t(0x08)   ///< DS1307 RAM register start address(08h–3Fh), size: 56 x 8bits

// Define register bit masks
#define DS1307_CLOCKHALT B10000000

#define DS1307_LO_BCD    B00001111
#define DS1307_HI_BCD    B11110000

#define DS1307_SEC_MASK    B01111111
#define DS1307_MIN_MASK    B01111111
#define DS1307_HR_MASK     B00111111
#define DS1307_DOW_MASK    B00000111
#define DS1307_DATE_MASK   B00111111
#define DS1307_MTH_MASK    B00011111
#define DS1307_YR_MASK     B11111111


class DFRobot_DS1307
{
public:

  /**
   * @enum  eTimeType_t
   * @brief  时间类型选择
   */
  typedef enum
  {
    eSEC  = 0,
    eMIN,
    eHR,
    eDOW,
    eDATE,
    eMTH,
    eYR,
  }eTimeType_t;

  /**
   * @enum  eSqwPinMode_t
   * @brief  Set the output square wave mode
   */
  typedef enum
  {
    eSquareWave_LOW   = 0x00,   /**< Not output square wave, output low level */
    eSquareWave_HIGH  = 0x80,   /**< Not output square wave, output HIGH level */
    eSquareWave_1Hz   = 0x10,   /**< 1Hz square wave */
    eSquareWave_4kHz  = 0x11,   /**< 4.096kHz square wave */
    eSquareWave_8kHz  = 0x12,   /**< 8.192kHz square wave */
    eSquareWave_32kHz = 0x13,   /**< 32.768kHz square wave */
  }eSqwPinMode_t;

public:

  /**
   * @fn DFRobot_DS1307
   * @brief 构造函数
   * @param pWire Wire.h里定义了Wire对象, 因此使用&Wire就能够指向并使用Wire中的方法
   * @param ds1307Addr DS1307的i2c通信地址
   * @return None
   */
  DFRobot_DS1307(TwoWire *pWire=&Wire, uint8_t ds1307Addr=DS1307_I2C_ADDR);

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
   * @n rtc[0]为 eSEC 类型, 范围为: 00-59
   * @n rtc[1]为 eMIN 类型, 范围为: 00-59
   * @n rtc[2]为 eHR 类型, 范围为: 00-23
   * @n rtc[3]为 eDOW 类型, 范围为: 01-07
   * @n rtc[4]为 eDATE 类型, 范围为: 01-31
   * @n rtc[5]为 eMTH 类型, 范围为: 01-12
   * @n rtc[6]为 eYR 类型, 范围为: 2000-2099
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

protected:
  /**
   * @fn bcd2bin(uint8_t val)
   * @brief BCD code to BIN code
   * @param val Input BCD code
   * @return Return BIN code
   */
  static uint8_t bcd2bin(uint8_t val);

  /**
   * @fn bin2bcd(uint8_t val)
   * @brief BIN code to BCD code
   * @param val Input BIN code
   * @return Return BCD code
   */
  static uint8_t bin2bcd(uint8_t val);

  /**
   * @fn writeReg
   * @brief 通过I2C总线写入寄存器值
   * @param reg  寄存器地址 8bits
   * @param pBuf 要写入数据的存放缓存
   * @param size 要写入数据的长度
   * @param deviceAddr 选择通信的IIC设备地址, _ds1307Addr或_eepromAddr
   * @return None
   */
  void writeReg(uint8_t reg, const void* pBuf, size_t size, uint8_t deviceAddr=DS1307_I2C_ADDR);

  /**
   * @fn readReg
   * @brief 通过I2C总线读取寄存器值
   * @param reg  寄存器地址 8bits
   * @param pBuf 要读取数据的存放缓存
   * @param size 要读取数据的长度
   * @param deviceAddr 选择通信的IIC设备地址, _ds1307Addr或_eepromAddr
   * @return 返回读取的长度, 返回0表示读取失败
   */
  size_t readReg(uint8_t reg, void* pBuf, size_t size, uint8_t deviceAddr=DS1307_I2C_ADDR);

private:
  uint8_t rtcBCD[7], rtcMask[7];     // used prior to read/set ds1307 registers;
  TwoWire *_pWire;       // I2C通信方式的指针
  uint8_t _ds1307Addr;   // DS1307的I2C通信地址
  uint8_t _eepromAddr;   // EEPROM的I2C通信地址
};
#endif
