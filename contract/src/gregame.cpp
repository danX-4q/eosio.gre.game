#include "gregame.hpp"
using namespace eosio;

ACTION gregame::ACTION_NAME__CREATE_GROUP(name user) {
   print_f("%s %s %d: Hello % from hello", __FILE__, __FUNCTION__, __LINE__, user);
}

EOSIO_DISPATCH(gregame, (ACTION_NAME__CREATE_GROUP))
//end-of-filed
