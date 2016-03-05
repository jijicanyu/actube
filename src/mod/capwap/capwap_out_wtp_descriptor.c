#include "cw/log.h"
#include "cw/conn.h"
#include "cw/capwap_items.h"
#include "cw/cw.h"

#include "cw/capwap.h"

static int cw_put_encryption_subelems(uint8_t *dst,int capwap_mode)
{
	int n=2;
	
	dst+=cw_put_byte(dst,n);	

	int i;
	for (i=0; i<n; i++){
		dst+=cw_put_byte(dst,0);
		dst+=cw_put_byte(dst,0);
		dst+=cw_put_byte(dst,0);
	}

	return 3*n+1;
}



int capwap_out_wtp_descriptor(struct conn *conn, struct cw_action_out *a, uint8_t * dst)
{

	mbag_t mbag = conn->config;

	// XXX Dummy WTP Descriptor Header
	uint8_t *d = dst+4;

	d+=cw_put_byte(d,conn->radios->count);	//max radios
	d+=cw_put_byte(d,2);	//radios in use

	d+=cw_put_encryption_subelems(d,conn->capwap_mode);

	mbag_item_t * i;
	i = mbag_get(mbag,CW_ITEM_WTP_HARDWARE_VERSION);
	if ( i ) {	
	 	d += cw_put_version(d,CW_SUBELEM_WTP_HARDWARE_VERSION,i->data);
	}
	else {
		cw_log(LOG_ERR, "Can't send Hardware Version in WTP Descriptor, not set.");
	}


	i = mbag_get(mbag,CW_ITEM_WTP_SOFTWARE_VERSION);

	if ( i ) {	
	 	d += cw_put_version(d,CW_SUBELEM_WTP_SOFTWARE_VERSION,i->data);
	}
	else {
		cw_log(LOG_ERR, "Can't send Software Version in WTP descriptor, not set.");
	}

	i = mbag_get(mbag,CW_ITEM_WTP_BOOTLOADER_VERSION);
	if ( i ) {	
	 	d += cw_put_version(d,CW_SUBELEM_WTP_BOOTLOADER_VERSION,i->data);
	}
	else {
		cw_log(LOG_ERR, "Can't send Bootloader Version in WTP descriptor, not set.");
	}


	int len = d-dst-4;
	return len + cw_put_elem_hdr(dst,a->elem_id,len);
}	