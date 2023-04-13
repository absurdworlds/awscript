function expressions()
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

	// error: missing_operand, stray_token
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

function statements()
{
	// error: missing_expression
	// expect: (while <invalid-expr> (do (stmt-list (+= x 10))))
	var x: int = 0;
	while { x += 10; }

	// error: missing_expression
	// expect: (while <invalid-expr> (do (stmt-list (+= x 10))))
	while;
}

function decls()
{
	function();

	// error: stray_token, missing_expression
	// (func void test1 (fn-args) (fn-body))
	function test1(*);

	// error: stray_token, missing_expression
	// (func void test2 (fn-args (var int x) (fn-body))
	function test2 var x: int);

	// error: stray_token, missing_comma
	// (func void test3 (fn-args (var <inferred type> x) (var <inferred type> y) (fn-body))
	function test3 (var x var y);
}

