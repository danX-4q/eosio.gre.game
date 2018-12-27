//raw included!

TABLE gameruntime {
    uint64_t        grt_game_id;        //当前这一局游戏的id
    uint64_t        grt_re_salt;        //上一次分红包时最后的salt值
    uint16_t        grt_nr_player;      //当前局的玩家数量；相同player多次出现只计为1
    uint8_t         grt_game_state;     //当前局游戏的状态
    
    asset           grt_m_total;        //组织者的累计佣金(应付)
    asset           grt_m_refund;       //组织者已提走的佣金(已付)
    asset           grt_p_total;        //组织者的累计佣金(应付)
    asset           grt_p_refund;       //组织者已提走的佣金(已付)
    
    void print() const
    {
        DEBUG_PRINT_VAR(grt_game_id);
        DEBUG_PRINT_VAR(grt_re_salt);
        DEBUG_PRINT_VAR(grt_nr_player);
        DEBUG_PRINT_VAR(grt_game_state);
        DEBUG_PRINT_VAR(grt_m_total);
        DEBUG_PRINT_VAR(grt_m_refund);
        DEBUG_PRINT_VAR(grt_p_total);
        DEBUG_PRINT_VAR(grt_p_refund);
    }
};
