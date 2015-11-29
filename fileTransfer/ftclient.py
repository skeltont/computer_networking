#
#*******************************************************************************************
# Ty Skelton
# skeltont@oregonstate.edu
# CS372
#
# References:
#   How to use argparse:
#       http://stackoverflow.com/a/
#   Non-blocking recv
#       http://stackoverflow.com/questions/16745409/what-does-pythons-socket-recv-return-for-non-blocking-sockets-if-no-data-is-r/16745561#16745561
#*******************************************************************************************
#

import argparse
import socket
import errno
import sys
import os
from time import sleep

def processArgs():
    parser = argparse.ArgumentParser(description=
    'Spin up a request to an ftp server for either a listing of it\'s current directory or a download of one of those files.'
    )
    parser.add_argument('-H', '--host', type=str)
    parser.add_argument('-P', '--port', type=int)
    parser.add_argument('-D', '--data-port', type=int)

    # Add rules so that -l and -g are mutually exclusive
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument('-l', '--list-directory', action='store_true')
    group.add_argument('-g', '--get-file', type=str)

    args = parser.parse_args()
    print args

    return args

def setupControl(args):
    # Initialize control socket
    print "> Attempting to connect to control socket"
    c = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    c.connect((args.host, args.port))
    c.settimeout(2)
    print "> Connected to control socket"
    return c

def setupData(c, args):
    d = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    d.bind((args.host, args.data_port))
    d.listen(1)

    if args.list_directory == True:
        print "> Sending LIST command"
        c.send(str(args.data_port) + ";" + str("-l"))
    else:
        print "> Sending GET command"
        c.send(str(args.data_port) + ";" + str("-g") + ";" + str(args.get_file))


    return d


def handleRequest(c, args):
    try:
        msg = c.recv(4096)
        print msg
    except socket.timeout as e:
        print "> Valid Command, attempting to connect to data socket"

        d = setupData(c, args)
        print "> Data socket created"

        conn, addr = d.accept()
        print "> Data socket accepted"

        if args.list_directory == True:
            try:
                msg = conn.recv(4096)
                print "Receiving directory structure from " + args.host + ":" + str(args.data_port)
                print msg
            except socket.error as e:
                print e
        else:
            try:
                msg = conn.recv(4096)
                if os.path.isfile(args.get_file):
                    f = open(args.get_file+".copy", 'w+')
                else:
                    f = open(args.get_file, 'w+')
                f.write(msg)
                f.close()
            except socket.error as e:
                print e

        conn.close()

def main():
    # Parse CLI arguments
    args = processArgs()

    # Send request to server
    c = setupControl(args)

    # Handle response
    handleRequest(c, args)

main()
