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
	codegen/array.aw
	codegen/chain.aw
	codegen/function.aw
	modules/main.aw
)

set(NEGATIVE_TESTS
	errors/recovery.aw
	errors/not_allowed_here.aw
)

foreach (test ${TESTS})
	add_test (
		NAME ${test}
		COMMAND awscriptc -n ${test}
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	)
endforeach ()

foreach (test ${NEGATIVE_TESTS})
	add_test (
		NAME ${test}
		COMMAND awscriptc -n ${test}
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	)
	set_property(TEST ${test} PROPERTY WILL_FAIL TRUE)
endforeach ()

