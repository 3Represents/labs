def read_int_list():
    return list(map(int, input().split()))

n, c, e = read_int_list()
dp = [[[0 for i in range(e + 1)] for j in range(c + 1)] for k in range(n + 1)]
for i in range(n + 1):
    dp[i][0][0] = 1
products = []
for i in range(n):
    products.append(read_int_list())

for i in range(n):
    for j in range(c + 1):
        for k in range(e + 1):
            if dp[i][j][k]:
                dp[i + 1][j][k] = 1
            if j >= products[i][0] and k >= products[i][1] and dp[i][j - products[i][0]][k - products[i][1]]:
                dp[i + 1][j][k] = 1

if dp[n][c][e]:
    print("Yes")
else:
    print("No")
