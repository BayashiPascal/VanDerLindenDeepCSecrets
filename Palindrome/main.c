#include "stdlib.h"
#include "stdio.h"
#include "string.h"

char const* const pathDictionary = "./nounlist.txt";
int const nbWordDictionary = 6801;
int const sizeMaxWord = 512;
char const* const seedPalindrome = "abc";
int const nbWordPalindrome = 10;

struct Word;
struct Word {

  // The word
  char* str;

  // Next word
  struct Word* next;

};

struct Dictionary {

  // The words in the dictionary
  struct Word* words;

};

struct Palindrome {

  // The words in the palindrome
  struct Word* words;

};

struct Dictionary LoadDictionary(
  char const* const pathDictionary) {

  // Create the dictionary
  struct Dictionary dict;

  // Open the file containing the dictionary
  FILE* const filePtr =
    fopen(
      pathDictionary,
      "r");

  // Declare a pointer to memorise the last added word
  struct Word* lastWord = NULL;

  // Declare a buffer to read a word
  char buffer[sizeMaxWord];

  // Loop on the words in the dictionary
  for (
    int iWord = 0;
    iWord < nbWordDictionary;
    ++iWord) {

    // Read one word
    fscanf(
      filePtr,
      "%s",
      buffer);

    // Create the word instance
    struct Word* word = malloc(sizeof(struct Word));
    word->str = strdup(buffer);
    word->next = NULL;

    // Add the word to the dictionary
    if (lastWord == NULL) {

      dict.words = word;

    } else {

      lastWord->next = word;

    }

    // Update the last word
    lastWord = word;

  }

  // Close the file containing the dictionary
  fclose(filePtr);

  // Return the dictionary
  return dict;

}

struct Word* WordClone(struct Word const* const that) {

  // Declare the result word
  struct Word* word = malloc(sizeof(struct Word));
  word->next = NULL;

  // Clone the string
  word->str = strdup(that->str);

  // Return the result word
  return word;

}

struct Word* WordReversed(struct Word const* const that) {

  // Declare the result word
  struct Word* word = malloc(sizeof(struct Word));
  word->next = NULL;

  // Get the length of the string
  int lenStr = strlen(that->str);

  // Allocate memory for the string
  word->str = malloc(sizeof(char) * (lenStr + 1));

  // Copy the reversed string
  for (
    int iChar = 0;
    iChar < lenStr;
    ++iChar) {

    word->str[iChar] = that->str[lenStr - 1 - iChar];

  }

  word->str[lenStr] = '\0';

  // Return the result word
  return word;

}

struct Word const* SearchNextWord(
  struct Dictionary const* const dict) {

  // TODO
  return dict->words;

}

struct Palindrome CreatePalindrome(
  char const* seedStr,
  struct Dictionary const* const dict,
  int const nbWord) {

  // Declare the result palindrome
  struct Palindrome palindrome;

  // Declare a pointer to memorise the last added word
  struct Word* ptrLast = NULL;

  // Starts the palindrome by adding the seed string
  palindrome.words = malloc(sizeof(struct Word));
  ptrLast = palindrome.words;
  ptrLast->str = strdup(seedStr);

  // Add the reversed seed to the palindrome
  ptrLast->next = WordReversed(ptrLast);

  // Declare a variable to memorise the current number of words in the
  // palindrome
  // Initially there are two words, the seed and its reversed version
  int curNbWord = 2;

  // Loop until we have the requested number of words
  while (curNbWord < nbWord) {

    // Search the next word to extend the palindrome
    struct Word const* const nextWord = SearchNextWord(dict);

    // Memorise the word following word to the current last added word
    struct Word* const ptrLastNext = ptrLast->next;

    // Add the next word to the palindrome
    ptrLast->next = WordClone(nextWord);

    // Update the last added word
    ptrLast = ptrLast->next;

    // Add the reversed next word to the palindrome
    ptrLast->next = WordReversed(ptrLast);

    // Reconnect the previous next word
    ptrLast->next->next = ptrLastNext;

    // Update the number of words in the palindrome
    curNbWord += 2;

  }

  // Return the palindrome
  return palindrome;

}

void PrintPalindrome(struct Palindrome const* const that) {

  // Declare a pointer to loop on the words of the palindrome
  struct Word const* ptr = that->words;

  // Loop on the words of the palindrome
  while (ptr != NULL) {

    // Print the word
    printf(
      "%s ",
      ptr->str);

    // Move to the next word
    ptr = ptr->next;

  }

  // Print a line return at the end of the palindrome
  printf("\n");

}

void FreeDictionary(struct Dictionary *const that) {

  // Loop on the words of the dictionary
  while (that->words != NULL) {

    // Free the word
    free(that->words->str);
    struct Word *ptr = that->words->next;
    free(that->words);
    that->words = ptr;

  }

}

void FreePalindrome(struct Palindrome *const that) {

  // Loop on the words of the palindrome
  while (that->words != NULL) {

    // Free the word
    free(that->words->str);
    struct Word *ptr = that->words->next;
    free(that->words);
    that->words = ptr;

  }

}

int main() {

  // Load the dictionary
  struct Dictionary dict = LoadDictionary(pathDictionary);

  // Create the palindrome
  struct Palindrome palindrome =
    CreatePalindrome(
      seedPalindrome,
      &dict,
      nbWordPalindrome);

  // Print the palindrome
  PrintPalindrome(&palindrome);

  // Free memory
  FreeDictionary(&dict);
  FreePalindrome(&palindrome);  

  return 0;

}
