CC = gcc
RM = rm

LDFLAGS= -lm

TARGET = month.cgi

OBJS = \
	cal_calculation.o  \
	month.o

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)


all : 
	make

clean :
	$(RM) $(OBJS) $(TARGET)

