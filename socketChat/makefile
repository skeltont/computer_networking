#
#*******************************************************************************************
# Ty Skelton
# skeltont@oregonstate.edu
# CS372
# *******************************************************************************************
#

CC = gcc
CPP = g++
O = -pthread

all: chatclient.c chatserve.cpp
	$(CC)  chatclient.c $(O) -o chatclient
	$(CPP) chatserve.cpp $(O) -o chatserve

chatclient:
	$(CC)  chatclient.c $(O) -o chatclient

chatserve:
	$(CPP) chatserve.cpp $(O) -o chatserve

clean:
	rm -rf chatclient
	rm -rf chatserve
