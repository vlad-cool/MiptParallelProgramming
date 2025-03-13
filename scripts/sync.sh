#!/bin/sh

SCRIPT_DIR=$(realpath $(dirname $0))
source $SCRIPT_DIR/ssh_info

cd $SCRIPT_DIR/..

rsync -a -r tasks Makefile --rsh=ssh\ -p\ $CLUSTER_SSH_PORT $CLUSTER_SSH_LOGIN@$CLUSTER_ADDRESS:parallel_programming
