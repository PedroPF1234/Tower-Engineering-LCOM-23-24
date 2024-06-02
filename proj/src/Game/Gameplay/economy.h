#pragma once

#include <stdint.h>
#include <lcom/lcf.h>

typedef struct Economy
{
  int32_t* unlock_prices;
  uint8_t unlock_count;

  int32_t* build_prices;
  uint8_t build_count;

  int32_t* upgrade_crossbow_prices;
  uint8_t upgrade_crossbow_count;

  int32_t* upgrade_cannon_prices;
  uint8_t upgrade_cannon_count;

  int32_t* upgrade_laser_prices;
  uint8_t upgrade_laser_count;
} Economy;


Economy* read_prices_info(char** prices_info);
