/*
 * \brief   Specific i.MX53 bootstrap implementations
 * \author  Stefan Kalkowski
 * \date    2012-10-24
 */

/*
 * Copyright (C) 2012-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#include <platform.h>
#include <spec/imx53/drivers/trustzone.h>
#include <spec/arm/imx_aipstz.h>
#include <spec/arm/imx_csu.h>

bool Bootstrap::secure_irq(unsigned i)
{
	using Board = Genode::Board_base;

	if (i == Board::EPIT_1_IRQ) return true;
	if (i == Board::EPIT_2_IRQ) return true;
	if (i == Board::SDHC_IRQ)   return true;
	return false;
}


Bootstrap::Platform::Board::Board()
: early_ram_regions(Memory_region { Trustzone::SECURE_RAM_BASE,
                                    Trustzone::SECURE_RAM_SIZE }),
  core_mmio(Memory_region { UART_1_MMIO_BASE, UART_1_MMIO_SIZE },
            Memory_region { EPIT_1_MMIO_BASE, EPIT_1_MMIO_SIZE },
            Memory_region { IRQ_CONTROLLER_BASE, IRQ_CONTROLLER_SIZE },
            Memory_region { CSU_BASE, CSU_SIZE })
{
	Aipstz aipstz_1(AIPS_1_MMIO_BASE);
	Aipstz aipstz_2(AIPS_2_MMIO_BASE);

	/* set exception vector entry */
	Cpu::Mvbar::write(0xfff00000); //FIXME

	/* enable coprocessor 10 + 11 access for TZ VMs */
	Cpu::Nsacr::access_t v = 0;
	Cpu::Nsacr::Cpnsae10::set(v, 1);
	Cpu::Nsacr::Cpnsae11::set(v, 1);
	Cpu::Nsacr::write(v);

	/* configure central security unit */
	Csu csu(Genode::Board_base::CSU_BASE, true, false, true, false);
}
