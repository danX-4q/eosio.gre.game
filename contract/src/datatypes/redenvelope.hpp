//raw included!

TABLE redenvelope {
    uint64_t        id;
    uint64_t        re_id;
    name            re_creator;

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
        DEBUG_PRINT_VAR(re_creator);
    }
};
