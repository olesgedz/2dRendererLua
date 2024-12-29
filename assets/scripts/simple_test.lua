--- This is comment
test_variable = 6 * 7

if test_variable == 42 then
    print("The value of test_variable is: " .. test_variable)
end

function factorial(n)
    if n == 1 then
        return 1
    end
    return n * factorial(n - 1)
end