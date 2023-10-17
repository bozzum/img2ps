# This file is part of img2ps.
# Copyright (c) 2022-2023, Patric Keller
# The code is made available under the MIT License, see LICENSE document

# To enable debug mode, do a "make DEBUG=1"

prj		= img2ps
ccOpt	= -DLODEPNG_NO_COMPILE_ENCODER

# paths & names
# ------------------------------------------------------------------------------
srcPath	?= src
incPath	?= inc
resPath	?= $(srcPath)/res
trgPath	?= bin
tmpPath	?= tmp

app		= $(trgPath)/$(prj)
srcs	= $(wildcard $(srcPath)/*.cpp)
ress	= $(wildcard $(resPath)/*.rc)
objs	= $(subst $(srcPath), $(tmpPath), $(srcs:.cpp=.o))
orcs	= $(subst $(resPath), $(tmpPath), $(ress:.rc=.ro))
deps	= $(subst $(srcPath), $(tmpPath), $(srcs:.cpp=.d))

# tools
# ------------------------------------------------------------------------------
CPP		= cpp
CXX		= g++
RC		= windres
LD		= g++
AWK		= awk
PK		= upx
CHK		= cppcheck
CP		= cp -f
RM		= rm -f
MD		= mkdir -p

# configuration
# ------------------------------------------------------------------------------
ccOpt	+= -Wall -Wextra -Wpedantic -Wformat=2 -Wnull-dereference \
		   -Wlogical-op -Winline -Wshadow
ccOpt	+= -I $(incPath) -I $(srcPath)
rcOpt	+= -O coff --preprocessor=$(CPP) -I $(incPath) -I $(srcPath) -I $(resPath)

ifdef DEBUG
ccOpt	+= -g -g3 -DDEBUG
else
ccOpt	+= -O3 -s -static -DNDEBUG
endif

ifeq ($(OS),Windows_NT)
objs	+= $(orcs)
app		:= $(addsuffix .exe, $(app))
endif

# targets
# ------------------------------------------------------------------------------
.SUFFIXES:
.PHONY: all release clean check

all: $(app)

release: clean all
ifneq (, $(shell which $(PK)))
	@echo [upx] $(app)
	@$(PK) --best --brute -q --no-color $(app)
else
	@echo "***** No '$(CHK)' installed... *****"
endif
	@echo [rm ] $(tmpPath)/
	@$(RM) -r -d $(tmpPath)/

clean:
	@echo [rm ] $(tmpPath)/
	@$(RM) -r -d $(tmpPath)/

spotless: clean
	@$(RM) $(app)

check:
ifneq (, $(shell which $(CHK)))
	@echo [chk] $(prj)
	@$(CHK) --project=$(prj).cppcheck --enable=all
else
	@echo "***** No '$(CHK)' installed... *****"
endif

# implicit rules
# ------------------------------------------------------------------------------
.PRECIOUS: $(tmpPath)/%.o $(tmpPath)/%.ro

# list based targets
# ------------------------------------------------------------------------------
$(app): $(objs) ; @-$(MD) $(dir $@)
	@$(AWK) -f $(resPath)/build.awk $(resPath)/build.num > $(tmpPath)/build.num
	@$(CP) -f $(tmpPath)/build.num $(resPath)/build.num
	@echo [ld ] $@
	@$(LD) $(ccOpt) -o $@ $^

# single object targets
# ------------------------------------------------------------------------------
$(tmpPath)/%.o: $(srcPath)/%.cpp $(MAKEFILE_LIST) ; @-$(MD) $(dir $@)
	@echo [c++] $<
	@$(CXX) $(ccOpt) -std=c++17 -c -o $@ $<
	@$(CXX) -I $(incPath) -I $(srcPath) -MM -MG -MP -MT $@ -MF $(tmpPath)/$*.d $<

$(tmpPath)/%.ro: $(resPath)/%.rc $(MAKEFILE_LIST) ; @-$(MD) $(dir $@)
	@echo [rc ] $<
	@$(RC) $(rcOpt) $< $@

# dependencies
# ..............................................................................

-include $(deps)
