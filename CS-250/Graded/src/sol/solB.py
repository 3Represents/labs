from collections import deque

n, m, k, s, t = list(map(int, input().split()))
s -= 1
t -= 1
portals = list(map(lambda x: int(x) - 1, input().split()))

graph = [list() for i in range(n + 1)]

for i in range(m):
    a, b = list(map(int, input().split()))
    a -=1
    b -= 1
    graph[a].append(b)
    graph[b].append(a)

for a in portals:
    graph[a].append(n)
    graph[n].append(a)

q = deque()
q.append(s)

dist = [-1 for i in range(n + 1)]
dist[s] = 0


while q:
    v = q.popleft()
    for u in graph[v]:
        if dist[u] == -1:
            dist[u] = dist[v] + 1
            q.append(u)
if dist[t] == -1:
    print("Impossible")
else:
    print(dist[t])
