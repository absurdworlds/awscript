aw_add_test(test_lang
	SOURCES
		lexer/lexer.c++
		parser/comments.c++
		parser/parser.c++
		semantic/resolve_deps.c++
)

target_link_libraries(test_lang
	PRIVATE
		awscript-lang
		aw::string
		aw::test
		aw::test-runner)
