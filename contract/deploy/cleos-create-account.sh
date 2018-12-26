#!/bin/bash

. deploy-danX.conf

source ${EOSIO_ENV}
cleos-a create account eosio gre12345game ${K6_PUB} -p eosio@active
cleos-a create account eosio master53danx ${K6_PUB} -p eosio@active
cleos-a create account eosio chenxd53danx ${K6_PUB} -p eosio@active
cleos-a create account eosio gre53palyer1 ${K6_PUB} -p eosio@active
cleos-a create account eosio gre53palyer2 ${K6_PUB} -p eosio@active
cleos-a create account eosio gre53palyer3 ${K6_PUB} -p eosio@active
cleos-a create account eosio gre53palyer4 ${K6_PUB} -p eosio@active
cleos-a create account eosio gre53palyer5 ${K6_PUB} -p eosio@active
