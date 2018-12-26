//raw included!

TABLE playerbalance {
    name            player;             //玩家账户
    asset           balance;            //当前存于合约的余额

    uint64_t primary_key() const
    {
        return (player.value);
    }

    void print() const
    {
        DEBUG_PRINT_VAR(player);
        DEBUG_PRINT_VAR(balance);
    }
};
