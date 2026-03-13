// Function: calculate
// Purpose: Tests parameter passing (positive stack offsets) 
// and arithmetic precedence (mixed ADD/MUL).
func calculate(x : integer, y : integer) => integer {
    local a : integer;
    local b : integer;
    a := x + y;
    b := a * 10;
    return (b);
    // Logic: Result = (x + y) * 10
    // If precedence works, we get (3+4)*10 = 70
    // If precedence fails, we might get 3+(4*10) = 43
}

func main() => void{
    local a : integer;
    local b : integer;
    local result : integer;

    a := 15;
    b := 42;

    // Test Call
     

    // Expected Output: 70
    write(calculate(a, b));
}