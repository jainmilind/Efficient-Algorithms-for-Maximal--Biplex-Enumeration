import sys
with open('pre_test1.txt', 'r') as f:
    s = f.read()

l = s.split()
print(l)
m = int(l[0])
left = int(l[1])
rght = int(l[2])
n = left + rght
adj = [[] for _ in range(n)]
for i in range(3, len(l), 2):
    x = int(l[i]) - 1
    y = int(l[i + 1]) + left - 1
    adj[x].append(y)
    adj[y].append(x)

ans = []
ans.append(str(n) + ' ' + str(left) + ' ' + str(m))

for i in range(n):
    adj[i].sort()
    ans.append(str(i))
    for x in adj[i]:
        ans[-1] += ' ' + str(x)
with open('test1.txt', 'w') as f:
    f.write('\n'.join(ans))
