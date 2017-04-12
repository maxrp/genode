/*
 * \brief  Serial output driver for core
 * \author Stefan Kalkowski
 * \date   2012-10-24
 */

/*
 * Copyright (C) 2012-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _CORE__SPEC__IMX__SERIAL_H_
#define _CORE__SPEC__IMX__SERIAL_H_

/* core includes */
#include <board.h>
#include <platform.h>

/* Genode includes */
#include <drivers/uart_base.h>

namespace Genode
{
	/**
	 * Serial output driver for core
	 */
	class Serial : public Imx_uart_base
	{
		public:

			Serial(unsigned baudrate)
			: Imx_uart_base(Platform::mmio_to_virt(Board::UART_1_MMIO_BASE),
			                /* ignored clock rate */ 0, baudrate) { }
	};
}

#endif /* _CORE__SPEC__IMX__SERIAL_H_ */
