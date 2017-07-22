MAKE = make

CC  = cc
C++ = CC3.0.1
CPP = /lib/cpp
LEX = lex

MDEP =
STRIPDEPEND = 

CFLAGS   =
CPPFLAGS =
C++FLAGS = -g +w
LDFLAGS  = -g
LEXFLAGS =

MDEPFLAGS =

LOCAL_CFLAGS   =
LOCAL_CPPFLAGS = -I/usr/local/InterViews3.1/include \
		 -I/usr/local/InterViews3.1/IV-look \
		 -I..

LOCAL_C++FLAGS =
LOCAL_LDFLAGS  = -L/usr/local/InterViews3.1/lib/SUN4 -L/usr/local/X11R5/lib -L/usr/local/lib \
                 -lIV -lX11 -lXext -lm

LOCAL_MDEPFLAGS = -I/usr/local/CC/3.0.1/include

OBJECTS	= MonitorDisplay.o Node.o NodeData.o DepList.o MainSelect.o SubSelect.o

SOURCES	= MonitorDisplay.cc Node.cc NodeData.cc DepList.cc MainSelect.cc SubSelect.cc

HEADERS	= Node.h NodeData.h DepList.h MainSelect.h Common.h SubSelect.h

all: MonitorDisplay

MonitorDisplay: $(OBJECTS) $(HEADERS)
	$(C++) -o MonitorDisplay $(OBJECTS) $(LDFLAGS) $(LOCAL_LDFLAGS)

MonitorDisplay.o: MonitorDisplay.cc
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) MonitorDisplay.cc

Node.o: Node.cc Node.h NodeData.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) Node.cc

MainSelect.o: MainSelect.cc MainSelect.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) MainSelect.cc

SubSelect.o: SubSelect.cc SubSelect.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) SubSelect.cc

NodeData.o: NodeData.cc NodeData.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) NodeData.cc

DepList.o: DepList.cc DepList.h
	$(C++) -c $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(C++FLAGS) $(LOCAL_C++FLAGS) DepList.cc

clean:
	rm -f *.o

vclean: clean
	rm -f MonitorDisplay *~*
