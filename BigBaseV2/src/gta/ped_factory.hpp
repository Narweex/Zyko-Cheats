#pragma once
#include "fwddec.hpp"

#pragma pack(push, 1)
class CPed
{
public:
	char m_padding[0x10B8];
	CPlayerInfo *m_playerinfo;
	uint8_t m_god; //0x0189
};
#pragma pack(pop)

class CPedFactory
{
public:
	virtual ~CPedFactory() = default;
	CPed *m_local_ped;
};
