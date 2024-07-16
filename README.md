# Tower Engineering - LCOM Project 23/24

> Tower Engineering is a Tower Defense inspired game where the player has a physical influence and presence on the game board rather that simply commanding the building process of turrets from a bird's eye view. In it, the player can have their character unlock and build turrets to ward off enemies from attacking their base/objective or make use of a handgun to shoot the enemies themselves. This game was developed for a project in the context of the curricular unit of Computer Laboratory from the Bachelor of Informatics and Computing Engineering at FEUP, whose goal was to develop an application that integrated several I/O devices, programmed and configurated by the team using MINIX.

## I/0 Devices

### Timer
The timer was crucial to develop the game. It wwas be used for both FPS and physics ticks per second, which made it possible for the gameplay not to be tied with the possibly shifting FPS values but rather have a fixed rate within a second. This implementation allowed for the best possible time managment with different features. Furthermore, anytime the RTC did not suit an "alarm" oriented feature, the timer served as a fallback to perform calculations. In adittion, the timer was also used to perform animations on game elements, such as the player's walk, the money digit fluctuations or even small animations on arena details. Their movement was tied to the physics ticking rate as that's a constant way of measuring time within the engine.

### Keyboard
As one of the main devices a computer has, the role of the keyboard was core to this game and yet very simple and straightforward. It provided a way to navigate and open several menus and control the character's movement. In the menus, the keyboard can be used to switch between options, while in-game, it can be used to move up, down, right, left and in the four diagonals.

### Mouse
The mouse's role is similar to that of the keyboard in the context of a game. We made use of every feature the mouse has to offer in streaming mode, enabling it to help navigate menus and play the game by using it to select turret blueprints, spots to build such turrets... etc. Using the its movement to track where the cursor is, the mouse also plays an important role as an aiming device for when the player desires to shoot enemies with their handgun. The character will shoot to where the player placed their cursor and clicked, making it so that the character can shoot in every possible direction from any distance.

### Graphics Card
As the project was decided to be a game, the graphics card had a clear and simple role. It provides the UI interaction for the player, taking accountability for every visual element which should be displayed. Making use of the VBE's features, we implemented all graphic mods with color spacing of 24 bits, which means we support all VBE 2.0 resolutions while still having the color space we desire. Our graphics card also implements double buffering with page flipping to allow for perfect image display, without any popping or ghosting. Furthermore, the graphics card makes use of sprites do display the many gameplay objects, some of which have animations. Lastly, it played a key role in the implementation of colision detection since we use shooting in our game.

## Keyboard Commands
| *In-game*                                          | *Menus*                                          |
|----------------------------------------------------|--------------------------------------------------|
| 	Move Right: <kbd>&rarr;</kbd> or <kbd>D</kbd>    |   Move Right: <kbd>&rarr;</kbd> or <kbd>D</kbd>  |
| 	Move Left: <kbd>&larr;</kbd> or <kbd>A</kbd>     |   Move Left: <kbd>&larr;</kbd> or <kbd>A</kbd>   |
| 	Move Up: <kbd>&uarr;</kbd> or <kbd>W</kbd>       |   Move Up: <kbd>&uarr;</kbd> or <kbd>W</kbd>     |
|   Move Down: <kbd>&darr;</kbd> or <kbd>S</kbd>     |   Move Down: <kbd>&darr;</kbd> or <kbd>S</kbd>   |
|   Move Diagonally Up Left: <kbd><kbd>&uarr;</kbd> or <kbd>W</kbd></kbd> + <kbd><kbd>&larr;</kbd> or <kbd>A</kbd></kbd> or vice versa |  Select: <kbd>ENTER</kbd> |
|   Move Diagonally Up Right: <kbd><kbd>&uarr;</kbd> or <kbd>W</kbd></kbd> + <kbd><kbd>&rarr;</kbd> or <kbd>D</kbd></kbd> or vice versa |  Pause: <kbd>ESC</kbd> |
|   Move Diagonally Down Left: <kbd><kbd>&darr;</kbd> or <kbd>S</kbd></kbd> + <kbd><kbd>&larr;</kbd> or <kbd>A</kbd></kbd> or vice versa |  Open Store/Base: <kbd>E</kbd> |
|   Move Diagonally Down Right: <kbd><kbd>&darr;</kbd> or <kbd>S</kbd></kbd> + <kbd><kbd>&rarr;</kbd> or <kbd>D</kbd></kbd> or vice versa |

Group members:

1. Pedro Fonseca       (up202108653@fe.up.pt)
2. José Quintas        (up202108712@fe.up.pt)
3. Maria Laranjeira    (up202004453@fe.up.pt)
4. Manuel Silva        (up202108874@fe.up.pt)
