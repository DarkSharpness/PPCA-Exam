

# generate 10 testpoints for subset sum problem, ensure there is a anwser
# for test 1,2,3, the number of elements are 10, 50, 100, and the value of each element is between 1 and 100
# for the others, the number of elements are 1000000, and the value is in long long
import random
import uuid

ns = [10, 50, 100, 1000, 100000, 1000000, 1000000, 1000000, 1000000, 1000000]
Vs = [100, 100, 100, 1000, 10000, 1000000000,
      1000000000, 1000000000, 1000000000, 1000000000]
for t in range(1, 11):
    n = ns[t-1]
    V = Vs[t-1]
    a = [random.randint(1, V) for i in range(n - 1)]
    s = random.randint(1, V)
    a.sort()
    s2 = 0
    for x in a:
        if s2 + x <= s:
            s2 += x
        else:
            break
    if s2 != s:
        a.append(s - s2)
    else:
        a.append(random.randint(1, V))
    random.shuffle(a)

    random_uuid = uuid.uuid4()
    with open(f"{t}.in", "w") as f:
        f.write(f"{n} {s}\n")
        f.write(" ".join(map(str, a)))
        f.write("\n")
        f.write(f"{random_uuid}\n")
