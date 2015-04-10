
#include <errno.h>

#include "capwap.h"
#include "intavltree.h"
#include "dbg.h"
#include "log.h"
#include "capwap_items.h"

int cw_in_check_join_resp(struct conn *conn, struct cw_action_in *a, uint8_t * data,
			 int len)
{

	cw_action_in_t * mlist[60];

	/* Check for mandatory elements */
	int n = cw_check_missing_mand(mlist,conn,a);
	if (n) {
		cw_dbg_missing_mand(DBG_ELEM,conn,mlist,n,a);
		conn->capwap_state=CW_STATE_JOIN;
		errno=EAGAIN;
		return -1; //CW_RESULT_MISSING_MAND_ELEM;
	}

	

	cw_item_t * jresult = cw_itemstore_get(conn->incomming,	CW_ITEM_RESULT_CODE);

	if ( jresult ) {
		return jresult->dword;
	}

	/* set result code to ok and change to configure state */
//	cw_itemstore_set_dword(conn->outgoing,CW_ITEM_RESULT_CODE,0);

	return 0;
}