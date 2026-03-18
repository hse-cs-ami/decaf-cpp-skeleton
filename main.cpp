#include <fstream>
#include <iostream>
#include <string>

#include <antlr4-runtime.h>
#include "DecafScanner.h"
#include "DecafParser.h"

// Считает количество ошибок, подавляет вывод по умолчанию.
class ErrorListener : public antlr4::BaseErrorListener {
public:
    int errors = 0;

    void syntaxError(antlr4::Recognizer *,
                     antlr4::Token *,
                     size_t line,
                     size_t col,
                     const std::string &msg,
                     std::exception_ptr) override
    {
        std::cerr << "line " << line << ":" << col << ": " << msg << "\n";
        ++errors;
    }
};

static void usage(const char *prog) {
    std::cerr << "Usage: " << prog << " -t scan|parse <file>\n";
}

int main(int argc, char *argv[]) {
    // Разбор аргументов командной строки.
    if (argc != 4 || std::string(argv[1]) != "-t") {
        usage(argv[0]);
        return 1;
    }

    std::string stage = argv[2];
    std::string filename = argv[3];

    if (stage != "scan" && stage != "parse") {
        usage(argv[0]);
        return 1;
    }

    std::ifstream stream(filename);
    if (!stream) {
        std::cerr << "Cannot open file: " << filename << "\n";
        return 1;
    }

    antlr4::ANTLRInputStream input(stream);
    DecafScanner lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);

    ErrorListener lexerErrors;
    lexer.removeErrorListeners();
    lexer.addErrorListener(&lexerErrors);

    if (stage == "scan") {
        tokens.fill();
        for (auto *token : tokens.getTokens()) {
            if (token->getType() == antlr4::Token::EOF) break;
            size_t line = token->getLine();
            std::string type;
            switch (token->getType()) {
                case DecafScanner::IDENTIFIER:    type = " IDENTIFIER";    break;
                case DecafScanner::INTLITERAL:    type = " INTLITERAL";    break;
                case DecafScanner::CHARLITERAL:   type = " CHARLITERAL";   break;
                case DecafScanner::STRINGLITERAL: type = " STRINGLITERAL"; break;
                default: break;
            }
            std::cout << line << type << " " << token->getText() << "\n";
        }
        return lexerErrors.errors > 0 ? 1 : 0;
    }

    // stage == "parse"
    DecafParser parser(&tokens);
    ErrorListener parserErrors;
    parser.removeErrorListeners();
    parser.addErrorListener(&parserErrors);

    parser.program();

    int total = lexerErrors.errors + parserErrors.errors;
    return total > 0 ? 1 : 0;
}
