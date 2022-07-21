aw_add_test(awscript_test_lang
	SOURCES
		lexer/lexer.c++)

target_link_libraries(awscript_test_lang
	PRIVATE
		awscript-lang
		aw::test
		aw::test-runner)
