- You have `S` dollars
- The first game costs `P`
- Each game after that costs `D` less dollars
- The minimum cost of a game is `M`, it cannot go below this
- How many games can you buy?

```cpp
int howManyGames(int p, int d, int m, int s) {
	int games = 0;
	
	while (s >= p) {
		games += 1;
		s -= p;
		p -= d;
		p = std::max(p, m);
	}
	
	return games;
}
```

This is bad though, it can be done with math. Ignoring that `m` for now...

$$
\begin{aligned}
g_1 &= p \\
g_2 &= p - d \\
g_3 &= p - 2d \\
g_n &= p - d(n - 1)
\end{aligned}
$$

The formula $g_n = p-d(n-1)$ can be used to find the cost of game `n`.

$$
\begin{aligned}
cost &= \sum_{i=1}^{n} p - d(i - 1) \\
&= n \cdot p -\sum_{i=1}^{n} d(i - 1) \\
&= n \cdot p - d \sum_{i=1}^{n} (i - 1) \\
&= n \cdot p - d( \sum_{i=1}^{n} i - \sum_{i=1}^{n}1) \\
&= n \cdot p - d( \sum_{i=1}^{n} i - n) \\
&= n \cdot p + n \cdot d - d\sum_{i=1}^{n} i \\
\end{aligned}
$$

$\sum_{i = 1}^{n} i$ is just $\frac{n(n+1)}{2}$

$$
\begin{aligned}
cost &= n \cdot p + n \cdot d - d\sum_{i=1}^{n} i \\
&= n \cdot p + n \cdot d - d\frac{n(n+1)}{2} \\
&= n \cdot p + n \cdot d - \frac{d}{2}n^2 - \frac{d}{2}n \\
&= - \frac{d}{2}n^2 + n \cdot p + n \cdot d - \frac{d}{2}n \\
&= - \frac{d}{2}n^2 + (p + \frac{d}{2})n
\end{aligned}
$$

We now have that $cost = - \frac{d}{2}n^2 + (p + \frac{d}{2})n$; a mathematical relationship between `cost` and `n`. Now to solve the problem, we will:
- determine for what `n` we reach `m`'s
- determine the `cost` of reaching this `n`
- if `s` is more than this, subtract it and the rest is trivial
- if `s` is less than this, use our formula to computer `n` given `cost = s`, and round down

We'll dive a bit more into that formula:
$$
\begin{aligned}
- \frac{d}{2}n^2 + (p + \frac{d}{2})n &= s \\
- \frac{d}{2}n^2 + (p + \frac{d}{2})n - s &= 0 \\
d \cdot n^2 - (2p + d)n + 2s &= 0
\end{aligned}
$$

And finally from this we can use the quadratic equation after extracting `A`, `B`, and `C`:

$$
\underbrace{d}_{A} \cdot n^2 \underbrace{- (2p + d)}_{C} \cdot n + \underbrace{2s}_{C} = 0
$$

```cpp
int howManyGames(int p, int d, int m, int s) {
	if (s < p) return 0;
	int until_m = (p - m) / d + 1;
	
    long long last_cost =
        1LL * until_m * (2LL * p - 1LL * (until_m - 1) * d) / 2;
	
	if (s < last_cost) {
		int n = (2 * p + d - std::sqrt((2.0 * p + d) * (2.0 * p + d) - 8.0 * d * s)) / (2 * d);
		return n;
	}
	
	int remaining = s - last_cost;
	int at_m = remaining / m;
	
	return until_m + at_m;
}
```
