#!/bin/bash
if [ ! $# -eq 2 ]
 then
  echo "╔╦╗╔╦╗╔═╗╔═╗╔═╗╦═╗"
  echo " ║║ ║║║ ║╚═╗║╣ ╠╦╝"
  echo "═╩╝═╩╝╚═╝╚═╝╚═╝╩╚═ v1.1 builder"
fi

if [ $# -eq 0 ]
  then
    echo -n "Usage:"
    echo -n $0
    echo " [-h] [debug|release|clean|update] [-n]"
    exit -1
fi

if [ $1 == "-h" ]
 then
  echo -n "Usage:"
  echo -n $0
  echo " [-h] [debug|release|clean|update] [-n]"
  exit -1
fi

if [ $1 = "clean" ]
  then
   echo -n "Cleaning up..."
   if [ ! -f "ddos" ]
    then
      echo "Already cleaned"
      exit
    fi
   rm ddos
   if [ $? -eq 0 ]
     then
      echo "OK"
   fi
   exit
fi
if [ $1 = "debug" ]
  then
   echo "Building debug"
   gcc util.c message.c socket.c ddos.c main.c -lpthread -o ddos
   exit
fi
if [ $1 = "release" ]
  then
   echo "Building release"
   gcc util.c message.c socket.c ddos.c main.c -march=native -Ofast -Os -lpthread -o ddos
   exit
fi
if [ $1 = "update" ]
 then
  echo "Updating..."
  git pull
  ./$0 clean -n
  ./$0 release -n
  exit
fi
echo -n "Unknown option:"
echo $1

