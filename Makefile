TARGET		= img2ps
OBJS		= main.o lodepng.o jpgd.o loadfile.o chromaproc.o \
			  analyse.o lumaproc.o emitstats.o dither.o emitps.o
RESS		= main.ro

CC_OPT		= -DLODEPNG_NO_COMPILE_ENCODER

# paths & names
# ------------------------------------------------------------------------------
SRC_PATH	?= src
INC_PATH	?= inc
RES_PATH	?= $(SRC_PATH)/res
TRG_PATH	?= bin

TMP_PATH	= tmp
OBJS_PATH	= $(TMP_PATH)
RESS_PATH	= $(TMP_PATH)

OBJS_RS		= $(addprefix $(OBJS_PATH)/,$(OBJS))
RESS_RS		= $(addprefix $(RESS_PATH)/,$(RESS))

# tools
# ------------------------------------------------------------------------------
CPP	= cpp
CXX	= g++
RC	= windres
LD	= g++
AWK	= awk
PK	= upx --best --no-color
CP	= cp -f
RM	= rm -f
MD	= mkdir -p

# configuration
# ------------------------------------------------------------------------------
CC_OPT		+= -Wall -Wextra -Wpedantic -I $(INC_PATH) -I $(SRC_PATH)
RESC_OPT	+= -O coff --preprocessor=$(CPP) -I $(INC_PATH) -I $(SRC_PATH) -I $(RES_PATH)

ifdef DEBUG
CC_OPT		+= -g -g3 -DDEBUG
else
CC_OPT		+= -s -O3 -static
endif

ifeq ($(OS),Windows_NT)
OBJS_RS		+= $(RESS_RS)
endif

AWK_BUILD	= "{if($$2 ~ /BUILD_NUM/) {$$3 = $$3 + 1};print $$0;}"

# targets
# ------------------------------------------------------------------------------
.SUFFIXES:
.PHONY: all release clean

all: $(TRG_PATH)/$(TARGET)

release: clean all
	-$(PK) $(TRG_PATH)/$(TARGET)

clean:
	-$(RM) $(OBJS_PATH)/*.o
	-$(RM) $(RESS_PATH)/*.ro
	-$(RM) -r $(TMP_PATH)/*
	-$(RM) $(TRG_PATH)/$(TARGET)

# implicit rules
# ------------------------------------------------------------------------------
.PRECIOUS: $(TMP_PATH)/%.o $(TMP_PATH)/%.ro

# list based targets
# ------------------------------------------------------------------------------
$(TRG_PATH)/$(TARGET): $(OBJS_RS) ; @-$(MD) $(dir $@)
	@$(AWK) -f $(RES_PATH)/build.awk $(RES_PATH)/build.num > $(TMP_PATH)/build.num
	@$(CP) -f $(TMP_PATH)/build.num $(RES_PATH)/build.num
	@echo ld $@
	@$(LD) $(CC_OPT) -o $@ $^

# single object targets
# ------------------------------------------------------------------------------
$(TMP_PATH)/%.o: $(SRC_PATH)/%.cpp $(MAKEFILE_LIST) ; @-$(MD) $(dir $@)
	@echo c++ $<
	@$(CXX) $(CC_OPT) -std=c++17 -c -o $@ $<

$(TMP_PATH)/%.ro: $(RES_PATH)/%.rc $(MAKEFILE_LIST) ; @-$(MD) $(dir $@)
	@echo rc $<
	@$(RC) $(RESC_OPT) $< $@
