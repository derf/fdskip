include config.mk

all: src/fdskip build-man

build-man:
	@${MAKE} -C man

src/fdskip: src/main.c
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $<

install: src/fdskip build-man
	@echo installing bin to ${bin_dir} and man to ${man_dir}
	@mkdir -p ${bin_dir} ${man_dir}/man1
	@cp src/fdskip ${bin_dir}/fdskip
	@cp man/fdskip.1 ${man_dir}/man1/fdskip.1
	@chmod 755 ${bin_dir}/fdskip
	@chmod 644 ${man_dir}/man1/fdskip.1

uninstall:
	rm -f ${bin_dir}/fdskip
	rm -f ${man_dir}/man1/fdskip.1

clean:
	@${MAKE} -C man clean
	rm -f src/fdskip

.PHONY: all install uninstall clean build-man
