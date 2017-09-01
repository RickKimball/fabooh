#--------------------------------------------------
# common_board.mk - select the board here
#

BOARD ?= bluepill
BOARDDIR = board/$(BOARD)
BOARDS = $(notdir $(wildcard $(FBD)board/*))

MSP430_BOARDS=msp430fr5739 msp430fr5969 msp430g2231in14 msp430g2452in20 msp430g2553in20
MSP430G_BOARDS=msp430g2231in14 msp430g2452in20 msp430g2553in20
NXP_BOARDS=lpc1114fn28
STM_BOARDS=bluepill
