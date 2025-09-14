package main

import (
	crand "crypto/rand"
	"fmt"
	"math"
	"math/big"
	"math/rand"
	"time"
)

func randomInRange(min, max *big.Int) *big.Int {
	randGen := rand.New(rand.NewSource(time.Now().UnixNano()))
	diff := new(big.Int).Sub(max, min)
	randNum := new(big.Int).Rand(randGen, diff)
	return randNum.Add(randNum, min)
}
func powMod(a, b, m *big.Int) *big.Int {
	return new(big.Int).Exp(a, b, m)
}

func jacobiSymbol(a, n *big.Int) int {
	if a.Cmp(big.NewInt(0)) == 0 {
		return 0
	}
	if a.Cmp(big.NewInt(1)) == 0 {
		return 1
	}

	k := big.NewInt(0)
	aCopy := new(big.Int).Set(a)
	two := big.NewInt(2)
	zero := big.NewInt(0)

	for new(big.Int).Mod(aCopy, two).Cmp(zero) == 0 {
		aCopy = new(big.Int).Div(aCopy, two)
		k.Add(k, big.NewInt(1))
	}

	s := 1
	if new(big.Int).Mod(k, two).Cmp(zero) != 0 {
		nMod8 := new(big.Int).Mod(n, big.NewInt(8))
		if nMod8.Cmp(big.NewInt(1)) == 0 || nMod8.Cmp(big.NewInt(7)) == 0 {
			s = 1
		} else {
			s = -1
		}
	}

	if new(big.Int).Mod(n, big.NewInt(4)).Cmp(big.NewInt(3)) == 0 &&
		new(big.Int).Mod(aCopy, big.NewInt(4)).Cmp(big.NewInt(3)) == 0 {
		s = -s
	}

	if aCopy.Cmp(big.NewInt(1)) == 0 {
		return s
	} else {
		return s * jacobiSymbol(new(big.Int).Mod(n, aCopy), aCopy)
	}
}

func solovayStrassenTest(n *big.Int, k int) (string, float64, time.Duration) {
	if n.Cmp(big.NewInt(2)) < 0 {
		return "Composite", 0, 0
	}
	if n.Cmp(big.NewInt(2)) == 0 || n.Cmp(big.NewInt(3)) == 0 {
		return "Prime", 0, 0
	}
	if new(big.Int).Mod(n, big.NewInt(2)).Cmp(big.NewInt(0)) == 0 {
		return "Composite", 0, 0
	}

	for i := 0; i < k; i++ {
		a := randomInRange(big.NewInt(2), new(big.Int).Sub(n, big.NewInt(2)))
		exp := new(big.Int).Div(new(big.Int).Sub(n, big.NewInt(1)), big.NewInt(2))
		x := powMod(a, exp, n)

		if x.Cmp(big.NewInt(1)) != 0 && x.Cmp(new(big.Int).Sub(n, big.NewInt(1))) != 0 {
			return "composite", 0, 0
		}

		jacobi := jacobiSymbol(a, n)
		var expected *big.Int
		if jacobi == -1 {
			expected = new(big.Int).Sub(n, big.NewInt(1))
		} else {
			expected = big.NewInt(int64(jacobi))
		}

		if x.Cmp(expected) != 0 {
			return "Composite", 0, 0
		}
	}

	errorProb := math.Pow(0.5, float64(k))
	return "Prime", errorProb, 0
}

func MillerRabin(n *big.Int, k int) (bool, time.Duration) {
	start := time.Now()

	if n.Cmp(big.NewInt(2)) < 0 {
		return false, time.Since(start)
	}
	if n.Cmp(big.NewInt(2)) == 0 || n.Cmp(big.NewInt(3)) == 0 {
		return true, time.Since(start)
	}
	if n.Bit(0) == 0 {
		return false, time.Since(start)
	}

	d := new(big.Int).Sub(n, big.NewInt(1))
	s := 0
	for d.Bit(0) == 0 {
		d.Rsh(d, 1)
		s++
	}

	for i := 0; i < k; i++ {
		a, err := crand.Int(crand.Reader, new(big.Int).Sub(n, big.NewInt(3)))
		if err != nil {
			panic(err)
		}
		a.Add(a, big.NewInt(2))

		x := new(big.Int).Exp(a, d, n)

		if x.Cmp(big.NewInt(1)) == 0 || x.Cmp(new(big.Int).Sub(n, big.NewInt(1))) == 0 {
			continue
		}

		for j := 0; j < s-1; j++ {
			x.Exp(x, big.NewInt(2), n)
			if x.Cmp(big.NewInt(1)) == 0 {
				return false, time.Since(start)
			}
			if x.Cmp(new(big.Int).Sub(n, big.NewInt(1))) == 0 {
				break
			}
		}

		if x.Cmp(new(big.Int).Sub(n, big.NewInt(1))) != 0 {
			return false, time.Since(start)
		}
	}

	return true, time.Since(start)
}

func main() {
	// Test a known prime (2^127 - 1)
	n, _ := new(big.Int).SetString("170141183460469231731687303715884105727", 10)
	iterations := 20

	fmt.Printf("Testing number: %s\n", n.String())

	// Solovay-Strassen test
	ssResult, ssError, _ := solovayStrassenTest(n, iterations)
	fmt.Printf("- Solovay-Strassen test: %s Error probability: ≤ %.15f\n", ssResult, ssError)

	// Miller-Rabin test
	mrPrime, _ := MillerRabin(n, iterations)
	fmt.Printf("- Miller-Rabin test: Prime=%v", mrPrime)
	if mrPrime {
		fmt.Printf("  Error probability: ≤ %.15f\n", math.Pow(0.25, float64(iterations)))
	}
	if (ssError > math.Pow(0.25, float64(iterations))) {
		fmt.Println("Miller-Rabin is better then Solovay-Strassen")
	} else {
		fmt.Println("Solovay-Strassen is better then Miller-Rabin")
	}
}
