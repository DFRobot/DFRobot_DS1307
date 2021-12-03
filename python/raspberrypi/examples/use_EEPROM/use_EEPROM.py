# -*- coding: utf-8 -*
'''!
  @file  use_EEPROM.py
  @brief  本demo演示了, 如何使用传感器上的EEPORM
  @details  可以通过其保存上次时间, 重新上电后用保存的时间设置DS1307
  @n 也可向EEPROM写入(读取)一些字符串, 或者uint8_t的数组, 掉电保存
  @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  @license  The MIT License (MIT)
  @author  [qsjhyy](yihuan.huang@dfrobot.com)
  @version  V1.0
  @date  2021-12-02
  @url  https://github.com/DFRobot/DFRobot_DS1307
'''
from __future__ import print_function
import sys
import os
sys.path.append(os.path.dirname(os.path.dirname(os.path.dirname(os.path.realpath(__file__)))))

from DFRobot_DS1307 import *

'''
  # Module I2C communication init
  # ds1307_addr I2C通信地址
  # bus I2C总线
'''
DS1307 = DFRobot_DS1307()


def setup():
  while (DS1307.begin() == False):
    print ('Please check that the device is properly connected')
    time.sleep(3)
  print("DS1307 begin successfully!!!")

  '''
    # 停止RTC计时功能
    # this bit is part of the seconds byte
  '''
  DS1307.stop

  '''
    # 根据给的数组, 设置所有时间
    # time_list 按如下格式编辑的数组
    #   time_list[0]为 e_SEC 类型, 范围为: 00-59
    #   time_list[1]为 e_MIN 类型, 范围为: 00-59
    #   time_list[2]为 e_HR 类型, 范围为: 00-23
    #   time_list[3]为 e_DOW 类型, 范围为: 01-07
    #   time_list[4]为 e_DATE 类型, 范围为: 01-31
    #   time_list[5]为 e_MTH 类型, 范围为: 01-12
    #   time_list[6]为 e_YR 类型, 范围为: 2000-2099
    #   注意: 超出范围的将导致设置错误
  '''
  # time_list = [5, 1, 7, 6, 9, 9, 2021]
  # DS1307.set_time(time_list)

  '''
    # 将当前时间存入EEPROM, 可用于掉电重启后, 将时间设置为最后一次保存的时间
    # 掉电时调用此接口, 重启时调用setTimeFromEEPROM()即可实现
  '''
  # DS1307.save_time_to_EEPROM

  '''
    # 将时间设置为最后一次保存的时间
    # 如果之前没调用过saveTimeToEEPROM(), 或者调用完后自行
    # 修改了EEPROM里面的内容, 则可能导致时间设置错误
    # 如果未使传感器掉电时复位主控板, 可能会使该接口误调用, 从而误更改时间
  '''
  DS1307.set_time_from_EEPROM

  '''
    # 启动RTC计时功能
    # this bit is part of the seconds byte
  '''
  DS1307.start

  print("--- EEPROM Read-Write Test---")
  str_data = "This is data from the eeprom!"   # data to write
  list_data = list(str_data)   # 字符串转列表
  '''
    # writes data to a EEPROM
    # reg EEPROM address
    # data written data
    # 用户可自由存储的数据大小为247个字节, 范围0-247
    # 最后8个字节是存储用户调用saveTimeToEEPROM()保存的时间数据
  '''
  # 将字符串列表的字符元素挨个转换为其对应的整数, 并写入EEPROM
  DS1307.write_EEPROM( 0, list(map( ord, list_data )) )
  print("Written Done!")

  print("Read Data: ", end='')
  '''
    # read the data from the EEPROM
    # reg EEPROM address
    # length read data length
    # read data list
  '''
  # 将从EEPROM读出来的整数列表的整数元素依次转换为其对应的字符, 再将得到的字符列表转换为字符串打印
  print(''.join( list(map( chr, DS1307.read_EEPROM(0, len(list_data)) )) ))

def loop():
  '''
    # 从rtc模块获取时间
    # 获取的时间的列表
    #   rtc[0]为 e_SEC 类型, 范围为: 00-59
    #   rtc[1]为 e_MIN 类型, 范围为: 00-59
    #   rtc[2]为 e_HR 类型, 范围为: 00-23
    #   rtc[3]为 e_DOW 类型, 范围为: 01-07
    #   rtc[4]为 e_DATE 类型, 范围为: 01-31
    #   rtc[5]为 e_MTH 类型, 范围为: 01-12
    #   rtc[6]为 e_YR 类型, 范围为: 2000-2099
  '''
  time_list = DS1307.get_time()
  print("time: %u/%u/%u-%u %u:%u:%u \n" %(time_list[6], time_list[5], 
  time_list[4], time_list[3], time_list[2], time_list[1], time_list[0]))

  '''
    # 将当前时间存入EEPROM, 可用于掉电重启后, 将时间设置为最后一次保存的时间
    # 掉电时调用此接口, 重启时调用setTimeFromEEPROM()即可实现
  '''
  DS1307.save_time_to_EEPROM

  time.sleep(1)


if __name__ == "__main__":
  setup()
  while True:
    loop()
