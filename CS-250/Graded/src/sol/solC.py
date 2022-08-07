def read_int_list():
    return list(map(int, input().split()))

n, m = read_int_list()

parent = [i for i in range(n)]

def find(v):
    if parent[v] != v:
        u = find(parent[v])
        parent[v] = u
        return u
    else:
        return v

def union(v, u):
    v = find(v)
    u = find(u)
    if v == u:
        return
    parent[u] = v

edges = []
for i in range(m):
    l = input().split()
    a, b, c = list(map(int, l[:-1]))
    color = 1 if l[3] == 'red' else -1
    edges.append((a - 1, b - 1, c, color))

edges.sort(key=lambda x: (-x[3], -x[2]))

red_cost = 0
blue_cost = 0
for (a, b, c, color) in edges:
    a = find(a)
    b = find(b)
    if a != b:
        union(a, b)
        if color == 1:
            red_cost += c
        else:
            blue_cost += c

print(red_cost, blue_cost)