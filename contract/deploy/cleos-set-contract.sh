#!/bin/bash

. deploy-danX.conf

source ${EOSIO_ENV}
cleos-a set contract gre12345game ${PWD} gregame.wasm gregame.abi -p gre12345game@active
