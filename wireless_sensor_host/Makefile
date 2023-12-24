TARGET=wireless_sensor_host

LIBS += libusb-1.0

CXX_SRC 	 += main.cpp
CXX_SRC 	 += Device.cpp
CXX_SRC 	 += DeviceManager.cpp
CXX_SRC 	 += threadname.cpp

PCDEV_ROOT=../../pcdev

UCDEV_ROOT=../../../uc/ucdev

CXX_INCLUDES += $(UCDEV_ROOT)/lib/bsprot

 

include $(PCDEV_ROOT)/build/make/all.mk
