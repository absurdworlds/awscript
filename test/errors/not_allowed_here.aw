foreign import: C {
	// error: not_allowed_here
	module test {
		func f();
	}
}

module test {
	// error: not_allowed_here
	module test;
	func f();
}
