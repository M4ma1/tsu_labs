#include <iostream>
#include <vector>
#include <cmath>
#include "big_number.h"
#include <map>

int discrete_logarithm(BN generator, BN modulus, int group_order, BN target_value)
{
    int step_size = sqrt(group_order) + 1;
    BN giant_step_base = generator.pow(step_size) % modulus;

    // Giant steps storage
    BN giant_step_keys[step_size + 1];
    int giant_step_exponents[step_size + 1];

    // Initialize arrays
    for (int i = 0; i <= step_size; i++)
    {
        giant_step_exponents[i] = -1;
    }

    // Compute giant steps
    for (int giant_idx = 1; giant_idx <= step_size; giant_idx++)
    {
        BN current_key = giant_step_base.pow(giant_idx) % modulus;
        bool key_exists = false;

        // Check for duplicate keys
        for (int i = 1; i <= step_size; i++)
        {
            if (giant_step_exponents[i] != -1 && giant_step_keys[i] == current_key)
            {
                key_exists = true;
                break;
            }
        }

        if (!key_exists)
        {
            giant_step_keys[giant_idx] = current_key;
            giant_step_exponents[giant_idx] = giant_idx;
        }
    }

    // Compute baby steps and look for match
    BN baby_step_values[step_size + 1];

    for (int baby_idx = 1; baby_idx <= step_size; baby_idx++)
    {
        baby_step_values[baby_idx] = (target_value * generator.pow(baby_idx)) % modulus;
        
        // Check for collision between giant and baby steps
        for (int i = 1; i <= step_size; i++)
        {
            if (giant_step_exponents[i] != -1 && baby_step_values[baby_idx] == giant_step_keys[i])
            {
                return (step_size * giant_step_exponents[i] - baby_idx) % group_order;
            }
        }
    }

    return 0;
}

int main() {

        BN generator, modulus, target;
        int group_order, temp_modulus;
        
        std::cout << "\n" << "Enter generator:\n";
        generator.cin_base10();
        std::cout << "Enter target value:\n";
        target.cin_base10();
        std::cout << "Enter modulus: ";
        std::cin >> temp_modulus;
        
        group_order = temp_modulus - 1;
        modulus = temp_modulus;
        
        std::cout << discrete_logarithm(generator, modulus, group_order, target);

    return 0;
}
