#pragma once

#include <stdint.h>
#include "../ImageAssets/sprite.h"

#define BIT_MASK(n) ((1 << n) - 1) // bit mask with n bits set to 1
#define Z_INDEX_PER_LAYER 3 // y level weight for z index
#define LOW_PRIORITY_Z_INDEX 1 // lowest priority z index
#define MEDIUM_PRIORITY_Z_INDEX 2 // medium priority z index
#define HIGH_PRIORITY_Z_INDEX 3 // highest priority z index
#define MAX_PRIORITY_Z_INDEX 4 // maximum priority z index


/**
 * @brief Represents a game object in the game.
 */
typedef struct GameObject {
  Sprite* sprite; // sprite of the game object
  int16_t x, y; // x and y positions of the game object
  int16_t origin_offset_x, origin_offset_y; // x and y offsets of the game object's origin
  uint16_t z_index; // z index of the game object
} GameObject;

/**
 * @brief Represents an Animated Game Object in the game.
 
 */
typedef struct AnimatedGameObject {
  GameObject* gameObject; // game object of the animated game object
  AnimatedSprite* animatedSprite; // animated sprite of the animated game object
} AnimatedGameObject;

/**
 * @brief Represents an array of GameObjects.
 */
typedef struct GameObjectArray {
  GameObject** gameObjects; // pointer to an array of game object pointers
  uint32_t capacity; // total capacity of the array
  uint32_t length; // current number of game objects in the array
} GameObjectArray;


/**
 * @brief Initializes a render pipeline.
 */
void init_render_pipeline();

/**
 * @brief Creates the game objects. Creates the object and adds it to the render pipeline.
 *
 * @return 0 upon success
 */
int create_gameobjects();

/**
 * @brief Destroys the game objects.
 *
 * @return 0 upon success
 */
int destroy_gameobjects();

/**
 * @brief Renders the game objects.
 */
void renderGameObjects();


/**
 * @brief Creates an animated game object.
 *
 * @param animatedSprite The animated sprite of the animated game object.
 * @param x The initial x position of the animated game object.
 * @param y The initial y position of the animated game object.
 * @param z_index The z index of the animated game object.
 * @return The pointer to the newly created animated game object.
 */
AnimatedGameObject* create_animated_gameobject(AnimatedSprite* animatedSprite, int16_t x, int16_t y, uint16_t z_index);

/**
 * @brief Destroys an animated game object and frees associated memory.
 *
 * @param animatedGameObject The pointer to the animated game object to be destroyed.
 */
void destroy_animated_gameobject(AnimatedGameObject* animatedGameObject);

/**
 * @brief Updates the animated game objects.
 */
void updateAnimatedGameObjects();

/**
 * @brief Switches sprites of an animated game object.
 *
 * @param animatedGameObject The pointer to the animated game object to update.
 * @param animatedSprite The pointer to the animated sprite to use.
 */
void switchAnimatedSpriteOfAnimatedGameObject(AnimatedGameObject* animatedGameObject, AnimatedSprite* animatedSprite);

/**
 * @brief Hides an animated game object.
 *
 * @param animatedGameObject The pointer to the animated game object to hide.
 */
void hideAnimatedGameObject(AnimatedGameObject* animatedGameObject);

/**
 * @brief Shows an animated game object.
 *
 * @param animatedGameObject The pointer to the animated game object to show.
 */
void showAnimatedGameObject(AnimatedGameObject* animatedGameObject);

/**
 * @brief Creates a game object.
 *
 * @param pic The xpm map of the game object.
 * @param x The initial x position of the game object.
 * @param y The initial y position of the game object.
 * @param origin_offset_x The x offset of the game object's origin.
 * @param origin_offset_y The y offset of the game object's origin.
 * @param z_index The z index of the game object.
 * @param square_shape Whether the game object is square shaped.
 * @param visible Whether the game object is visible.
 * @return The pointer to the newly created game object.
 */
GameObject* create_gameobject(xpm_map_t pic, int16_t x, int16_t y, int16_t origin_offset_x, int16_t origin_offset_y, uint16_t z_index, bool square_shape, bool visible);

/**
 * @brief Creates a game object from a sprite.
 *
 * @param sprite The sprite of the game object.
 * @param x The initial x position of the game object.
 * @param y The initial y position of the game object.
 * @param origin_offset_x The x offset of the game object's origin.
 * @param origin_offset_y The y offset of the game object's origin.
 * @param z_index The z index of the game object.
 * @return The pointer to the newly created game object.
 */
