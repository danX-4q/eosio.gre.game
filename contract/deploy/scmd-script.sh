#!/bin/bash

. deploy-danX.conf

sh_file="$1"

ssh ${R_USER}@${R_HOST} -t "cd ${R_DIR} && ./${sh_file} || { exit 1; }"
