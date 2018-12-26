#!/bin/bash

. deploy-danX.conf
source ${EOSIO_ENV}

set -x

function pre() {
    cleos-a transfer eosio gre53palyer1 "1000.0000 SYS"
    cleos-a transfer eosio gre53palyer2 "1000.0000 SYS"
    cleos-a transfer eosio gre53palyer3 "1000.0000 SYS"
    cleos-a transfer eosio gre53palyer4 "1000.0000 SYS"
    cleos-a transfer eosio gre53palyer5 "1000.0000 SYS"

    cleos-a set account permission gre12345game active '{"threshold": 1,"keys": [{"key": "'${K6_PUB}'","weight": 1}],"accounts": [{"permission":{"actor":"gre12345game","permission":"eosio.code"},"weight":1}]}' owner -p gre12345game
}

function post() {
    :
}

function ts_1() {
    cleos-a transfer gre53palyer1 gre12345game "1.0000 SYS" -p gre53palyer1
    cleos-a push action gre12345game c4r4e '["gre53palyer1"]' -p gre53palyer1@active
}


pre
ts_1
post
