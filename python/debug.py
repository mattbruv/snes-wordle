
from pickle import SHORT_BINBYTES


f = open("../data/lake_01.map", "rb")
data = list(f.read())
print(data)

# IT LOOKS LIKE INFORMATION IS STORED IN LITTLE ENDIANN!!

def genMap():
    data = []
    for y in range(0, 30):
        for x in range(0, 32):
            n = 1 << 5
            n = n << 8
            index = y + x * 30
            n |= index
            print(x, y, n, index, "{0:16b}".format(n))
            high = n >> 8
            low = n & 0x00ff
            data.append(low)
            data.append(high)
    return data


mapData = genMap()
print(mapData)