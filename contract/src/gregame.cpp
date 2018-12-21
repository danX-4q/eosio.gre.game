#include "gregame.hpp"
using namespace eosio;

#define DEBUG_PRINT_POS() print_f("% % %\n", __FILE__, __FUNCTION__, __LINE__)

gregame::gregame(name receiver, name code,  datastream<const char*> ds): 
    contract(receiver, code, ds),
    tbl_gameconf(code, code.value)
{
    bool    is_inited(false);
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

ACTION gregame::ACTION_NAME__CREATE_GROUP (
    name        grp_name,
    name        grp_creator,
    std::string grp_topic,
    std::string grp_desc,
    uint16_t    grp_nr_p_min,
    uint16_t    grp_nr_p_max,
    uint32_t    grp_amp_min,
    uint32_t    grp_amp_max,
    uint8_t     grp_commission
){
    DEBUG_PRINT_POS();
    require_auth(grp_creator);
    //type_table__group tbl_group(get_self(), "global"_n.value);
    type_table__group tbl_group(get_self(), grp_creator.value);

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
    prints("tbl_group.size = "); printui(size); prints("\n");
    
    DEBUG_PRINT_POS();
}

EOSIO_DISPATCH(gregame, (ACTION_NAME__CREATE_GROUP))
//end-of-file
