#!/bin/bash
if [ ! $# -eq 2 ] #FIXME:normal way of chceking whether -n argument used
 then
  echo "╔╦╗╔╦╗╔═╗╔═╗╔═╗╦═╗"
  echo " ║║ ║║║ ║╚═╗║╣ ╠╦╝"
  echo "═╩╝═╩╝╚═╝╚═╝╚═╝╩╚═ v1.2 builder"
fi

if [ $# -eq 0 ]
  then
    echo -n "Usage:"
    echo -n $0
    echo " [-h] [debug|release|clean|update] [-n]"
    echo " -h - display this message"
    echo " -n - do not display logo"
    echo " debug - build for debug(no optimization)"
    echo " release - build for release(maximum optimization)"
    echo " clean - delete old build"
    echo " update - try automatically download new version and build it for release"
    exit -1
fi

if [ $1 == "-h" ]
 then
  echo -n "Usage:"
  echo -n $0
  echo " [-h] [debug|release|clean|update] [-n]"
  echo " -h - display this message"
  echo " -n - do not display logo"
  echo " debug - build for debug(no optimization)"
  echo " release - build for release(maximum optimization)"
  echo " clean - delete old build"
  echo " update - try automatically download new version and build it for release"
  exit 0
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
   gcc packet.c memcrashed.c ping.c util.c message.c socket.c ddos.c main.c -v -lm -lpthread -o ddos
   exit
fi
if [ $1 = "release" ]
  then
   echo "Building release"
   gcc packet.c ping.c util.c message.c socket.c ddos.c main.c -lm -Ofast -Os -lpthread -o ddos
   exit
fi
if [ $1 = "update" ]
 then
  echo "Updating..."
  git pull
  if [ $? != 0 ]
   then
    echo "+----------------|ERROR!!!|----------------+"
    echo "|git pull failed!                          |"
    echo "|It seems that you need to update manually |"
    echo "|To update manually clone repository again |"
    echo "|(using git clone) and build (using this   |"
    echo "|script)                                   |"
    echo "+------------------------------------------+"
    exit -1
  fi
  ./$0 clean -n
  ./$0 release -n
  exit
fi
echo -n "Unknown option:"
echo $1

