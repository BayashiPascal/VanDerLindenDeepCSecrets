#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "stdbool.h"
#include "time.h"
#include "math.h"

// Shortcut to get a random value in [0.0, 1.0]
#define rnd() ((double)rand())/((double)RAND_MAX)

char const* const pathDictionary = "./nounlist.txt";
int const nbWordDictionary = 6801;
//char const* const pathDictionary = "./test.txt";
//int const nbWordDictionary = 5;

int const sizeMaxWord = 512;
char const* const headPalindrome = "A man, a plan";
char const* const seedPalindrome = "aca";
char const* const tailPalindrome = "a canal, Panama";
int const nbWordPalindrome = 1;

struct Word;
struct Word {

  // The word
  char* str;

  // Next word
  struct Word* next;

  // Flag to memorise if the word has been already used
  bool used;

};

struct Dictionary {

  // The words in the dictionary
  struct Word* words;

};

struct Palindrome {

  // The words in the palindrome
  struct Word* words;

  // Length of the palindrome
  int length;

};

enum DirSearch {

  atHead,
  atTail

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

    // Read one word, leave space at the beginning of the buffer
    // to add the prefix later
    int ret =
      fscanf(
        filePtr,
        "%s",
        buffer + 3);
    (void)ret;

    // Create the word instance
    struct Word* word = malloc(sizeof(struct Word));
    word->next = NULL;
    word->used = false;

    // Get the voyel at the head of the word, if any
    char* ptrVoyelAtHead =
      strchr(
        "aeiouAEIOU",
        buffer[0]);

    // If there is a voyel at the head of the word
    if (ptrVoyelAtHead != NULL) {

      // Add "an " to the head of the buffer
      buffer[0] = 'a';
      buffer[1] = 'n';
      buffer[2] = ' ';

      // Set the word's string
      word->str = strdup(buffer);

    // Else, there is no voyel at the head of the word
    } else {

      // Add " a " to the head of the buffer
      buffer[0] = ' ';
      buffer[1] = 'a';
      buffer[2] = ' ';

      // Set the word's string
      word->str = strdup(buffer + 1);

    }

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

bool WordIsInverseOf(
  struct Word const* const that,
  struct Word const* const tho) {

  // Get the length of each word
  int lenThat = strlen(that->str);
  int lenTho = strlen(tho->str);

  // Loop on the characters of the words
  for (
    int iChar = 0, jChar = 0;
    iChar < lenThat && jChar < lenTho;
    ++iChar, ++iChar) {

    // Skip punctuation in that
    while (
      iChar < lenThat &&
      (that->str[iChar] == ' ' ||
      that->str[iChar] == ',')) {

      ++iChar;

    }

    // Skip punctuation in tho
    while (
      jChar < lenTho &&
      (that->str[jChar] == ' ' ||
      that->str[jChar] == ',')) {

      ++jChar;

    }

    // If the character in one word and the one at the opposite position
    // in the other word are not equal
    if (that->str[iChar] != tho->str[lenTho - 1 - jChar]) {

      // The words are not the inverse of each other, return false
      return false;

    }

  }

