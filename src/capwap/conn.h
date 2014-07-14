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


#ifndef __CONN_H
#define __CONN_H

#include <stdint.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "fragman.h"
#include "cwmsg.h"
#include "cwrmsg.h"


struct conn{
	int sock;
	struct sockaddr_storage addr;
	int recv_timeout;

	FRAGMAN * fragman;
//	void (* process_message)(void *,uint8_t *,uint8_t *, int);

//	void (* process_message)(void *,struct cwrmsg *);  //uint8_t *,uint8_t *, int);
//	void * pmsgarg;

	int fragid;

	int seqnum;

	int last_seqnum_received; 
	int last_message_id_received;

	struct cwmsg * last_response;
	struct cwmsg swm;
	int last_response_seqnum;
	int last_response_rid;

	uint8_t buffer[65536];
	int mtu;

	/* receive and send methods */

	int (*recv_packet)(struct conn  *, uint8_t *,int);
	int (*send_packet)(struct conn *, const uint8_t *, int);

	int (*read)(struct conn *, uint8_t*, int);
	int (*write)(struct conn *, const uint8_t*, int);

	/* optional packet queue */
	uint8_t ** q; //[WTPMAN_QSIZE];
	int qsize;
	int qrpos;
	int qwpos;
	sem_t q_sem;
	uint8_t * cur_packet;
	int cur_packet_len;
	int cur_packet_pos;

#ifdef WITH_DTLS
	/* dtls stuff */
	int (*dtls_start)(struct conn*);
	int (*dtls_accept)(struct conn*);

	char * dtls_psk;
	int dtls_psk_len;

	char * dtls_cert_file;
	char * dtls_key_file;
	void * dtls_data;
	char * dtls_cipher;
#endif	
	/* used to link the conn obj with other objects */
	void * data;
};



struct conn * conn_create(int sock, struct sockaddr * addr,
//	void (*process_message)(void *arg,uint8_t *hdr,uint8_t *msg, int len), 
//	void (*process_message)(void *arg,struct cwrmsg *), //uint8_t *hdr,uint8_t *msg, int len), 
//	void *pmsgarg, 
	int qsize);


extern int conn_send_cwmsg(struct conn * conn, struct cwmsg * cwmsg);
extern void conn_process_packet(struct conn * conn, uint8_t *packet, int len,int(*cb)(void*,struct cwrmsg*),void *cbarg);
extern struct cwrmsg * conn_get_message(struct conn * conn);

extern int conn_send_packet(struct conn * conn,const uint8_t * buffer, int len);
extern void conn_destroy(struct conn * conn);

uint8_t * conn_q_get_packet(struct conn * conn);
extern int conn_q_recv_packet(struct conn * conn, uint8_t * buffer,int len);

extern int conn_recv_packet(struct conn* conn,uint8_t *buf,int len);

#define conn_get_next_seqnum(conn) (conn->seqnum=((conn->seqnum+1)&0xff))
#define conn_get_last_seqnum(conn) (conn->seqnum&0xff)


/* connlist stuff */
#include "avltree.h"
struct connlist {
/*	struct conn ** connlist; */

	struct avltree * t;
	int len;
	pthread_mutex_t connlist_mutex;
};


struct connlist * connlist_create(int len);
void connlist_lock(struct connlist * cl);
void connlist_unlock(struct connlist * cl);
void conlist_destroy(struct connlist * cl);
struct conn * connlist_get(struct connlist * cl, const struct sockaddr * addr);
struct conn * connlist_add(struct connlist * cl, struct conn * conn);
void connlist_remove(struct connlist *cl,struct conn * conn);
void connlist_destroy(struct connlist * cl);








#endif /* __CONLIST_H */
