#!/bin/bash

. deploy-danX.conf

scp ../src/*.abi ../src/*.wasm \
    ./deploy-*.conf ./cleos-create-account.sh \
    $SSH_URL
