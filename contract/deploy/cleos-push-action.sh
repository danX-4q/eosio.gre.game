#!/bin/bash

. deploy-danX.conf

source ${EOSIO_ENV}
cleos-a push action gre12345game c4group '["gre12345game"]' -p gre12345game@active
