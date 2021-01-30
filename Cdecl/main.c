#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "string.h"

// Define the list of end tokens
char const* endTokens = " *()[];,={}";

// Enumeration for the index of the pointers to previous/next token
enum TokenPosition {

  left,
  right

};

// Structure for one token in the declaration
struct Token;
struct Token {

  // Token
  char* str;

  // Previous and next tokens
  struct Token* at[2];

};

// Allocate memory for a struct Token and initialise its properties
struct Token* AllocToken(void) {

  // Allocate memory for the token
  struct Token* token = malloc(sizeof(struct Token));

  // Initialise the properties
  token->str = NULL;
  token->at[left] = NULL;
  token->at[right] = NULL;

  // Return the token
  return token;

}

// Return true if a char is a token terminating character
bool IsCharEndOfToken(char const* const chr) {

  // If the argument is the null character
  if (*chr == '\0') {

    // This is a end token, return true
    return true;

  }

  // If the argument is in the list of end tokens
  char* endToken =
    strchr(
     endTokens,
     *chr);
  if (endToken != NULL) {

    // This is an end token, return true
    return true;

  }

  // If we reach here the char is not an end token, return false
  return false;

}

// Search the end of a struct or union inline declaration
char const* GetEndStructUnion(char const* ptrCharEnd) {

  // Discard the spaces after the struct or union keyword
  while (
    *ptrCharEnd == ' ' &&
    *ptrCharEnd != '\0') {

    ++ptrCharEnd;

  }

  // Declare a variable to manage curly braces
  int braceLevel = 0;

  // If the char is an opening curly brace
  if (*ptrCharEnd == '{') {

    // Increment the curly brace level
    ++braceLevel;

  }

  // Loop on the char of the declaration until the end of the
  // struct or union
  while (
    *ptrCharEnd != '\0' &&
    (braceLevel > 0 ||
    IsCharEndOfToken(ptrCharEnd) == false)) {

    // Move to the next character in the declaration
    ++ptrCharEnd;

    // If the char is an opening curly brace
    if (*ptrCharEnd == '{') {

      // Increment the curly brace level
      ++braceLevel;

    }

    // If the char is a closing curly brace
    if (*ptrCharEnd == '}') {

      // Decrement the curly brace level
      --braceLevel;

      if (braceLevel == 0) {

        ++ptrCharEnd;

      }

    }

  }

  return ptrCharEnd;

}

// Search the end of an assignment
char const* GetEndAssign(char const* ptrCharEnd) {

  // Declare a variable to manage curly braces
  int braceLevel = 0;

  // If the char is an opening curly brace
  if (*ptrCharEnd == '{') {

    // Increment the curly brace level
    ++braceLevel;

  }

  // Loop on the char of the declaration until the end of the
  // struct or union
  while (
    *ptrCharEnd != '\0' &&
    (braceLevel > 0 ||
    (*ptrCharEnd != ',' &&
    *ptrCharEnd != ';'))) {

    // If the char is an opening curly brace
    if (*ptrCharEnd == '{') {

      // Increment the curly brace level
      ++braceLevel;

    }

    // Move to the next character in the declaration
    ++ptrCharEnd;

    // If the char is a closing curly brace
    if (*ptrCharEnd == '}') {

      // Decrement the curly brace level
      --braceLevel;

    }

  }

  return ptrCharEnd;

}

// Extract the token as a list of struct Token from the declaration string
struct Token* ExtractTokens(char const* const decl) {

  // If the declaration is empty
  if (
    decl == NULL ||
    *decl == '\0') {

    // Nothing to extract
    return NULL;

  }

  // Declare a variable to memorise the first token
  struct Token *firstToken = NULL;

  // Declare a variable to memorise the current last token
  struct Token *lastToken = NULL;

  // Declare a pointer to loop on the declaration
  char const *ptrChar = decl;

