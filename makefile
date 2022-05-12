
# vim: set tabstop=4 smarttab

CC ?= gcc

buildtype ?= release

CPPFLAGS += -Wfatal-errors
CPPFLAGS += -I ./src
CPPFLAGS += -I ./vendor/liburing-liburing-2.1/src/include/

CXXFLAGS += -Wall -Wextra -Werror
CXXFLAGS += -std=c++2a
CXXFLAGS += -fconcepts

ifeq ($(buildtype), release)
CPPFLAGS += -D RELEASE

CXXFLAGS += -O2
CXXFLAGS += -flto

LDFLAGS += -static
else
CPPFLAGS += -D DEBUGGING
CXXFLAGS += -O0
CXXFLAGS += -g
CXXFLAGS += -Wno-unused-variable
CXXFLAGS += -Wno-unused-but-set-variable
CXXFLAGS += -Wno-unused-function
CXXFLAGS += -Wno-unused-parameter
endif

LDLIBS += -L ./vendor/liburing-liburing-2.1/src
LDLIBS += -luring

on_error ?= do_nothing

ifeq ($(on_error), do_nothing)
ON_ERROR =
else ifeq ($(on_error), open_editor)
ON_ERROR += || ($$EDITOR $<; false)
else
$(error "invalid on_error option!");
endif

buildprefix ?= build/cc-$(CC)/$(buildtype)

LDLIBS += -lstdc++

all: $(buildprefix)/pool

.PRECIOUS: %/

%/:
	@ mkdir -p $@

build/srclist.mk: | build/
	find -name '*.cpp' ! -path '*/junk/*' | sed 's/^/srcs += /' > $@

include build/srclist.mk

# ARGS += -i examples/foobar.zd

run: $(buildprefix)/pool
	$< $(ARGS)

valrun: $(buildprefix)/pool
	valgrind --gen-suppressions=yes --suppressions=./stl-val.supp --track-origins=yes --keep-debuginfo=yes $< $(ARGS)

$(buildprefix)/pool: $(patsubst %.cpp,$(buildprefix)/%.o,$(srcs))
	@ echo "linking $@"
	@ $(CC) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@

$(buildprefix)/%.o $(buildprefix)/%.d: %.cpp | $(buildprefix)/%/
	@ echo "compiling $<"
	@ $(CC) -c $(CPPFLAGS) $(CXXFLAGS) $< -MD -o $(buildprefix)/$*.o $(ON_ERROR)

https//%: | https/
	@ mkdir -p "$(@D)"
	wget --no-use-server-timestamps -O "$@" https://$*

fetch: https//github.com/axboe/liburing/archive/refs/tags/liburing-2.1.zip | vendor/
	unzip -o -qq $< -d $|
	make -C $|/liburing-liburing-2.1 -j8

format:
	find -name '*.cpp' -o -name '*.hpp' | xargs -d \\n clang-format -i --verbose

clean:
	rm -rf build pool

distclean: clean
	# Let me keep my vscode project level config
	for l in $$(cat .gitignore); do if [ $$l != ".vscode" ]; then rm -rf $$l; fi done

ifneq "$(MAKECMDGOALS)" "fetch"
include $(patsubst %.cpp,$(buildprefix)/%.d,$(srcs))
endif







