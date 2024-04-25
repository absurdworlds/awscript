func test_precedence()
{
	var a = 0;
	var b = 0;
	var c = 0;
	var d = 0;
	var g = 0;

	// ast: (= a (- (+ b (+ c (* d (* g 2)))) 4))
	a = b + c + d * g * 2 - 4;
}

func calculate() : int
{
	return 0;
}

func test_rightassoc()
{
	var x : int;
	var y : int;
	var z : int;
	// ast: (= x (= y (= z (call calculate)))
	x = y = z = calculate();
}

func test_leftassoc()
{
	var x = 1;
	var y = 1;
	var z = 1;
	// ast: (- (- (- x y) z)
	x - y - z
}

func main()
{
	test_leftassoc()
	test_rightassoc()
	test_precedence()
}
