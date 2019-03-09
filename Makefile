# Makefile for cust_seek plugin
ifeq ($(OS),Windows_NT)
    SUFFIX = dll
else
    SUFFIX = so
endif

CC=gcc
CXX=g++
STD=gnu99
CFLAGS+=-fPIC -I /usr/local/include -Wall
CXXFLAGS+=-fPIC -I /usr/local/include -Wall
ifeq ($(DEBUG),1)
CFLAGS +=-g -O0
CXXFLAGS +=-g -O0
endif

PREFIX=/usr/local/lib/deadbeef
PLUGNAME=cust_seek
LIBS=

all: cust_seek

cust_seek:
	$(CC) -std=$(STD) -c $(CFLAGS) -c $(PLUGNAME).c
	$(CC) -std=$(STD) -shared $(CXXFLAGS) -o $(PLUGNAME).$(SUFFIX) $(PLUGNAME).o $(LIBS) $(LDFLAGS)

install:
	cp $(PLUGNAME).$(SUFFIX) $(PREFIX)

clean:
	rm -fv $(PLUGNAME).o $(PLUGNAME).$(SUFFIX)
