#include "campaign.h"

uint8_t current_state = 0;

CampaignStage campaign[] =
{
	{"STAGE 1", "FOREST", 0},
	{"STAGE 2", "FOREST", 1},
	{"STAGE 3", "CAVE", 2},
	{"STAGE 4", "CAVE", 3},
	{"STAGE 5", "CASTLE", 4},
};
uint8_t campaign_state_count = ARRAY_COUNT(campaign);

void Campaign_NextStage(void)
{
	if(current_state < campaign_state_count - 1)
	{
		current_state++;
	}
}

void Campaign_Reset(void)
{
    current_state = 0;
}