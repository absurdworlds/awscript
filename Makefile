
default: ParserTest

LexerTest:
	@ $(MAKE) -C 'tests/lexer_test' $(TARGET)

ParserTest:
	@ $(MAKE) -C 'tests/ParserTest' $(TARGET)

libraries: libhrscript libhrscript-utils
.PHONY: libraries

libhrscript:
	@ $(MAKE) -C 'source/hrscript' $(TARGET)

libhrscript-utils:
	@ $(MAKE) -C 'source/utils' $(TARGET)
