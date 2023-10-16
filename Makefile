target		= img2ps
objs		= main.o lodepng.o jpgd.o loadfile.o chromaproc.o \
			  analyse.o lumaproc.o emitstats.o dither.o emitps.o
ress		= main.ro

ccOpt		= -DLODEPNG_NO_COMPILE_ENCODER

# paths & names
# ------------------------------------------------------------------------------
srcPath		?= src
incPath		?= inc
resPath		?= $(srcPath)/res
trgPath		?= bin

tmpPath		= tmp
objsPath	= $(tmpPath)
ressPath	= $(tmpPath)

objsRes		= $(addprefix $(objsPath)/,$(objs))
ressRes		= $(addprefix $(ressPath)/,$(ress))

# tools
# ------------------------------------------------------------------------------
CPP	= cpp
CXX	= g++
RC	= windres
LD	= g++
AWK	= awk
PK	= upx --best --no-color
CHK	= cppcheck
CP	= cp -f
RM	= rm -f
MD	= mkdir -p

# configuration
# ------------------------------------------------------------------------------
ccOpt		+= -Wall -Wextra -Wpedantic -I $(incPath) -I $(srcPath)
RESC_OPT	+= -O coff --preprocessor=$(CPP) -I $(incPath) -I $(srcPath) -I $(resPath)

ifdef DEBUG
ccOpt		+= -g -g3 -DDEBUG
else
ccOpt		+= -s -O3 -static
endif

ifeq ($(OS),Windows_NT)
objsRes		+= $(ressRes)
endif

AWK_BUILD	= "{if($$2 ~ /BUILD_NUM/) {$$3 = $$3 + 1};print $$0;}"

# targets
# ------------------------------------------------------------------------------
.SUFFIXES:
.PHONY: all release clean check

all: $(trgPath)/$(target)

release: clean all
	-$(PK) $(trgPath)/$(target)

clean:
	-$(RM) $(objsPath)/*.o
	-$(RM) $(ressPath)/*.ro
	-$(RM) -r $(tmpPath)/*
	-$(RM) $(trgPath)/$(target)

check:
	-$(CHK) --project=$(target).cppcheck --enable=all

# implicit rules
# ------------------------------------------------------------------------------
.PRECIOUS: $(tmpPath)/%.o $(tmpPath)/%.ro

# list based targets
# ------------------------------------------------------------------------------
$(trgPath)/$(target): $(objsRes) ; @-$(MD) $(dir $@)
	@$(AWK) -f $(resPath)/build.awk $(resPath)/build.num > $(tmpPath)/build.num
	@$(CP) -f $(tmpPath)/build.num $(resPath)/build.num
	@echo ld $@
	@$(LD) $(ccOpt) -o $@ $^

# single object targets
# ------------------------------------------------------------------------------
$(tmpPath)/%.o: $(srcPath)/%.cpp $(MAKEFILE_LIST) ; @-$(MD) $(dir $@)
	@echo c++ $<
	@$(CXX) $(ccOpt) -std=c++17 -c -o $@ $<

$(tmpPath)/%.ro: $(resPath)/%.rc $(MAKEFILE_LIST) ; @-$(MD) $(dir $@)
	@echo rc $<
	@$(RC) $(RESC_OPT) $< $@
