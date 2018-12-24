//raw included!

TABLE group {
    name                grp_name;           //组名称，本质是一个base32风格的标识符
    name                grp_creator;        //创建者的账户名称
    std::string         grp_topic;          //组的主题
    std::string         grp_desc;           //组的描述
    uint16_t            grp_nr_p_min;       //起玩的最小人数
    uint16_t            grp_nr_p_max;       //组的最多人数
    asset               grp_amp_min;
    asset               grp_amp_max;
    uint8_t             grp_commission;     //红包被组创建者抽水的百分比
    uint8_t             grp_status;         //组的状态：0,正常；1,正常但不能进；2,解散
    
    uint64_t primary_key() const
    {
        return (grp_name.value);
    }
};
