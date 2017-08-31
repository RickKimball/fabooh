#--------------------------------------------------
# common_board.mk - select the board here
#
BOARD ?= bluepill
BOARDDIR = board/$(BOARD)
BOARDS = $(notdir $(wildcard $(FBD)board/*))
