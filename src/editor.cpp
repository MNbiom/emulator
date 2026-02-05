#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include "TextEditor.h"
#include <string>

#include "editor.h"
#include "assembler.h"

TextEditor editor;

bool isPaused = true;
int rtps = 10;
bool rtpsLimit = true;
bool stepInstruction = false;

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

    //compile button
    ImGui::Begin("Code editor");
    ImGui::Indent(ImGui::GetStyle().FramePadding.y);
    if (ImGui::Button("Compile")) init_rom();
    ImGui::Unindent(ImGui::GetStyle().FramePadding.y);
    //play/pause
    ImGui::SameLine(0, 25);
    if (ImGui::Button(isPaused ? "▶" : "⏸", ImVec2(ImGui::GetItemRectSize().y,0))) isPaused = !isPaused;
    //step
    ImGui::SameLine();
    if (ImGui::Button("Step")) stepInstruction = true;
    ImGui::SetItemTooltip("F10");
    //rtps
    ImGui::SameLine();
    ImGui::PushItemWidth(100);
    ImGui::DragInt("##", &rtps, 1, 1, INT_MAX, nullptr, ImGuiSliderFlags_ClampOnInput);
    ImGui::SetItemTooltip("rtps");
    ImGui::PopItemWidth();
    //unlimited rtps
    ImGui::SameLine();
    ImGui::Checkbox("rtps limit", &rtpsLimit);


    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
    editor.Render("CodeEditor");
    ImGui::PopFont();

    ImGui::End();
    ImGui::PopStyleVar();
}