#include <iostream>
#include <tuple>
#include "BigNumber.h"

// Modular subtraction that handles negative results correctly
BN mod_sub(const BN& a, const BN& b, const BN& n) {
    if (a >= b) {
        return (a - b) % n;
    } else {
        return (n - (b - a)) % n;
    }
}

// Partition functions for the pseudo-random walk
// The group is divided into 3 subsets S1, S2, S3 based on value mod 3
bool in_S1(const BN& x) {
    return x % 3 == 1;
}

bool in_S2(const BN& x) {
    return x % 3 == 2;
}

bool in_S3(const BN& x) {
    return x % 3 == 0;
}

// The iteration function f(γ) from the PDF
// For γ = g^u * a^v, computes next element and updates exponents
// According to the PDF's rules:
// - If γ ∈ S1: multiply by a (β in PDF)
// - If γ ∈ S2: square the element  
// - If γ ∈ S3: multiply by g (α in PDF)
std::tuple<BN, BN, BN> F(const BN& x, const BN& u, const BN& v, const BN& g, const BN& a, const BN& p, const BN& n) {
    BN new_x, new_u, new_v;
    BN one = 1;
    BN two = 2;

    if (in_S1(x)) {
        // Case 1: γ ∈ S1 → multiply by a (β in PDF)
        // γ_new = a * γ = a * g^u * a^v = g^u * a^(v+1)
        new_x = (a * x) % p;
        new_u = u;                    // u remains same
        new_v = (v + one) % n;       // v increases by 1
    } else if (in_S2(x)) {
        // Case 2: γ ∈ S2 → square the element
        // γ_new = γ^2 = (g^u * a^v)^2 = g^(2u) * a^(2v)
        new_x = (x * x) % p;
        new_u = (two * u) % n;       // u doubles
        new_v = (two * v) % n;       // v doubles
    } else { // in_S3(x)
        // Case 3: γ ∈ S3 → multiply by g (α in PDF)
        // γ_new = g * γ = g * g^u * a^v = g^(u+1) * a^v
        new_x = (g * x) % p;
        new_u = (u + one) % n;       // u increases by 1
        new_v = v;                   // v remains same
    }

    return { new_x, new_u, new_v };
}

// Extended Euclidean Algorithm for modular inverse
BN mod_inverse(const BN& a, const BN& m) {
    BN m0 = m;
    BN y = 0, x = 1;
    BN zero = 0;
    BN one = 1;

    if (m == one)
        return zero;

    BN a_temp = a;
    BN m_temp = m;

    while (a_temp > one) {
        BN q = a_temp / m_temp;
        BN t = m_temp;

        m_temp = a_temp % m_temp;
        a_temp = t;
        t = y;

        y = x - q * y;
        x = t;
    }

    if (x < zero)
        x += m0;

    return x;
}

// Solves the linear congruence: r * z ≡ diff (mod n)
// This corresponds to solving: (b_T - b_H) * x ≡ (a_H - a_T) (mod n) from PDF
BN solve_mod_equation(const BN& r, const BN& diff, const BN& n) {
    BN zero = 0;
    BN one = 1;

    // Check if solution exists using gcd
    BN d = r.gcd(n);
    if (diff % d != zero) {
        throw std::invalid_argument("No solutions");
    }

    // Reduce equation: (r/d) * z ≡ (diff/d) (mod n/d)
    BN n_d = n / d;
    BN diff_d = diff / d;
    BN r_d = r / d;

    // Find modular inverse of (r/d) mod (n/d)
    BN r_d_inv = mod_inverse(r_d, n_d);

    // Solution in reduced modulus
    BN z0 = (diff_d * r_d_inv) % n_d;

    return z0;
}

