#!/bin/bash

. deploy-danX.conf
source ${EOSIO_ENV}

set -x

function pre() {
    cleos-a transfer eosio master53danx "1000.0000 SYS"
    cleos-a transfer eosio chenxd53danx "1000.0000 SYS"

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
    #转账下注
    cleos-a transfer gre53palyer1 gre12345game "10.0000 SYS" -p gre53palyer1

    #转账退款
    cleos-a transfer gre12345game gre53palyer1 "2.0000 SYS" -p gre12345game
    cleos-a transfer gre12345game gre53palyer1 "1.0000 SYS" -p gre12345game
    
    #合约退款

    #封红包
    cleos-a push action gre12345game c4r4e '["gre53palyer1"]' -p gre53palyer1@active

    #异常转账
    cleos-a transfer master53danx gre12345game "2.0000 SYS" -p master53danx
    cleos-a transfer chenxd53danx gre12345game "2.0000 SYS" -p chenxd53danx
}

function ts_2() {
    #转账下注
    cleos-a transfer gre53palyer1 gre12345game "10.0000 SYS" -p gre53palyer1
    cleos-a transfer gre53palyer2 gre12345game "10.0000 SYS" -p gre53palyer2
    cleos-a transfer gre53palyer3 gre12345game "10.0000 SYS" -p gre53palyer3
    cleos-a transfer gre53palyer4 gre12345game "2.0000 SYS" -p gre53palyer4     #预存但不足额


    #封红包
    cleos-a push action gre12345game c4r4e '["gre53palyer1"]' -p gre53palyer1@active

    #return
    #抢红包
        #预存但不足额
    cleos-a push action gre12345game g4r4e '["gre53palyer4", "8544"]' -p gre53palyer4@active
        #无预存
    cleos-a push action gre12345game g4r4e '["gre53palyer5", "87365"]' -p gre53palyer5@active
        #正常抢完
    cleos-a push action gre12345game g4r4e '["gre53palyer1", "23"]' -p gre53palyer1@active
    cleos-a push action gre12345game g4r4e '["gre53palyer2", "564"]' -p gre53palyer2@active
    cleos-a push action gre12345game g4r4e '["gre53palyer3", "1498"]' -p gre53palyer3@active
        #红包已抢完；状态不对，抛出异常
    cleos-a push action gre12345game g4r4e '["gre53palyer2", "777"]' -p gre53palyer2@active

    #连封第二个红包
    cleos-a push action gre12345game c4r4e '["gre53palyer2"]' -p gre53palyer2@active

    #正常抢完
    cleos-a push action gre12345game g4r4e '["gre53palyer1", "253"]' -p gre53palyer1@active &&
    cleos-a push action gre12345game g4r4e '["gre53palyer2", "567"]' -p gre53palyer2@active &&
    cleos-a push action gre12345game g4r4e '["gre53palyer3", "5678"]' -p gre53palyer3@active
}

pre
#ts_1
ts_2
post
