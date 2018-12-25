#ifndef __FILE_GROUPMEMBER_HPP__
#define __FILE_GROUPMEMBER_HPP__


#define ACTION_NAME__JOIN_GROUP2  j4group2

#define CORE_SYMBOL "SYS"
#define CORE_SYMBOL_P 4

#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/singleton.hpp>
using namespace eosio;

CONTRACT groupmember : public eosio::contract {
    public:
        using contract::contract;

        groupmember(name receiver, name code,  datastream<const char*> ds);

        ACTION ACTION_NAME__JOIN_GROUP2(
            name        grp_name,
            name        grp_creator,
            name        gm_account
        );

    private:

};

#endif
//end-of-file
