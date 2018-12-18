SYSTEMC_HOME    = /usr/local/systemc-2.3.1
TARGET_ARCH     = linux64
TOP_DIR = $(PWD)

SYSTEMC_INC_DIR = $(SYSTEMC_HOME)/include
SYSTEMC_LIB_DIR = $(SYSTEMC_HOME)/lib-$(TARGET_ARCH)

FLAGS           = -g -std=c++98 -Wall -pedantic -Wno-long-long \
                 -DSC_INCLUDE_DYNAMIC_PROCESSES -fpermissive \
                 -I$(SYSTEMC_INC_DIR) -w
LDFLAGS         = -L$(SYSTEMC_LIB_DIR) -lsystemc -lm

SRCS = $(wildcard $(TOP_DIR)/src/*.cpp)
$(info $(SRCS))
OBJS = $(SRCS:.cpp=.o)

main:
	g++ -o model $(LDFLAGS) $(FLAGS) $(SRCS)

run: main
	export LD_LIBRARY_PATH=$(SYSTEMC_LIB_DIR); $(TOP_DIR)/model

play: run
	gtkwave wave.vcd
