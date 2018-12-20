//raw included!

//TABLE gameconf {
struct gameconf {
    uint64_t        game_id;            //仅用于存储，可使用固定值
    uint16_t        game_nr_p_min;      //起玩的最小人数
    uint16_t        game_nr_p_max;      //组的最多人数
    uint32_t        game_amp_min;       //红包人均金额的最小值
    uint32_t        game_amp_max;       //红包人均金额的最大值
    uint8_t         game_commission;    //红包被平台抽水的百分比
    
    /*uint64_t primary_key() const
    {
        return (game_id);
    }*/

    void print() const
    {
        prints("game_id = "); printui(game_id); prints("\n");
        prints("game_nr_p_min = "); printui(game_nr_p_min); prints("\n");
        prints("game_nr_p_max = "); printui(game_nr_p_max); prints("\n");
        prints("game_amp_min = "); printui(game_amp_min); prints("\n");
        prints("game_amp_max = "); printui(game_amp_max); prints("\n");
        prints("game_commission = "); printui(game_commission); prints("\n");        
    }
};
