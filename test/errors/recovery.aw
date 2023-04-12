function main()
{
/*
	// expect: (call test (1 2))
	test(1 2)

	// expect: (call test (1))
	test(1;

	// expect: (var int x (if <invalid-expr> (then 1) (else 3)))
	var x: int = if then 1 else 3;

	// expect: (+ 2 2)
	2+2+;

	// expect: (<invalid-expr>)
	();

	// expect:  (var int y (if (<invalid-expr>) (then 1) (else 2)))
	var y: int = if () 1 2;*/

	// expect:  (var int y (if (x) (then) (else)))
	var z: int = if (x);
}
