#include <iostream>
#include <fstream>
#include <regex>
#include <map>
#include <string>

enum TokenType
{
    KEYWORD,        // 0
    IDENTIFIER,     // 1
    CONSTANT,       // 2
    STRING_LITERAL, // 3
    OPERATOR,       // 4
    PUNCTUATOR,     // 5
    PREPROCESSOR,   // 6
    WHITESPACE,     // 7
    COMMENT,        // 8
    UNKNOWN         // 9
};

std::map<TokenType, std::string> tokenTypeToString = {
    {KEYWORD, "KEYWORD"},
    {IDENTIFIER, "IDENTIFIER"},
    {CONSTANT, "CONSTANT"},
    {STRING_LITERAL, "STRING_LITERAL"},
    {OPERATOR, "OPERATOR"},
    {PUNCTUATOR, "PUNCTUATOR"},
    {PREPROCESSOR, "PREPROCESSOR"},
    {WHITESPACE, "WHITESPACE"},
    {COMMENT, "COMMENT"},
    {UNKNOWN, "UNKNOWN"}};

std::map<TokenType, std::regex> tokenPatterns = {
    {KEYWORD, std::regex("\\b(auto|break|case|char|const|continue|default|do|double|else|enum|extern|float|for|goto|if|inline|int|long|register|return|short|signed|sizeof|static|struct|switch|typedef|union|unsigned|void|volatile|while)\\b")},
    {IDENTIFIER, std::regex("[a-zA-Z_][a-zA-Z0-9_]*")},
    {CONSTANT, std::regex("\\b([0-9]+)\\b")},
    {STRING_LITERAL, std::regex("\"([^\"\\\\]|\\\\.)*\"")},
    {OPERATOR, std::regex("[\\+\\-\\*/%&|^<>!]?=|[<>]=?|[\\+\\-\\*/%&|^<>!]")},
    {PUNCTUATOR, std::regex("[\\[\\]\\(\\)\\{\\}\\.,;]")},
    {PREPROCESSOR, std::regex("^#[a-zA-Z]+")},
    {WHITESPACE, std::regex("\\s+")},
    {COMMENT, std::regex("//.*|/\\*.*?\\*/")}};

void lexicalAnalyzer(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::string::const_iterator searchStart(line.cbegin());
        while (searchStart != line.cend())
        {
            bool matchFound = false;
            for (const auto &pair : tokenPatterns)
            {
                std::smatch match;
                if (std::regex_search(searchStart, line.cend(), match, pair.second))
                {
                    std::cout << "Token: " << match.str() << ", Type: " << tokenTypeToString[pair.first] << std::endl;
                    searchStart += match.position() + match.length();
                    matchFound = true;
                    break;
                }
            }
            if (!matchFound)
            {
                std::cerr << "Unknown token: " << *searchStart << std::endl;
                ++searchStart;
            }
        }
    }
}

int main()
{
    lexicalAnalyzer("test.c");
    return 0;
}
