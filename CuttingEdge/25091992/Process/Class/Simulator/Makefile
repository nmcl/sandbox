MAKE = make

CC  = cc
C++ = CC
CPP = /lib/cpp
LEX = lex

MDEP = /usr/local/Arjuna/bin/makedepend
STRIPDEPEND = /usr/local/Arjuna/bin/stripdepend

CFLAGS   = -g
CPPFLAGS =
C++FLAGS = -g +w
LDFLAGS  = -g
LEXFLAGS =

MDEPFLAGS =

LOCAL_CFLAGS   =
LOCAL_CPPFLAGS = -I/usr/local/Arjuna/include -I.. -I.

LOCAL_C++FLAGS =
LOCAL_LDFLAGS  = -L../ -lThreads \
	-L/usr/lib -lm -llwp

LOCAL_MDEPFLAGS = -I/usr/local/include/CC

OBJECTS	= Display.o List.o Machine.o MachineControl.o OA.o ReplicatedObject.o \
	Transaction.o Simulation.o

SOURCES	= Display.cc List.cc Machine.cc MachineControl.cc OA.cc ReplicatedObject.cc \
	Transaction.cc Simulation.cc

HEADERS	= Display.h List.h Machine.h MachineControl.h OA.h ReplicatedObject.h \
	Transaction.h Common.h

all: Simulation

Simulation: $(OBJECTS) $(HEADERS)
	$(C++) -o Simulation $(OBJECTS) $(LDFLAGS) $(LOCAL_LDFLAGS)

Simulation.o: Simulation.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Simulation.cc

Display.o: Display.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Display.cc

List.o: List.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) List.cc

Machine.o: Machine.cc Machine.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Machine.cc

MachineControl.o: MachineControl.cc MachineControl.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) MachineControl.cc

OA.o: OA.cc OA.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) OA.cc

ReplicatedObject.o: ReplicatedObject.cc ReplicatedObject.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) ReplicatedObject.cc

Transaction.o: Transaction.cc Transaction.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Transaction.cc

clean:
	rm -f *.o

vclean: clean
	rm -f Simulation *~* *.bak

depend:
	$(MDEP) $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(MDEPFLAGS) $(LOCAL_MDEPFLAGS) $(SOURCES)

# DO NOT DELETE THIS LINE -- make depend depends on it.

Display.o: ../Process.h ../common.h ../lwp_thread.h ../thread.h
Display.o: /usr/include/lwp/lwp.h /usr/include/sys/stdtypes.h
Display.o: /usr/include/sys/sysmacros.h /usr/local/include/CC/iostream.h
Display.o: /usr/local/include/CC/memory.h /usr/local/include/CC/sys/time.h
Display.o: /usr/local/include/CC/sys/types.h /usr/local/include/CC/time.h
Display.o: Common.h Display.h List.h Machine.h ReplicatedObject.h
Display.o: Transaction.h
List.o: ../Process.h ../common.h ../lwp_thread.h ../thread.h
List.o: /usr/include/lwp/lwp.h /usr/include/sys/stdtypes.h
List.o: /usr/include/sys/sysmacros.h /usr/local/include/CC/sys/time.h
List.o: /usr/local/include/CC/sys/types.h /usr/local/include/CC/time.h
List.o: Common.h List.h Transaction.h
Machine.o: ../Process.h ../common.h ../lwp_thread.h ../thread.h
Machine.o: /usr/include/lwp/lwp.h /usr/include/sys/stdtypes.h
Machine.o: /usr/include/sys/sysmacros.h /usr/local/include/CC/sys/time.h
Machine.o: /usr/local/include/CC/sys/types.h /usr/local/include/CC/time.h
Machine.o: Common.h List.h Machine.h Transaction.h
MachineControl.o: ../Process.h ../Random.h ../common.h ../lwp_thread.h
MachineControl.o: ../thread.h /usr/include/lwp/lwp.h
MachineControl.o: /usr/include/sys/stdtypes.h /usr/include/sys/sysmacros.h
MachineControl.o: /usr/local/include/CC/iostream.h
MachineControl.o: /usr/local/include/CC/memory.h
MachineControl.o: /usr/local/include/CC/sys/time.h
MachineControl.o: /usr/local/include/CC/sys/types.h
MachineControl.o: /usr/local/include/CC/time.h Common.h List.h Machine.h
MachineControl.o: MachineControl.h Transaction.h
OA.o: ../Process.h ../common.h ../lwp_thread.h ../thread.h
OA.o: /usr/include/lwp/lwp.h /usr/include/sys/stdtypes.h
OA.o: /usr/include/sys/sysmacros.h /usr/local/include/CC/sys/time.h
OA.o: /usr/local/include/CC/sys/types.h /usr/local/include/CC/time.h Common.h
OA.o: List.h OA.h ReplicatedObject.h Transaction.h
ReplicatedObject.o: ../Process.h ../Random.h ../common.h ../lwp_thread.h
ReplicatedObject.o: ../thread.h /usr/include/lwp/lwp.h
ReplicatedObject.o: /usr/include/sys/stdtypes.h /usr/include/sys/sysmacros.h
ReplicatedObject.o: /usr/local/include/CC/iostream.h
ReplicatedObject.o: /usr/local/include/CC/memory.h
ReplicatedObject.o: /usr/local/include/CC/sys/time.h
ReplicatedObject.o: /usr/local/include/CC/sys/types.h
ReplicatedObject.o: /usr/local/include/CC/time.h Common.h List.h Machine.h
ReplicatedObject.o: ReplicatedObject.h Transaction.h
Simulation.o: ../Process.h ../Random.h ../common.h ../lwp_thread.h ../thread.h
Simulation.o: /usr/include/lwp/lwp.h /usr/include/sys/stdtypes.h
Simulation.o: /usr/include/sys/sysmacros.h /usr/local/include/CC/iostream.h
Simulation.o: /usr/local/include/CC/memory.h /usr/local/include/CC/sys/time.h
Simulation.o: /usr/local/include/CC/sys/types.h /usr/local/include/CC/time.h
Simulation.o: Common.h Display.h List.h Machine.h MachineControl.h
Simulation.o: ReplicatedObject.h Transaction.h
Transaction.o: ../Process.h ../Random.h ../common.h ../lwp_thread.h
Transaction.o: ../thread.h /usr/include/lwp/lwp.h /usr/include/sys/stdtypes.h
Transaction.o: /usr/include/sys/sysmacros.h /usr/local/include/CC/iostream.h
Transaction.o: /usr/local/include/CC/memory.h /usr/local/include/CC/sys/time.h
Transaction.o: /usr/local/include/CC/sys/types.h /usr/local/include/CC/time.h
Transaction.o: Common.h List.h OA.h ReplicatedObject.h Transaction.h
