import sys

input = sys.stdin.readline

def grocery():
    """
    Adapted from https://codeforces.com/blog/entry/71884
    """
    data = []
    while True:
        line = input()
        if line:
            data += [list(map(int, line.split()))]
        else:
            break

    n, c, e = data[0]
    a = [data[i][0] for i in range(1, len(data))]
    b = [data[i][1] for i in range(1, len(data))]
    d = [[[0 for _ in range(e+1)] for _ in range(c+1)] for _ in range(n+1)]
    d[0][0][0] = 1

    for i in range(1, n+1):
        for j in range(c+1):
            for k in range(e+1):
                l = j - a[i-1]
                m = k - b[i-1]

                if (l < 0) or (m < 0):
                    d[i][j][k] = d[i-1][j][k]
                else:
                    d[i][j][k] = max(d[i-1][j][k], d[i-1][l][m])

    print('Yes\n' if d[n][c][e] else 'No\n') 


if __name__ == '__main__':
    grocery()