#include <math.h>
#include "gregame.hpp"
#include "utility/printkit.hpp"

using namespace eosio;

gregame::gregame(name receiver, name code,  datastream<const char*> ds): 
    contract(receiver, code, ds)
{
    bool    is_inited(false);
    DEBUG_PRINT_VAR(receiver);
    DEBUG_PRINT_VAR(code);
    //test_asset_usage(); return;

    type_table__gameconf    tbl_gameconf(code, code.value);
    is_inited = tbl_gameconf.exists();
    if (!is_inited) {
        //首次，需初始化
        init(tbl_gameconf);
    } else {
        #warning "调试-合约中跨用户操作数据表"
        gameconf dlt_gc = tbl_gameconf.get();
        dlt_gc.game_commission += 1;
        tbl_gameconf.set(dlt_gc, _code);
    }
}

void gregame::init(type_table__gameconf &tbl_gameconf)
{
    gameconf    dlt_gc{
        0x64616e58, 
        5, 1000,
        //1*1000, 100*1000,   //*1000: 调整单位为小数点后4位
        {int64_t(0.5 * pow(10,CORE_SYMBOL_P)), {CORE_SYMBOL,CORE_SYMBOL_P}},
        {int64_t(100 * pow(10,CORE_SYMBOL_P)), {CORE_SYMBOL,CORE_SYMBOL_P}},
        5
    };
    dlt_gc.print();
    tbl_gameconf.set(dlt_gc, _code);
}

void gregame::test_asset_usage()
{
    {
        //编译通过，运行报错
        //asset tmp{1,{}};DEBUG_PRINT_VAR(tmp);
    }
    {
        //编译不通过
        //asset tmp{2,{"SYS"}};DEBUG_PRINT_VAR(tmp);
    }
    {
        asset tmp{3,{"SYS",4}};DEBUG_PRINT_VAR(tmp);
    }
    {
        //编译不通过
        //asset tmp4{"3.0000 SYS"};DEBUG_PRINT_VAR(tmp4);
    }
}

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

EOSIO_DISPATCH(gregame, (ACTION_NAME__CREATE_GROUP)(ACTION_NAME__JOIN_GROUP))
//end-of-file
