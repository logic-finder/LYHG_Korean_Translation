# Settings
SHELL := /bin/sh
CC := gcc
CFLAGS := -O -Wall -W -pedantic -g
SUFFIXES :=
SUFFIXES := .c .o .h

# Compilations
### This is the default goal. ###
htmlgen: htmlgen.o
	$(CC) $^ $(CFLAGS) -o $@

htmlgen.o: htmlgen.h

# Miscellaneous
srcdir := ../raw
destdir := ../generated
gen_cmd = ./htmlgen $(srcdir)/$1.txt $(destdir) $1.html
test := echo asdf

.PHONY: gen
# Its value is assigned from the command line.
name =
gen:
	@echo name=$(name)
	$(call gen_cmd,$(name))

.PHONY: genall
txt_files := $(notdir $(wildcard $(srcdir)/*.txt))
_txt_files := $(txt_files:%.txt=%)
genall:
	@set -e; \
	 for i in $(_txt_files); do \
		 echo name=$$i; \
		 echo $(call gen_cmd,$$i); \
		 $(call gen_cmd,$$i); \
		 echo ""; \
	 done 