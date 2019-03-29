/*
  Created by Fabrizio Di Vittorio (fdivitto2013@gmail.com) - <http://www.fabgl.com>
  Copyright (c) 2019 Fabrizio Di Vittorio.
  All rights reserved.

  This file is part of FabGL Library.

  FabGL is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  FabGL is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with FabGL.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef _PS2CONTROLLER_H_INCLUDED
#define _PS2CONTROLLER_H_INCLUDED


/**
 * @file
 *
 * @brief This file contains fabgl::PS2ControllerClass definition and the PS2Controller instance.
 */


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/queue.h"

#include "fabglconf.h"


namespace fabgl {



/**
 * @brief The PS2 device controller class.
 *
 * The PS2 controller uses ULP coprocessor and RTC slow memory to communicate with up to two PS2 devices.<br>
 * The ULP coprocessor continuously monitor CLK and DATA lines for incoming data. Optionally can send commands to the PS2 devices.
 */
class PS2ControllerClass {

public:

  /**
  * @brief Initialize PS2 device controller.
  *
  * Initializes the PS2 controller assigning GPIOs to DAT and CLK lines.
  *
  * @param port0_clkGPIO The GPIO number of Clock line for PS/2 port 0.
  * @param port0_datGPIO The GPIO number of Data line for PS/2 port 0.
  * @param port1_clkGPIO The GPIO number of Clock line for PS/2 port 1 (GPIO_NUM_39 to disable).
  * @param port1_datGPIO The GPIO number of Data line for PS/2 port 1 (GPIO_NUM_39 to disable).
  */
  void begin(gpio_num_t port0_clkGPIO, gpio_num_t port0_datGPIO, gpio_num_t port1_clkGPIO = GPIO_NUM_39, gpio_num_t port1_datGPIO = GPIO_NUM_39);

  /**
   * @brief Get the number of scancodes available in the controller buffer.
   *
   * @param PS2Port PS2 port number (0 = port 0, 1 = port1).
   *
   * @return The number of scancodes available to read.
   */
  int dataAvailable(int PS2Port);

  /**
   * @brief Get a scancode from the queue.
   *
   * @param timeOutMS Timeout in milliseconds. -1 means no timeout (infinite time).
   * @param isReply Set true when waiting for a reply from a command sent to the device.
   * @param PS2Port PS2 port number (0 = port 0, 1 = port1).
   *
   * @return The first scancode of the queue (-1 if no data is available in the timeout period).
   */
  int getData(int timeOutMS, bool isReply, int PS2Port);

  /**
   * @brief Send a command to the device.
   *
   * @param PS2Port PS2 port number (0 = port 0, 1 = port1).
   * @param data Byte to send to the PS2 device.
   */
  void sendData(uint8_t data, int PS2Port);


private:

  static void IRAM_ATTR rtc_isr(void * arg);

  // address of next word to read in the circular buffer
  int                   m_readPos[2];

  // address of next word to read in the circular buffer. Set by sendData() and used by getReplyData()
  volatile int          m_replyReadPos[2];

  // task that is waiting for TX ends
  volatile TaskHandle_t m_TXWaitTask[2];

  // task that is waiting for RX event
  volatile TaskHandle_t m_RXWaitTask[2];

};





} // end of namespace



extern fabgl::PS2ControllerClass PS2Controller;



#endif

