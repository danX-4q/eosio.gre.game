#!/bin/bash

. deploy-danX.conf

chmod +x *.sh

scp ../src/*.abi ../src/*.wasm \
    ./deploy-*.conf ./cleos-*.sh \
    $SSH_URL
