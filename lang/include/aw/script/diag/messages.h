#ifndef DIAG
#define DIAG(type, id, msg)
#endif

DIAG(error, generic_error, "%0")
DIAG(error, not_implemented_yet, "Not implemented yet: %0.")

DIAG(error, illegal_token, "Unexpected token ‘%0’.")
DIAG(error, unexpected_token, "Unexpected token: expected ‘%1’, got ‘%0’.")
DIAG(error, unexpected_eof, "Unexpected end of file while looking for ‘%0’.")
DIAG(error, expected, "Unexpected token ‘%0’: expected %1.")
DIAG(error, expected_variable_decl, "Unexpected token ‘%0’: expected variable declaration.")
DIAG(error, expected_initializer, "Unexpected token ‘%0’: expected initializer.")
DIAG(error, expected_identifier, "Unexpected token ‘%0’: expected identifier.")
DIAG(error, expected_expression, "Unexpected token ‘%0’: expected expression.")
DIAG(error, expected_a_statement, "Unexpected token ‘%0’: expected a statement.")
DIAG(error, expected_declaration, "Unexpected token ‘%0’: expected declaraion.")
DIAG(error, expected_type_name, "Unexpected token ‘%0’: expected type name.")
DIAG(error, expected_a_type, "Expected a type. ‘%0’ is not a type.")
DIAG(error, expected_semicolon_after_expression, "Expected ‘;’ after expression.")
DIAG(error, expected_semicolon_after_var, "Expected ‘;’ after variable declaration.")
DIAG(error, expected_semicolon_after_do_while, "Expected ‘;’ after do-while statement.")
DIAG(error, uninitialized_constant, "Constant must be initialized.")

DIAG(error, is_not_declared, "‘%0’ is not declared in this scope.")
DIAG(error, undefined_function, "Undefined function: ‘%0’.")
DIAG(error, undefined_variable, "Undefined variable: ‘%0’.")
DIAG(error, redefined_variable, "Redefined variable: ‘%0’.")
DIAG(error, is_not_a_struct_type, "Left-hand side ‘.’ operator is not a struct type.")
DIAG(error, identifier_required, "Right-hand side of the ‘.’ operator must be an identifier.")
DIAG(error, initializer_requires_a_type, "Variable ‘%0’ must have an explicit type.")
DIAG(error, no_such_field, "Struct ‘%0’ does not have a field named ‘%1’.")
DIAG(error, symbol_already_defined, "Symbol already defined: ‘%0’.")
DIAG(error, variadic_parameter, "Variadic parameter must be at the end of the parameter list.")
DIAG(error, not_enough_arguments, "Function parameter mismatch: ‘%0’. Expects: %1, Actual: %2")
DIAG(error, type_mismathch, "Type mismatch: ‘%0’ vs ‘%1’.")
DIAG(error, unknown_type, "Unknown type: ‘%0’.")

#undef DIAG
