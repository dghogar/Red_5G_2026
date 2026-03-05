Q = @

MODE :=

LIBDIRS:= \
	test_helpers \
	sdk \
	qrtr \
	tqcm

GOBINETDIRS:= \
	sdk/gobiusbnet_examples

TESTDIRS:= \
	sdk/examples \
	sdk/mock_tests \
	tqcm/examples \
	tqcm/examples/data_connection_example \
	tqcm/examples/multiple_rmnet_example

export HAVE_STRLCPY := $(shell \
  echo '#include <string.h>' > conftest.c; \
  echo 'int main(void) { char buf[10]; strlcpy(buf, "test", sizeof(buf)); return 0; }' >> conftest.c; \
  $(CC) $(CFLAGS) -o conftest conftest.c 2>/dev/null && echo 1 || echo 0; \
  rm -f conftest conftest.c)

ifeq ($(HAVE_STRLCPY),1)
   export CFLAGS = -DHAVE_STRLCPY
endif

all: $(LIBDIRS)
examples: $(LIBDIRS) $(TESTDIRS)
examples_static: MODE += static
examples_static: clean $(LIBDIRS) $(TESTDIRS)
gobi_examples: $(LIBDIRS) $(GOBINETDIRS)

stack: MODE += stack
stack: $(LIBDIRS)

static: MODE += static
static: $(LIBDIRS)

.PHONY:  $(LIBDIRS) $(GOBINETDIRS) $(TESTDIRS)
$(LIBDIRS) $(GOBINETDIRS) $(TESTDIRS):
	$(Q)$(MAKE) $(MODE) -C $@

.PHONY: clean
clean:
	$(Q)rm -f telit_qmi_sdk.tag
	$(Q)rm -rf latex libs
	$(Q)for dir in $(LIBDIRS) $(GOBINETDIRS) $(TESTDIRS); do \
		$(MAKE) -C $$dir clean; \
	done

.PHONY: docs
docs:
	$(Q)./makedocs.sh

.PHONY: clean_docs
clean_docs:
	$(Q)rm -rf docs

.PHONY: install
install:
	$(Q)for d in $(LIBDIRS); do \
		make -C $$d install ; \
	done

.PHONY: uninstall
uninstall:
	$(Q)for d in $(LIBDIRS); do \
		make -C $$d uninstall ; \
	done
