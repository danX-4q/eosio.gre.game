#include <math.h>
#include "gregame.hpp"
#include "utility/eosio_misc.hpp"

using namespace eosio;

#define M_ACCOUNT ("master53danx"_n)
#define P_ACCOUNT ("chenxd53danx"_n)

gregame::gregame(name receiver, name code,  datastream<const char*> ds): 
    contract(receiver, code, ds)
{
    DEBUG_PRINT_VAR(receiver);
    DEBUG_PRINT_VAR(code);
    DEBUG_PRINT_VAR(this->get_self());

    bool    is_inited(false);
    type_table__gameconf    tbl_gameconf(receiver, receiver.value); //support: action+recipent
    is_inited = tbl_gameconf.exists();
    if (!is_inited) {
        //首次，需初始化
        init(tbl_gameconf);
    }
}

void gregame::init(type_table__gameconf &tbl_gameconf)
{
    gameconf    dlt_gc{
        .game_magic = 0x64616e58, //hex: danX
        .game_re_amount = {int64_t(4 * pow(10,CORE_SYMBOL_P)), {CORE_SYMBOL,CORE_SYMBOL_P}},
        .game_re_split = 3,
        .game_m_cmsn = {int64_t(0.4 * pow(10,CORE_SYMBOL_P)), {CORE_SYMBOL,CORE_SYMBOL_P}},
        .game_p_cmsn = {int64_t(0.6 * pow(10,CORE_SYMBOL_P)), {CORE_SYMBOL,CORE_SYMBOL_P}},
        .game_m = M_ACCOUNT,
        .game_p = P_ACCOUNT,
    };
    dlt_gc.print();
    tbl_gameconf.set(dlt_gc, this->get_self());

    ////////////////////////////////////////////////////////

    type_table__gameruntime     tbl_gameruntime(this->get_self(), this->get_self().value);
    gameruntime dlt_grt{
        .grt_game_id = 0,
        .grt_nr_player = 0,
        .grt_game_state = 0,
        .grt_m_total = {0, {CORE_SYMBOL,CORE_SYMBOL_P}},
        .grt_m_refund = {0, {CORE_SYMBOL,CORE_SYMBOL_P}},
        .grt_p_total = {0, {CORE_SYMBOL,CORE_SYMBOL_P}},
        .grt_p_refund = {0, {CORE_SYMBOL,CORE_SYMBOL_P}},
    };
    dlt_grt.print();
    tbl_gameruntime.set(dlt_grt, this->get_self());

}

void gregame::rcpnt_transfer(
    name        from,
    name        to,
    asset       quantity,
    string      memo
)
{
    DEBUG_PRINT_POS();
    DEBUG_PRINT_VAR(from);
    DEBUG_PRINT_VAR(to);
    DEBUG_PRINT_VAR(quantity);

    //主要目的：确保是CORE_SYMBOL资产
    EOSIO_ASSERT_EX((quantity > asset{0, {CORE_SYMBOL,CORE_SYMBOL_P}}));

    if (to == this->get_self()) {
        this->rcpnt_transfer_to_myself(from, to, quantity, memo);

    } else if (from == this->get_self()) {
        //转出款，限制：
        //1. 到平台账户
        //2. 到组织者账户
        //3. 到player账户
        //不允许当前合约账户把资金转到其它账户！！！

        //为什么在这里，而不在refund更新余额？
        //因为它们最后才会进入这个函数


        //require_auth(from);

        if (to == P_ACCOUNT) {
            //转到平台: 提成
            this->rcpnt_transfer_to_p(from, to, quantity, memo);
        } else if (to == M_ACCOUNT) {
            //转到组织者：提成
            this->rcpnt_transfer_to_m(from, to, quantity, memo);
        } else {
            //转到玩家：预存的金额+抢红包的所得
            this->rcpnt_transfer_to_player(from, to, quantity, memo);
        }
    }
}

