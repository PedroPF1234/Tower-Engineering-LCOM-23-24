#include <lcom/lcf.h>
#include <stdint.h>

#include "economy.h"

Economy* read_prices_info(char** prices_info) {

  Economy* new_economy = (Economy*)malloc(sizeof(Economy));

  uint8_t state = 0;

  uint8_t unlock_count = 0; 
  uint8_t build_count = 0;
  uint8_t upgrade_crossbow_count = 0;
  uint8_t upgrade_cannon_count = 0;
  uint8_t upgrade_laser_count = 0;

  uint16_t current_line = 0;

  char* info = prices_info[current_line++];

  uint32_t accumulator = 0;
  uint8_t pseudo_state = 0;

  while (*info != '\0') {
    char current = *info;
    if (current == ' ') {
      switch (pseudo_state)
      {
      case 0:
        unlock_count = accumulator;
        accumulator = 0;
        break;

      case 1:
        build_count = accumulator;
        accumulator = 0;
        break;

      case 2: 
        upgrade_crossbow_count = accumulator;
        accumulator = 0;
        break;

      case 3:
        upgrade_cannon_count = accumulator;
        accumulator = 0;
        break;

      case 4:
        upgrade_laser_count = accumulator;
        accumulator = 0;
        break;
      default:
        break;
      }
      pseudo_state++;
    } else if ((current >= '0') && (current <= '9')) {
      accumulator = accumulator * 10 + (current - '0');
    }
    info++;
  }

  int32_t* unlock_prices = (int32_t*)malloc(sizeof(int32_t) * unlock_count);
  int32_t* build_prices = (int32_t*)malloc(sizeof(int32_t) * build_count);
  int32_t* upgrade_crossbow = (int32_t*)malloc(sizeof(int32_t) * upgrade_crossbow_count);
  int32_t* upgrade_cannon = (int32_t*)malloc(sizeof(int32_t) * upgrade_cannon_count);
  int32_t* upgrade_laser = (int32_t*)malloc(sizeof(int32_t) * upgrade_laser_count);


  pseudo_state = 0;
  accumulator = 0;
  info = prices_info[current_line++];

  while (state != 5) {

    uint8_t index = 0;
    uint8_t array_index = 0;
    char current_char = info[index++];

    while (current_char != '\0') {

      if (current_char == ' ') {
        
        switch (state)
        {
        case 0:
          unlock_prices[array_index++] = accumulator;
          accumulator = 0;
          break;

        case 1:
          build_prices[array_index++] = accumulator;
          accumulator = 0;
          break;

        case 2:
          upgrade_crossbow[array_index++] = accumulator;
          accumulator = 0;
          break;

        case 3:
          upgrade_cannon[array_index++] = accumulator;
          accumulator = 0;
          break;

        case 4:
          upgrade_laser[array_index++] = accumulator;
          accumulator = 0;
          break;
        default:
          break;
        }

      } else if (current_char >= '0' && current_char <= '9') {
        accumulator = accumulator * 10 + (current_char - '0');
      }
      current_char = info[index++];
    }

    info = prices_info[current_line++];
    state++;

  }

  new_economy->build_count = build_count;
  new_economy->build_prices = build_prices;
  new_economy->unlock_count = unlock_count;
  new_economy->unlock_prices = unlock_prices;
  new_economy->upgrade_crossbow_count = upgrade_crossbow_count;
  new_economy->upgrade_crossbow_prices = upgrade_crossbow;
  new_economy->upgrade_cannon_count = upgrade_cannon_count;
  new_economy->upgrade_laser_count = upgrade_laser_count;
  new_economy->upgrade_laser_prices = upgrade_laser;

  return new_economy;
}
