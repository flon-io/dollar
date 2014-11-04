
NAME=dollar

default: $(NAME).o

.DEFAULT spec clean bin:
	$(MAKE) -C tmp/ $@ NAME=$(NAME)

# copy up-to-date version of dep libs into src/
#
upgrade:
	cp -v ../aabro/src/*.[ch] src/
	cp -v ../flutil/src/*.[ch] src/

cs: clean spec

.PHONY: spec clean upgrade cs bin

