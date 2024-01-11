current_byte = 0
read_bit = 0
byte_len = 8
bit_len = 0
garbage_bit_count = 0

def character_counter(text):
    character_count = {}
    interval = [0, 1]

    for char in text:
        if char in character_count:
            character_count[char] += 1
        else:
            character_count[char] = 1

    sorted_non_empty_characters = dict(sorted(character_count.items(), key=lambda x: x[1], reverse=True))

    for char in sorted_non_empty_characters:
        interval.append(sorted_non_empty_characters[char] + interval[-1])

    return sorted_non_empty_characters, interval

def write_bit(bit, file):
    global current_byte, byte_len
    current_byte >>= 1
    if bit:
        current_byte |= 0x80
    byte_len -= 1
    if byte_len == 0:
        file.write(current_byte.to_bytes(1, "little"))
        byte_len = 8

def write_bits_and_follow(bit, bit_to_follow, file):
    write_bit(bit, file)
    while bit_to_follow:
        write_bit(bit ^ 1, file)
        bit_to_follow -= 1

current_byte = 0
bit_len = 8

def find_index_for_symbol(symbols, target_symbol):
    index = 0
    for symbol in symbols:
        if target_symbol == symbol:
            return index + 2
        index += 1
    print("Error in finding index")

def output_bit(bit, output_file):
    global current_byte, bit_len
    current_byte >>= 1
    if bit & 1:
        current_byte |= 0x80
    bit_len -= 1
    if bit_len == 0:
        bit_len = 8
        output_file.write(current_byte.to_bytes(1, "little"))

def bit_plus_follow(bit, bits_to_follow, output_file):
    output_bit(bit, output_file)
    for _ in range(bits_to_follow):
        output_bit(~bit, output_file)

def encode_file(input_file):
    global current_byte
    file_content = open(input_file, 'r').read()
    sorted_symbols, accumulation_array = character_counter(file_content)
    
    output_file = open("encoded", "w+b")
    output_file.write(len(sorted_symbols).to_bytes(1, "little"))
    for symbol in sorted_symbols:
        output_file.write(symbol.encode())
        output_file.write(sorted_symbols[symbol].to_bytes(4, "little"))

    with open(input_file, 'r') as input_file:
        low_value = 0
        high_value = (1 << 16) - 1
        delete_value = accumulation_array[-1]
        diff_value = high_value - low_value + 1
        first_quarter, half_quarter, third_quarter = (high_value + 1) // 4, (high_value + 1) // 2, (high_value + 1) // 4 * 3
        bits_to_follow_count = 0

        for symbol_read in input_file.read():
            j = find_index_for_symbol(sorted_symbols, symbol_read)
            high_value = int(low_value + accumulation_array[j] * diff_value / delete_value - 1)
            low_value = int(low_value + accumulation_array[j - 1] * diff_value / delete_value)

            while True:
                if high_value < half_quarter:
                    bit_plus_follow(0, bits_to_follow_count, output_file)
                    bits_to_follow_count = 0
                elif low_value >= half_quarter:
                    bit_plus_follow(1, bits_to_follow_count, output_file)
                    bits_to_follow_count = 0
                    low_value -= half_quarter
                    high_value -= half_quarter
                elif low_value >= first_quarter and high_value < third_quarter:
                    bits_to_follow_count += 1
                    low_value -= first_quarter
                    high_value -= first_quarter
                else:
                    break
                low_value += low_value
                high_value += high_value + 1

            diff_value = high_value - low_value + 1

        high_value = low_value + accumulation_array[1] * diff_value // delete_value - 1
        low_value = low_value + accumulation_array[0] * diff_value // delete_value

        while True:
            if high_value < half_quarter:
                bit_plus_follow(0, bits_to_follow_count, output_file)
                bits_to_follow_count = 0
            elif low_value >= half_quarter:
                bit_plus_follow(1, bits_to_follow_count, output_file)
                bits_to_follow_count = 0
                low_value -= half_quarter
                high_value -= half_quarter
            elif low_value >= first_quarter and high_value < third_quarter:
                bits_to_follow_count += 1
                low_value -= first_quarter
                high_value -= first_quarter
            else:
                break
            low_value += low_value
            high_value += high_value + 1
        bits_to_follow_count += 1
        if low_value < first_quarter:
            bit_plus_follow(0, bits_to_follow_count, output_file)
            bits_to_follow_count = 0
        else:
            bit_plus_follow(1, bits_to_follow_count, output_file)
            bits_to_follow_count = 0

        current_byte >>= bit_len
        output_file.write(current_byte.to_bytes(1, "little"))

    output_file.close()


def read_bit_from_file(input_file):
    global read_bit, bit_len, garbage_bit_count

    if bit_len == 0:
        read_byte = input_file.read(1)
        read_bit = int.from_bytes(read_byte, "little")
        if read_byte == b"":
            garbage_bit_count += 1
            read_bit = 255
            if garbage_bit_count > 14:
                exit(1)
        bit_len = 8

    extracted_bit = read_bit & 1
    read_bit >>= 1
    bit_len -= 1
    return extracted_bit


def decode_file(input_file):
    global current_byte, bit_len
    current_byte = 0 
    bit_len = 0
    dictionary = {}
    interval = [0,1]
    with open(input_file, 'rb') as input_file:
        num = ord(input_file.read(1))
        for _ in range(num):
            character = input_file.read(1)
            while True:
                try:
                    character = character.decode()
                    break
                except:
                    character = character + input_file.read(1)
            value = int.from_bytes(input_file.read(4), "little")
            dictionary[character] = value

        for character in dictionary:
            interval.append(interval[-1] + dictionary[character])

        with open("decoded", 'w+b') as output_file:
            low, high = 0, 65535
            divisor = interval[-1]
            first_quarter, half, third_quarter = (high + 1) // 4, (high + 1) // 2, (high + 1) // 4 * 3
            temp1 = high - low + 1
            value = 0
            for _ in range(16):
                extracted_bit = read_bit_from_file(input_file)
                value += value + extracted_bit
            print(value)
            while True:
                frequency = ((value - low + 1) * divisor - 1) // temp1

                j = 1
                while interval[j] <= frequency:
                    j += 1

                high = low + interval[j] * temp1 // divisor - 1
                low = low + interval[j - 1] * temp1 // divisor

                while True:
                    if high < half:
                        pass

                    elif low >= half:
                        low -= half
                        high -= half
                        value -= half

                    elif low >= first_quarter and high < third_quarter:
                        low -= first_quarter
                        high -= first_quarter
                        value -= first_quarter

                    else:
                        break

                    low += low
                    high += high + 1
                    extracted_bit = read_bit_from_file(input_file)
                    value += value + extracted_bit
                # check = list(dictionary.keys())[j-1]
                # print(check)
                if j == 1:
                    break
                output_file.write(list(dictionary.keys())[j - 2].encode())
                temp1 = high - low + 1

if __name__ == "__main__":
    while True:
        print("[*]Would you like to (D)ecode, (E)ncode, (Ex)it")
        user_input = input()

        if user_input == 'E':
            encode_file("file.txt")

        elif user_input == 'D':
            print("file to decode: ")
            decode_file(input())

        elif user_input == 'Ex':
            break

        else:
            print("you misclicked")