void gregame::rcpnt_transfer_to_myself(
    name        from,
    name        to,
    asset       quantity,
    string      memo
)
{
    //合约账户不能接收来自组织者及平台账户的资金
    EOSIO_ASSERT_EX(from != P_ACCOUNT);
    EOSIO_ASSERT_EX(from != M_ACCOUNT);

    //收到款
    type_table__playbalance   tbl_playbalance(this->get_self(), this->get_self().value);
    auto itr = tbl_playbalance.find(from.value);
    if (itr == tbl_playbalance.end()) {
        //首次转账
        tbl_playbalance.emplace(this->get_self(), [&]( auto& row ) {
            row.player = from;
            row.balance = quantity;
        });
    } else {
        //非首次转账
        tbl_playbalance.modify(itr, this->get_self(), [&]( auto& row ) {
            row.player = from;
            row.balance = itr->balance + quantity;
        });            
    }
}

void gregame::rcpnt_transfer_to_p(
    name        from,
    name        to,
    asset       quantity,
    string      memo
)
{
    type_table__gameruntime     tbl_gameruntime(this->get_self(), this->get_self().value);
    gameruntime grt = tbl_gameruntime.get();
    EOSIO_ASSERT_EX(quantity + grt.grt_p_refund <= grt.grt_p_total);
    grt.grt_p_refund += quantity;
    tbl_gameruntime.set(grt, this->get_self());
}

void gregame::rcpnt_transfer_to_m(
    name        from,
    name        to,
    asset       quantity,
    string      memo
)
{
    type_table__gameruntime     tbl_gameruntime(this->get_self(), this->get_self().value);
    gameruntime grt = tbl_gameruntime.get();
    EOSIO_ASSERT_EX(quantity + grt.grt_m_refund <= grt.grt_m_total);
    grt.grt_m_refund += quantity;
    tbl_gameruntime.set(grt, this->get_self());
}

void gregame::rcpnt_transfer_to_player(
    name        from,
    name        to,
    asset       quantity,
    string      memo
)
{
    type_table__playbalance   tbl_playbalance(this->get_self(), this->get_self().value);
    auto itr = tbl_playbalance.find(to.value);
    EOSIO_ASSERT_EX(itr != tbl_playbalance.end());
    EOSIO_ASSERT_EX(itr->balance >= quantity);

    tbl_playbalance.modify(itr, this->get_self(), [&]( auto& row ) {
        row.player = to;
        row.balance = itr->balance - quantity;
    });
}

ACTION gregame::AN__CREATE_RED_ENVELOPE(
    name        player
)
{
    require_auth(player);

    type_table__gameconf        tbl_gameconf(this->get_self(), this->get_self().value);
    gameconf    gconf = tbl_gameconf.get();
    
    type_table__gameruntime     tbl_gameruntime(this->get_self(), this->get_self().value);
    gameruntime grt = tbl_gameruntime.get();
    EOSIO_ASSERT_EX(grt.grt_game_state == 0);

    type_table__playbalance     tbl_playbalance(this->get_self(), this->get_self().value);
    auto itr = tbl_playbalance.find(player.value);
    EOSIO_ASSERT_EX(itr != tbl_playbalance.end());
    EOSIO_ASSERT_EX(itr->balance >= gconf.game_re_amount);
    tbl_playbalance.modify(itr, this->get_self(), [&]( auto& row ) {
        row.player = player;
        row.balance = itr->balance - gconf.game_re_amount;
    });

    type_table__redenvelope     tbl_redenvelope(this->get_self(), this->get_self().value);
    tbl_redenvelope.emplace(this->get_self(), [&]( auto& row ) {
        row.id = tbl_redenvelope.available_primary_key();
        row.re_id = grt.grt_game_id;
        row.re_creator = player;
    });

    grt.grt_m_total += gconf.game_m_cmsn;
    grt.grt_p_total += gconf.game_p_cmsn;
    grt.grt_game_state = 1;
    tbl_gameruntime.set(grt, this->get_self());
}

