#ifndef __FILE_GREGAME_HPP__
#define __FILE_GREGAME_HPP__

#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/singleton.hpp>

#include "utility/printkit.hpp"

#include "gregame_conf.hpp"

//actino name:
//  不是标准的c/c++标识符
//  [a-z1-5]{1,13}, 第十三个字符(如果存在)不能>'j'
//  '4', 此处用作分割符
#define AN__CREATE_RED_ENVELOPE    c4r4e
#define AN__GRAB_RED_ENVELOPE      g4r4e
#define AN__TERM_RED_ENVELOPE      t4r4e
#define AN__REFUND                 refund
#define AN__EXTRACT_M              extract2m
#define AN__EXTRACT_P              extract2p

//using namespace eosio;

CONTRACT gregame : public eosio::contract {
    public:
        using contract::contract;
        typedef eosio::name         name;
        typedef eosio::asset        asset;
        typedef eosio::datastream<const char*>   datastream__const_char;
        typedef std::string         string;

        gregame(name receiver, name code,  datastream__const_char ds);
        ACTION AN__CREATE_RED_ENVELOPE(
            name        player
        );

        ACTION AN__GRAB_RED_ENVELOPE(
            name        player,
            uint16_t    re_num_a
        );

        ACTION AN__TERM_RED_ENVELOPE(
            name        game_master
        );

        void transfer(
            name        from,
            name        to,
            asset       quantity,
            string      memo
        );

    private:
        #include "datatypes/gameconf.hpp"       //受限于eosio.cdt的非常规用法
        typedef eosio::singleton<"gameconf"_n, gameconf>        type_table__gameconf;

        #include "datatypes/gameruntime.hpp"    //受限于eosio.cdt的非常规用法
        typedef eosio::singleton<"gameruntime"_n, gameruntime>  type_table__gameruntime;
        
        #include "datatypes/playerbalance.hpp"  //受限于eosio.cdt的非常规用法
        typedef eosio::multi_index<"playerbalance"_n, playerbalance> type_table__playerbalance;

    private:
        void init(type_table__gameconf &tbl_gameconf);

};

#endif
//end-of-file
