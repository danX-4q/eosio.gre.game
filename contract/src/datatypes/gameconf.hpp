//raw included!

//struct gameconf { -*- struct不支持cleos get table ...
TABLE gameconf {
    uint64_t        game_magic;         //仅用于存储，可使用固定值
    asset           game_re_amount;     //每个红包的总金额
    uint16_t        game_re_split;      //每个红包切分成几份
    asset           game_m_cmsn;        //每个红包被组织者提成的佣金
    asset           game_p_cmsn;        //每个红包被平台提成的佣金
    name            game_p;             //平台佣金提成的账号

    void print() const
    {
        DEBUG_PRINT_VAR(game_magic);
        DEBUG_PRINT_VAR(game_re_amount);
        DEBUG_PRINT_VAR(game_re_split);
        DEBUG_PRINT_VAR(game_m_cmsn);
        DEBUG_PRINT_VAR(game_p_cmsn);
        DEBUG_PRINT_VAR(game_p);
    }
};
