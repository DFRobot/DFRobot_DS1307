# -*- coding: utf-8 -*
'''!
  @file  DFRobot_DS1307.py
  @brief  Define infrastructure of DFRobot_DS1307 class
  @details  The clock/calendar provides seconds, minutes, hours, 
  @n  day, date, month, and year information.
  @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  @license  The MIT License (MIT)
  @author  [qsjhyy](yihuan.huang@dfrobot.com)
  @version  V1.0
  @date  2021-12-02
  @url  https://github.com/DFRobot/DFRobot_DS1307
'''
import sys
import time

import smbus

import logging
from ctypes import *


logger = logging.getLogger()
# logger.setLevel(logging.INFO)   # Display all print information
logger.setLevel(logging.FATAL)   # If you don’t want to display too many prints, only print errors, please use this option
ph = logging.StreamHandler()
formatter = logging.Formatter("%(asctime)s - [%(filename)s %(funcName)s]:%(lineno)d - %(levelname)s: %(message)s")
ph.setFormatter(formatter) 
logger.addHandler(ph)


## EEPROM I2C address
EEPROM_I2C_ADDR = 0x50
## DS1307 I2C address
DS1307_I2C_ADDR = 0x68
## 基础年份为2000年
DS1307_BASE_YR  = 2000

# DS1307寄存器地址
## DS1307 Seconds register address
DS1307_SEC_REG  = 0x00
## DS1307 Minutes register address
DS1307_MIN_REG  = 0x01
## DS1307 Hour register address
DS1307_HR_REG   = 0x02
## DS1307 DAY register address
DS1307_DOW_REG  = 0x03
## DS1307 Date register address
DS1307_DATE_REG = 0x04
## DS1307 Month register address
DS1307_MTH_REG  = 0x05
## DS1307 Year register address
DS1307_YR_REG   = 0x06
## DS1307 Control register address
DS1307_CTL_REG  = 0x07
## DS1307 RAM register start address(08h–3Fh), size: 56 x 8bits
DS1307_RAM_REG  = 0x08

# Define register bit masks
DS1307_CLOCKHALT = 0b10000000
DS1307_LO_BCD    = 0b00001111
DS1307_HI_BCD    = 0b11110000
DS1307_SEC_MASK  = 0b01111111
DS1307_MIN_MASK  = 0b01111111
DS1307_HR_MASK   = 0b00111111
DS1307_DOW_MASK  = 0b00000111
DS1307_DATE_MASK = 0b00111111
DS1307_MTH_MASK  = 0b00011111
DS1307_YR_MASK   = 0b11111111


