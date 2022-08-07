import sys

input = sys.stdin.readline

def bfs():
    data = []
    while True:
        line = input()
        if line:
            data += [list(map(int, line.split()))]
        else:
            break

    n, m, _, s, t = data[0]
    
    if t == s:
        print('0\n')
        return

    s -= 1
    t -= 1
    a, b, c = data[1], [], []
    for i in range(2, len(data)):
        b += [data[i][0]]
        c += [data[i][1]]

    adj = [[] for _ in range(n+1)]
    for i in a:
        adj[i-1] += [n]
        adj[n] += [i-1]
    for i in range(m):
        adj[b[i]-1] += [c[i]-1]
        adj[c[i]-1] += [b[i]-1]

    cost = 1
    queue, visited = [], set()
    
    for i in adj[s]:
        if i == t:
            print(f'{cost}\n')
            return
        else:
            queue += [i]
            visited.add(i)
    
    while queue:
        cost += 1
        len_q = len(queue)
        
        for i in range(len_q):
            for j in adj[queue[i]]:
                if j == t:
                    print(f'{cost}\n')
                    return
                elif not (j in visited):
                    queue += [j]
                    visited.add(j)

        queue = queue[len_q:]

    print('Impossible\n')
    

if __name__ == '__main__':
    bfs()