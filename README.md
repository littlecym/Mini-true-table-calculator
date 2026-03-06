### Brute force true table calculator
This is a mini true table brute force calculator.

#### Example
input
```C++
(p|q|r)&(p|!q|!s)&(q|!r|s)&(!p|r|s)&(!p|q|!s)&(p|!q|!r)&(!p|!q|s)&(!p|!r|!s)
```
output
```C++
4 var(s).
p       q       r       s       result
0       0       0       0       0
1       0       0       0       0
0       1       0       0       1
1       1       0       0       0
0       0       1       0       0
1       0       1       0       0
0       1       1       0       0
1       1       1       0       0
0       0       0       1       0
1       0       0       1       0
0       1       0       1       0
1       1       0       1       1
0       0       1       1       1
1       0       1       1       0
0       1       1       1       0
1       1       1       1       0
```

input
```C++
((p->q)->(r->s))<->((p->r)->(q->s))
```
output
```C++
4 var(s).
p       q       r       s       result
0       0       0       0       1
1       0       0       0       1
0       1       0       0       0
1       1       0       0       1
0       0       1       0       0
1       0       1       0       1
0       1       1       0       1
1       1       1       0       1
0       0       0       1       1
1       0       0       1       1
0       1       0       1       1
1       1       0       1       1
0       0       1       1       1
1       0       1       1       1
0       1       1       1       1
1       1       1       1       1
```