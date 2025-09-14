package main

import (
	"crypto/rand"
	"fmt"
	"math/big"
)

func main() {
	// Generate a strong prime using Gordon's method
	bits := 15 // Bit length for the primes (can be adjusted)
	p, err := GenerateStrongPrime(bits)
	if err != nil {
		fmt.Println("Error generating strong prime:", err)
		return
	}

	fmt.Printf("Generated strong prime (%d bits):\n%v\n", p.BitLen(), p)
}

// GenerateStrongPrime generates a strong prime using Gordon's method (1984)
func GenerateStrongPrime(bits int) (*big.Int, error) {
	// Step 1: Generate large primes s and t of approximately equal size
	s, err := rand.Prime(rand.Reader, bits/2)
	if err != nil {
		return nil, fmt.Errorf("failed to generate prime s: %v", err)
	}

	t, err := rand.Prime(rand.Reader, bits/2)
	if err != nil {
		return nil, fmt.Errorf("failed to generate prime t: %v", err)
	}

	// Step 2: Find first prime in the sequence r = 2*i*t + 1
	r, err := findPrimeInSequence(t, func(i *big.Int) *big.Int {
		// r = 2*i*t + 1
		r := new(big.Int).Mul(i, t)
		r.Mul(r, big.NewInt(2))
		r.Add(r, big.NewInt(1))
		return r
	})
	if err != nil {
		return nil, fmt.Errorf("failed to find prime r: %v", err)
	}

	// Step 3: Compute p0 = 2*(s^(r-2) mod r)*s - 1
	p0, err := computeP0(s, r)
	if err != nil {
		return nil, fmt.Errorf("failed to compute p0: %v", err)
	}

	// Step 4: Find first prime in the sequence p = 2*j*r*s + p0
	p, err := findPrimeInSequence(new(big.Int).Mul(r, s), func(j *big.Int) *big.Int {
		// p = 2*j*r*s + p0
		p := new(big.Int).Mul(j, r)
		p.Mul(p, s)
		p.Mul(p, big.NewInt(2))
		p.Add(p, p0)
		return p
	})
	if err != nil {
		return nil, fmt.Errorf("failed to find prime p: %v", err)
	}

	return p, nil
}

// findPrimeInSequence finds the first prime in a sequence starting from a random i0
func findPrimeInSequence(coeff *big.Int, sequence func(*big.Int) *big.Int) (*big.Int, error) {
	// Generate a random starting point i0
	i0, err := rand.Int(rand.Reader, new(big.Int).Exp(big.NewInt(2), big.NewInt(32), nil))
	if err != nil {
		return nil, err
	}

	// Create a copy to increment
	i := new(big.Int).Set(i0)

	for {
		candidate := sequence(i)
		if candidate.ProbablyPrime(20) {
			return candidate, nil
		}
		i.Add(i, big.NewInt(1))
	}
}

// computeP0 computes p0 = 2*(s^(r-2) mod r)*s - 1
func computeP0(s, r *big.Int) (*big.Int, error) {
	// Compute r-2
	rMinus2 := new(big.Int).Sub(r, big.NewInt(2))

	// Compute s^(r-2) mod r
	exp := new(big.Int).Exp(s, rMinus2, r)

	// Multiply by 2*s
	twoS := new(big.Int).Mul(big.NewInt(2), s)
	result := new(big.Int).Mul(exp, twoS)

	// Subtract 1
	result.Sub(result, big.NewInt(1))

	return result, nil
}
