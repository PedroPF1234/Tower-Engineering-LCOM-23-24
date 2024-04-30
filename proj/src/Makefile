# name of the program (Minix service)
PROG=proj

.PATH: ${.CURDIR}/Devices/
.PATH: ${.CURDIR}/Devices/Graphics/
.PATH: ${.CURDIR}/Devices/KeyBoard/
.PATH: ${.CURDIR}/Devices/Mouse/
.PATH: ${.CURDIR}/Devices/Timer/
.PATH: ${.CURDIR}/ImageAssets/
.PATH: ${.CURDIR}/GameObjects/
.PATH: ${.CURDIR}/Game/
.PATH: ${.CURDIR}/Game/Menu/

# source code files to be compiled
SRCS = proj.c graphics.c kbc.c mouse.c timer.c utils.c device_controller.c sprite.c gameobject.c game.c menu.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CPPFLAGS += -pedantic

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
