#include "common.hpp"
#include "features.hpp"
#include "natives.hpp"
#include "script.hpp"
#include "player_info.h"
#include "gta_util.hpp"
#include "fiber_pool.hpp"

namespace zyko
{
	/*bool local_player_info_class::is_in_vehicle()
	{
		return PED::IS_PED_IN_VEHICLE(player_ped, PED::GET_VEHICLE_PED_IS_IN(player_ped, 0), 0);
	}*/

	bool local_player_info_class::network_is_activity_session()
	{
		return NETWORK::NETWORK_IS_ACTIVITY_SESSION();
	}

	bool local_player_info_class::is_cutscene_playing()
	{
		return CUTSCENE::IS_CUTSCENE_PLAYING();
	}

	int local_player_info_class::net_player_id()
	{
		return NETWORK::NETWORK_GET_NETWORK_ID_FROM_ENTITY(player_ped);
	}

	int local_player_info_class::net_vehicle_id()
	{
		QUEUE_JOB_BEGIN_CLAUSE()
		{
			return NETWORK::NETWORK_GET_NETWORK_ID_FROM_ENTITY(PED::GET_VEHICLE_PED_IS_IN(player_ped, 0));
		}
		QUEUE_JOB_END_CLAUSE

			return NULL;
	}
}