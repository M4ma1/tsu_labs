package main

import (
	"fmt"
	"math/big"
	"math/rand"
	"time"
)

type Factor struct {
	P *big.Int // prime factor
	E int      // exponent
}

// LucasTest implements the Lucas primality test
func LucasTest(n *big.Int, factors []Factor, t int) bool {
	if n.Cmp(big.NewInt(2)) <= 0 {
		return false
	}
	if n.Bit(0) == 0 { // even number
		return n.Cmp(big.NewInt(2)) == 0
	}

	one := big.NewInt(1)
	two := big.NewInt(2)
	nMinus1 := new(big.Int).Sub(n, one)
	nMinus2 := new(big.Int).Sub(n, two)

	for i := 0; i < t; i++ {
		// Step 1.1: Choose random a, 2 ≤ a ≤ n-2
		a := new(big.Int)
		a.Rand(rand.New(rand.NewSource(time.Now().UnixNano())), nMinus2)
		a.Add(a, two)

		// Step 1.2: Check if a^(n-1) ≡ 1 mod n
		mod := new(big.Int).Exp(a, nMinus1, n)
		if mod.Cmp(one) != 0 {
			return false
		}

		// Step 1.3: Check a^((n-1)/pi) ≡ 1 mod n for all prime factors
		allNonOne := true
		for _, factor := range factors {
			divisor := new(big.Int).Div(nMinus1, factor.P)
			mod := new(big.Int).Exp(a, divisor, n)
			if mod.Cmp(one) == 0 {
				allNonOne = false
				break
			}
		}

		if allNonOne {
			return true
		}
	}

	return false
}
func factorize(n *big.Int) []Factor {
	if n.Cmp(big.NewInt(1)) <= 0 {
		return nil
	}

	var factors []Factor
	zero := big.NewInt(0)
	one := big.NewInt(1)
	two := big.NewInt(2)
	temp := new(big.Int).Set(n)

	// Handle 2 separately
	e := 0
	for new(big.Int).Mod(temp, two).Cmp(zero) == 0 {
		e++
		temp.Div(temp, two)
	}
	if e > 0 {
		factors = append(factors, Factor{P: new(big.Int).Set(two), E: e})
	}

	// Check odd divisors up to sqrt(n)
	i := big.NewInt(3)
	sqrt := new(big.Int).Sqrt(temp)
	for i.Cmp(sqrt) <= 0 {
		e = 0
		for new(big.Int).Mod(temp, i).Cmp(zero) == 0 {
			e++
			temp.Div(temp, i)
			sqrt.Sqrt(temp) // Update sqrt since temp changed
		}
		if e > 0 {
			factors = append(factors, Factor{P: new(big.Int).Set(i), E: e})
		}
		i.Add(i, two)
	}

	if temp.Cmp(one) > 0 {
		factors = append(factors, Factor{P: new(big.Int).Set(temp), E: 1})
	}

	return factors
}

func main() {
	// Example usage
	// n := big.NewInt(1334733877147062382486934807105197899496002201113849920496510541601) // Number to test
	// 170141183460469231731687303715884105727
	n, _ := new(big.Int).SetString("5555", 10)
	t := 5 // Number of iterations

	// Precompute factors of n-1 (for demonstration, in real code you'd need to factorize n-1)
	nMinus1 := new(big.Int).Sub(n, big.NewInt(1))
	factors := factorize(nMinus1)
	fmt.Println(factors)

	// Time and run Lucas test
	start := time.Now()
	isPrime := LucasTest(n, factors, t)
	elapsed := time.Since(start)

	fmt.Printf("Lucas test result: %t\n", isPrime)
	fmt.Printf("Time taken: %v\n", elapsed)

	// Compare with standard library's ProbablyPrime
	start = time.Now()
	stdlibResult := n.ProbablyPrime(t)
	elapsed = time.Since(start)

	fmt.Printf("Standard library ProbablyPrime(%d) result: %t\n", t, stdlibResult)
	fmt.Printf("Time taken: %v\n", elapsed)
}
