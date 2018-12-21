#!/bin/bash

. deploy-danX.conf

source ${EOSIO_ENV}
cleos-a create account eosio gre12345game ${K6_PUB} -p eosio@active
cleos-a create account eosio chenxd53danx ${K6_PUB} -p eosio@active
