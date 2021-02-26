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

  // Flag to memorize if this token is a declarator
  bool isDeclarator;

  // Flag to memorise if this token is part of a function declaration
  bool isArgument; 

};

// Allocate memory for a struct Token and initialise its properties
struct Token* AllocToken(void) {

  // Allocate memory for the token
  struct Token* token = malloc(sizeof(struct Token));

  // Initialise the properties
  token->str = NULL;
  token->at[left] = NULL;
  token->at[right] = NULL;
  token->isDeclarator = false;
  token->isArgument = false;

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
    strstr(token->str, "=") == token->str ||
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

  // Declare a variable to memorise the level of square brackets
  int levelBracket = 0;

  // Loop until the next token which is not a keyword
  while (
    token != NULL &&
    (levelBracket > 0 ||
    token->isArgument == true ||
    IsKeyword(token) == true)) {

    // If the token is an opening square bracket
    if (token->str[0] == '[') {

      // Increment the level of square bracket
      ++levelBracket;

    }

    token = token->at[right];

    // If the next token is a closing square bracket
    if (
      token != NULL &&
      token->str[0] == ']') {

      // Decrement the level of square bracket
      --levelBracket;

    }

  }

  // If we have found a declarator
  if (token != NULL) {

    token->isDeclarator = true;

  }

  // Return the declarator
  return token;

}

// Parse the size of an array
// 'token' in argument is the opening square bracket of the size
struct Token* ParseArraySize(struct Token* const token) {

  // Declare a variable to memorise the bracket level
  int level = 1;

  // Pointer to loop on the token
  struct Token* ptr = token->at[right];

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
struct Token* ParseFunctionArguments(struct Token* const token) {

  // Print the declarator is a function
  printf("function taking as argument(s): (");

  // Declare a variable to memorise the bracket level
  int level = 1;

  // Pointer to loop on the token
  struct Token* ptr = token->at[right];

  // If the list of argument is not empty
  if (ptr->str[0] != ')') {

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

      // Mark the token as being an argument function
      ptr->isArgument = true;

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

  }

  // Print the end of the argument list
  printf(") and returning ");

  // Return the last token of the argument list
  return ptr;

}

// Parse an array dimensions declaration of a token
struct Token* ParseArrayDimensions(struct Token* token) {

  // Print the declarator is an array
  printf("array of [");

  // Parse the array sizes
  struct Token* ptr = token;
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

  // Return the pointer after the end of the array dimensions declaration
  return ptr;

}

// Get for the next token to process to the left when we encounter a comma
struct Token* GetTokenBeforeComma(struct Token const* const token) {

  // Declare a pointer to the next token to the left to process
  struct Token* ptr = token->at[left];

  // Declare a variable to memorise the closing parenthesis
  int levelParenthesis = 0;

  // Loop until the previous declarator
  while (
    ptr != NULL &&
    ptr->isDeclarator == false) {

    // If it's a closing parenthesis
    if (ptr->str[0] == ')') {

      // Increment the level of parenthesis
      ++levelParenthesis;

    }

    // If it's an opening parenthesis
    if (ptr->str[0] == '(') {

      // Decrement the level of parenthesis
      --levelParenthesis;

    }

    // Move to the left
    ptr = ptr->at[left];

  }

  // Loop until the remaining parenthesis scopes are closed
  while (
    ptr != NULL &&
    levelParenthesis > 0) {

    // If it's a closing parenthesis
    if (ptr->str[0] == ')') {

      // Increment the level of parenthesis
      ++levelParenthesis;

    }

    // If it's an opening parenthesis
    if (ptr->str[0] == '(') {

      // Decrement the level of parenthesis
      --levelParenthesis;

    }

    // Move to the left
    ptr = ptr->at[left];

  }

  // Here we are on the previous declarator or its fist opening
  // parenthesis, move to the left token
  if (ptr != NULL) {

    ptr = ptr->at[left];

  }

  // Return the next token to process
  return ptr;

}

enum FSMStatus {

  FSMStatus_init,
  FSMStatus_A,
  FSMStatus_B,
  FSMStatus_C,
  FSMStatus_D,
  FSMStatus_E,
  FSMStatus_F,
  FSMStatus_terminate,
  FSMStatus_end,
  FSMStatus_nb

};

struct FSM;
struct FSM {

