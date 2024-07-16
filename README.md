# Tower Engineering - LCOM Project 23/24

> Tower Engineering is a Tower Defense inspired game where the player has a physical influence and presence on the game board rather that simply commanding the building process of turrets from a bird's eye view. In it, the player can have their character unlock and build turrets to ward off enemies from attacking their base/objective or make use of a handgun to shoot the enemies themselves. This game was developed for a project in the context of the curricular unit of Computer Laboratory from the Bachelor of Informatics and Computing Engineering at FEUP, whose goal was to develop an application that integrated several I/O devices, programmed and configurated by the team using MINIX.

## The Game
### Main Menu

The Main Menu offers 3 options: To play, to access the instructions or to quit the game, the latter shutting it down and returning to the MINIX Virtual Machine. 

<p align="center" justify="center">
  <img src="https://github.com/user-attachments/assets/3722369e-d96b-4d75-b97d-8edcf002fe7f"/>
</p>
<p align="center">
  <b><i>Fig 1. Main Menu </i></b>
</p>
<br>
<br />

The instructions menu consists of a static menu which contains a brief explanation of everything the player needs to know to understand how to play our game.

<p align="center" justify="center">
  <img src="https://github.com/user-attachments/assets/c8911822-4a1c-408f-a140-d96fb0488232"/>
</p>
<p align="center">
  <b><i>Fig 2. Instructions Menu </i></b>
</p>
<br>
<br />

The play button opens an arena selection for the player to choose in which map to play. Each map differs in the layout of the path traveled by the enemies, which is presented as a miniature for each arena option so that the player is aware of what they are choosing.

<p align="center" justify="center">
  <img src="https://github.com/user-attachments/assets/fe7e536f-e681-48fc-b36a-c5a0ef578ad6"/>
</p>
<p align="center">
  <b><i>Fig 3. Arena Selection </i></b>
</p>
<br>
<br />

After selecting the arena and proceeding, the game starts. A player begins the game with 500 Money and no handgun or mounted turrets. Furthermore, as soon as the game starts, the enemies start spawning and making their way along the map's path to the player's base. This way, firstly, the player must run to one of the many turret placeholders and build a turret there. For each enemy killed, the player gains 50 Money and so, from the this point on, it's up to each player to decide on how to invest their gains to build up their defenses. A player can either unlock new more powerful turrets, upgrade built turrets, or invest in the handgun. 

<p align="center" justify="center">
  <img src="https://github.com/user-attachments/assets/bfcb40e2-eb65-4c76-8fa8-7454a5403d75"/>
</p>
<p align="center">
  <b><i>Fig 4. Start of the Game </i></b>
</p>
<br>
<br />

<p align="center" justify="center">
  <img src="https://github.com/user-attachments/assets/a8245f0c-787c-43c3-bbed-8701ffc1c476"/>
</p>
<p align="center">
  <b><i>Fig 5. Turrent Mount Menu </i></b>
</p>
<br>
<br />

<p align="center" justify="center">
  <img src="https://github.com/user-attachments/assets/8c7f69ed-8b7f-4a32-987a-5edaa1b5bfd4"/>
</p>
<p align="center">
  <b><i>Fig 6. Mounted Turret </i></b>
</p>
<br>
<br />

<p align="center" justify="center">
  <img src="https://github.com/user-attachments/assets/36fdbc58-bcbd-4faf-a045-54010cc26d2a"/>
</p>
<p align="center">
  <b><i>Fig 7. Shop </i></b>
</p>
<br>
<br />

<p align="center" justify="center">
  <img src="https://github.com/user-attachments/assets/9d7a15bd-7027-4c7e-89fa-cf5fbeb26903"/>
</p>
<p align="center">
  <b><i>Fig 8. Base Menu </i></b>
</p>
<br>
<br />


More specifically regarding the turret's menu, a player has the option to spend money to upgrade it, which increases the turret's damage, choose its shooting style, or unmount it, gaining a percentage of what it cost to build it, freeing that space to build a different turret.


Enemies lifepoints as well as the base's are represented by a green bar above the respective game object. For each bit of damage taken the green bar decreases, until it is fully red. When the base loses all its life points the player loses the game.
<p align="center" justify="center">
  <img src="https://github.com/user-attachments/assets/40618e6c-395b-4fa7-bde1-2774d9cd01c2"/>
</p>
<p align="center">
  <b><i>Fig 9. Game Over Menu </i></b>
</p>
<br>
<br />


## I/0 Devices

### Timer
The timer was crucial to develop the game. It was used for both FPS and physics ticks per second, which made it possible for the gameplay not to be tied with the possibly shifting FPS values but rather have a fixed rate within a second. This implementation allowed for the best possible time managment with different features. Furthermore, anytime the RTC did not suit an "alarm" oriented feature, the timer served as a fallback to perform calculations. In adittion, the timer was also used to perform animations on game elements, such as the player's walk, the money digit fluctuations or even small animations on arena details. Their movement was tied to the physics ticking rate as that's a constant way of measuring time within the engine.

### Keyboard
As one of the main devices a computer has, the role of the keyboard is core to this game and yet very simple and straightforward. It provided a way to navigate and open several menus and control the character's movement. In the menus, the keyboard can be used to switch between options, while in-game, it can be used to move up, down, right, left and in the four diagonals.

### Mouse
The mouse's role is similar to that of the keyboard in the context of a game. We made use of every feature the mouse has to offer in streaming mode, enabling it to help navigate menus and play the game by using it to select turret blueprints, spots to build such turrets... etc. Using its movement to track where the cursor is, the mouse also plays an important role as an aiming device for when the player desires to shoot enemies with their handgun. The character will shoot to where the player placed their cursor and clicked, making it so that the character can shoot in every possible direction from any distance.

### Graphics Card
As the project was decided to be a game, the graphics card has a clear and simple role. It provides the UI interaction for the player, taking accountability for every visual element which should be displayed. Making use of the VBE's features, we implemented all graphic mods with color spacing of 24 bits, which means our graphics card supports all VBE 2.0 resolutions while still having the color space we desire. It also implements double buffering with page flipping to allow for perfect image display, without any popping or ghosting. Furthermore, the graphics card makes use of sprites do display the many gameplay objects, some of which have animations. Lastly, it played a key role in the implementation of colision detection since we integrated the shooting action in our game.

### RTC (Real Time Clock)
Every action that had a determined timing to occur was implemented using the RTC. Therefore, this device had a fundamental role in setting the frequency of the sucessive spawning of enemies and of each turret's shooting. It determined when said action should happen and after which time it should happen again.

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

## The Group
### Group Name: T05-G02
### Group Members:
1. Pedro Fonseca       (up202108653@fe.up.pt)
2. José Quintas        (up202108712@fe.up.pt)
3. Maria Laranjeira    (up202004453@fe.up.pt)
4. Manuel Silva        (up202108874@fe.up.pt)
