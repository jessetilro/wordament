# Wordament

Renders solutions to a Wordament puzzle. It does so by:

* Parsing a text file describing the puzzle.
* Building an efficiently searchable data structure from the provided dictionary (an alphabetically sorted list of all valid words in a specific language). It concerns a hashmap with three letter prefixes as keys and a linked list of valid words with the given prefix as values. Alternatively a trie was considered, but would be a bit overkill.
* Doing a breadth first search traversal on the puzzle graph to find all possible words, starting from each cell of the puzzle in a different thread. It stops traversing paths early when they cannot form a valid word anymore.
* Sorting the results by word length using a merge sort algorithm, and outputting the sorted list of words.

# To Do

* Add the score system, to be able to sort words by the amount of points they're worth.
* Automatically recognize the current puzzle, for example using OCR.
* Automatically enter the words into the puzzle (PC version of the game), for example by simulating mouse input.
