#ifndef __FILE_GREGAME_HPP__
#define __FILE_GREGAME_HPP__

//actino name:
//  不是标准的c/c++标识符
//  [a-z1-5]{1,13}, 第十三个字符(如果存在)不能>'j'
//  '4', 此处用作分割符
#define ACTION_NAME__CREATE_GROUP c4group
#define ACTION_NAME__JOIN_GROUP   j4group
#define ACTION_NAME__CREATE_R_E   c4re
#define ACTION_NAME__GRAB_R_E     g4re
#define ACTION_NAME__LEAVE_GROUP  l4group
#define ACTION_NAME__DELETE_GROUP d4group

#include <eosiolib/eosio.hpp>
#include<eosiolib/singleton.hpp>
using namespace eosio;

CONTRACT gregame : public eosio::contract {
    public:
        using contract::contract;

        gregame(name receiver, name code,  datastream<const char*> ds);
        ACTION ACTION_NAME__CREATE_GROUP(name user);

    private:
        #include "datatypes/gameconf.hpp"       //受限于eosio.cdt的非常规用法
        #include "datatypes/group.hpp"          //受限于eosio.cdt的非常规用法
        typedef eosio::multi_index<"group"_n, group> type_table__group;
        //typedef eosio::multi_index<"gameconf"_n, gameconf> type_table__gameconf;

        typedef eosio::singleton<"gameconf"_n, gameconf> type_table__gameconf;
        type_table__gameconf    tbl_gameconf;

    private:
        void init();


};

#endif
//end-of-file
