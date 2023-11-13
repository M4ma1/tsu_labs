import heapq
class Node:
    def __init__(self, char=None, frequency=None):
        self.char = char
        self.frequency = frequency
        self.left = None
        self.right = None

    def __lt__(self, other):
        return self.frequency < other.frequency

#     def insert(self, data):
# # Compare the new value with the parent node
#         if self.data:
#            if data < self.data:
#               if self.left is None:
#                  self.left = Node(data)
#               else:
#                  self.left.insert(data)
#            elif data > self.data:
#                  if self.right is None:
#                     self.right = Node(data)
#                  else:
#                     self.right.insert(data)
#         else:
#            self.data = data

# Print the tree
    def PrintTree(self):
        if self.left:
           self.left.PrintTree()
        print( self.data),
        if self.right:
           self.right.PrintTree()


class Haffman:
    def getLetterCount(message):
        # Returns a dictionary with keys of single letters and values of the
        # count of how many times they appear in the message parameter.
        letterCount = {
            'A': 0, 'B': 0, 'C': 0, 'D': 0, 'E': 0, 'F': 0, 'G': 0, 'H': 0, 'I': 0, 'J': 0, 'K': 0, 'L': 0, 'M': 0,
            'N': 0, 'O': 0, 'P': 0, 'Q': 0, 'R': 0, 'S': 0, 'T': 0, 'U': 0, 'V': 0, 'W': 0, 'X': 0, 'Y': 0, 'Z': 0,
            'a': 0, 'b': 0, 'c': 0, 'd': 0, 'e': 0, 'f': 0, 'g': 0, 'h': 0, 'i': 0, 'j': 0, 'k': 0, 'l': 0, 'm': 0,
            'n': 0, 'o': 0, 'p': 0, 'q': 0, 'r': 0, 's': 0, 't': 0, 'u': 0, 'v': 0, 'w': 0, 'x': 0, 'y': 0, 'z': 0,
            'А': 0, 'Б': 0, 'В': 0, 'Г': 0, 'Д': 0, 'Е': 0, 'Ё': 0, 'Ж': 0, 'З': 0, 'И': 0, 'Й': 0, 'К': 0, 'Л': 0,
            'М': 0, 'Н': 0, 'О': 0, 'П': 0, 'Р': 0, 'С': 0, 'Т': 0, 'У': 0, 'Ф': 0, 'Х': 0, 'Ц': 0, 'Ч': 0, 'Ш': 0,
            'Щ': 0, 'Ъ': 0, 'Ы': 0, 'Ь': 0, 'Э': 0, 'Ю': 0, 'Я': 0, 'а': 0, 'б': 0, 'в': 0, 'г': 0, 'д': 0, 'е': 0,
            'ё': 0, 'ж': 0, 'з': 0, 'и': 0, 'й': 0, 'к': 0, 'л': 0, 'м': 0, 'н': 0, 'о': 0, 'п': 0, 'р': 0, 'с': 0,
            'т': 0, 'у': 0, 'ф': 0, 'х': 0, 'ц': 0, 'ч': 0, 'ш': 0, 'щ': 0, 'ъ': 0, 'ы': 0, 'ь': 0, 'э': 0, 'ю': 0,
            'я': 0
        }
        for letter in message:
            if letter in letterCount:
                letterCount[letter] += 1

        # return letterCount
   
    # def sort_dict(dictionary):
        non_empty_list = dict([(k,v) for k,v in letterCount.items() if v])
        sorted_non_empty_list = dict(sorted(non_empty_list.items(), key=lambda x:x[1]))
        return sorted_non_empty_list
    
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

    # The last remaining node is the root of the Huffman tree
        return heap[0]

    def huffman_codes(node, code="", mapping=None):
        if mapping is None:
            mapping = {}

        if node is not None:
            if node.char is not None:
                mapping[node.char] = code
            Haffman.huffman_codes(node.left, code + "0", mapping)
            Haffman.huffman_codes(node.right, code + "1", mapping)

        return mapping
    
# Use the insert method to add nodes
# root = Node(12)
# root.insert(6)
# root.insert(14)
# root.insert(3)
# root.PrintTree()

aboab= "Certainly! Here is the dictionary printed as a string:"
# # print(Haffman.getLetterCount(aboab))
print(Haffman.getLetterCount(aboab))


huffman_tree_root = Haffman.build_huffman_tree(Haffman.getLetterCount(aboab))
huffman_codes_mapping = Haffman.huffman_codes(huffman_tree_root)

print("Huffman Codes:")
for char, code in huffman_codes_mapping.items():
    print(f"{char}: {code}")