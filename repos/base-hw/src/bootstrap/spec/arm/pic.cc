/*
 * \brief  Programmable interrupt controller for core
 * \author Stefan Kalkowski
 * \date   2012-10-24
 */

/*
 * Copyright (C) 2012-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#include <hw/spec/arm/cortex_a15.h>
#include <spec/arm/pic.h>
#include <platform.h>

using Memory_map = Hw::Cpu_memory_map<Bootstrap::CPU_MMIO_BASE>;
using Bootstrap::Platform;

void Bootstrap::Pic::init_cpu_local()
{
	/* disable the priority filter */
	_cpui.write<Cpu_interface::Pmr::Priority>(_distr.min_priority());

	/* disable preemption of IRQ handling by other IRQs */
	_cpui.write<Cpu_interface::Bpr::Binary_point>(~0);

	/* enable device */
	_cpui.write<Cpu_interface::Ctlr::Enable>(1);
}


Hw::Pic::Pic()
: _distr(Platform::mmio_to_virt(Memory_map::IRQ_CONTROLLER_DISTR_BASE)),
  _cpui (Platform::mmio_to_virt(Memory_map::IRQ_CONTROLLER_CPU_BASE)),
  _last_iar(Cpu_interface::Iar::Irq_id::bits(spurious_id)),
  _max_irq(_distr.max_irq())
{
	/* disable device */
	_distr.write<Distributor::Ctlr::Enable>(0);

	/* configure every shared peripheral interrupt */
	for (unsigned i = min_spi; i <= _max_irq; i++) {
		_distr.write<Distributor::Icfgr::Edge_triggered>(0, i);
		_distr.write<Distributor::Ipriorityr::Priority>(0, i);
		_distr.write<Distributor::Icenabler::Clear_enable>(1, i);
	}
	/* enable device */
	_distr.write<Distributor::Ctlr::Enable>(1);
}
