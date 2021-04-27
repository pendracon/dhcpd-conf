TARGET = dhcpd-conf
CC = gcc
LIBS = -lgdbm -lnewt
OBJS = main.o screen.o help.o data.o
#OPTS = -D__DEBUG__ -D__TRACE__
PREFIX=/usr/sbin

$(TARGET) : $(OBJS)
	$(CC) -o ../$(TARGET) $(LIBS) $(OBJS)

main.o : main.c includes.h main-proto.h screen-proto.h
	$(CC) -c main.c $(OPTS)

screen.o : screen.c includes.h screen-proto.h
	$(CC) -c screen.c $(OPTS)

help.o : help.c includes.h help-proto.h
	$(CC) -c help.c $(OPTS)

data.o : data.c includes.h data-proto.h
	$(CC) -c data.c $(OPTS)

install : $(TARGET)
	install ../$(TARGET) $(PREFIX) 

clean :
	rm -f ../$(TARGET) $(OBJS)
