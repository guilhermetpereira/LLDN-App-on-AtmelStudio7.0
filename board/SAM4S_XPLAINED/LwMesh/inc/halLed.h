/**
 * \file halLed.h
 *
 * \brief LEDs interface
 *
 * Copyright (C) 2012-2013, Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 * $Id: halLed.h 8367 2013-07-25 17:18:50Z ataradov $
 *
 */

#ifndef _HAL_LED_H_
#define _HAL_LED_H_

/*- Includes ---------------------------------------------------------------*/
#include "hal.h"
#include "halGpio.h"

/*- Definitions ------------------------------------------------------------*/
HAL_GPIO_PIN(LED0, C, 10);
HAL_GPIO_PIN(LED1, C, 17);

/*- Implementations --------------------------------------------------------*/

/*************************************************************************//**
*****************************************************************************/
INLINE void HAL_LedInit(void)
{
	HAL_GPIO_LED0_out();
	HAL_GPIO_LED0_set();

	HAL_GPIO_LED1_out();
	HAL_GPIO_LED1_set();
}

/*************************************************************************//**
*****************************************************************************/
INLINE void HAL_LedClose(void)
{
	HAL_GPIO_LED0_in();
	HAL_GPIO_LED1_in();
}

/*************************************************************************//**
*****************************************************************************/
INLINE void HAL_LedOn(uint8_t i)
{
	if(0 == i)
		HAL_GPIO_LED0_clr();
	else if(1 == i)
		HAL_GPIO_LED1_clr();	
}

/*************************************************************************//**
*****************************************************************************/
INLINE void HAL_LedOff(uint8_t i)
{
	if(0 == i)
		HAL_GPIO_LED0_set();
	else if(1 == i)
		HAL_GPIO_LED1_set();
}

/*************************************************************************//**
*****************************************************************************/
INLINE void HAL_LedToggle(uint8_t i)
{
	if(0 == i)
		HAL_GPIO_LED0_read() ? HAL_GPIO_LED0_clr() : HAL_GPIO_LED0_set();
	else if(1 == i)
		HAL_GPIO_LED1_read() ? HAL_GPIO_LED1_clr() : HAL_GPIO_LED1_set();
}

#endif // _HAL_LED_H_
