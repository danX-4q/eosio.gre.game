//raw included!

TABLE grabre {
    uint64_t        id;
    uint64_t        re_id;
    uint16_t        re_pos;         //第几个红包: base from 0
    name            re_player;
    uint16_t        re_play_num;    //用户传上来的明数
    uint64_t        re_salt_num;
    asset           re_amount;      //最终分派到的金额

    uint64_t primary_key() const
    {
        return (id);
    }

    uint64_t get_re_id() const
    {
        return (re_id);
    }

    void print() const
    {
        DEBUG_PRINT_VAR(id);
        DEBUG_PRINT_VAR(re_id);
        DEBUG_PRINT_VAR(re_pos);
        DEBUG_PRINT_VAR(re_player);
        DEBUG_PRINT_VAR(re_play_num);
        DEBUG_PRINT_VAR(re_salt_num);
        DEBUG_PRINT_VAR(re_amount);
    }
};
