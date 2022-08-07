import sys

input = sys.stdin.readline

def kruskal():
    line = input()
    data = [list(map(int, line.split()))]
    while True:
        line = input()
        if line:
            line = line.split()
            line[:3] = list(map(int, line[:3]))
            data += [line]
        else:
            break
    
    n, m = data[0]
    
    if m == 0:
        print(0, 0, '\n')
        return

    happy_red, happy_blue = dict(), dict()
    for prop in data[1:]:
        edge = (prop[0], prop[1])
        if prop[3] == 'red':
            happy_red[edge] = prop[2]
        else:
            happy_blue[edge] = prop[2]
    
    edges_red = sorted(happy_red, key=happy_red.get, reverse=True)
    edges_blue = sorted(happy_blue, key=happy_blue.get, reverse=True)

    A = []
    p = list(range(n + 1))
    rank = [0 for _ in range(n + 1)]

    def find_set(x):
        while x != p[x]:
            x = p[x]
        return p[x]

    def link(x, y):
        if rank[x] > rank[y]:
            p[y] = x
        else:
            p[x] = y
            if rank[x] == rank[y]:
                rank[y] += 1

    def union(x, y):
        link(find_set(x), find_set(y))
        
    for (u, v) in edges_red + edges_blue:
        if find_set(u) != find_set(v):
            A += [(u, v)]
            union(u, v)

    res_red = sum(happy_red[edge] for edge in A if edge in happy_red)
    res_blue = sum(happy_blue[edge] for edge in A if edge in happy_blue)
    print(res_red, res_blue, '\n')
    

if __name__ == '__main__':
    kruskal()