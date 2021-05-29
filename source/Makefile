IVERILOG_INSTALL_DIR =$(HOME)/projects/install/iverilog
OBJECTS = lint.o massageHandler.o configuration.o namingConvention.o dftChecks.o parameterChecks.o processChecks.o synthesisChecks.o
INCDIRS = -I$(IVERILOG_INSTALL_DIR)/include/iverilog
LIBDIRS = -L$(IVERILOG_INSTALL_DIR)/lib/ivl
CFLAGS = -g -fPIC
#CFLAGS = -O2 -fPIC
LDFLAGS = -shared
CC = gcc

all: install

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCDIRS) -c $<
lint.tgt: $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS)
install: lint.tgt
	cp lint.tgt $(IVERILOG_INSTALL_DIR)/lib/ivl
	cp lint*.conf $(IVERILOG_INSTALL_DIR)/lib/ivl
clean:
	rm -rf lint.tgt *.o
