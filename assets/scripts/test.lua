-- This is lua global variable
some_variable = 7 * 6

-- This is a lua table.
config = {
	title = "Game Engine",
	fullscreen = false,
	resolution = {
		width = 800,
		height = 600
	}
}

function factorial(n)
	if n == 0 then
		return 1
	else
		return n * factorial(n - 1)
	end
end

print(factorial(7))

print("The cube of 3 is ".. cube(3))