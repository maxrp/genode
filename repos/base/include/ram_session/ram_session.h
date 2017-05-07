/*
 * \brief  RAM session interface
 * \author Norman Feske
 * \date   2006-05-11
 */

/*
 * Copyright (C) 2006-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _INCLUDE__RAM_SESSION__RAM_SESSION_H_
#define _INCLUDE__RAM_SESSION__RAM_SESSION_H_

#include <base/stdint.h>
#include <base/ram_allocator.h>
#include <dataspace/capability.h>
#include <ram_session/capability.h>
#include <session/session.h>

namespace Genode {

	struct Ram_dataspace;
	typedef Capability<Ram_dataspace> Ram_dataspace_capability;

	struct Ram_session_client;
	struct Ram_session;
}


/**
 * RAM session interface
 */
struct Genode::Ram_session : Session, Ram_allocator
{
	static const char *service_name() { return "RAM"; }

	enum { CAP_QUOTA = 8 };

	typedef Ram_session_client Client;


	/**
	 * Destructor
	 */
	virtual ~Ram_session() { }

	/**
	 * Define reference account for the RAM session
	 *
	 * \param   ram_session    reference account
	 *
	 * \return  0 on success
	 *
	 * Each RAM session requires another RAM session as reference
	 * account to transfer quota to and from. The reference account can
	 * be defined only once.
	 */
	virtual int ref_account(Ram_session_capability ram_session) = 0;

	/**
	 * Transfer quota to another RAM session
	 *
	 * \param ram_session  receiver of quota donation
	 * \param amount       amount of quota to donate
	 * \return             0 on success
	 *
	 * Quota can only be transfered if the specified RAM session is
	 * either the reference account for this session or vice versa.
	 */
	virtual int transfer_quota(Ram_session_capability ram_session, size_t amount) = 0;

	/**
	 * Return current quota limit
	 */
	virtual size_t quota() = 0;

	/**
	 * Return used quota
	 */
	virtual size_t used() = 0;

	/**
	 * Return amount of available quota
	 */
	size_t avail()
	{
		size_t q = quota(), u = used();
		return q > u ? q - u : 0;
	}


	/*********************
	 ** RPC declaration **
	 *********************/

	GENODE_RPC_THROW(Rpc_alloc, Ram_dataspace_capability, alloc,
	                 GENODE_TYPE_LIST(Quota_exceeded, Out_of_metadata),
	                 size_t, Cache_attribute);
	GENODE_RPC(Rpc_free, void, free, Ram_dataspace_capability);
	GENODE_RPC(Rpc_ref_account, int, ref_account, Ram_session_capability);
	GENODE_RPC(Rpc_transfer_quota, int, transfer_quota, Ram_session_capability, size_t);
	GENODE_RPC(Rpc_quota, size_t, quota);
	GENODE_RPC(Rpc_used, size_t, used);

	GENODE_RPC_INTERFACE(Rpc_alloc, Rpc_free, Rpc_ref_account,
	                     Rpc_transfer_quota, Rpc_quota, Rpc_used);
};

#endif /* _INCLUDE__RAM_SESSION__RAM_SESSION_H_ */
