# -*- coding: utf-8 -*
'''!
  @file  set_time_and_SQW.py
  @brief  设置时间并开始计时，设置sqw引脚输出
  @details  读取时间和sqw引脚模式
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
    # 根据选择设置的时间类型, 传入对应范围的数值, 设置时间
    # type 要获取的时间类型:
    #   e_SEC, e_MIN, e_HR, e_DOW, e_DATE, e_MTH, e_YR
    # type_time 根据选择的时间类型, 写入对应范围的数值
    #   读取 e_SEC 类型时, 范围为: 00-59
    #   读取 e_MIN 类型时, 范围为: 00-59
    #   读取 e_HR 类型时, 范围为: 00-23
    #   读取 e_DOW 类型时, 范围为: 01-07
    #   读取 e_DATE 类型时, 范围为: 01-31
    #   读取 e_MTH 类型时, 范围为: 01-12
    #   读取 e_YR 类型时, 范围为: 2000-2099
  '''
  DS1307.set_type_time(DS1307.e_YR, 2000)

  '''
    # 从rtc模块获取单个类型的时间
    # type 要获取的时间类型:
    #   e_SEC, e_MIN, e_HR, e_DOW, e_DATE, e_MTH, e_YR
    # 根据选择的时间类型, 返回对应范围的数值
    #   读取 e_SEC 类型时, 范围为: 00-59
    #   读取 e_MIN 类型时, 范围为: 00-59
    #   读取 e_HR 类型时, 范围为: 00-23
    #   读取 e_DOW 类型时, 范围为: 01-07
    #   读取 e_DATE 类型时, 范围为: 01-31
    #   读取 e_MTH 类型时, 范围为: 01-12
    #   读取 e_YR 类型时, 范围为: 2000-2099
  '''
  print("get type time:  %u" %( DS1307.get_type_time(DS1307.e_YR) ) )

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
  time_list = [5, 1, 7, 6, 9, 9, 2021]
  DS1307.set_time(time_list)

  '''
    # 启动RTC计时功能
    # this bit is part of the seconds byte
  '''
  DS1307.start

  '''
    # control the operation of the SQW/OUT pin
    # mode SQW Pin 输出模式:
    #   e_square_wave_LOW, e_square_wave_HIGH, e_square_wave_1Hz, 
    #   e_square_wave_4kHz, e_square_wave_8kHz, e_square_wave_32kHz
  '''
  DS1307.set_SQW_pin_mode(DS1307.e_square_wave_1Hz)


def loop():
  '''
    # 获取 SQW/OUT pin 当前输出模式
    # 输出模式:
    #   e_square_wave_LOW, e_square_wave_HIGH, e_square_wave_1Hz, 
    #   e_square_wave_4kHz, e_square_wave_8kHz, e_square_wave_32kHz
  '''
  if DS1307.e_square_wave_1Hz == DS1307.get_SQW_pin_mode():
    print("SQW/OUT pin: 1Hz | ", end='')

  '''
    # 从rtc模块获取时间
    # 获取的时间的列表
    #   time_list[0]为 e_SEC 类型, 范围为: 00-59
    #   time_list[1]为 e_MIN 类型, 范围为: 00-59
    #   time_list[2]为 e_HR 类型, 范围为: 00-23
    #   time_list[3]为 e_DOW 类型, 范围为: 01-07
    #   time_list[4]为 e_DATE 类型, 范围为: 01-31
    #   time_list[5]为 e_MTH 类型, 范围为: 01-12
    #   time_list[6]为 e_YR 类型, 范围为: 2000-2099
  '''
  time_list = DS1307.get_time()
  print("time: %u/%u/%u-%u %u:%u:%u \n" %( time_list[6], time_list[5],
  time_list[4], time_list[3], time_list[2], time_list[1], time_list[0] ))

  time.sleep(1)


if __name__ == "__main__":
  setup()
  while True:
    loop()
