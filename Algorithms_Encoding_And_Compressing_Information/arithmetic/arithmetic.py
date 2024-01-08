from collections import OrderedDict

encoded_message = ''
current_byte = 0
interval = []
frequency_dictionary = {}
read_bit = 0
byte_len = 8
bit_len = 0
garbage_bit_count = 0

def character_counter(text):
    character_count = {}
    interval = [0]

    for char in text:
        if char in character_count:
            character_count[char] += 1
        else:
            character_count[char] = 1

    non_empty_characters = dict([(k, v) for k, v in character_count.items() if v])
    sorted_non_empty_characters = OrderedDict(sorted(non_empty_characters.items(), key=lambda x: x[1], reverse=True))

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
        file.write(current_byte.to_bytes(1, "little", signed=False))
        byte_len = 8

def write_bits_and_follow(bit, bit_to_follow, file):
    write_bit(bit, file)
    while bit_to_follow:
        write_bit(bit ^ 1, file)
        bit_to_follow -= 1

def encode_file(input_file):
    global frequency_dictionary, encoded_message, interval

    file_content = open(input_file, 'r').read()
    frequency_dictionary, interval = character_counter(file_content)
    with open("encoded", "w+b") as output_file:
        output_file.write(len(frequency_dictionary).to_bytes(1, "little"))
        for character in frequency_dictionary:
            output_file.write(character.encode())
            output_file.write(frequency_dictionary[character].to_bytes(4, 'little'))

        low, high = 0, 65535
        divisor = interval[-1]
        first_quarter, half, third_quarter = (high + 1) // 4, (high + 1) // 2, (high + 1) // 4 * 3
        bits_to_follow_count = 0
        temp = high - low + 1
        for char in file_content:
            index = list(frequency_dictionary.keys()).index(char) + 1
            high = low + interval[index] * temp // divisor - 1
            low = low + interval[index - 1] * temp // divisor
            while True:
                if high < half:
                    write_bits_and_follow(0, bits_to_follow_count, output_file)
                    bits_to_follow_count = 0

                elif low >= half:
                    write_bits_and_follow(1, bits_to_follow_count, output_file)
                    bits_to_follow_count = 0
                    low -= half
                    high -= half

                elif low >= first_quarter and high < third_quarter:
                    bits_to_follow_count += 1
                    low -= first_quarter
                    high -= first_quarter

                else:
                    break

                low += low
                high += high + 1
            temp = high - low + 1

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
    dictionary = {}
    interval = [0]
    with open(input_file, 'rb') as input_file:
        num = ord(input_file.read(1))
        for i in range(num):
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
            for i in range(16):
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
            decode_file("encoded")

        elif user_input == 'Ex':
            break

        else:
            print("you misclicked")