  // Loop on the char of the declaration
  while (*ptrChar != '\0') {

    // Discard the leading spaces 
    while (*ptrChar == ' ') {

      ++ptrChar;

    }

    // Declare a second pointer to scan to the end of the current token
    char const* ptrCharEnd = ptrChar;

    // Loop on the char of the declaration until the end of the current
    // token
    while (
      *ptrCharEnd != '\0' &&
      IsCharEndOfToken(ptrCharEnd) == false) {

      // Move to the next character in the declaration
      ++ptrCharEnd;

    }

    // If this token is a struct or union type
    char* ptrUnion =
      strstr(
        ptrChar,
        "union ");
    char* ptrStruct =
      strstr(
        ptrChar,
        "struct ");
    if (
      ptrChar == ptrUnion ||
      ptrChar == ptrStruct) {

      // Get the end of the struct or union declaration
      ptrCharEnd = GetEndStructUnion(ptrCharEnd);

    }

    // If this token is an assignment
    if (*ptrChar == '=') {

      // Get the end of the assignment
      ptrCharEnd = GetEndAssign(ptrCharEnd);

    }

    // If this token is the beginning of a block
    if (*ptrChar == '{') {

      // Loop on the char of the declaration until the end of the
      // block
      while (
        *ptrCharEnd != '\0' &&
        *ptrCharEnd != '}') {

        // Move to the next character in the declaration
        ++ptrCharEnd;

      }

    }

    // Allocate memory for the new token
    struct Token* token = AllocToken();

    // If the first token hasn't been set yet
    if (firstToken == NULL) {

      // Memorise the first token
      firstToken = token;

    }

    // Get the length of the token
    size_t lenToken = ptrCharEnd - ptrChar;

    // If the token is only one char long, ptrCharEnd == ptrChar an
    // lengthToken == 0, correct that
    lenToken = (lenToken == 0 ? 1 : lenToken);

    // Allocate memory for the token in the struct Token
    token->str = malloc(sizeof(char) * (lenToken + 1));

    // Copy the token from the declaration to the struct Token
    memcpy(
      token->str,
      ptrChar,
      lenToken);
    token->str[lenToken] = '\0';

    // Set the previous token pointer
    token->at[left] = lastToken;

    // If there is a last created token
    if (lastToken != NULL) {

      // Set the next token pointer in the last created token
      lastToken->at[right] = token;

    }

    // Update the last created token
    lastToken = token;

    // Move the scanning pointer to the start of the next token
    ptrChar = (ptrChar == ptrCharEnd ? ptrChar + 1 : ptrCharEnd);

  }

  // Return the first token, i.e. the hed of the list of tokens
  return firstToken;

}

// Return true if a struct Token is a keyword
bool IsKeyword(struct Token const* const token) {

  // If the token starts with struct or union
  if (
    strstr(token->str, "struct ") == token->str ||
    strstr(token->str, "union ") == token->str) {

    // Return true
    return true;

  }

  // Declare the list of keywords
  #define NB_KEYWORD 24
  char const* keywords[NB_KEYWORD] = {
    "unsigned",
    "signed",
    "const",
    "volatile",
    "static",
    "extern",
    "register",
    "auto",
    "void",
    "char",
    "short",
    "int",
    "long",
    "float",
    "double",
    "(", ")",
    "[", "]",
    "*",
    ";",
    ",",
    "struct",
    "union"};

  // Loop on the keywords
  for (
    int iKeyword = 0;
    iKeyword < NB_KEYWORD;
    ++iKeyword) {

    // If the token is a keyword
    if (strcmp(keywords[iKeyword], token->str) == 0) {

      // Return true
      return true;

    }

  }

  return false;

}

// Search the next declarator in a list of tokens
struct Token* GetNextDeclarator(struct Token* tokens) {

  // Declare a pointer to loop on the tokens
  struct Token* token = tokens;

  // Loop until the next token which is not a keyword
  while (
    token != NULL &&
    IsKeyword(token) == true) {

    token = token->at[right];

  }

  // Return the declarator
  return token;

}

// Parse the size of an array
// 'token' in argument is the opening square bracket of the size
struct Token const* ParseArraySize(struct Token const* const token) {

  // Declare a variable to memorise the bracket level
  int level = 1;

  // Pointer to loop on the token
  struct Token const* ptr = token->at[right];

  // Separator
  char sep[2] = {'\0', '\0'};

