#!/bin/bash

. deploy-danX.conf
source ${EOSIO_ENV}

set -x

echo '******************************'
cleos-a get table gre12345game gre12345game gameconf

echo '******************************'
cleos-a get table gre12345game gre12345game gameruntime

echo '******************************'
cleos-a get table gre12345game gre12345game playerbalance

#echo '******************************'
#cleos-a get currency balance eosio.token gre12345game
