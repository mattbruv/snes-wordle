
def printCols(cols):
    test = list(map(lambda x: hex(x*2), cols))
    print(test)


columns = list(map(lambda x: 0x1000 + (x * 480), range(0,34)))

print(columns)

left = columns[0:32]
left[0] = columns[32]
left[1] = columns[33]
print(len(left))
print(left)