  // If we reach here, the words are the inverse of each other, return true
  return true;

}

bool WordIsPalindrome(struct Word const* const that) {

  // Check if the word is its own inverse
  bool isPalindrome =
    WordIsInverseOf(
      that,
      that);

  // Return the result
  return isPalindrome;

}

bool WordTailMatchSeed(
  struct Word const* const that,
  char* const seed) {

  // Declare a variable to memorise the result
  bool isMatching = true;

  // Get the length of the seed
  int const lenSeed = strlen(seed);

  // Get the length of the word
  int const lenWord = strlen(that->str);

  // Declare a variable to memorise the position in the word's string
  int iWordChar = 0;

  // Loop on the seed
  for (
    int iSeedChar = 0;
    iSeedChar < lenSeed && 
    iWordChar < lenWord && 
    isMatching == true;
    ++iSeedChar, ++iWordChar) {

    // Skip the punctuation in the seed
    while (
      iSeedChar < lenSeed &&
      (seed[lenSeed - 1 - iSeedChar] == ',' ||
      seed[lenSeed - 1 - iSeedChar] == ' ')) {

      ++iSeedChar;

    }

    // Skip the punctuation in the word
    while (
      iWordChar < lenWord && 
      (that->str[lenWord - 1 - iWordChar] == ',' ||
      that->str[lenWord - 1 - iWordChar] == ' ')) {

      ++iWordChar;

    }

    // If the corresponding char in the word doesn't match the char
    // in the seed
    if (
      iSeedChar < lenSeed && 
      iWordChar < lenWord && 
      seed[lenSeed - 1 - iSeedChar] != that->str[lenWord - 1 - iWordChar]) {

      // Update the result
      isMatching = false;

    }

  }

  // If we have reached the end of the world
  if (iWordChar == lenWord) {

    // Update the result
    isMatching = false;

  }

  // If the word is matching
  if (isMatching == true) {

    // Declare a variable to loop on the new seed char
    int iSeedChar = 0;

    // Loop on the remaining characters of the word
    while (iWordChar < lenWord) {

      // If the char in the word is not a punctuation
      if (
        that->str[lenWord - 1 - iWordChar] != ' ' &&
        that->str[lenWord - 1 - iWordChar] != ',') {

        // Copy the char in the seed
        seed[iSeedChar] = that->str[lenWord - 1 - iWordChar];

        // Move to the next char in the seed
        ++iSeedChar;

      }

      // Move to the next char in the word
      ++iWordChar;

    }

    // Set the null character at the end of the new seed
    seed[iSeedChar] = '\0';

  }

  // Return the result
  return isMatching;

}

bool WordHeadMatchSeed(
  struct Word const* const that,
  char* const seed) {

  // Declare a variable to memorise the result
  bool isMatching = true;

  // Get the length of the seed
  int const lenSeed = strlen(seed);

  // Get the length of the word
  int const lenWord = strlen(that->str);

  // Declare a variable to memorise the position in the word's string
  int iWordChar = 0;

  // Loop on the seed
  for (
    int iSeedChar = 0;
    iSeedChar < lenSeed && 
    iWordChar < lenWord && 
    isMatching == true;
    ++iSeedChar, ++iWordChar) {

    // Skip the punctuation in the seed
    while (
      iSeedChar < lenSeed &&
      (seed[iSeedChar] == ',' ||
      seed[iSeedChar] == ' ')) {

      ++iSeedChar;

    }

    // Skip the punctuation in the word
    while (
      iWordChar < lenWord && 
      (that->str[iWordChar] == ',' ||
      that->str[iWordChar] == ' ')) {

      ++iWordChar;

    }

    // If the corresponding char in the word doesn't match the char
    // in the seed
    if (
      iSeedChar < lenSeed && 
      iWordChar < lenWord && 
      seed[iSeedChar] != that->str[iWordChar]) {

      // Update the result
      isMatching = false;

    }

  }

  // If we have reached the end of the world
  if (iWordChar == lenWord) {

    // Update the result
    isMatching = false;

  }

  // If the word is matching
  if (isMatching == true) {

    // Declare a variable to loop on the new seed's char
    int iSeedChar = 0;

    // Declare a variable to loop back on the remaining of the word
    int jWordChar = lenWord - 1;

    // Loop on the remaining characters of the word
    while (iWordChar <= jWordChar) {

      // If the char in the word is not a punctuation
      if (
        that->str[jWordChar] != ' ' &&
        that->str[jWordChar] != ',') {

        // Copy the char in the seed
        seed[iSeedChar] = that->str[jWordChar];

        // Move to the next char in the seed
        ++iSeedChar;

      }

      // Move to the next char in the word
      --jWordChar;

    }

    // Set the null character at the end of the new seed
    seed[iSeedChar] = '\0';

  }

  // Return the result
  return isMatching;

}

struct Word* SearchNextWord(
  struct Dictionary const* const dict,
  char* const seed,
  enum DirSearch const dirSearch,
  bool const searchPalindrome) {

  // Declare a pointer to loop on words in the dictionary
  struct Word* ptrWord = dict->words;

  // Declare a variable to memorise if we have found the next word
  bool isMatching = false;

  // Declare a variable to memorise the current filtering of word
  // (palindrome or not)
  bool filterPalindrome = searchPalindrome;

  // Loop on words in dictionary
  while (
    ptrWord != NULL &&
    isMatching == false) {

    // If the word has not already been used and mach the current
    // filtering (palindrome or not)
    if (
      ptrWord->used == false &&
      (filterPalindrome == false ||
      WordIsPalindrome(ptrWord) == filterPalindrome)) {

      // If we search a word based on its tail
      if (dirSearch == atTail) {

        // Check if this word is matching
        isMatching =
          WordTailMatchSeed(
            ptrWord,
            seed);

        // If the word is not matching
        if (isMatching == false) {

          // Move to the next word
          ptrWord = ptrWord->next;

        }

      // Else, we search a word based on its head
      } else {

        // Check if this word is matching
        isMatching =
          WordHeadMatchSeed(
            ptrWord,
            seed);

        // If the word is not matching
        if (isMatching == false) {

          // Move to the next word
          ptrWord = ptrWord->next;

        }

      }

    } else {

      // Move to the next word
      ptrWord = ptrWord->next;

    }

    // If we have reached the end of the dictionary and the filtering
    // of palindrome was active
    if (
      ptrWord == NULL &&
      filterPalindrome == true) {

      // Search again from the beginning without filtering
      filterPalindrome = false;
      ptrWord = dict->words;

    }

  }

  // Return the next word
  return ptrWord;

}

void PrintPalindrome(struct Palindrome const* const that) {

  // Declare a pointer to loop on the words of the palindrome
  struct Word const* ptr = that->words;

  // Separator between words
  char sep = ',';

  // Loop on the words of the palindrome
  while (ptr != NULL) {

    // If it's the last word
    if (ptr->next == NULL) {

      // Update the separator
      sep = '.';

    }

    // Print the word
    printf(
      "%s%c ",
      ptr->str,
      sep);

    // Move to the next word
    ptr = ptr->next;

  }

  // Print a line return at the end of the palindrome
  printf("\n");

}

struct Palindrome CreatePalindrome(
  struct Dictionary const* const dict,
  int const nbWord) {

  // Declare the result palindrome
  struct Palindrome palindrome;

  // Declare a pointer to memorise the last added word
  struct Word* ptrLast = NULL;

  // Starts the palindrome by adding the head string
  palindrome.words = malloc(sizeof(struct Word));
  ptrLast = palindrome.words;
  ptrLast->str = strdup(headPalindrome);

  // Add the tail string
  ptrLast->next = malloc(sizeof(struct Word));
  ptrLast->next->str = strdup(tailPalindrome);
  ptrLast->next->next = NULL;

  // Declare a variable to memorise the current number of words in the
  // palindrome (except the head and tail)
  int curNbWord = 0;

  // Declare a buffer to memorise the current seed for the searched word
  char seed[sizeMaxWord];

  // Initialise the current seed
  sprintf(
    seed,
    "%s",
    seedPalindrome);

  // Declare a variable to memorise the current direction of search
  // for the next word
  enum DirSearch dirSearch = atHead;

  // Declare a variable to memorise the next word to add
  // Initialise it to the last added word to start the following loop
  struct Word* nextWord = ptrLast;

  // Loop until we couldn't find the next word or, 
  // we have enough words and the last found word is a palindrome
  while (
    nextWord != NULL &&
    (curNbWord < nbWord ||
    WordIsPalindrome(nextWord) == false)) {

    // Declare a flag to memorise if we search a palindrome word
    bool searchPalindrome = false;

    // If we have enough words in the palindrome
    if (curNbWord >= nbWord) {

      // We will search preferentially palindrome to terminate the
      // creation of the palindrome
      searchPalindrome = true;

    }

    // Search the next word to extend the palindrome
    nextWord =
      SearchNextWord(
        dict,
        seed,
        dirSearch,
        searchPalindrome);

    // If we could find the next word
    if (nextWord != NULL) {

      // Mark the word as used
      nextWord->used = true;

      // Memorise the word following word to the current last added word
      struct Word* const ptrLastNext = ptrLast->next;

      // Add the next word to the palindrome
      ptrLast->next = WordClone(nextWord);

      // Reconnect the previous next word
      ptrLast->next->next = ptrLastNext;

      // If we are searching for the next word with the seed at the tail
      if (dirSearch == atTail) {

        // Inverse the search direction
        dirSearch = atHead;

      // Else, we are searching for the next word with the seed at the head
      } else {

        // Update the last added word
        ptrLast = ptrLast->next;

        // Inverse the search direction
        dirSearch = atTail;

      }

      // Update the number of words in the palindrome
      curNbWord += 1;

    // Else, we couldn't find the next word
    } else {

      printf("Failed to find a word that ");
      if (dirSearch == atHead) {

        printf("starts ");

      } else {

        printf("ends ");

      }

      printf(
        "with \"%s\". The palindrome was %d word long.\n",
        seed,
        curNbWord);

    }

  }

  // If we could create the palindrome
  if (
    nextWord != NULL &&
    curNbWord >= nbWord &&
    WordIsPalindrome(nextWord) == true) {

    // Update the length of the palindrome to signify success
    palindrome.length = curNbWord;

  // Else, we couldn't create the palindrome
  } else {

    // Set the length to 0 to signify failure
    palindrome.length = 0;

  }

  // Return the palindrome
  return palindrome;

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

void ShuffleDictionary(struct Dictionary* const dict) {

  // Loop as many time as there are words in the dictionary
  for (
    int i = 0;
    i < nbWordDictionary;
    ++i) {

    // Get the random index of the first word
    int iWord = (int)round(rnd() * (double)(nbWordDictionary - 1));

    // Get the random index of the second word
    int jWord = (int)round(rnd() * (double)(nbWordDictionary - 1));

    // Get a pointer to the first word
    struct Word* ptrIWord = dict->words;
    while (iWord > 0) {

      ptrIWord = ptrIWord->next;
      --iWord;

    }

    // Get a pointer to the second word
    struct Word* ptrJWord = dict->words;
    while (jWord > 0) {

      ptrJWord = ptrJWord->next;
      --jWord;

    }

    // Swap the string of the two words
    char* str = ptrIWord->str;
    ptrIWord->str = ptrJWord->str;
    ptrJWord->str = str;

  }

}

void ResetDictionary(struct Dictionary* const that) {

  // Declare a pointer to loop on the words
  struct Word* ptr = that->words;

  // Loop on the words
  while (ptr != NULL) {

    // Reset the used flag of the word
    ptr->used = false;

    // Move to the next word
    ptr = ptr->next;

  }

}

int main() {

  // Load the dictionary
  struct Dictionary dict = LoadDictionary(pathDictionary);

  // Initialise the random generator
  srand(time(NULL));

  // Declare the result palindrome
  struct Palindrome palindrome;
  palindrome.length = 0;

  // Loop until we find the palindrome with requested length
  while (palindrome.length < nbWordPalindrome) {

    // Shuffle the dictionary
    ShuffleDictionary(&dict);

    // Reset the used flag of words
    ResetDictionary(&dict);

    // Create the palindrome
    palindrome =
      CreatePalindrome(
        &dict,
        nbWordPalindrome);

    // Print the palindrome
    PrintPalindrome(&palindrome);

    // Free memory
    FreePalindrome(&palindrome);  

  }

  // Free memory
  FreeDictionary(&dict);

  return 0;

}
