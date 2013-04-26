#
# fabooh blink makefile
#

PROJECT =adc_temp
LIBS ?=
USER_OBJS=$(OUT_DIR_F)fix16_trig.o $(OUT_DIR_F)fix16.o $(OUT_DIR_F)fix16_sqrt.o

#
# minimize size by turning off some memory and flash hungry features
#

include ../../../common.mk

$(OUT_DIR_F)fix16_trig.o: $(FBD)$(_3RD_PARTY_DIR)/libfixmath/fix16_trig.c
	$(CC) $(C_FLAGS_F) -c $< -o $@

$(OUT_DIR_F)fix16.o: $(FBD)$(_3RD_PARTY_DIR)/libfixmath/fix16.c
	$(CC) $(C_FLAGS_F) -c $< -o $@

$(OUT_DIR_F)fix16_sqrt.o: $(FBD)$(_3RD_PARTY_DIR)/libfixmath/fix16_sqrt.c
	$(CC) $(C_FLAGS_F) -c $< -o $@
	
