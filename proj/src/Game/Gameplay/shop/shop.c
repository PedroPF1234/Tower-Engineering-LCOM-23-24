#include <lcom/lcf.h>
#include <stdint.h>

#include "shop.h"
#include "../../../ImageAssets/Shop.xpm"

Shop* initializeShop(int16_t x, int16_t y) {
  Shop* new_shop = (Shop*)malloc(sizeof(Shop));

  Sprite* shop = create_sprite((xpm_map_t)ShopSprite, x, y, false, true);
  new_shop->shopObject = create_gameobject_from_sprite(shop, x, y, -(shop->width/2),
  -(shop->height), y * Z_INDEX_PER_LAYER + LOW_PRIORITY_Z_INDEX);

  return new_shop;
}

void destroyShop(Shop* shop) {
  destroy_gameobject(shop->shopObject);
  free(shop);
}