class DFRobot_DS1307(object):
  '''!
    @brief 定义DFRobot_DS1307基类
    @details 用于驱动气体传感器
  '''

  # 时间类型选择
  e_SEC  = 0
  e_MIN  = 1
  e_HR   = 2
  e_DOW  = 3
  e_DATE = 4
  e_MTH  = 5
  e_YR   = 6

  # Set the output square wave mode
  ## Not output square wave, output low level
  e_square_wave_LOW   = 0x00
  ## Not output square wave, output HIGH level
  e_square_wave_HIGH  = 0x80
  ## 1Hz square wave
  e_square_wave_1Hz   = 0x10
  ## 4.096kHz square wave
  e_square_wave_4kHz  = 0x11
  ## 8.192kHz square wave
  e_square_wave_8kHz  = 0x12
  ## 32.768kHz square wave
  e_square_wave_32kHz = 0x13

  def __init__(self, ds1307_addr=DS1307_I2C_ADDR, bus=1):
    '''!
      @brief Module I2C communication init
      @param ds1307_addr I2C通信地址
      @param bus I2C总线
    '''
    self._ds1307_addr = ds1307_addr
    self._eeprom_addr = EEPROM_I2C_ADDR
    self._i2c = smbus.SMBus(bus)
    self._rtc_mask = [ DS1307_SEC_MASK, DS1307_MIN_MASK, DS1307_HR_MASK, 
    DS1307_DOW_MASK, DS1307_DATE_MASK, DS1307_MTH_MASK, DS1307_YR_MASK ]

  def begin(self):
    '''!
      @brief Initialize sensor
      @return  返回初始化状态
      @retval True indicate initialization succeed
      @retval False indicate initialization failed
    '''
    ret = True
    self._rtc_BCD = self._read_reg(DS1307_SEC_REG, 8)
    logger.info(self._rtc_BCD[0])
    if not self._rtc_BCD:
        ret = False
    self.start
    return ret

  def get_time(self):
    '''!
      @brief 从rtc模块获取时间
      @return 获取的时间的列表
      @n rtc[0]为 e_SEC 类型, 范围为: 00-59
      @n rtc[1]为 e_MIN 类型, 范围为: 00-59
      @n rtc[2]为 e_HR 类型, 范围为: 00-23
      @n rtc[3]为 e_DOW 类型, 范围为: 01-07
      @n rtc[4]为 e_DATE 类型, 范围为: 01-31
      @n rtc[5]为 e_MTH 类型, 范围为: 01-12
      @n rtc[6]为 e_YR 类型, 范围为: 2000-2099
    '''
    rtc = [0] * 7
    self._rtc_BCD = self._read_reg(DS1307_SEC_REG, 8)
    for i in range(0, 7):
      rtc[i] = self._bcd_to_bin(self._rtc_BCD[i] & self._rtc_mask[i])
    rtc[self.e_YR] += DS1307_BASE_YR
    return rtc

  def get_type_time(self, type):
    '''!
      @brief 从rtc模块获取单个类型的时间
      @param type 要获取的时间类型:
      @n e_SEC, e_MIN, e_HR, e_DOW, e_DATE, e_MTH, e_YR
      @return 根据选择的时间类型, 返回对应范围的数值
      @n 读取 e_SEC 类型时, 范围为: 00-59
      @n 读取 e_MIN 类型时, 范围为: 00-59
      @n 读取 e_HR 类型时, 范围为: 00-23
      @n 读取 e_DOW 类型时, 范围为: 01-07
      @n 读取 e_DATE 类型时, 范围为: 01-31
      @n 读取 e_MTH 类型时, 范围为: 01-12
      @n 读取 e_YR 类型时, 范围为: 2000-2099
    '''
    time = self._read_reg(type, 1)[0]
    type_time = self._bcd_to_bin(time & self._rtc_mask[type])
    if self.e_YR == type:
      type_time += DS1307_BASE_YR
    return type_time

  def set_time(self, rtc):
    '''!
      @brief 根据给的数组, 设置所有时间
      @param rtc 按如下格式编辑的数组
      @n rtc[0]为 e_SEC 类型, 范围为: 00-59
      @n rtc[1]为 e_MIN 类型, 范围为: 00-59
      @n rtc[2]为 e_HR 类型, 范围为: 00-23
      @n rtc[3]为 e_DOW 类型, 范围为: 01-07
      @n rtc[4]为 e_DATE 类型, 范围为: 01-31
      @n rtc[5]为 e_MTH 类型, 范围为: 01-12
      @n rtc[6]为 e_YR 类型, 范围为: 2000-2099
      @n 注意: 超出范围的将导致设置错误
    '''
    rtc[self.e_YR] -= DS1307_BASE_YR
    for i in range(0, 7):
      self._rtc_BCD[i] = (self._rtc_BCD[i] & (~self._rtc_mask[i])) | (self._bin_to_bcd(rtc[i] & self._rtc_mask[i]))
    self._write_reg(DS1307_SEC_REG, self._rtc_BCD)

  def set_type_time(self, type, type_time):
    '''!
      @brief 根据选择设置的时间类型, 传入对应范围的数值, 设置时间
      @param type 要获取的时间类型:
      @n e_SEC, e_MIN, e_HR, e_DOW, e_DATE, e_MTH, e_YR
      @param type_time 根据选择的时间类型, 写入对应范围的数值
      @n 读取 e_SEC 类型时, 范围为: 00-59
      @n 读取 e_MIN 类型时, 范围为: 00-59
      @n 读取 e_HR 类型时, 范围为: 00-23
      @n 读取 e_DOW 类型时, 范围为: 01-07
      @n 读取 e_DATE 类型时, 范围为: 01-31
      @n 读取 e_MTH 类型时, 范围为: 01-12
      @n 读取 e_YR 类型时, 范围为: 2000-2099
    '''
    if self.e_YR == type:
      type_time -= DS1307_BASE_YR
    self._rtc_BCD[type] = (self._rtc_BCD[type] & (~self._rtc_mask[type])) | (self._bin_to_bcd(type_time & self._rtc_mask[type]))
    self._write_reg(type, self._rtc_BCD[type])

  @property
  def start(self):
    '''!
      @brief 启动RTC计时功能
      @n this bit is part of the seconds byte
    '''
    self._rtc_BCD[self.e_SEC] &= ~DS1307_CLOCKHALT
    self._write_reg(DS1307_SEC_REG, self._rtc_BCD[self.e_SEC])

  @property
  def stop(self):
    '''!
      @brief 停止RTC计时功能
      @n this bit is part of the seconds byte
    '''
    self._rtc_BCD[self.e_SEC] |= DS1307_CLOCKHALT
    self._write_reg(DS1307_SEC_REG, self._rtc_BCD[self.e_SEC])

  def set_SQW_pin_mode(self, mode):
    '''!
      @brief control the operation of the SQW/OUT pin
      @param mode SQW Pin 输出模式:
      @n e_square_wave_LOW, e_square_wave_HIGH, e_square_wave_1Hz, 
      @n e_square_wave_4kHz, e_square_wave_8kHz, e_square_wave_32kHz
    '''
    self._write_reg(DS1307_CTL_REG, mode)

  def get_SQW_pin_mode(self):
    '''!
      @brief 获取 SQW/OUT pin 当前输出模式
      @return 输出模式:
      @n e_square_wave_LOW, e_square_wave_HIGH, e_square_wave_1Hz, 
      @n e_square_wave_4kHz, e_square_wave_8kHz, e_square_wave_32kHz
    '''
    return self._read_reg(DS1307_CTL_REG, 1)[0]

  @property
  def save_time_to_EEPROM(self):
    '''!
      @brief 将当前时间存入EEPROM, 可用于掉电重启后, 将时间设置为最后一次保存的时间
      @n 掉电时调用此接口, 重启时调用setTimeFromEEPROM()即可实现
    '''
    self._rtc_BCD = self._read_reg(DS1307_SEC_REG, 8)
    self.write_EEPROM(248, self._rtc_BCD)   # 用EEPROM最后8个字节来存储时间

  @property
  def set_time_from_EEPROM(self):
    '''!
      @brief 将时间设置为最后一次保存的时间
      @n 如果之前没调用过saveTimeToEEPROM(), 或者调用完后自行
      @n 修改了EEPROM里面的内容, 则可能导致时间设置错误
      @note 如果未使传感器掉电时复位主控板, 可能会使该接口误调用, 从而误更改时间
    '''
    self._rtc_BCD = self.read_EEPROM(248, 8)   # EEPROM最后8个字节是我们之前存储的时间
    self._write_reg(DS1307_SEC_REG, self._rtc_BCD)

  def write_EEPROM(self, reg, data):
    '''!
      @brief writes data to a EEPROM
      @param reg EEPROM address
      @param data written data
    '''
    size = len(data)
    while size > 8:
      self._write_reg(reg, data[reg : (reg + 8)], EEPROM_I2C_ADDR)
      reg += 8
      size -= 8
      time.sleep(0.01)
    self._write_reg(reg, data, EEPROM_I2C_ADDR)
    time.sleep(0.01)

  def read_EEPROM(self, reg, length):
    '''!
      @brief read the data from the EEPROM
      @param reg EEPROM address
      @param length read data length
      @return read data list
    '''
    return self._read_reg(reg, length, EEPROM_I2C_ADDR)

  def _bcd_to_bin(self, val):
    '''!
      @brief BCD code to BIN code
      @param val Input BCD code
      @return Return BIN code
    '''
    return val - 6 * (val >> 4)

  def _bin_to_bcd (self, val):
    '''!
      @brief BIN code to BCD code
      @param val Input BIN code
      @return Return BCD code
    '''
    return val + 6 * int(val / 10)

  def _write_reg(self, reg, data, addr = DS1307_I2C_ADDR):
    '''!
      @brief writes data to a register
      @param reg register address
      @param data written data
      @param addr 选择通信的I2C设备地址, ds1307或eeprom
    '''
    if isinstance(data, int):
        data = [data]
        #logger.info(data)
    self._i2c.write_i2c_block_data(addr, reg, data)

  def _read_reg(self, reg, length, addr = DS1307_I2C_ADDR):
    '''!
      @brief read the data from the register
      @param reg register address
      @param length read data length
      @param addr 选择通信的I2C设备地址, ds1307或eeprom
      @return read data list
    '''
    return self._i2c.read_i2c_block_data(addr, reg, length)
