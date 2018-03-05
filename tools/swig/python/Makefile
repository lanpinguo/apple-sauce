#
# PYTHON_H is the path to headers for the version of Python
# installed on the target switch. If not set by the target O/S
# or the platform Makefile, default to the Python 2.6.6. headers
# in the vendor directory.
#
ifeq ($(PYTHON_H),)
   export PYTHON_H = $(OFDPA_BASE)/../vendor/python/python2.6.6
endif

# steer OSS tools to the correct version of SWIG
export SWIGVER = 3.0.0

export CFLAGS+= -Wno-unused-but-set-variable -I$(PYTHON_H) -fPIC -I$(abspath $(OFDPA_BASE)api/include) -Wall -L../build -lrpc_client

wrapper = ofdpawrap
base_modname = OFDPA_python

#
# Simple rule for compiling our applications, no
# incremental steps.
#
.DEFAULT_GOAL := _$(base_modname).so

_$(base_modname).so: $(wrapper).o
	$(CC) $(CFLAGS) -shared $< -Wl,-Bdynamic -lrpc_client -o $@
	chmod +x $(base_modname).py
	cp -fp $(base_modname).py $(abspath $(OUTPATH)/build)
	cp -fp $@ $(abspath $(OUTPATH)/build)

$(wrapper).c: $(wrapper).i
	swig -python -verbose -Wall -o $@ $<

$(wrapper).i: $(realpath $(wildcard $(OFDPA_BASE)/application/api/include/*.h))
	$(abspath $(OFDPA_BASE)/tools/swig/python/genifile.pl) -o $@ $^

