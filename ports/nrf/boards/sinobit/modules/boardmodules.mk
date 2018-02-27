BOARD_SINOBIT_DIR = boards/sinobit/modules

INC += -I./$(BOARD_SINOBIT_DIR)
CFLAGS += -DBOARD_SPECIFIC_MODULES

SRC_BOARD_MODULES = $(addprefix $(BOARD_SINOBIT_DIR)/,\
                      microbitdisplay.c \
                      microbitimage.c \
                      iters.c \
                      microbitconstimage.c \
                      microbitconstimagetuples.c \
                      sinobitdisplay.c \
                      modsinobit.c \
                      modmicrobit.c \
                      )

OBJ += $(addprefix $(BUILD)/, $(SRC_BOARD_MODULES:.c=.o))
