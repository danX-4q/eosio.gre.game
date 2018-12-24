//raw included!

TABLE groupmember {
    uint64_t            id;
    name                grp_name;           //组名称，本质是一个base32风格的标识符
    name                grp_creator;        //创建者的账户名称
    name                gm_account;         //入群者的账户名称
    asset               gm_da;              //入群者已预存的资产
    uint8_t             gm_status;          //入群者的状态:1,在群中;2:不在群中(进来过，且已离开)
    
    uint64_t primary_key() const
    {
        //return (grp_name.value);
        return (id);
    }

    uint64_t get_grp_name() const
    {
        return (grp_name.value);
    }
};
