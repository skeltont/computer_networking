#
#*******************************************************************************************
# Ty Skelton
# skeltont@oregonstate.edu
# CS372
# *******************************************************************************************
#

CC = gcc
O = -pthread

all: ftserver.c
	$(CC) -Wall ftserver.c $(O) -o ftserver

chatclient:

ftserver:
	$(CC) ftserver.c $(O) -o ftserver

clean:
	rm -rf ftserver
