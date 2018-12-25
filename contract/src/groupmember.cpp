#include <math.h>
#include "groupmember.hpp"
#include "utility/printkit.hpp"

using namespace eosio;

groupmember::groupmember(name receiver, name code,  datastream<const char*> ds): 
    contract(receiver, code, ds)
{
    DEBUG_PRINT_VAR(receiver);
    DEBUG_PRINT_VAR(code);
}

ACTION groupmember::ACTION_NAME__JOIN_GROUP2(
    name        grp_name,
    name        grp_creator,
    name        gm_account
)
{
    DEBUG_PRINT_VAR(this->get_self());
    DEBUG_PRINT_VAR(this->_code);
    DEBUG_PRINT_POS();

    DEBUG_PRINT_VAR(gm_account);
    require_auth(this->get_self());
    asset gm_da = asset{int64_t(1.1 * pow(10,CORE_SYMBOL_P)), {CORE_SYMBOL,CORE_SYMBOL_P}}; // equals to 1 EOS

    action(
        permission_level{gm_account, "active"_n},
        "eosio.token"_n, "transfer"_n,
        std::make_tuple(gm_account, name("gre12345game"), gm_da, std::string("some memo here"))
    ).send();

    DEBUG_PRINT_POS();
}


//EOSIO_DISPATCH(groupmember, (ACTION_NAME__JOIN_GROUP2))
//https://developers.eos.io/eosio-home/docs/writing-a-custom-dispatcher
extern "C" {

    struct data_para {
        name        grp_name;
        name        grp_creator;
        name        gm_account;
    };


    void apply(uint64_t receiver, uint64_t code, uint64_t action) {
        DEBUG_PRINT_VAR(name(receiver));
        DEBUG_PRINT_VAR(name(code));
        auto _self = receiver;

        if( code != receiver &&
            code == name("gre12345game").value && 
            action == name("j4group2").value) {
            //execute_action(name(receiver), name(code), &groupmember::ACTION_NAME__JOIN_GROUP2);
            auto data = unpack_action_data<data_para>();

            require_auth(data.gm_account);
            asset gm_da = asset{int64_t(1.1 * pow(10,CORE_SYMBOL_P)), {CORE_SYMBOL,CORE_SYMBOL_P}}; // equals to 1 EOS

            eosio::action(
                permission_level{data.gm_account, "active"_n},
                "eosio.token"_n, "transfer"_n,
                std::make_tuple(data.gm_account, name("gre12345game"), gm_da, std::string("some memo here"))
            ).send();

        }

        if( code == _self ) {
            switch(action) {
                case name("j4group2").value: 
                    execute_action(name(receiver), name(code), &groupmember::ACTION_NAME__JOIN_GROUP2);
                    break;
            }
        }
        DEBUG_PRINT_POS();
    }

}//extern "C"

//end-of-file
