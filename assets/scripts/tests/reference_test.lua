print(cppVar)

counter = 0

function init()
    print("Init function called")
end

function update()
    counter = counter + 1
    print("Update function called " .. counter)
end
