A = [[]]
B = [[]]

ra = 18
ca = 16

rb = 16
cb = 18

for i in range(ra):
    A.append([])
    for j in range(ca):
        A[i].append(i + j)

for i in range(rb):
    B.append([])
    for j in range(cb):
        B[i].append(i + 2*j)