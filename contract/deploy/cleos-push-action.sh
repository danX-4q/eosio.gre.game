#!/bin/bash

. deploy-danX.conf

source ${EOSIO_ENV}
cleos-a push action gre12345game c4group '["1.gre.game", "gre12345game", "topic", "desc", 2, 100, "1.0000 SYS", "5.0000 SYS", 1]' -p gre12345game@active
cleos-a push action gre12345game c4group '["2.gre.game", "chenxd53danx", "topic", "desc", 2, 100, "5.0000 SYS", "10.0000 SYS", 2]' -p chenxd53danx@active
cleos-a push action gre12345game c4group '["3.gre.game", "chenxd53danx", "topic", "desc", 2, 100, "10.0000 SYS", "15.0000 SYS", 3]' -p chenxd53danx@active

#权限校验将失败
#cleos-a push action gre12345game c4group '["3.gre.game", "chenxd53danx", "topic", "002-desc", 2, 100, "5.0000 SYS", "10.0000 SYS", 3]' -p gre12345game@active

echo '******************************'
cleos-a get table gre12345game gre12345game gameconf
echo '******************************'
cleos-a get table gre12345game gre12345game group
echo '******************************'
cleos-a get table gre12345game chenxd53danx group
#cleos-a get table gre12345game global group