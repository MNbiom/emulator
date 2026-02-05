#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include "TextEditor.h"
#include <string>

#include "editor.h"
#include "assembler.h"

TextEditor editor;

TextEditor::LanguageDefinition DeafultAssemblerLang() {
    TextEditor::LanguageDefinition langDef;
    langDef.mCaseSensitive = true;

    langDef.mName = "DeafultASM";

    langDef.mSingleLineComment = ";";
    langDef.mCommentStart = ";";
    langDef.mCommentEnd = "\n";

    langDef.mTokenRegexStrings.push_back(std::make_pair<std::string, TextEditor::PaletteIndex>("0[bB][0-1]+", TextEditor::PaletteIndex::Number)); //bin
    langDef.mTokenRegexStrings.push_back(std::make_pair<std::string, TextEditor::PaletteIndex>("0[xX][0-9a-fA-F]+", TextEditor::PaletteIndex::Number)); //hex
    langDef.mTokenRegexStrings.push_back(std::make_pair<std::string, TextEditor::PaletteIndex>("[0-9]+", TextEditor::PaletteIndex::Number)); //dec
    langDef.mTokenRegexStrings.push_back(std::make_pair<std::string, TextEditor::PaletteIndex>("[a-zA-Z_][a-zA-Z0-9_]*", TextEditor::PaletteIndex::Identifier));

    static const char* const keywords[] = {
        "ADD",
        "SUB"
    };
    
    for (auto& k : keywords)
        langDef.mKeywords.insert(k);
    
    return langDef;
}

void do_editor(){
    static bool init = true;
    if (init){
        editor.SetLanguageDefinition(DeafultAssemblerLang());

        TextEditor::Palette palette = editor.GetPalette();
        palette[(int)TextEditor::PaletteIndex::Background] = 0xff1F1F1F;
        palette[(int)TextEditor::PaletteIndex::Identifier] = 0xffffffff;
        palette[(int)TextEditor::PaletteIndex::Number] = 0xffA8CEB5;
        palette[(int)TextEditor::PaletteIndex::Comment] = 0xff55996A;
        palette[(int)TextEditor::PaletteIndex::Keyword] = 0xffFEDC9C;
        editor.SetPalette(palette);

        init = false;
    }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1, ImGui::GetStyle().FramePadding.y));
    ImGui::SetNextWindowSize(ImVec2(500, 900-20-1), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(1600-500-1, 20), ImGuiCond_FirstUseEver);

    ImGui::Begin("Code editor");
    if (ImGui::Button("Run")) init_rom();

    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
    editor.Render("CodeEditor");
    ImGui::PopFont();

    ImGui::End();
    ImGui::PopStyleVar();
}