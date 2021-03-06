//
// Copyright 2018 Staysail Systems, Inc. <info@staysail.tech>
// Copyright 2018 Capitar IT Group BV <info@capitar.com>
//
// This software is supplied under the terms of the MIT License, a
// copy of which should be located in the distribution where this
// file was obtained (LICENSE.txt).  A copy of the license may also be
// found online at https://opensource.org/licenses/MIT.
//

#ifndef CORE_SOCKET_H
#define CORE_SOCKET_H

extern int  nni_sock_sys_init(void);
extern void nni_sock_sys_fini(void);

extern int         nni_sock_find(nni_sock **, uint32_t);
extern void        nni_sock_rele(nni_sock *);
extern int         nni_sock_open(nni_sock **, const nni_proto *);
extern void        nni_sock_close(nni_sock *);
extern void        nni_sock_closeall(void);
extern int         nni_sock_shutdown(nni_sock *);
extern uint16_t    nni_sock_proto(nni_sock *);
extern uint16_t    nni_sock_peer(nni_sock *);
extern const char *nni_sock_proto_name(nni_sock *);
extern const char *nni_sock_peer_name(nni_sock *);

extern int nni_sock_setopt(
    nni_sock *, const char *, const void *, size_t, int);
extern int  nni_sock_getopt(nni_sock *, const char *, void *, size_t *, int);
extern int  nni_sock_recvmsg(nni_sock *, nni_msg **, int);
extern int  nni_sock_sendmsg(nni_sock *, nni_msg *, int);
extern void nni_sock_send(nni_sock *, nni_aio *);
extern void nni_sock_recv(nni_sock *, nni_aio *);
extern uint32_t nni_sock_id(nni_sock *);

// nni_sock_pipe_add adds the pipe to the socket. It is called by
// the generic pipe creation code.  It also adds the socket to the
// ep list, and starts the pipe.  It does all these to ensure that
// we have complete success or failure, and there is no point where
// a pipe could wind up orphaned.
extern int  nni_sock_pipe_add(nni_sock *, nni_pipe *);
extern void nni_sock_pipe_remove(nni_sock *, nni_pipe *);
extern int  nni_sock_pipe_start(nni_sock *, nni_pipe *p);

extern int  nni_sock_ep_add(nni_sock *, nni_ep *);
extern void nni_sock_ep_remove(nni_sock *, nni_ep *);

// These are socket methods that protocol operations can expect to call.
// Note that each of these should be called without any locks held, since
// the socket can reenter the protocol.

// nni_socket_sendq obtains the upper writeq.  The protocol should
// receive messages from this, and place them on the appropriate pipe.
extern nni_msgq *nni_sock_sendq(nni_sock *);

// nni_socket_recvq obtains the upper readq.  The protocol should
// inject incoming messages from pipes to it.
extern nni_msgq *nni_sock_recvq(nni_sock *);

extern void nni_sock_reconntimes(nni_sock *, nni_duration *, nni_duration *);

// nni_sock_flags returns the socket flags, used to indicate whether read
// and or write are appropriate for the protocol.
extern uint32_t nni_sock_flags(nni_sock *);

// nni_ctx_open is used to open/create a new context structure.
// Contexts are not supported by most protocols, but for those that do,
// this can offer some improvements for massive concurrency/scalability.
// Returns NNG_ENOTSUP for protocols that lack context support.  This adds
// another reference (hold) on the socket on success, and the newly
// created context is also held by the caller. Not supported by raw mode
// sockets (will also return NNG_ENOTSUP).
extern int nni_ctx_open(nni_ctx **, nni_sock *);

// nni_ctx_find finds a context given its id.  The last argument should
// be true if the context is acquired merely to close it, false otherwise.
// (If the socket for the context is being closed, then this will return
// NNG_ECLOSED unless the final argument is true.)
extern int nni_ctx_find(nni_ctx **, uint32_t, bool);

// nni_ctx_rele is called to release a hold on the context.  These holds
// are acquired by either nni_ctx_open or nni_ctx_find.  If the context
// is being closed (nni_ctx_close was called), and this is the last reference,
// then the underlying context is freed, and the implicit socket hold
// by the context is also released.
extern void nni_ctx_rele(nni_ctx *);

// nni_ctx_close is used to close the context.  It also implictly releases
// the context.
extern void nni_ctx_close(nni_ctx *);

// nni_ctx_id returns the context ID, which can be used with nni_ctx_find.
extern uint32_t nni_ctx_id(nni_ctx *);

// nni_ctx_recv is an asychronous receive.
extern void nni_ctx_recv(nni_ctx *, nni_aio *);

// nni_ctx_send is an asychronous receive.
extern void nni_ctx_send(nni_ctx *, nni_aio *);

// nni_ctx_getopt is used to get a context option.
extern int nni_ctx_getopt(nni_ctx *, const char *, void *, size_t *, int);

// nni_ctx_setopt is used to set a context option.
extern int nni_ctx_setopt(nni_ctx *, const char *, const void *, size_t, int);

#endif // CORE_SOCKET_H
