file(GLOB_RECURSE SOURCES *.aw)

add_custom_target(testfiles SOURCES ${SOURCES})

set(TESTS
	syntax/as.aw
	syntax/comments.aw
	syntax/if.aw
	syntax/operators.aw
	syntax/precedence.aw
	syntax/struct.aw
	syntax/subscript.aw
	errors/invalid_decls.aw
	errors/not_allowed_here.aw
	errors/not_implemented_yet.aw
	errors/recovery.aw
	codegen/array.aw
	codegen/array_of_structs.aw
	codegen/chain.aw
	codegen/function.aw
	modules/main.aw
)

set(NEGATIVE_TESTS
	errors/invalid_decls.aw
	errors/not_allowed_here.aw
	errors/not_implemented_yet.aw
	errors/recovery.aw
)

foreach (test ${TESTS})
	add_test (
		NAME ${test}
		COMMAND awscriptc -n ${test}
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	)
	set_property(TEST ${test} PROPERTY TIMEOUT 3)
endforeach ()

foreach (test ${NEGATIVE_TESTS})
	set_property(TEST ${test} PROPERTY WILL_FAIL TRUE)
endforeach ()
