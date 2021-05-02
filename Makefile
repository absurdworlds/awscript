
default: ParserTest

LexerTest:
	@ $(MAKE) -C 'tests/lexer_test' $(TARGET)

ParserTest:
	@ $(MAKE) -C 'tests/parser_test' $(TARGET)

libraries: libawscript libawscript-utils
.PHONY: libraries

libawscript:
	@ $(MAKE) -C 'source/hrscript' $(TARGET)

libawscript-utils:
	@ $(MAKE) -C 'source/utils' $(TARGET)
