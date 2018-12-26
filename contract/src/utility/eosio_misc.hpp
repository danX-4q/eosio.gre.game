#pragma once

#define EOSIO_DISPATCH_RHOOK( TYPE, MEMBERS ) \
extern "C" { \
   void apply( uint64_t receiver, uint64_t code, uint64_t action ) { \
      bool to_continue = recipient_hook( receiver, code, action ); \
      if (!to_continue) { return; } \
      if( code == receiver ) { \
         switch( action ) { \
            EOSIO_DISPATCH_HELPER( TYPE, MEMBERS ) \
         } \
         /* does not allow destructor of thiscontract to run: eosio_exit(0); */ \
      } \
   } \
} \
 /// @}  dispatcher


#define EOSIO_ASSERT_EX(x) { \
   char  szMsg[512] = {0}; \
   snprintf(szMsg, sizeof(szMsg)-1, \
      "%s %s %d: eosio_assert: %s", \
      __FILE__, __FUNCTION__, __LINE__, \
      #x); \
   eosio_assert((x), szMsg); }
