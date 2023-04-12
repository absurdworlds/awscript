function main()
{
	// error: missing_comma
	// expect: (call test (1 2))
	test(1 2)

	// error: missing_closing_paren
	// expect: (call test (1))
	test(1;

	// error: missing_expression
	// expect: (var int x (if <invalid-expr> (then 1) (else 3)))
	var x: int = if then 1 else 3;

	// error: missing_operand
	// expect: (+ 2 2)
	2+2+;

	// error: missing_operand, illegal_token
	// expect: (+ 2 2)
	2+2+$;

	// error: missing_operand
	// expect: 2
	2 * *;

	// error: missing_expression
	// expect: (<invalid-expr>)
	();

	// error: missing_expression, missing_else
	// expect:  (var int y (if (<invalid-expr>) (then 1) (else 2)))
	var y: int = if () 1 2;

	// error: missing_expression, missing_else
	// expect:  (var int y (if (x) (then) (else)))
	var z: int = if (x);
}