  // Loop until the closing bracket
  while (
    ptr != NULL &&
    level > 0) {

    // Print the token
    printf(
      "%s%s",
      sep,
      ptr->str);

    // Move to the next token
    ptr = ptr->at[right];

    // Update the separator
    sep[0] = ' ';

    // If it's an opening square bracket
    if (ptr->str[0] == '[') {

      // Increase the level
      ++level;

    // Else, if it's a closing square bracket
    } else if (ptr->str[0] == ']') {

      // Decrease the level
      --level;

    }

  }

  // Return the closing bracket
  return ptr;

}

// Parse the arguments of a function
// 'token' in argument is the opening parenthesis of the argument list
struct Token const* ParseFunctionArguments(struct Token const* const token) {

  // Print the declarator is a function
  printf("a function taking as argument(s): (");

  // Declare a variable to memorise the bracket level
  int level = 1;

  // Pointer to loop on the token
  struct Token const* ptr = token->at[right];

  // Separator
  char sep[2] = {'\0', '\0'};

  // Loop until the closing parenthesis
  while (
    ptr != NULL &&
    level > 0) {

    // Print the token
    printf(
      "%s%s",
      sep,
      ptr->str);

    // Move to the next token
    ptr = ptr->at[right];

    // Update the separator
    sep[0] = ' ';

    // If it's an opening parenthesis
    if (ptr->str[0] == '(') {

      // Increase the level
      ++level;

    // Else, if it's a closing parenthesis
    } else if (ptr->str[0] == ')') {

      // Decrease the level
      --level;

    }

  }

  // Print the end of the argument list
  printf(") returning ");

  // Return the last token of the argument list
  return ptr;

}

// Parse an array dimensions declaration of a declarator
struct Token const* ParseArrayDimensions(struct Token const* declarator) {

  // Print the declarator is an array
  printf("an array of [");

  // Parse the array sizes
  struct Token const* ptr = declarator->at[right];
  while (
    ptr != NULL &&
    ptr->str[0] == '[') {

    ptr = ParseArraySize(ptr);
    if (ptr != NULL) {

      ptr = ptr->at[right];
      if (ptr->str[0] == '[') {

        printf("] by [");

      } else {

        printf("]");

      }

    }

  }

  // Print a space at the end of the dimensions declaration parsing
  printf(" ");

  // Return the pointer at the end of the array dimensions declaration
  return ptr;

}

