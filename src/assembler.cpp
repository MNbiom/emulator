#include "assembler.h"

#include <cstdint>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>

#include "isa.h"
#include "editor.h"

#include <iostream>

uint32_t rom[1<<ROM_ADDRESS_SIZE];
int getLine[1<<ROM_ADDRESS_SIZE];

void init_rom(){
    reset_cpu();
    isPaused = true;
    std::string editorContent = editor.GetText();

    std::fill(std::begin(rom), std::end(rom), 0);
    std::fill(std::begin(getLine), std::end(getLine), 0);
    uint16_t pc_ = 0;
    uint16_t page_ = 0;
    uint32_t lineNr = 0;

    std::unordered_map<std::string, uint16_t> labels;
    std::istringstream content(editorContent);
    std::string line;
    while(std::getline(content, line)){ //labels
        if (line.empty()) continue; //skip rest if empty
        auto pos = line.find(";");
        if (pos != std::string::npos) line = line.substr(0, pos); //delete comments

        std::istringstream lineContent(line);
        std::vector<std::string> tokens;
        std::string token;
        while (lineContent >> token) tokens.push_back(token); //convert line to tokens
        if (tokens.empty()) continue; //skip rest if empty after deleting comments
        if (tokens[tokens.size()-1].back() == ':'){
            std::string label = tokens.back().substr(0, tokens.back().size() - 1);
            labels[label] = pc_;
            labels[label + "_page"] = page_;
        }
        else if (tokens[0] == "]next_page]"){
            page_++;
            pc_ = 0;
        }
        else pc_++;
    }

    content.clear();
    content.seekg(0);
    pc_ = 0;
    page_ = 0;
    while(std::getline(content, line)){ //rest
        lineNr++;
        bool page_swap = 0;
        if (line.empty()) continue; //skip rest if empty
        auto pos = line.find(";");
        if (pos != std::string::npos) line = line.substr(0, pos); //delete comments
        
        std::istringstream lineContent(line);
        std::vector<std::string> tokens;
        std::string token;
        while (lineContent >> token) if (token.back() != ':')tokens.push_back(token); //convert line to tokens
        if (tokens.empty()) continue; //skip rest if empty after deleting comments
        std::cout << line << ": ";

        for (const std::string& token : tokens){
            auto tmp = codes.find(token);
            if (tmp != codes.end()){ //mnemonics
                rom[(page_ << 6) | pc_] |= tmp->second;
                continue;
            }
            auto tmp2 = labels.find(token);
            if (tmp2 != labels.end()){ //labels
                rom[(page_ << 6) | pc_] |= tmp2->second;
                continue;
            }
            if (token == "]next_page]"){
                page_++;
                pc_ = 0;
                page_swap = 1;
                break;
            }
            try { //try to convert a number
                if (token.size() > 2 && token[0] == '0'){
                    if (token[1] == 'b') rom[(page_ << 6) | pc_] |= std::stoul(token.substr(2), nullptr, 2); //0b
                    else if (token[1] == 'x') rom[(page_ << 6) | pc_] |= std::stoul(token.substr(2), nullptr, 16); //0x
                }
                else rom[(page_ << 6) | pc_] |= std::stoul(token, nullptr, 10); //decimal
            }
            catch (...){
                std::cout << "Invalid syntax at line - " << lineNr << " - " << line << "\n";
            }
        }
        if (!page_swap){
            std::cout << "pc = " << pc_ << ", page = " << page_ << ", " << rom[(page_ << 6) | pc_] << "\n";
            getLine[(page_ << 6) | pc_] = lineNr - 1;
            pc_++;
        }
    }
}