// Main Pollard's Rho algorithm for discrete logarithm
// Solves: g^x ≡ a (mod p) where n is the group order (p-1 for prime p)
BN pollard_rho(const BN& g, const BN& p, const BN& n, const BN& a) {
    // Initialize tortoise and hare pointers (Floyd's cycle finding)
    // Both start at group identity element 1 = g^0 * a^0
    BN x1 = 1, x2 = 1; // Group elements for tortoise and hare
    BN u1 = 0, u2 = 0; // Exponents for g (α in PDF)
    BN v1 = 0, v2 = 0; // Exponents for a (β in PDF)
    BN zero = 0, one = 1;

    std::cout << "x1\tu1\tv1\tx2\tu2\tv2\n";
    std::cout << "-----------------------\n";
    std::cout << x1 << "\t" << u1 << "\t" << v1 << "\t" << x2 << "\t" << u2 << "\t" << v2 << "\n";

    int max_iterations = 20000;
    int iterations = 0;

    // Main loop: tortoise moves 1 step, hare moves 2 steps per iteration
    while (iterations < max_iterations) {
        // Tortoise: one step forward
        std::tie(x1, u1, v1) = F(x1, u1, v1, g, a, p, n);

        // Hare: two steps forward
        std::tie(x2, u2, v2) = F(x2, u2, v2, g, a, p, n);
        std::tie(x2, u2, v2) = F(x2, u2, v2, g, a, p, n);

        iterations++;

        std::cout << x1 << "\t" << u1 << "\t" << v1 << "\t" << x2 << "\t" << u2 << "\t" << v2 << "\n";

        // Collision detected: x1 == x2 (tortoise and hare meet)
        if (x1 == x2) {
            std::cout << "Collision found!" << std::endl;
            
            // From PDF: we have g^{u1} * a^{v1} = g^{u2} * a^{v2}
            // Substitute a = g^x: g^{u1} * g^{x*v1} = g^{u2} * g^{x*v2}
            // This gives: u1 + x*v1 ≡ u2 + x*v2 (mod n)
            // Rearranged: (v1 - v2) * x ≡ (u2 - u1) (mod n)
            
            BN r = mod_sub(v1, v2, n); // r = (v1 - v2) mod n

            if (r == zero) {
                // r = 0 means equation is trivial, need different collision
                std::cout << "r is zero, cannot solve equation. Continuing..." << std::endl;
                continue;
            }
            
            BN diff = mod_sub(u2, u1, n); // diff = (u2 - u1) mod n

            try {
                // Solve: r * x ≡ diff (mod n)
                BN z0 = solve_mod_equation(r, diff, n);
                
                // There might be multiple solutions due to gcd(r, n) = d > 1
                BN d = r.gcd(n);
                BN i = zero;
                std::cout << "Testing " << d << " possible solutions..." << std::endl;

                // Test all possible solutions: z0 + i*(n/d) for i = 0,...,d-1
                while (i < d) {
                    BN z = (z0 + i * (n / d)) % n;

                    // Verify candidate solution: check if g^z ≡ a (mod p)
                    BN verification = g.pow_mod(z, p);
                    if (verification == a) {
                        std::cout << "Found solution: " << z << std::endl;
                        return z;
                    }
                    i = i + one;
                }

                std::cout << "No valid solution found for this collision." << std::endl;
            } catch (const std::exception& e) {
                std::cout << "Error solving equation: " << e.what() << std::endl;
            }
        }
    }

    std::cout << "Max iterations reached without finding solution." << std::endl;
    throw std::runtime_error("Solution not found");
}

int main() {
    // Example: solve 8900^x ≡ 7330 mod 10069
    BN g = 22;  // Generator (α in PDF)
    BN p = 599; // Prime modulus
    BN a = 183;  // Target element (β in PDF)  
    BN n = p - 1; // Group order (for prime p)

    std::cout << "Solving: " << g << "^x ≡ " << a << " mod " << p << std::endl;
    std::cout << "Order of group: " << n << std::endl << std::endl;

    try {
        BN result = pollard_rho(g, p, n, a);
        std::cout << std::endl << "Discrete logarithm found: " << result << std::endl;
        std::cout << "Verification: " << g << "^" << result << " mod " << p << " = " << g.pow_mod(result, p) << std::endl;
        std::cout << "Expected: " << a << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
