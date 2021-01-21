#!/bin/sh
HOST=$ULTIMATE_HOSTNAME
USER='guest'
PASSWD='guest'
SRC_FILE_PATH=$1
SRC_FILE_NAME="${SRC_FILE_PATH##*/}"
DEST_PATH=$ULTIMATE_DEST_PATH

ftp -n $HOST <<END_SCRIPT
quote USER $USER
quote PASS $PASSWD
binary
cd $DEST_PATH
del $SRC_FILE_NAME
put $SRC_FILE_PATH ./$SRC_FILE_NAME
quit
END_SCRIPT
exit 0