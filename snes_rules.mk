#---------------------------------------------------------------------------------
# path to tools
#---------------------------------------------------------------------------------
export PATH	:= $(PVSNESLIB_HOME)/devkitsnes/bin:$(PATH)
LIBDIRSOBJS  := $(PVSNESLIB_HOME)/pvsneslib/lib

#---------------------------------------------------------------------------------
# on windows, linkfile can only manage path like E:\pvsneslib\lib\crt0_snes.obj
# this one doesn't work /e/pvsneslib/lib/crt0_snes.obj
#---------------------------------------------------------------------------------
ifeq ($(OS),Windows_NT)
DRIDIROBJW   := $(wordlist 1,1,$(subst /, ,${LIBDIRSOBJS}))
REPDIROBJSW  := $(subst /,\\,${LIBDIRSOBJS})
REPDIROBJSW1 := $$(echo "$(REPDIROBJSW)" | sed 's/^.\{3\}//')
LIBDIRSOBJSW := ${DRIDIROBJW}:\\${REPDIROBJSW1}
else
LIBDIRSOBJSW := ${LIBDIRSOBJS}
endif

#---------------------------------------------------------------------------------
# the compiler executables
#---------------------------------------------------------------------------------

export CC	:=	816-tcc
export TA	:=	tasm
export AS	:=	wla-65816
export LD	:=	wlalink
export PY	:=	816-opt.py

export CTF	:=	constify

export GFXCONV	:=	$(PVSNESLIB_HOME)/devkitsnes/tools/gfx2snes
export SMCONV	:=	$(PVSNESLIB_HOME)/devkitsnes/tools/smconv
export BRCONV	:=	$(PVSNESLIB_HOME)/devkitsnes/tools/snesbrr
export TXCONV	:=	$(PVSNESLIB_HOME)/devkitsnes/tools/bin2txt
export SNTOOLS	:=	$(PVSNESLIB_HOME)/devkitsnes/tools/snestools

export EMU	:=	$(PVSNESLIB_HOME)/emulators/zsnes/ZSNESW

#---------------------------------------------------------------------------------
# Add default flag for compiling
#---------------------------------------------------------------------------------
CFLAGS += -I$(PVSNESLIB_HOME)/pvsneslib/include -I${PVSNESLIB_HOME}/devkitsnes/include -I$(CURDIR)


#---------------------------------------------------------------------------------
# Search all .asm and .c files until 3rd level in the tree 
#---------------------------------------------------------------------------------
SRC = src
CFILES = $(wildcard *.c)
CFILES+= $(wildcard $(SRC)/*.c)
CFILES+= $(wildcard $(SRC)/*/*.c)
CFILES+= $(wildcard $(SRC)/*/*/*.c)
SFILES = $(wildcard *.asm)
SFILES += $(wildcard $(SRC)/*.asm)
SFILES += $(wildcard $(SRC)/*/*.asm)
SFILES += $(wildcard $(SRC)/*/*/*.asm)
export OFILES	:=	$(BINFILES:.bin=.obj) $(CFILES:.c=.obj) $(SFILES:.asm=.obj)

# The first rule available in makefile become the default one
# We want "all" as default rule, but we will declare it later !
.DEFAULT_GOAL := all

#---------------------------------------------------------------------------------
%.ps: %.c
	@echo Compiling to .ps ... $(notdir $<)
	$(CC) $(CFLAGS) -Wall -c $< -o $@

.PRECIOUS: %.asm
#---------------------------------------------------------------------------------
# for libc.asm file only, we need to use -x flag of wla dx to create labels SECTIONSTART_[section name]
# and SECTIONEND_[sectionname] at the beginning and end of each section.
#---------------------------------------------------------------------------------
%.obj: %.asm
	@echo Doing obj files ... $(fname)
	@CURRENTFILE=$<; \
		if [ $$CURRENTFILE = libc.asm ]; then \
		echo "Building with -x flag: $(AS) -s -x -o $@ $<"; \
		$(AS) -s -x -o $@ $<; \
	else \
		echo "Building: $(AS) -s -o $@ $<"; \
		$(AS) -s -o $@ $<; \
	fi

#---------------------------------------------------------------------------------
%.asm: %.ps
	@echo Assembling ... $(notdir $<)
	$(PY) $< >$*.asp 
	@echo Moving constants ... $(notdir $<)
	$(CTF) $*.c $*.asp $@	
	@rm $*.asp

#---------------------------------------------------------------------------------
%.sfc: $(OFILES)
	@echo Creating linkfile ...
	@echo [objects] > linkfile
	@for i in $(OFILES); do \
		echo $$i >> linkfile; \
	done

	@for i in $(shell ls $(LIBDIRSOBJSW)); do \
		echo $(LIBDIRSOBJSW)\\$$i >> linkfile; \
	done
        
	@echo Linking ... $(notdir $@)
	$(LD) -d -D -s -v -L ${LIBDIRSOBJS} linkfile $@

	@sed 's/://' <$(ROMNAME).sym >$(ROMNAME).tmp
	@mv $(ROMNAME).tmp $(ROMNAME).sym
		
	@echo
	@echo Build finished successfully !
	@echo

#---------------------------------------------------------------------------------
%.pic: %.bmp
	@echo convert bitmap ... $(notdir $<)
	$(GFXCONV) -pr -pc16 -n -gs8 -pe0 -fbmp -m $<

#---------------------------------------------------------------------------------
%.brr: %.wav
	@echo convert wav file ... $(notdir $<)
	$(BRCONV) -e $< $@

.PHONY: cleanBuildRes cleanRom cleanGfx

cleanBuildRes:
	@echo clean build resources
	@rm -f $(OFILES) linkfile

cleanRom:
	@echo clean rom
	@rm -f $(ROMNAME).sfc $(ROMNAME).sym
	
cleanGfx:
	@echo clean graphics data
	@rm -f *.pic *.map *.pal *.pc7 *.mp7
