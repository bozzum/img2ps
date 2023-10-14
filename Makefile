TARGET		= img2ps.exe
OBJS		= main.o lodepng.o jpgd.o loadfile.o chromaproc.o \
			  analyse.o lumaproc.o emitstats.o dither.o emitps.o
RESS		= main.ro
LIBS		=
CC_OPT		= -DLODEPNG_NO_COMPILE_ENCODER

# paths & names
# ------------------------------------------------------------------------------
CC_PATH		?= C:/msys64/mingw64/bin
TOOL_PATH	?= C:/Tools
SYS_PATH	?= C:/msys64/usr/bin

SRC_PATH	?= src
INC_PATH	?= inc
RES_PATH	?= $(SRC_PATH)/res
TRG_PATH	?= bin

TMP			= tmp
TMP_PATH	= $(TMP)
OBJS_PATH	= $(TMP_PATH)
RESS_PATH	= $(TMP_PATH)

OBJS_RS		= $(addprefix $(OBJS_PATH)/,$(OBJS))
RESS_RS		= $(addprefix $(RESS_PATH)/,$(RESS))

# tools
# ------------------------------------------------------------------------------
CPP	= $(CC_PATH)/cpp
CC	= $(CC_PATH)/gcc
CXX	= $(CC_PATH)/g++
RC	= $(CC_PATH)/windres
AR	= $(CC_PATH)/ar
LD	= $(CC_PATH)/g++
AWK	= $(TOOL_PATH)/awk
PK	= $(TOOL_PATH)/upx --best --no-color
CP	= $(SYS_PATH)/cp -f
RM	= $(SYS_PATH)/rm -f
MD	= $(SYS_PATH)/mkdir -p
RD	= $(SYS_PATH)/rmdir -p

# configuration
# ------------------------------------------------------------------------------
CC_OPT		+= -Wall -Wextra -Wpedantic -I $(INC_PATH) -I $(SRC_PATH)
RESC_OPT	+= -O coff --preprocessor=$(CPP) -I $(INC_PATH) -I $(SRC_PATH) -I $(RES_PATH)

ifdef DEBUG
CC_OPT		+= -g -g3 -DDEBUG
else
CC_OPT		+= -s -Ofast
endif

CC_OPT		+= -mconsole

AWK_BUILD	= "{if($$2 ~ /BUILD_NUM/) {$$3 = $$3 + 1};print $$0;}"

# targets
# ------------------------------------------------------------------------------
.SUFFIXES:
.PHONY: all release clean

all: $(TMP_PATH) $(TRG_PATH) $(TRG_PATH)/$(TARGET)

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
%.exe: $(OBJS_RS) $(RESS_RS) $(LIBS)
	@$(AWK) -f $(RES_PATH)/build.awk $(RES_PATH)/build.num > $(TMP_PATH)/build.num
	@$(CP) -f $(TMP_PATH)/build.num $(RES_PATH)/build.num
	@echo ld $@
	@$(LD) $(CC_OPT) -o $@ $^

$(LIBS): ;

# single object targets
# ------------------------------------------------------------------------------
$(TMP_PATH)/%.o: $(SRC_PATH)/%.c $(MAKEFILE_LIST) ; @-$(MD) $(dir $@)
	@echo cc $<
	@$(CC) $(CC_OPT) -std=c99 -c -o $@ $<

$(TMP_PATH)/%.o: $(SRC_PATH)/%.cpp $(MAKEFILE_LIST) ; @-$(MD) $(dir $@)
	@echo c++ $<
	@$(CXX) $(CC_OPT) -std=c++20 -c -o $@ $<

$(TMP_PATH)/%.ro: $(RES_PATH)/%.rc $(MAKEFILE_LIST) ; @-$(MD) $(dir $@)
	@echo rc $<
	@$(RC) $(RESC_OPT) $< $@

# structure targets
# ------------------------------------------------------------------------------
$(TMP_PATH):
	@echo md $(dir $@)
	@-$(MD) $(dir $@)

$(TRG_PATH):
	@echo md $(dir $@)
	@-$(MD) $(dir $@)
