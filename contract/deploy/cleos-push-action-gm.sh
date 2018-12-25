#!/bin/bash

. deploy-danX.conf
source ${EOSIO_ENV}


function ts_2() {
    cleos-a push action gre12345game c4group '["1.gre.game", "gre12345game", "topic", "desc", 2, 100, "1.0000 SYS", "5.0000 SYS", 1]' -p gre12345game@active
    cleos-a push action gre12345game c4group '["2.gre.game", "chenxd53danx", "topic", "desc", 2, 100, "5.0000 SYS", "10.0000 SYS", 2]' -p chenxd53danx@active

    #prepare for j4group
    cleos-a transfer eosio chenxd53danx "1000.0000 SYS"
    #重要！需要给自己开放eosio.code权限
    cleos-a set account permission chenxd53danx active '{"threshold": 1,"keys": [{"key": "'${K6_PUB}'","weight": 1}],"accounts": [{"permission":{"actor":"chenxd53danx","permission":"eosio.code"},"weight":1}]}' owner -p chenxd53danx
    cleos-a set contract chenxd53danx ${PWD} groupmember.wasm groupmember.abi -p chenxd53danx@active

    #success
    cleos-a push action gre12345game j4group2 '["1.gre.game", "gre12345game", "chenxd53danx"]' -p chenxd53danx@active
}

ts_2


CMD='cleos-a get table gre12345game gre12345game gameconf'
echo '******************************' && echo "$CMD"
cleos-a get table gre12345game gre12345game gameconf

CMD='cleos-a get table gre12345game global group'
echo '******************************' && echo "$CMD"
cleos-a get table gre12345game global group

CMD='cleos-a get table gre12345game global groupmember'
echo '******************************' && echo "$CMD"
cleos-a get table gre12345game global groupmember

CMD='cleos-a get currency balance eosio.token chenxd53danx'
echo '******************************' && echo "$CMD"
cleos-a get currency balance eosio.token chenxd53danx

CMD='cleos-a get currency balance eosio.token gre12345gam'
echo '******************************' && echo "$CMD"
cleos-a get currency balance eosio.token gre12345game