ACTION gregame::AN__GRAB_RED_ENVELOPE(
    name        player,
    uint16_t    re_num
)
{
    require_auth(player);

    type_table__gameruntime     tbl_gameruntime(this->get_self(), this->get_self().value);
    gameruntime grt = tbl_gameruntime.get();
    EOSIO_ASSERT_EX(grt.grt_game_state == 1);

    type_table__gameconf        tbl_gameconf(this->get_self(), this->get_self().value);
    gameconf    gconf = tbl_gameconf.get();
    
    type_table__playbalance     tbl_playbalance(this->get_self(), this->get_self().value);
    auto itr = tbl_playbalance.find(player.value);
    EOSIO_ASSERT_EX(itr != tbl_playbalance.end());
    EOSIO_ASSERT_EX(itr->balance >= gconf.game_re_amount);

    type_table__grabre          tbl_grabre(this->get_self(), this->get_self().value);
    auto reid_index = tbl_grabre.get_index<"reid"_n>();
    auto itr_a = reid_index.lower_bound(grt.grt_game_id);
    for(; itr_a!= reid_index.end(); ++itr_a){
        EOSIO_ASSERT_EX(itr_a->re_player != player);
    }
    tbl_grabre.emplace(this->get_self(), [&]( auto& row ) {
        row.id = tbl_grabre.available_primary_key();
        row.re_id = grt.grt_game_id;
        row.re_pos = grt.grt_nr_player;
        row.re_player = player;
        row.re_num = re_num;
    });
    grt.grt_nr_player += 1;
    if (grt.grt_nr_player == gconf.game_re_split) {
        grt.grt_game_state = 2;

        //最后一个红包被抢后，合约自动入账
        //1. 分红包
        #warning "示例程序，使用简单的算法而已"
        itr_a = reid_index.lower_bound(grt.grt_game_id);
        uint64_t    salt = 0;
        uint128_t   total = 0;
        std::vector<uint64_t> salt_num;
        for(; itr_a!= reid_index.end(); ++itr_a){
            uint64_t H32 = (salt & 0x00ffffff00000000UL) >> 32; //其实只留了24位
            uint64_t L32 = salt & 0x00000000ffffffffUL;
            salt = (L32 << 32) | H32;
            salt ^= itr_a->re_player.value;
            salt |= itr_a->re_num;
            if(salt == 0) { salt = itr_a->re_player.value; }
            total += salt;
            salt_num.push_back(salt);
        }

        std::vector<uint64_t> re_amount;
        asset   left = gconf.game_re_amount - gconf.game_m_cmsn - gconf.game_p_cmsn;
        itr_a = reid_index.lower_bound(grt.grt_game_id);
        for(size_t i = 0; i < salt_num.size() - 1; ++i, ++itr_a){
            int64_t amount = int64_t(double(salt_num[i]) / total);
            EOSIO_ASSERT_EX(left.amount > amount);
            left.amount -= amount;

            auto itr = tbl_playbalance.find(itr_a->re_player.value);
            EOSIO_ASSERT_EX(itr != tbl_playbalance.end());
            tbl_playbalance.modify(itr, this->get_self(), [&]( auto& row ) {
                row.player = itr_a->re_player;
                row.balance = itr->balance + asset{amount, {CORE_SYMBOL,CORE_SYMBOL_P}};
            });
        }
        EOSIO_ASSERT_EX(left.amount > 0);
        auto itr = tbl_playbalance.find(itr_a->re_player.value);
        EOSIO_ASSERT_EX(itr != tbl_playbalance.end());
        tbl_playbalance.modify(itr, this->get_self(), [&]( auto& row ) {
            row.player = itr_a->re_player;
            row.balance = itr->balance + left;
        });

        //2. 重置状态
        grt.grt_game_id += 1;
        grt.grt_nr_player = 0;
        grt.grt_game_state = 0;
    }
    tbl_gameruntime.set(grt, this->get_self());
}

extern "C" {
    bool recipient_hook(uint64_t receiver, uint64_t code, uint64_t action)
    {
        if( code != receiver &&
            code == name("eosio.token").value && 
            action == name("transfer").value) {
            execute_action(name(receiver), name(code), &gregame::rcpnt_transfer);
            return (false);
        } else {
            return (true);
        }
    }
}//extern "C"

EOSIO_DISPATCH_RHOOK(gregame, (AN__CREATE_RED_ENVELOPE))

//end-of-file
