#
#*******************************************************************************************
# Ty Skelton
# skeltont@oregonstate.edu
# CS372
# *******************************************************************************************
#

CC = gcc
CPP = g++

all: chatclient.c chatserve.cpp
	$(CC)  chatclient.c -o chatclient
	$(CPP) chatserve.cpp -o chatserve

chatclient:
	$(CC)  chatclient.c -o chatclient

chatserve:
	$(CPP) chatserve.cpp -o chatserve

clean:
	rm -rf chatclient
	rm -rf chatserve
