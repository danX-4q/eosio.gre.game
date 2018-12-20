#include "gregame.hpp"
using namespace eosio;

gregame::gregame(name receiver, name code,  datastream<const char*> ds): 
    contract(receiver, code, ds),
    tbl_gameconf(code, code.value)
{
    bool        is_inited(false);
    is_inited = tbl_gameconf.exists();
    if (!is_inited) {
        //首次，需初始化
        init();
    }
}

void gregame::init()
{
    gameconf    dlt_gc{
        0x64616e58, 
        5, 1000,
        1*1000, 100*1000,   //*1000: 调整单位为小数点后4位
        5
    };
    dlt_gc.print();
    tbl_gameconf.set(dlt_gc, _code);
}

ACTION gregame::ACTION_NAME__CREATE_GROUP(name user) {
   print_f("%s %s %d: Hello % from hello", __FILE__, __FUNCTION__, __LINE__, user);
}

EOSIO_DISPATCH(gregame, (ACTION_NAME__CREATE_GROUP))
//end-of-file
