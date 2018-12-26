#include <math.h>
#include "gregame.hpp"
#include "utility/eosio_dispatch_rhook.hpp"

using namespace eosio;

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
        .game_p = "chenxd53danx"_n,
    };
    dlt_gc.print();
    tbl_gameconf.set(dlt_gc, this->get_self());
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

    if (to == this->get_self()) {
        DEBUG_PRINT_VAR(quantity);
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

    } else if (from == this->get_self()) {
        //转出款
        //require_auth(from);
    }
}

ACTION gregame::AN__CREATE_RED_ENVELOPE(
    name        player
)
{
    DEBUG_PRINT_POS();
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

#if 0
ACTION gregame::ACTION_NAME__CREATE_GROUP (
    name        grp_name,
    name        grp_creator,
    std::string grp_topic,
    std::string grp_desc,
    uint16_t    grp_nr_p_min,
    uint16_t    grp_nr_p_max,
    asset       grp_amp_min,
    asset       grp_amp_max,
    uint8_t     grp_commission
){
    DEBUG_PRINT_POS();
    DEBUG_PRINT_VAR(this->get_self());
    DEBUG_PRINT_VAR(this->_code);

    DEBUG_PRINT_VAR(grp_amp_min.amount);
    DEBUG_PRINT_VAR(grp_amp_max.amount);
    DEBUG_PRINT_VAR(grp_amp_min);
    DEBUG_PRINT_VAR(grp_amp_max);

    require_auth(grp_creator);

    //表格的主键,size等,都按照scope分流了
    type_table__group tbl_group(get_self(), "global"_n.value);
    //type_table__group tbl_group(get_self(), grp_creator.value);

    char    szMesg[512] = {0};
    auto    itr = tbl_group.find(grp_name.value);
    snprintf(szMesg, sizeof(szMesg)-1, "grp_name(%s) has been created.", grp_name.to_string().c_str());
    eosio_assert(itr == tbl_group.end(), szMesg);
    
    tbl_group.emplace(grp_creator, [&]( auto& row ) {
        row.grp_name = grp_name;
        row.grp_creator = grp_creator;
        row.grp_topic = grp_topic;
        row.grp_desc = grp_desc;
        row.grp_nr_p_min = grp_nr_p_min;
        row.grp_nr_p_max = grp_nr_p_max;
        row.grp_amp_min = grp_amp_min;
        row.grp_amp_max = grp_amp_max;
        row.grp_commission = grp_commission;
        row.grp_status = 0;
    });

    uint16_t size = 0;
    for (auto itrStep = tbl_group.cbegin(); itrStep != tbl_group.cend(); ++itrStep) {
        ++size;
    }
    DEBUG_PRINT_VAR(size);

    DEBUG_PRINT_POS();
}

ACTION gregame::ACTION_NAME__JOIN_GROUP(
    name        grp_name,
    name        grp_creator,
    name        gm_account
)
{
    //1. 待加入的群组是否存在及状态正常
    //type_table__group tbl_group(get_self(), grp_creator.value);
    type_table__group tbl_group(get_self(), "global"_n.value);
    auto    itr = tbl_group.find(grp_name.value);
    char    szMesg[512] = {0};
    snprintf(szMesg, sizeof(szMesg)-1, 
        "grp_name(%s)+grp_creator(%s) has not been created.", 
        grp_name.to_string().c_str(),
        grp_creator.to_string().c_str());
    eosio_assert(itr != tbl_group.end(), szMesg);

    require_auth(gm_account);
    //2. 当前组员不在组中
    //type_table__groupmember tbl_groupmember(get_self(), gm_account.value);
    type_table__groupmember tbl_groupmember(get_self(), "global"_n.value);
    auto groupname_index = tbl_groupmember.get_index<"groupname"_n>();
    auto itr_a = groupname_index.lower_bound(grp_name.value);
    for(; itr_a!= groupname_index.end(); ++itr_a){
        snprintf(szMesg, sizeof(szMesg)-1, 
            "gm_account(%s) has joind into grp_name(%s)+grp_creator(%s).", 
            gm_account.to_string().c_str(),
            grp_name.to_string().c_str(),
            grp_creator.to_string().c_str());
        eosio_assert(itr_a->grp_creator != grp_creator, szMesg);
    }
    //确认了，还未加入；可以继续后续流程：//////////////////////////////////////


    //3. 当前组员转入抵押的资产
    asset gm_da = asset{int64_t(1.1 * pow(10,CORE_SYMBOL_P)), {CORE_SYMBOL,CORE_SYMBOL_P}}; // equals to 1 EOS

    action(
        permission_level{gm_account, "active"_n},
        "eosio.token"_n, "transfer"_n,
        std::make_tuple(gm_account, _self, gm_da, std::string("some memo here"))
    ).send();

    //4. 正确入群
    tbl_groupmember.emplace(gm_account, [&]( auto& row ) {
        row.id = tbl_groupmember.available_primary_key();
        row.grp_name = grp_name;
        row.grp_creator = grp_creator;
        row.gm_account = gm_account;
        //row.gm_da = asset{0,{CORE_SYMBOL,CORE_SYMBOL_P}};
        row.gm_da = gm_da;
        row.gm_status = 1;
    });
    
}

ACTION gregame::ACTION_NAME__JOIN_GROUP2(
    name        grp_name,
    name        grp_creator,
    name        gm_account
)
{
    //1. 待加入的群组是否存在及状态正常
    //type_table__group tbl_group(get_self(), grp_creator.value);
    type_table__group tbl_group(get_self(), "global"_n.value);
    auto    itr = tbl_group.find(grp_name.value);
    char    szMesg[512] = {0};
    snprintf(szMesg, sizeof(szMesg)-1, 
        "grp_name(%s)+grp_creator(%s) has not been created.", 
        grp_name.to_string().c_str(),
        grp_creator.to_string().c_str());
    eosio_assert(itr != tbl_group.end(), szMesg);

    DEBUG_PRINT_POS();
    //require_auth(gm_account);
    require_recipient(gm_account);
    DEBUG_PRINT_POS();
}

EOSIO_DISPATCH(gregame, (ACTION_NAME__CREATE_GROUP)(ACTION_NAME__JOIN_GROUP)(ACTION_NAME__JOIN_GROUP2))
#endif

//end-of-file
