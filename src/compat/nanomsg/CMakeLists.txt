#
# Copyright 2018 Capitar IT Group BV <info@capitar.com>
# Copyright 2018 Staysail Systems, Inc. <info@staysail.tech>
#
# This software is supplied under the terms of the MIT License, a
# copy of which should be located in the distribution where this
# file was obtained (LICENSE.txt).  A copy of the license may also be
# found online at https://opensource.org/licenses/MIT.
#

set(COMPAT_SOURCES compat/nanomsg/nn.c)
set(COMPAT_HEADERS compat/nanomsg/nn.h)

set(NNG_SOURCES ${NNG_SOURCES} ${COMPAT_SOURCES} PARENT_SCOPE)
set(NNG_HEADERS ${NNG_HEADERS} ${COMPAT_HEADERS} PARENT_SCOPE)
