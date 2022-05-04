#pragma once
#include "common.hpp"
#include "gta/fwddec.hpp"
#include "gta/natives.hpp"

class transaction_queue
{
public:
	char pad_0000[57]; //0x0000
	bool busy_one;//0x0039
	char pad_003A[14]; //0x003A
	bool busy_two; //0x0048
	char pad_0049[7]; //0x0049
	bool busy_three; //0x0050
}; //Size: 0x0051

namespace big::functions
{
	using run_script_threads_t = bool(*)(std::uint32_t ops_to_execute);
	using get_native_handler_t = rage::scrNativeHandler(*)(rage::scrNativeRegistrationTable*, rage::scrNativeHash);
	using fix_vectors_t = void(*)(rage::scrNativeCallContext*);
	using construct_basket_t = bool (*)(transaction_queue* transact_helper, int* transaction_id, int category_type, int action_type, int target);
	using add_item_to_basket_t = bool (*)(transaction_queue* tq, int* item);
	using process_transaction = bool (*)(transaction_queue* tq, int transaction, int64_t unk3);
}
