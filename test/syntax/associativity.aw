// Operator associativity test.
// If parsed correctly, both statements in each function
// should be parsed to the same AST
function rightassoc()
{
	x = y = z = calculate();
	x = (y = (z = calculate()));
}
function leftassoc()
{
	x - y - z - calculate();
	((x - y) - z) - calculate();
}
