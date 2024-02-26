#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// Token types
typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_KEYWORD,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_OPERATOR,
    TOKEN_SEPARATOR,
    TOKEN_COMMENT,
    TOKEN_ERROR,
    TOKEN_END
} TokenType;

// Token structure
typedef struct {
    TokenType type;
    char value[100]; // Assuming maximum token length is 100 characters
} Token;

// Function to check if a string is a keyword
int isKeyword(const char *str) {
    char *keywords[] = {"int", "float", "if", "else", "while", "for", "return"};
    int numKeywords = sizeof(keywords) / sizeof(keywords[0]);
    for (int i = 0; i < numKeywords; ++i) {
        if (strcmp(str, keywords[i]) == 0)
            return 1;
    }
    return 0;
}

// Function to get next token from input file
Token getNextToken(FILE *fp) {
    Token token;
    char c = fgetc(fp);

    // Skip whitespace characters
    while (isspace(c))
        c = fgetc(fp);

    // Identify token type
    if (isalpha(c) || c == '_') { // Identifier or keyword
        int i = 0;
        token.value[i++] = c;
        while ((c = fgetc(fp)) && (isalnum(c) || c == '_')) {
            token.value[i++] = c;
        }
        token.value[i] = '\0';
        if (isKeyword(token.value))
            token.type = TOKEN_KEYWORD;
        else
            token.type = TOKEN_IDENTIFIER;
        ungetc(c, fp);
    } else if (isdigit(c)) { // Number
        int i = 0;
        token.value[i++] = c;
        while ((c = fgetc(fp)) && (isdigit(c) || c == '.')) {
            token.value[i++] = c;
        }
        token.value[i] = '\0';
        token.type = TOKEN_NUMBER;
        ungetc(c, fp);
    } else if (c == '\"') { // String
        int i = 0;
        token.value[i++] = c;
        while ((c = fgetc(fp)) && c != '\"') {
            token.value[i++] = c;
        }
        if (c == '\"') {
            token.value[i++] = c;
            token.value[i] = '\0';
            token.type = TOKEN_STRING;
        } else {
            token.type = TOKEN_ERROR;
        }
    } else if (c == '/' && (c = fgetc(fp)) == '/') { // Single-line comment
        int i = 0;
        token.value[i++] = '/';
        token.value[i++] = '/';
        while ((c = fgetc(fp)) && c != '\n') {
            token.value[i++] = c;
        }
        token.value[i] = '\0';
        token.type = TOKEN_COMMENT;
    } else if (c == '/' && (c = fgetc(fp)) == '*') { // Multi-line comment
        int i = 0;
        token.value[i++] = '/';
        token.value[i++] = '*';
        while ((c = fgetc(fp))) {
            token.value[i++] = c;
            if (c == '*' && (c = fgetc(fp)) == '/') {
                token.value[i++] = c;
                break;
            }
        }
        token.value[i] = '\0';
        token.type = TOKEN_COMMENT;
    } else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '&' || c == '|' || c == '!') { // Operators
        int i = 0;
        token.value[i++] = c;
        if ((c == '+' || c == '-' || c == '=' || c == '&' || c == '|') && (c = fgetc(fp)) == c) {
            token.value[i++] = c;
        } else if ((c == '=' || c == '<' || c == '>') && (c = fgetc(fp)) == '=') {
            token.value[i++] = c;
        } else {
            ungetc(c, fp);
        }
        token.value[i] = '\0';
        token.type = TOKEN_OPERATOR;
    } else if (c == ';' || c == ',' || c == '(' || c == ')' || c == '{' || c == '}') { // Separators
        token.value[0] = c;
        token.value[1] = '\0';
        token.type = TOKEN_SEPARATOR;
    } else if (c == EOF) { // End of file
        token.type = TOKEN_END;
    } else { // Error
        token.value[0] = c;
        token.value[1] = '\0';
        token.type = TOKEN_ERROR;
    }

    return token;
}

// Function to print token type
const char* tokenTypeToString(TokenType type) {
    switch(type) {
        case TOKEN_IDENTIFIER:
            return "Identifier";
        case TOKEN_KEYWORD:
            return "Keyword";
        case TOKEN_NUMBER:
            return "Number";
        case TOKEN_STRING:
            return "String";
        case TOKEN_OPERATOR:
            return "Operator";
        case TOKEN_SEPARATOR:
            return "Separator";
        case TOKEN_COMMENT:
            return "Comment";
        case TOKEN_ERROR:
            return "Error";
        case TOKEN_END:
            return "End";
        default:
            return "Unknown";
    }
}

int main() {
    FILE *fp;
    Token token;

    // Open file for reading
    fp = fopen("input.txt", "r");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    // Tokenize input
    while ((token = getNextToken(fp)).type != TOKEN_END) {
        printf("%-12s: %s\n", tokenTypeToString(token.type), token.value);
    }

    // Close file
    fclose(fp);

    return 0;
}
