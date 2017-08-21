#include <stdio.h>
#include <string.h>
#include <math.h>

// 10 -> 221, 100 -> 39826, 1000 -> 5893024, 10000 -> 782828208, 100000 -> 97681513944
#define N 5000000

char primes[N+1];
char mu[N+1];

long long calc_smart(int val)
{
    int max = (int)floor(sqrt(val));
    int x, y;
    long long result1 = 0, result2 = 0;
    for (x = 2; x <= max; x++)
        for (y = 1; y < x; y++) {
            // x*x'+y*y' <= val, x'>y'
            // y' <= (val - x*x') / y, y' < x'
            // when x'(x+y) <= val, y' can be any from 1 to x'-1
            long long tmp1 = val / (x + y);
            int lo, hi, delta, z;
            long long periodsum = 0;
            long long maxk;
            result1 += tmp1 * (tmp1 - 1) / 2;
            // when x'(x+y) > val, y' can be any from 1 to floor((val-x*x')/y)
            // sum_{x'=val/(x+y)+1}^{val/x}floor((val-x*x')/y)
            // x'=val/(x+y)+z
            // sum_{z=1}^{val/x-val/(x+y)}floor(( val-val/(x+y)*x-x*z )/y)
            // sum_{z=y*k+1}^{y*k+y}floor((...-x*z)/y) = -x*y*k + sum_{z=1}^{y}floor((...-x*z)/y)
            delta = val - tmp1 * x;
            hi = delta / x;
            maxk = hi / y;
            for (z = 1; z <= y; z++)
                periodsum += (delta - x * z) / y;
            result1 += maxk * periodsum - x * y * maxk * (maxk - 1) / 2;
            for (z = y * maxk + 1; z <= hi; z++) {
                result1 += (delta - x * z) / y;
            }
            // result2 is the same with x' <= max
            if (tmp1 <= max) {
                result2 += tmp1 * (tmp1 - 1) / 2;
                hi = max - tmp1;
                maxk = hi / y;
                result2 += maxk * periodsum - x * y * maxk * (maxk - 1) / 2;
                for (z = y * maxk + 1; z <= hi; z++)
                    result2 += (delta - x * z) / y;
            } else {
                result2 += max * (max - 1) / 2;
            }
        }
    return 2 * result1 - result2;
}

int main()
{
    int x, y, d;
    long long result, prev;
    memset(primes, 1, sizeof(primes));
    for (x = 2; x * x <= N; x++) if (primes[x])
        for (y = x * x; y <= N; y += x) primes[y] = 0;
    memset(mu, 1, sizeof(mu));
    for (x = 2; x * x <= N; x++) if (primes[x]) {
        for (y = x; y <= N; y += x) mu[y] = -mu[y];
        for (y = x * x; y <= N; y += x * x) mu[y] = 0;
    }
    for (; x <= N; x++) if (primes[x])
        for (y = x; y <= N; y += x) mu[y] = -mu[y];
    // assuming that N is even,
    // sum_{a<=N}floor(3*(a-1)/2) = sum_{i=0}^{N/2-1}(6*i+1) = 3*(N/2-1)*(N/2)+(N/2)
    result = (3ll * N / 2 - 2) * N / 2;
    x = 0;
    prev = 0;
    for (d = 1; d <= N; d++) if (mu[d]) {
        int div = N / d;
        if (div != x) {
            x = div;
            prev = calc_smart(div);
            if (!prev)
                break;
        }
        result += 2 * mu[d] * prev;
    }
    printf("%lld\n", 2 * result + N * (N + 1ll) / 2);
    return 0;
}
