from collections import OrderedDict
encoded_message = ''


def counter(txt):
    count={}
    interval=[0]

    for char in txt:
        if char in count:
            count[char] += 1
        else:
            count[char] = 1

    non_empty_list = dict([(k,v) for k,v in count.items() if v])
    sorted_non_empty_list = OrderedDict(sorted(non_empty_list.items(), key=lambda x:x[1], reverse=True))
    
    for char in sorted_non_empty_list:
        interval.append(sorted_non_empty_list[char]+interval[-1])

    return sorted_non_empty_list, interval

def bits_plus_follow(bit, bittofollow):
    global encoded_message
    encoded_message += bit
    while bittofollow:
        bit = str(int(bit) ^ 1)
        encoded_message += bit
        bittofollow -= 1
    


def encode(file):
    f = open(file, 'r').read()
    low = 0
    high = 65535
    tmp = high - low + 1
    dictionary, interval = counter(f)
    devisor = interval[-1]
    first_qtr = int((high+1) / 4)
    half = first_qtr << 1
    third_qtr = first_qtr * 3
    bits_to_follow = 0
    for char in f:
        print(char)
        j = list(dictionary.keys()).index(char)+1
        high = low + int(interval[j]*(tmp)/ devisor)-1 # check if it works (change "high - low - +1" for new param)
        low = low + int(interval[j-1]*(tmp)/devisor)
        print(low, high)
        while True:
            if high < half:
                bits_plus_follow('0', bits_to_follow)
                bits_to_follow = 0 

            elif low >= half:

                bits_plus_follow('1', bits_to_follow)
                bits_to_follow = 0 
  
                low -= half
                high -= half
            
            elif low >= first_qtr and high < third_qtr:
                bits_to_follow += 1
                low -= first_qtr
                high -= first_qtr
            
            else: 
                break

            low += low
            high += high + 1
        tmp = high - low + 1

    print(encoded_message)
    encoded_integer = int(encoded_message, 2)

    # Calculate the number of bytes required for the integer
    num_bytes = (len(encoded_message) + 7) // 8

    # Convert the integer to bytes
    encoded_bytes = encoded_integer.to_bytes(num_bytes, byteorder='little')

    open("encoded", 'wb').write(encoded_bytes)

if __name__ == "__main__":
    while True:
        print("[*]Would you like to (D)ecode, (E)ncode, (Ex)it")
        inp = input()

        if inp == 'E':
            encode("file.txt")
        
        elif inp == 'D':
            print("file to decode: ")
            f = input()
            

        elif inp == 'Ex':
            break

        else:
            print("you misclicked")

