
eosio.token资产操作模式与权限汇总：

////////////////////////////////////////////////////////////

1. 交费：通过notify通讯模型，所需的eosio.code权限将倒置。

CA[ua] (action)-> eosio.token::transfer，实现: ub -> ua, 需要： ub@active授权给ua@eosio.code。
	代扣费模式；由CA决定转多少钱；出于安全，ub可监听eosio.token::transfer事件，按需拒绝交易。

CA[ua] (notify)-> CB[ub], CB[ub] (action)-> eosio.token::transfer，实现: ub -> ua, 需要： ub@active授权给ub@eosio.code。
	通知缴费模式；由CB决定转多少钱；出于完整，ua可监听eosio.token::transfer事件，记录ub的抵押总额，判断是否足额。

cleos[ub], 实现： ub -> ua, 无需额外的权限设置。
	主动缴费模型；由cleos[ub]决定转多少钱；出于完整，ua可监听eosio.token::transfer事件，记录ub的抵押总额，判断是否足额。

////////////////////////////////////////////////////////////

2. 退费：

CA[ub] (action)-> eosio.token::transfer, 实现：ua -> ub，需要：ua@active授权给ua@eosio.code；注意，这里虽然是ub发起的，但授权时与ub无关！
	抵押者发起退费；由CA记录ub可退费的总额，避免超额。

CA[ua] (action)-> eosio.token::transfer, 实现：ua -> ub, 无需额外的权限设置。
	平台主动退费；由CA记录ub可退费的总额，避免超额。

cleos[ua], 实现： ub -> ua, 无需额外的权限设置。
	平台主动退费；由cleos[ub]决定转多少钱；出于完整，ua可监听eosio.token::transfer事件，记录ub的抵押总额，判断是否足额。
