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

using namespace eosio;

CONTRACT gregame : public eosio::contract {
    public:
        using contract::contract;

        ACTION ACTION_NAME__CREATE_GROUP(name user);
};

#endif
//end-of-filed