  enum FSMStatus status;
  struct Token* tokens;
  struct Token* identifier;
  struct Token* nextToken[2];
  void (*actions[FSMStatus_nb])(struct FSM*);

};

void ActionInit(struct FSM* fsm) {

  fsm->identifier = GetNextDeclarator(fsm->tokens);
  fsm->nextToken[right] = fsm->identifier->at[right];
  fsm->nextToken[left] = fsm->identifier->at[left];
  printf(
    "%s is ",
    fsm->identifier->str);
  fsm->status = FSMStatus_A;

}

void ActionA(struct FSM* fsm) {

  if (fsm->nextToken[right] && fsm->nextToken[right]->str[0] == '[') {

    printf("array of ");
    fsm->nextToken[right] = ParseArraySize(fsm->nextToken[right]);
    fsm->status = FSMStatus_C;

  } else {

    fsm->status = FSMStatus_B;

  }

}

void ActionB(struct FSM* fsm) {

  if (fsm->nextToken[right] && fsm->nextToken[right]->str[0] == '(') {

    fsm->nextToken[right] = ParseFunctionArguments(fsm->nextToken[right]);
    fsm->nextToken[right] = fsm->nextToken[right]->at[right];
    fsm->status = FSMStatus_D;

  } else {

    fsm->status = FSMStatus_C;

  }

}

void ActionC(struct FSM* fsm) {

  fsm->status = FSMStatus_D;

}

void ActionD(struct FSM* fsm) {

  fsm->status = FSMStatus_E;

}

void ActionE(struct FSM* fsm) {

  fsm->status = FSMStatus_F;

}

void ActionF(struct FSM* fsm) {

  if (fsm->nextToken[left] && fsm->nextToken[left]->str[0] == '(') {

    while (fsm->nextToken[right] && fsm->nextToken[right]->str[0] != ')') {

      fsm->nextToken[right] = fsm->nextToken[right]->at[right];

    }
    if (fsm->nextToken[right]) {

      fsm->nextToken[right] = fsm->nextToken[right]->at[right];

    }
    fsm->nextToken[left] = fsm->nextToken[left]->at[left];
    fsm->status = FSMStatus_A;

  } else if (
    fsm->nextToken[left] &&
    strcmp(fsm->nextToken[left]->str, "const") == 0) {

    printf(
      "%s ",
      fsm->nextToken[left]->str);
    fsm->nextToken[left] = fsm->nextToken[left]->at[left];
    fsm->status = FSMStatus_E;

  } else if (
    fsm->nextToken[left] &&
    strcmp(fsm->nextToken[left]->str, "volatile") == 0) {

    printf(
      "%s ",
      fsm->nextToken[left]->str);
    fsm->nextToken[left] = fsm->nextToken[left]->at[left];
    fsm->status = FSMStatus_E;

  } else if (
    fsm->nextToken[left] &&
    fsm->nextToken[left]->str[0] == '*') {

    printf("pointer to ");
    fsm->nextToken[left] = fsm->nextToken[left]->at[left];
    fsm->status = FSMStatus_E;

  } else {

    fsm->status = FSMStatus_terminate;

  }

}

void ActionTerminate(struct FSM* fsm) {

  do {

    printf(
      "%s ",
      fsm->nextToken[left]->str);
    fsm->nextToken[left] = fsm->nextToken[left]->at[left];

  } while (fsm->nextToken[left]);
  printf("\n");
  fsm->status = FSMStatus_end;

}

// Initialise the FSM
void FSMInit(struct FSM* const fsm) {

  fsm->status = FSMStatus_init;
  fsm->actions[FSMStatus_init] = ActionInit;
  fsm->actions[FSMStatus_A] = ActionA;
  fsm->actions[FSMStatus_B] = ActionB;
  fsm->actions[FSMStatus_C] = ActionC;
  fsm->actions[FSMStatus_D] = ActionD;
  fsm->actions[FSMStatus_E] = ActionE;
  fsm->actions[FSMStatus_F] = ActionF;
  fsm->actions[FSMStatus_terminate] = ActionTerminate;
  fsm->actions[FSMStatus_end] = NULL;

}

// Step the finite state machine
void FSMStep(struct FSM* const fsm) {

  (*(fsm->actions[fsm->status]))(fsm);

}

// Parse a list of struct Token and print the explanation in english
// of the declaration it represents
void ParseTokens(struct Token* const tokens) {

  // Declare the finite state machine
  struct FSM fsm;

  // Initialise the finite state machine
  fsm.tokens = tokens;
  FSMInit(&fsm);

  // Run the finite state machine
  while (fsm.status != FSMStatus_end) {

    FSMStep(&fsm);

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
