
NAME=dollar

default: $(NAME).o

.DEFAULT spec clean bin:
	$(MAKE) -C tmp/ $@ NAME=$(NAME)

# copy up-to-date version of dep libs into src/
#
stamp:
	cd $(REP) && git log -n 1 | sed 's/^/\/\//' >> ../$(NAME)/$(FIL)
upgrade:
	cp -v ../aabro/src/*.[ch] src/
	cp -v ../flutil/src/flutil.[ch] src/
	find src/aabro.* -exec $(MAKE) stamp REP=../aabro FIL={} \;
	find src/flutil.* -exec $(MAKE) stamp REP=../flutil FIL={} \;

cs: clean spec

.PHONY: spec clean upgrade cs bin

