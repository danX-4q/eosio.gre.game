#!/bin/bash

. deploy-danX.conf
source ${EOSIO_ENV}

function ts_1() {
    cleos-a push action gre12345game c4group '["1.gre.game", "gre12345game", "topic", "desc", 2, 100, "1.0000 SYS", "5.0000 SYS", 1]' -p gre12345game@active
    cleos-a push action gre12345game c4group '["2.gre.game", "chenxd53danx", "topic", "desc", 2, 100, "5.0000 SYS", "10.0000 SYS", 2]' -p chenxd53danx@active
    cleos-a push action gre12345game c4group '["3.gre.game", "chenxd53danx", "topic", "desc", 2, 100, "10.0000 SYS", "15.0000 SYS", 3]' -p chenxd53danx@active

    #权限校验将失败
    #cleos-a push action gre12345game c4group '["3.gre.game", "chenxd53danx", "topic", "002-desc", 2, 100, "5.0000 SYS", "10.0000 SYS", 3]' -p gre12345game@active
}

function ts_2() {
    cleos-a push action gre12345game c4group '["1.gre.game", "gre12345game", "topic", "desc", 2, 100, "1.0000 SYS", "5.0000 SYS", 1]' -p gre12345game@active
    cleos-a push action gre12345game c4group '["2.gre.game", "chenxd53danx", "topic", "desc", 2, 100, "5.0000 SYS", "10.0000 SYS", 2]' -p chenxd53danx@active

    #prepare for j4group
    cleos-a set account permission chenxd53danx active '{"threshold": 1,"keys": [{"key": "'${K6_PUB}'","weight": 1}],"accounts": [{"permission":{"actor":"gre12345game","permission":"eosio.code"},"weight":1}]}' owner -p chenxd53danx
    cleos-a transfer eosio chenxd53danx "1000.0000 SYS"
    
    #success
    cleos-a push action gre12345game j4group '["1.gre.game", "gre12345game", "chenxd53danx"]' -p chenxd53danx@active
    #failed
    cleos-a push action gre12345game j4group '["1.gre.game", "gre54321game", "chenxd53danx"]' -p chenxd53danx@active
}

ts_2


CMD='cleos-a get table gre12345game gre12345game gameconf'
echo '******************************' && echo "$CMD"
cleos-a get table gre12345game gre12345game gameconf

CMD='cleos-a get table gre12345game gre12345game group'
echo '******************************' && echo "$CMD"
cleos-a get table gre12345game gre12345game group

CMD='cleos-a get table gre12345game chenxd53danx group'
echo '******************************' && echo "$CMD"
cleos-a get table gre12345game chenxd53danx group

CMD='cleos-a get table gre12345game chenxd53danx groupmember'
echo '******************************' && echo "$CMD"
cleos-a get table gre12345game chenxd53danx groupmember

CMD='cleos-a get currency balance eosio.token chenxd53danx'
echo '******************************' && echo "$CMD"
cleos-a get currency balance eosio.token chenxd53danx

CMD='cleos-a get currency balance eosio.token gre12345gam'
echo '******************************' && echo "$CMD"
cleos-a get currency balance eosio.token gre12345game
