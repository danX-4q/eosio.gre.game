
PRJ_DIR = $(shell pwd)
CDT_DIR = "/home/root/docker-workspace/eosio.cdt/"

default:
	cd $(CDT_DIR) && docker-compose exec eosio.cdt bash -c "cd $(PRJ_DIR) && make -f Makefile-inner-docker" || \
	{ echo "eosio.cdt:eosio-cpp error."; exit 1; }

clean:
	cd $(CDT_DIR) && docker-compose exec eosio.cdt bash -c "cd $(PRJ_DIR) && make -f Makefile-inner-docker clean" || \
	{ echo "eosio.cdt:eosio-cpp error."; exit 1; }
