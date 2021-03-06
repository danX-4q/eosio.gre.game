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
            uint16_t    re_play_num
        );

        ACTION AN__TERM_RED_ENVELOPE(
            name        game_master
        );

        void rcpnt_transfer(
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
        
        #include "datatypes/playbalance.hpp"    //受限于eosio.cdt的非常规用法
        typedef eosio::multi_index<"playbalance"_n, playbalance> type_table__playbalance;

        #include "datatypes/redenvelope.hpp"    //受限于eosio.cdt的非常规用法
        typedef eosio::multi_index<"redenvelope"_n, redenvelope, 
            eosio::indexed_by<"redenvelope"_n, eosio::const_mem_fun<redenvelope, uint64_t, &redenvelope::get_re_id>>
        > type_table__redenvelope;

        #include "datatypes/grabre.hpp"         //受限于eosio.cdt的非常规用法
        typedef eosio::multi_index<"grabre"_n, grabre, 
            eosio::indexed_by<"reid"_n, eosio::const_mem_fun<grabre, uint64_t, &grabre::get_re_id>>
        > type_table__grabre;

    private:
        void init(type_table__gameconf &tbl_gameconf);

        void rcpnt_transfer_to_p(
            name        from,
            name        to,
            asset       quantity,
            string      memo
        );

        void rcpnt_transfer_to_m(
            name        from,
            name        to,
            asset       quantity,
            string      memo
        );

        void rcpnt_transfer_to_player(
            name        from,
            name        to,
            asset       quantity,
            string      memo
        );

        void rcpnt_transfer_to_myself(
            name        from,
            name        to,
            asset       quantity,
            string      memo
        );
};

#endif
//end-of-file
