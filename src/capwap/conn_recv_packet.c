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

#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>

#include "conn.h"

int conn_recv_packet(struct conn* conn,uint8_t *buf,int len)
{
	int n;
	int flags=0;
	while( (n = recv(conn->sock,(char*)buf,len,flags)) < 0 ){
		if (errno!=EINTR)
		{
			if ( errno==EAGAIN )
				return n;

		}
	}
	return n;
}


