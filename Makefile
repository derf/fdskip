include config.mk

all: src/skipr build-man

build-man:
	@${MAKE} -C man

src/skipr: src/main.c
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $<

install: bin/skipr
	@echo installing bin to ${bin_dir} and man to ${man_dir}
	@mkdir -p ${bin_dir} ${man_dir}/man1
	@cp src/skipr ${bin_dir}/skipr
	@cp src/skipr.1 ${man_dir}/man1/skipr.1
	@chmod 755 ${bin_dir}/skipr
	@chmod 644 ${man_dir}/man1/skipr.1

uninstall:
	rm -f ${bin_dir}/skipr
	rm -f ${man_dir}/man1/skipr.1

clean:
	@${MAKE} -C man clean
	rm -f src/skipr

.PHONY: all install uninstall clean build-man
