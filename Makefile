ifeq ($(strip $(PVSNESLIB_HOME)),)
$(error "Please create an environment variable PVSNESLIB_HOME with path to its folder and restart application. (you can do it on windows with <setx PVSNESLIB_HOME "/c/snesdev">)")
endif

include snes_rules.mk
 
.PHONY: process all
 
#---------------------------------------------------------------------------------
# ROMNAME is used in snes_rules file
export ROMNAME := wordle

all: $(ROMNAME).sfc

gfx: preProcess

preProcess:
	python python/build.py

clean: cleanBuildRes cleanRom cleanGfx
	rm data -r
