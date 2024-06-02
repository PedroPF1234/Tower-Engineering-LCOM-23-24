#pragma once


/**
 * @brief Initializes the instructions.
 *
 * This function sets up the necessary buttons and background for the instructions screen.
 */
void initializeInstructions();

/**
 * @brief Enters the instructions screen.
 *
 * This function resets device states and makes the instruction screen elements visible.
 */
void enterInstructions();

/**
 * @brief Updates the instructions screen.
 *
 * This function checks for hovered buttons and keyboard inputs, updating the instructions screen accordingly.
 */
void updateInstructions();

/**
 * @brief Exits the instructions screen.
 *
 * This function hides the instructions screen elements and resets related variables.
 */
void exitInstructions();


/**
 * @brief Destroys the instructions.
 *
 * This function frees memory and resources allocated for the instructions screen.
 */
void destroyInstructions();
