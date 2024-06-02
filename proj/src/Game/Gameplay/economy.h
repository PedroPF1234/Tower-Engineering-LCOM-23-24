#pragma once

#include <stdint.h>
#include <lcom/lcf.h>

/**
 * @brief Struct that represents the prices of towers and upgrades of the game.
 * 
 * Contains the prices of unlocking, building and upgrading the towers.
 * 
 * @param unlock_prices An array of integers that represent the prices to unlock each tower.
 * @param unlock_count The size of the array of unlock prices.
 * 
 * @param build_prices An array of integers that represent the prices to build each tower.
 * @param build_count The size of the array of build prices.
 * 
 * @param upgrade_crossbow_prices An array of integers that represent the prices to upgrade the crossbow tower.
 * @param upgrade_crossbow_count The size of the array of upgrade crossbow prices.
 * 
 * @param upgrade_cannon_prices An array of integers that represent the prices to upgrade the cannon tower.
 * @param upgrade_cannon_count The size of the array of upgrade cannon prices.
 * 
 * @param upgrade_laser_prices An array of integers that represent the prices to upgrade the laser tower.
 * @param upgrade_laser_count The size of the array of upgrade laser prices.
*/
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

  int32_t weapon_price;

} Economy;

/**
 * @brief Reads the prices information from the corresponding file.
 * 
 * Reads the prices information from the corresponding file and initializes the Economy object.
 * 
 * @param prices_info The array of strings that represent the prices information.
*/
Economy* read_prices_info(char** prices_info);