GameObject* create_gameobject_from_sprite(Sprite* sprite, int16_t x, int16_t y, int16_t origin_offset_x, int16_t origin_offset_y, uint16_t z_index);

/**
 * @brief Creates a game object without a sprite.
 *
 * @param x The initial x position of the game object.
 * @param y The initial y position of the game object.
 * @param origin_offset_x The x offset of the game object's origin.
 * @param origin_offset_y The y offset of the game object's origin.
 * @param z_index The z index of the game object.
 * @return The pointer to the newly created game object.
 */
GameObject* create_spriteless_gameobject(int16_t x, int16_t y, int16_t origin_offset_x, int16_t origin_offset_y, uint16_t z_index);

/**
 * @brief Adds a sprite to a spriteless game object.
 *
 * @param gameObject The pointer to the game object to add the sprite to.
 * @param sprite The pointer to the sprite to add.
 */
void add_sprite_to_spriteless_gameobject(GameObject* gameObject, Sprite* sprite);

/**
 * @brief Removes a sprite from a spriteless game object.
 *
 * @param gameObject The pointer to the game object to remove the sprite from.
 */
void remove_sprite_from_spriteless_gameobject(GameObject* gameObject);

/**
 * @brief Updates the z index of a game object.
 *
 * @param gameObject The pointer to the game object to update.
 * @param z_index The new z index of the game object.
 */
void updateGameObjectZIndex(GameObject* gameObject, uint16_t z_index);

/**
 * @brief Destroys a game object, removes it from the render pipeline and frees associated memory.
 *
 * @param gameObject The pointer to the game object to be destroyed.
 */
void destroy_gameobject(GameObject* gameObject);

/**
 * @brief Destroys a game object without erasing its associated sprite. Removes Game Object from the render pipeline and frees associated memory.
 *
 * @param gameObject The pointer to the game object to be destroyed.
 */
void destroy_gameobject_safe_sprite(GameObject* gameObject);

/**
 * @brief Updates the sprite of a game object.
 *
 * @param gameObject The pointer to the game object to update.
 * @param sprite The pointer to the sprite to use.
 */
void updateGameObjectSprite(GameObject* gameObject, Sprite* sprite);

/**
 * @brief Hides a game object by removing it from the render pipeline.
 *
 * @param gameObject The pointer to the game object to hide.
 */
void hideGameObject(GameObject* gameObject);

/**
 * @brief Shows a game object by inserting in the render pipeline.
 *
 * @param gameObject The pointer to the game object to show.
 */
void showGameObject(GameObject* gameObject);


/**
 * @brief Creates a new GameObjectArray with a specified capacity.
 *
 * @param capacity The initial capacity of the GameObjectArray.
 * @return A new GameObjectArray.
 */
GameObjectArray newGameObjectArray(uint32_t capacity);

/**
 * @brief Adds a GameObject to a GameObjectArray.
 *
 * @param array A pointer to the GameObjectArray.
 * @param newGameObject A pointer to the GameObject to be added.
 */
void insertGameObjectArray(GameObjectArray *array, GameObject *newGameObject);

/**
 * @brief Retrieves a GameObject from a GameObjectArray at a specified index.
 *
 * @param array A pointer to the GameObjectArray.
 * @param index The index of the GameObject to retrieve.
 * @return A pointer to the GameObject at the specified index, or NULL if the index is out of bounds.
 */
GameObject* getGameObjectArray(GameObjectArray *array, uint32_t index);

/**
 * @brief Removes a GameObject from a GameObjectArray.
 *
 * @param array A pointer to the GameObjectArray.
 * @param gameObject A pointer to the GameObject to be removed.
 */
void removeGameObjectArray(GameObjectArray *array, GameObject *gameObject);

/**
 * @brief Destroys a GameObjectArray and frees associated memory.
 *
 * @param array A pointer to the GameObjectArray to be destroyed.
 */
void destroyGameObjectArray(GameObjectArray *array);

/**
 * @brief Shows all the GameObjects in the GameObjectArray
 *
 * @param gameObjects A pointer to the GameObject Array.
 */
void showGameObjects(GameObjectArray* gameObjects);

/**
 * @brief Hides all the GameObjects in the GameObjectArray
 *
 * @param gameObjects A pointer to the GameObject Array.
 */
void hideGameObjects(GameObjectArray* gameObjects);

