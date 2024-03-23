#include <iostream>
#include <fstream>
#include <regex>
#include <map>
#include <string>

using namespace std;

enum TokenType
{
    STRING_LITERAL, // 3
    PREPROCESSOR,   // 6
    COMMENT,        // 8
    KEYWORD,        // 0
    IDENTIFIER,     // 1
    OPERATOR,       // 4
    CONSTANT,       // 2
    PUNCTUATOR,     // 5
    WHITESPACE,     // 7
    UNKNOWN         // 9
};

std::map<TokenType, std::string> tokenTypeToString = {
     {STRING_LITERAL, "STRING_LITERAL"},
    {KEYWORD, "KEYWORD"},
    {IDENTIFIER, "IDENTIFIER"},
    {CONSTANT, "CONSTANT"},
    {OPERATOR, "OPERATOR"},
    {PUNCTUATOR, "PUNCTUATOR"},
    {PREPROCESSOR, "PREPROCESSOR"},
    {WHITESPACE, "WHITESPACE"},
    {COMMENT, "COMMENT"},
    {UNKNOWN, "UNKNOWN"}
};

std::map<TokenType, std::regex> tokenPatterns = {
    {STRING_LITERAL, std::regex("\"([^\"\\\\]|\\\\[\\s\\S])*\"")},
    {KEYWORD, std::regex("\\b(auto|break|case|char|const|continue|default|do|double|else|enum|extern|float|for|goto|if|inline|int|long|register|return|short|signed|sizeof|static|struct|switch|typedef|union|unsigned|void|volatile|while)\\b")},
    {IDENTIFIER, std::regex("[a-zA-Z_][a-zA-Z0-9_]*")},
   {CONSTANT, std::regex("(((\\+-)*\\+|(-\\+)*-?)(0|[1-9][0-9]*)(\\.([0-9]*))?((e|E)(\\+|-)?[0-9]+)?)")},
    {OPERATOR, std::regex("[+\\-\\*/%&|^<>!=]?=|[<>]=?|[+\\-*/%&|^<>!]")},
    {PUNCTUATOR, std::regex("[\\[\\]\\(\\)\\{\\}\\.,;]")},
    {PREPROCESSOR, std::regex("^[[:blank:]]*#[[:blank:]]*[A-Za-z_][A-Za-z0-9_]*[[:blank:]]*<[^>]*>?")},
    {WHITESPACE, std::regex("\\s+")},
    {COMMENT, std::regex("//.*|/\\*[^*]*\\*+(?:[^/*][^*]*\\*+)*/")}
};


void lexicalAnalyzer(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    // Combined regex pattern
    std::regex combinedPattern = std::regex(
        "\"([^\"\\\\]|\\\\[\\s\\S])*\"" // STRING_LITERAL
        "|\\b(auto|break|case|char|const|continue|default|do|double|else|enum|extern|float|for|goto|if|inline|int|long|register|return|short|signed|sizeof|static|struct|switch|typedef|union|unsigned|void|volatile|while)\\b" // KEYWORD
        "|^[[:blank:]]*#[[:blank:]]*[A-Za-z_][A-Za-z0-9_]*[[:blank:]]*<[^>]*>?" // PREPROCESSOR
        "|[a-zA-Z_][a-zA-Z0-9_]*" // IDENTIFIER
        "|(((\\+-)*\\+|(-\\+)*-?)(0|[1-9][0-9]*)(\\.([0-9]*))?((e|E)(\\+|-)?[0-9]+)?)" // CONSTANT
        "|//.*|/\\*.*?\\*/" // COMMENT
        "|[\\+\\-\\*/%&|^<>!]?=|[<>]=?|[\\+\\-\\*/%&|^<>!]" // OPERATOR
        "|[\\[\\]\\(\\)\\{\\}\\.,;]" // PUNCTUATOR
        "|\\s+" // WHITESPACE

    );

    std::string line;
    while (std::getline(file, line)) {
        std::vector<std::string> matches;
        std::sregex_iterator it(line.begin(), line.end(), combinedPattern);
        std::sregex_iterator reg_end;
        for (; it != reg_end; ++it) {
            matches.push_back((*it).str()); // Store each match in the vector
        }

        // Iterate over matches and determine their types
        for (const auto& match : matches) {
            bool typeFound = false;
            for (const auto& pair : tokenPatterns) {
                if (std::regex_match(match, pair.second)) {
                    if (pair.first == WHITESPACE || pair.first == COMMENT) {
                        typeFound = true;
						continue;
					}
                    std::cout << "Token: " << match << ", Type: " << tokenTypeToString[pair.first] << std::endl;
                    typeFound = true;
                    break;
                }
            }
            if (!typeFound) {
                std::cout << "Unknown token: " << match << std::endl;
            }
        }
    }
}

int main()
{
    lexicalAnalyzer("test.txt");

    cout << "Hello World" << endl;
    /*Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    CompilerForC::MainForm form;
    Application::Run(% form);*/
    return 0;
}
