
default: ParserTest

LexerTest:
	@ $(MAKE) -C 'tests/LexerTest'

ParserTest:
	@ $(MAKE) -C 'tests/ParserTest'

