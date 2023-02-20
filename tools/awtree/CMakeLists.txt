
add_executable(awtree
	awtree.c++
	ast_printer.h
	ast_printer_default.c++
	ast_printer_default.h
	ast_printer_awdoc.h
	ast_printer_awdoc.c++
)

target_link_libraries(awtree
	PRIVATE
		aw::io
		aw::utility
		awscript-lang)
