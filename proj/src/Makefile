# name of the program (Minix service)
PROG=proj

.PATH: ${.CURDIR}/Devices/
.PATH: ${.CURDIR}/Devices/Graphics/
.PATH: ${.CURDIR}/Devices/KeyBoard/
.PATH: ${.CURDIR}/Devices/Mouse/
.PATH: ${.CURDIR}/Devices/Timer/
.PATH: ${.CURDIR}/Devices/RTC/
.PATH: ${.CURDIR}/ImageAssets/
.PATH: ${.CURDIR}/GameObjects/
.PATH: ${.CURDIR}/Game/
.PATH: ${.CURDIR}/Game/Menu/
.PATH: ${.CURDIR}/Game/Player/
.PATH: ${.CURDIR}/Game/Instructions/
.PATH: ${.CURDIR}/Game/Gameplay/
.PATH: ${.CURDIR}/Game/Gameplay/Player/
.PATH: ${.CURDIR}/Game/Gameplay/Tower/
.PATH: ${.CURDIR}/Game/Gameplay/Enemy/
.PATH: ${.CURDIR}/Game/Gameplay/Arena/

# source code files to be compiled
SRCS = proj.c graphics.c kbc.c mouse.c timer.c rtc.c utils.c device_controller.c sprite.c gameobject.c game.c menu.c keyboardkeys.c player.c instructions.c gameplay.c towers.c enemy.c arena.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CPPFLAGS += -pedantic

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