// Parse a declarator
void ParseDeclarator(struct Token const* declarator) {

  // Print the declarator name
  printf(
    "%s is ",
    declarator->str);

  // Declare two pointers to the next token to the left and right of
  // the declarator to be processed
  struct Token const* ptrNext[2];
  ptrNext[left] = declarator->at[left];
  ptrNext[right] = declarator->at[right];

  // Declare a variable to memorise the current processing direction
  enum TokenPosition procDir = right;

  // Loop until we have parsed all the toekn to the left and right
  while (
    ptrNext[left] != NULL ||
    ptrNext[right] != NULL) {

    // If there is a next token to process in the current processing
    // direction
    if (ptrNext[procDir] != NULL) {

      // If the token to the right is an opening square bracket
      if (
        procDir == right &&
        ptrNext[right] != NULL &&
        ptrNext[right]->str[0] == '[') {

        // Parse the array dimensions
        struct Token const* ptrEndArrayDimensions =
          ParseArrayDimensions(declarator);

        // Move the next token to parse on the right of the declarator
        ptrNext[right] = ptrEndArrayDimensions->at[right];

      // Else, if the token to the right is an opening parenthesis
      } else if (
        procDir == right &&
        ptrNext[right] != NULL &&
        ptrNext[right]->str[0] == '(') {

        // Parse the function arguments
        struct Token const* ptrEndFunctionArguments =
          ParseFunctionArguments(declarator->at[right]);

        // Move the next token to parse on the right of the declarator
        ptrNext[right] = ptrEndFunctionArguments->at[right];

      // Else, if the token to the right is a closing parenthesis
      } else if (
        procDir == right &&
        ptrNext[right] != NULL &&
        ptrNext[right]->str[0] == ')') {

        // Change the processing direction to the left
        procDir = left;

        // Move the next token to parse on the right of the declarator
        ptrNext[right] = ptrNext[right]->at[right];

      // Else, if the token to the right is ';'
      } else if (
        procDir == right &&
        ptrNext[right] != NULL &&
        ptrNext[right]->str[0] == ';') {

        // Change the processing direction to the left
        procDir = left;

        // End the parsing to the right
        ptrNext[right] = NULL;

      // Else, if the token to the right is ','
      } else if (
        procDir == right &&
        ptrNext[right] != NULL &&
        ptrNext[right]->str[0] == ',') {

        // Change the processing direction to the left
        procDir = left;

        // End the parsing to the right
        ptrNext[right] = NULL;

      // Else, if the token to the left is an opening parenthesis
      } else if (
        procDir == left &&
        ptrNext[left] != NULL &&
        ptrNext[left]->str[0] == '(') {

        // Change the processing direction to the right
        procDir = right;

        // Move the next token to parse on the left of the declarator
        ptrNext[left] = ptrNext[left]->at[left];

      // Else, if the token to the left is 'const'
      } else if (
        procDir == left &&
        ptrNext[left] != NULL &&
        strcmp(ptrNext[left]->str, "const") == 0) {

        // Print "read-only"
        printf("read-only ");

        // Move the next token to parse on the left of the declarator
        ptrNext[left] = ptrNext[left]->at[left];

      // Else, if the token to the left is '*'
      } else if (
        procDir == left &&
        ptrNext[left] != NULL &&
        strcmp(ptrNext[left]->str, "*") == 0) {

        // Print "pointer to"
        printf("pointer to ");

        // Move the next token to parse on the left of the declarator
        ptrNext[left] = ptrNext[left]->at[left];

      // Else, for any other token to the left
      } else if (
        procDir == left &&
        ptrNext[left] != NULL) {

        // Print the token
        printf(
          "%s ",
          ptrNext[left]->str);

        // Move the next token to parse on the left of the declarator
        ptrNext[left] = ptrNext[left]->at[left];

      // Else, it's an unexpected sequence
      } else {

        // Print an error message and exit
        printf("\nSorry, unexpected sequence encountered. Giving up\n");
        if (ptrNext[left] != NULL) {

          printf(
            "Next token to process at left was %s\n",
            ptrNext[left]->str);

        }

        if (ptrNext[right] != NULL) {

          printf(
            "Next token to process at right was %s\n",
            ptrNext[right]->str);

        }

        exit(1);

      }

    // Else, there is no next token to process in the current direction
    } else {

      // Switch the processing direction
      procDir = (procDir == left ? right : left);

    }

  }

  // Line return to end the declarator definition
  printf("\n");

}

// Parse a list of struct Token and print the explanation in english
// of the declaration it represents
void ParseTokens(struct Token* const tokens) {

  // Declare a pointer to loop on the tokens
  struct Token* token = tokens;

  // Loop on the tokens
  while (token != NULL) {

    // Go to the next declarator
    struct Token* declarator = GetNextDeclarator(token);

    // If we have found a declarator
    if (declarator != NULL) {

      // Parse the declarator
      ParseDeclarator(declarator);

      // Move to the token following the declarator or the next call
      // to GetNextDeclarator will return the same declarator
      token = declarator->at[right];

    // Else, we haven't found a declarator, stop here
    } else {

      token = NULL;

    }

  }

}

// Free the memory used by a list of struct Token
void FreeTokens(struct Token** tokens) {

  // Pointer to loop on the tokens
  struct Token* token = *tokens;

  // Loop on the tokens
  while (token != NULL) {

    // Memorise the next token before freeing the current token
    struct Token* next = token->at[right];

    // Free the current token
    free(token->str);
    free(token);

    // Move to the next token
    token = next;

  }

  // Reset the pointer in argument
  *tokens = NULL;

}

// Parse a declaration and print its explanation in english
void cdecl(char const* const decl) {

  printf(
    "Parsing the declaration: %s\n",
    decl);

  // Break the declaration into tokens
  struct Token* tokens = ExtractTokens(decl);

  // Parse the tokens
  ParseTokens(tokens);

  // Free the tokens
  FreeTokens(&tokens);

}

// Main function
int main(
  int argc,
  char** argv) {

  // Check the argument
  if (argc != 2) {

    // Print the usage 
    printf("usage: ./main \"your C declaration here\"\n");
    exit(1); 

  }

  // Parse the declaration given in argument
  cdecl(argv[1]);

  // Return the success code
  return 0;

}
