#!/usr/bin/env bash

ABS_FILE=$(readlink -m "$0")
DIR=$(dirname $ABS_FILE)
echo $DIR
echo $ABS_FILE
cd $DIR
node  index.js
