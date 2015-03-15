/*
    This file is part of libcapwap.

    libcapwap is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libcapwap is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

*/

/**
 * @file
 * @brief Implementation of cwmsg_addelem_ac_descriptor.
 */ 

#include <string.h>
#include "capwap.h"


static int add_subelem(uint8_t * buffer,int type, uint32_t vendor,bstr_t version)
{

	*((uint32_t*)buffer) = htonl(vendor);
	*((uint32_t*)(buffer+4)) = htonl( (type<<16) | bstr_len(version));

	memcpy(buffer+8,bstr_data(version),bstr_len(version));

	return 8+bstr_len(version);
}

/**
 * Add an ac descriptor message element.
 * @param msg pointer to the message
 * @param acinfo acinfo structure where data is taken from
 */
void cwmsg_addelem_ac_descriptor(struct cwmsg *msg,struct ac_info * acinfo)
{
	uint8_t buffer[600];
	uint8_t * acd = buffer;

	*((uint32_t*)(acd))=htonl((acinfo->stations<<16) | (acinfo->limit));
	*((uint32_t*)(acd+4))=htonl((acinfo->active_wtps<<16) | acinfo->max_wtps);
	*((uint32_t*)(acd+8))=htonl((acinfo->security<<24) | (acinfo->rmac<<16) | acinfo->dtls_policy );


	int len = 12;

	switch (msg->capwap_mode){
		case CWMODE_CISCO:
			len+=add_subelem(buffer+len,0,CW_VENDOR_ID_CISCO,acinfo->cisco_hardware_version);
			len+=add_subelem(buffer+len,1,CW_VENDOR_ID_CISCO,acinfo->cisco_software_version);
			break;
		default:
			len+=add_subelem(buffer+len,0,0,acinfo->hardware_version);
			len+=add_subelem(buffer+len,1,0,acinfo->software_version);

	}

	cwmsg_addelem(msg,CWMSGELEM_AC_DESCRIPTOR,acd,len);
}

