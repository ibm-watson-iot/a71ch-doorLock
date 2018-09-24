#*******************************************************************************
#  Copyright (c) 2018 IBM Corp.
#
#  All rights reserved. This program and the accompanying materials
#  are made available under the terms of the Eclipse Public License v1.0
#  and Eclipse Distribution License v1.0 which accompany this distribution.
#
#  The Eclipse Public License is available at
#     http://www.eclipse.org/legal/epl-v10.html
#  and the Eclipse Distribution License is available at
#    http://www.eclipse.org/org/documents/edl-v10.php.
#
#  Contributors:
#     Ranjan Dasgupta - initial drop of WIoTP samples for NXP i.MX
#
#*******************************************************************************/

#
# The Makefile to build Door Lock demo device code for NXP i.MX operating environment with
# A71CH Secure Element.
#

current_dir := $(shell pwd)
parent_dir := $(shell dirname ${current_dir})

ifndef blddir
    blddir = ${current_dir}
endif

ifndef CLIENTDIR
    CLIENTDIR = /opt/iotnxpimxclient/
endif

CC ?= gcc
RM = rm -f

ifndef INSTALL
INSTALL = install
endif
INSTALL_PROGRAM = $(INSTALL)
INSTALL_DATA =  $(INSTALL) -m 644


CINCS = -I/usr/local/include
CFLAGS = $(CINCS) -fPIC -Wall -Wextra -O2 -g
LDFLAGS = -lwiotpnxpimxa71ch

SAMPLE_FILES = doorLock
SAMPLES = ${addprefix ${blddir}/,${SAMPLE_FILES}}

.PHONY: all clean ${SAMPLES}

all: install

build: ${SAMPLES}

${SAMPLES}: ${blddir}/%: ${blddir}/%.c
        $(CC) -o $@ $< ${CFLAGS} ${LDFLAGS}

install: build
        mkdir -p $(CLIENTDIR)bin
        mkdir -p $(CLIENTDIR)config
        mkdir -p $(CLIENTDIR)certs
        $(INSTALL_PROGRAM) ${blddir}/doorLock $(CLIENTDIR)bin/.
        $(INSTALL_DATA) ${blddir}/*.pem $(CLIENTDIR)certs/.
        $(INSTALL_DATA) ${blddir}/*.cfg $(CLIENTDIR)config/.

uninstall:
        -${RM} $(CLIENTDIR)bin/doorLock

clean:
        -${RM} ${SAMPLE_FILES}

print-%  : ; @echo $* = $($*)

