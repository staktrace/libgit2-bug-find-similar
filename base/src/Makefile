#!gmake
#
# The contents of this file are subject to the Netscape Public License
# Version 1.0 (the "NPL"); you may not use this file except in
# compliance with the NPL.  You may obtain a copy of the NPL at
# http://www.mozilla.org/NPL/
#
# Software distributed under the NPL is distributed on an "AS IS" basis,
# WITHOUT WARRANTY OF ANY KIND, either express or implied. See the NPL
# for the specific language governing rights and limitations under the
# NPL.
#
# The Initial Developer of this code under the NPL is Netscape
# Communications Corporation.  Portions created by Netscape are
# Copyright (C) 1998 Netscape Communications Corporation.  All Rights
# Reserved.

DEPTH=../..

DEFINES =-D_IMPL_NS_BASE

LIBRARY_NAME = raptorbase

CPPSRCS =				\
	nsArena.cpp			\
	nsAtomTable.cpp			\
  nsBTree.cpp \
	nsByteBuffer.cpp		\
	nsCRT.cpp			\
	nsDeque.cpp		\
	nsInputStream.cpp		\
  nsRBTree.cpp \
	nsString.cpp			\
	nsUnicharBuffer.cpp		\
	nsUnicharInputStream.cpp	\
	nsURL.cpp			\
	nsVoidArray.cpp			\
	$(NULL)

EXPORTS =				\
	nscore.h			\
  nsBTree.h \
	nsCRT.h				\
  nsDeque.h \
	nsIArena.h			\
	nsIAtom.h			\
	nsIByteBuffer.h			\
	nsIInputStream.h		\
	nsITimer.h			\
	nsITimerCallback.h		\
	nsIUnicharBuffer.h		\
	nsIUnicharInputStream.h		\
	nsIURL.h			\
  nsRBTree.h \
	nsString.h			\
	nsVoidArray.h			\
	nsUnitConversion.h		\
	$(NULL)

MODULE = raptor

REQUIRES = xpcom netlib raptor

include $(DEPTH)/config/config.mk

TARGET = $(LIBARY)

include $(DEPTH)/config/rules.mk
