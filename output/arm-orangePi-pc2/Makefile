# global definitions


ifeq ($(OFDPA_BASE),)
   OFDPA_BASE := $(shell echo $(dir $(abspath $(firstword $(MAKEFILE_LIST)))) | sed -e 's/\(.*\)output\/.*/\1/')
endif
export OFDPA_BASE

ifeq ($(OUTPATH),)
   OUTPATH := $(dir $(abspath $(firstword $(MAKEFILE_LIST))))
endif
export OUTPATH

ifeq ($(TOOLCHAIN_BASE_DIR),)
   TOOLCHAIN_BASE_DIR = 
endif
export TOOLCHAIN_BASE_DIR

ifeq ($(origin BUILDROOT), undefined)
   BUILDROOT =
   export BUILDROOT
endif

ifeq ($(GCC_LIB_VER),)
   GCC_LIB_VER = 4.8.2
endif
export GCC_LIB_VER

ifeq ($(TARGET_ARCHITECTURE),)
   TARGET_ARCHITECTURE = arm-broadcom-linux-gnueabi
endif
export TARGET_ARCHITECTURE

L7_TARGETOS_VARIANT = xldk_3.6.4
export L7_TARGETOS_VARIANT
L7_CPU = p2020_qc
export L7_CPU

ifeq ($(CROSS_COMPILE),)
   CROSS_COMPILE = 
endif
export CROSS_COMPILE

ifeq ($(KERNEL_SRC),)
   KERNEL_SRC = $(realpath $(OFDPA_BASE)/systems/cpu/$(L7_CPU)/$(L7_TARGETOS_VARIANT)/kernel)
endif
export KERNEL_SRC

ifeq ($(PLATFORM_CODE),)
   PLATFORM_CODE = $(realpath $(OFDPA_BASE)/systems/platform/iTN185B/)
endif
export PLATFORM_CODE

ifeq ($(kernel_version),)
   kernel_version=3_6
endif
export kernel_version

ifeq (,$(L7_SDK_PLATFORM))
   L7_SDK_PLATFORM=iproc
endif
export L7_SDK_PLATFORM

OFDPA_CFLAGS += -DOFDPA_OAM -DSABER2_ETHOAM_EXTEND -DSUPPORT_MPLS_PORT_VLAN_LIST 
OFDPA_CFLAGS += -DMPLS_UNTAG_SUPPORT
OFDPA_CFLAGS += -DINCLUDE_FPGA_OAM
OFDPA_CFLAGS += -DPUSH_POP_SWAP_SUPPORT
OFDPA_CFLAGS += -DLEGACY_SWITCH_SUPPORT
OFDPA_CFLAGS += -DINCLUDE_FLEX_COUNTER_DISABLE

include $(OFDPA_BASE)/tools/Make.common
