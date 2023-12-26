import heapq
class Node:
    def __init__(self, char=None, frequency=None):
        self.char = char
        self.frequency = frequency
        self.left = None
        self.right = None

    def __lt__(self, other):
        return self.frequency < other.frequency
    
    def print_tree(self, prefix="", is_left=True):
        if self.right is not None:
            self.right.print_tree(prefix + ("│   " if is_left else "    "), False)
        print(prefix + ("└── " if is_left else "┌── ") + str(self.char) + ":" + str(self.frequency))
        if self.left is not None:
            self.left.print_tree(prefix + ("    " if is_left else "│   "), True)

    def getLetterCount(message):
        # Returns a dictionary with keys of single letters and values of the
        # count of how many times they appear in the message parameter.
        letterCount = {'?':0,'Å':0}

        for letter in message:
            if letter not in letterCount:
                letterCount[letter] = 0
            
            else:
                letterCount[letter] += 1

        non_empty_list = dict([(k,v) for k,v in letterCount.items() if v])
        sorted_non_empty_list = dict(sorted(non_empty_list.items(), key=lambda x:x[1]))
        return sorted_non_empty_list
    

    # The last remaining node is the root of the Huffman tree

    def huffman_codes(node, code="", mapping=None):
        if mapping is None:
            mapping = {}

        if node is not None:
            if node.char is not None:
                mapping[node.char] = code
            Node.huffman_codes(node.left, code + "0", mapping)
            Node.huffman_codes(node.right, code + "1", mapping)

        return mapping
    
    def build_huffman_tree(frequency_dict):
        # Create a priority queue (min heap) to store nodes
        heap = [Node(char, frequency) for char, frequency in frequency_dict.items()]
        heapq.heapify(heap)

        # Build the Huffman tree
        while len(heap) > 1:
            left_child = heapq.heappop(heap)
            right_child = heapq.heappop(heap)

                # Create a new node with the combined frequency
            combined_frequency = left_child.frequency + right_child.frequency
            internal_node = Node(frequency=combined_frequency)

                # Set the left and right children
            internal_node.left = left_child
            internal_node.right = right_child

            # Add the new internal node back to the heap
            heapq.heappush(heap, internal_node)
    
        return heap[0]


def encode_message_to_binary(message, huffman_codes):
    encoded_message = ""
    for char in message:
        encoded_message += huffman_codes[char]

    # Convert the binary string to an integer
    encoded_integer = int(encoded_message, 2)

    # Calculate the number of bytes required for the integer
    num_bytes = (len(encoded_message) + 7) // 8

    # Convert the integer to bytes
    encoded_bytes = encoded_integer.to_bytes(num_bytes, byteorder='little')

    return encoded_bytes


def decode_message_from_binary(encoded_message, huffman_tree):
    decoded_message = ""
    current_node = huffman_tree
    temp = int.from_bytes(encoded_message, 'little')
    encoded_message = bin(temp)[2:]
    for bit in encoded_message:
        if bit == "0":
            current_node = current_node.left
        else:
            current_node = current_node.right

        if current_node.char is not None:
            decoded_message += current_node.char
            current_node = huffman_tree  # Reset to the root for the next character

    return decoded_message


if __name__ == "__main__":
    huffman_tree_root = Node.build_huffman_tree(Node.getLetterCount('txt'))
    huffman_codes_mapping = Node.huffman_codes(huffman_tree_root)
    count = Node.getLetterCount('00')
    while True:
        print("[*]Would you like to (D)ecode, (E)ncode, (Ex)it")
        inp = input()

        if inp == 'E':
            txt = open('file.txt', 'r').read()
            count = Node.getLetterCount(txt)
            huffman_tree_root = Node.build_huffman_tree(count)
            huffman_codes_mapping = Node.huffman_codes(huffman_tree_root)
            with open("encoded", "wb") as file:  # Writing binary data to file
                encoded_data = encode_message_to_binary(txt, huffman_codes_mapping)
                file.write(encoded_data)

        elif inp == 'D':
            print("file to decode: ")
            f = input()
            with open(f, "rb") as file:  # Reading binary data from file
                binary_data = file.read()
                huffman_tree_root = Node.build_huffman_tree(count)
                with open("decoded", "w") as file:
                    file.write(decode_message_from_binary(binary_data, huffman_tree_root))

        elif inp == 'Ex':
            break

        else:
            print("you misclicked")