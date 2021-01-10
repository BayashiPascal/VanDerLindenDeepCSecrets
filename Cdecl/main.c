#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "string.h"

// Define the list of end tokens
char const* endTokens = " *()[];,=&";

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
  struct Token* firstToken = NULL;

  // Declare a variable to memorise the current last token
  struct Token* lastToken = NULL;

  // Declare a pointer to loop on the declaration
  char const* ptrChar = decl;

  // Loop on the char of the declaration
  while (*ptrChar != '\0') {

    // Discard the leading spaces 
    while (*ptrChar == ' ') ++ptrChar;

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
    char* foundUnion =
      strstr(
        ptrChar,
        "union ");
    char* foundStruct =
      strstr(
        ptrChar,
        "struct ");
    if (
      foundUnion != NULL ||
      foundStruct != NULL) {

      // Discard the spaces after the struct or union keyword
      while (
        *ptrCharEnd == ' ' &&
        *ptrCharEnd != '\0') {

        ++ptrCharEnd;

      }

      // Loop on the char of the declaration until the end of the current
      // token
      while (
        *ptrCharEnd != '\0' &&
        IsCharEndOfToken(ptrCharEnd) == false) {

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

  // Return the first token
  return firstToken;

}

// Return true if a struct Token is a keyword
bool IsKeyword(struct Token const* const token) {

  // Declare the list of keywords
  char const* keywords =
    "unsigned "
    "signed "
    "const "
    "volatile "
    "static "
    "extern "
    "register "
    "auto "
    "void "
    "char "
    "short "
    "int "
    "long "
    "float "
    "double "
    "struct "
    "union ";

  // If the keywords contain the token or the token starts with
  // struct or union
  if (
    strstr(keywords, token->str) != NULL ||
    strstr(token->str, "struct ") == token->str ||
    strstr(token->str, "union ") == token->str) {

    // Return true
    return true;

  }

  return false;

}

// Return true if a list of struct Token contains a string
bool ContainsString(
  struct Token const* const tokens,
  char* str) {

  // Declare a pointer to loop on the tokens
  struct Token const* token = tokens;

  // Loop on the token
  while (token != NULL) {

    // If the token contains the string
    if (strstr(token->str, str) != NULL) {

      // Return true
      return true;

    }

    // Move to the next token
    token = token->at[right];

  }

  // If we reach here the list of tokens doesn't contains the string,
  // return false
  return false;

}

// Return true if a list of struct Token can be parsed by ParseTokens
bool IsValidTokens(struct Token const* const tokens) {

  // If the first token is not a keyword
  if (IsKeyword(tokens) == false) {

    // This is not a parsable declaration, return false
    return false;

  }

  // If the list of token includes ',', '{' or '=' character
  if (
    ContainsString(tokens, ",") == true ||
    ContainsString(tokens, "=") == true ||
    ContainsString(tokens, "{") == true) {

    // This is not a parsable declaration, return false
    return false;

  }

  // If we reach here the list of tokens is parsable, return true
  return true;

}

// Parse a list of struct Token and print the explanation in english
// of the declaration it represents
void ParseTokens(struct Token const* const tokens) {

  // If the list of tokens is not a supported declaration
  bool isValid = IsValidTokens(tokens);
  if (isValid == false) {

    // Print a message and stop here
    printf("This declaration cannot be parse, sorry!\n");
    return;

  }

  // Declare a pointer to loop on the tokens
  struct Token const* token = tokens;

  // Loop on the tokens
  while (token != NULL) {

    // Print the token
    printf(
      "%s|",
      token->str);

    // Move to the next token
    token = token->at[right];

  }

  printf("\n");

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
