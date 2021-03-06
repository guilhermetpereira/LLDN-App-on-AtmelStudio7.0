/**
 * \file
 *
 * \brief SPI driver for megaRF.
 *
 * Copyright (c) 2013 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
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
 */

#include "spi_megarf.h"

/* ! \internal Local storage of SPI interrupt callback function */
static spi_int_callback_t spi_int_callback;

/**
 * \internal
 * \brief Interrupt handler for SPI interrupt
 *
 * This function will handle interrupt on SPI and
 * call the callback function.
 */

ISR(SPI_STC_vect)
{
	if (spi_int_callback) {
		spi_int_callback();
	}
}

/**
 * \brief Calculates the SPI baudrate divider.
 *
 * \param baudrate The expected baudrate on the SPI.
 * \param clkper_hz  SPI module input clock frequency (Peripheral clock, Hz).
 * \param spi The SPI module address
 *
 * \return Status of operation.
 *   \retval >=0  Success.
 *   \retval  <0  Error.
 */
int8_t spi_set_baud_div(volatile void *spi, uint32_t baudrate,
		uint32_t clkper_hz)
{
	uint32_t divisor;
	uint8_t divisor_8bit;
	uint8_t ctrl;

	/* Sanity check, requested baudrate is lower than system clock */
	Assert(clkper_hz > baudrate);

	/*
	 * Get wanted divisor rounded up so we don't get speed higher than
	 * requested baudrate.
	 */
	divisor = (clkper_hz + baudrate - 1) / baudrate;

	if (divisor > 128) {
		/*
		 * Highest possible divisor is 128 so fail since we can't get
		 * low enough baudrate.
		 */
		return -1;
	}

	/*
	 * We now know that the divisor is 128 or lower so move it into a 8-bit
	 * variable to make sure the following comparison is more optimized.
	 */
	divisor_8bit = divisor;

	spi_double_speed_disable(spi);

	/*
	 * For divisor values between the possible ones round up to the closest
	 * higher one to avoid higher baudrate than requested.
	 */
	if (divisor_8bit > 64) {
		ctrl = SPI_PRESCALER_DIV128_gc;
	} else if (divisor_8bit > 32) {
		ctrl = SPI_PRESCALER_DIV64_gc;
	} else if (divisor_8bit > 16) {
		ctrl = SPI_PRESCALER_DIV64_gc;

		/* Enable double baud rate */
		spi_double_speed_enable(spi);
	} else if (divisor_8bit > 8) {
		ctrl = SPI_PRESCALER_DIV16_gc;
	} else if (divisor_8bit > 4) {
		ctrl = SPI_PRESCALER_DIV16_gc;

		/* Enable double baud rate */
		spi_double_speed_enable(spi);
	} else if (divisor_8bit > 2) {
		ctrl = SPI_PRESCALER_DIV4_gc;
	} else {
		ctrl = SPI_PRESCALER_DIV4_gc;

		/* Enable double baud rate */
		spi_double_speed_enable(spi);
	}

	/* Update register and make sure to clear out any leftover bits */
	SPCR = (SPCR & ~(SPI_PRESCALER_gm)) | ctrl;

	return 1;
}

/**
 * \brief Set SPI interrupt callback function
 *
 * This function allows the caller to set and change the interrupt callback
 * function. Without setting a callback function the interrupt handler in the
 * driver will only clear the interrupt flags.
 *
 * \param callback Reference to a callback function
 */
void spi_set_interrupt_callback(spi_int_callback_t callback)
{
	spi_int_callback = callback;
}
