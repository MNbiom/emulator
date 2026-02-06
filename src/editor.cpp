#include "imgui.h"
#include "TextEditor.h"
#include <string>

#include "editor.h"
#include "assembler.h"
#include "isa.h"

TextEditor editor;

bool showEditorWindow = true;
bool resetEditorPos = true;
bool resetEditorSize = true;

bool isPaused = true;
int rtps = 10;
bool rtpsLimit = true;
bool stepInstruction = false;
bool highlightCurrentLine = true;
bool followHighlights = false;

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

void highlight_line(){
    if (followHighlights) editor.SetCursorPosition(TextEditor::Coordinates{getLine[pc], 0});
    editor.SetSelection(
        TextEditor::Coordinates{getLine[pc], 0},
        TextEditor::Coordinates{getLine[pc], INT_MAX}
    );
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
        palette[(int)TextEditor::PaletteIndex::ErrorMarker] = 0x800000ff;
        editor.SetPalette(palette);

        init = false;
    }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1, ImGui::GetStyle().FramePadding.y));
    if (resetEditorSize){
        ImGui::SetNextWindowSize(ImVec2(500, 900-20-1));
        resetEditorSize = false;
    }
    if (resetEditorPos){
        ImGui::SetNextWindowPos(ImVec2(1600-500-1, 20));
        resetEditorPos = false;
    }

    //compile button
    ImGui::Begin("Code editor", &showEditorWindow);
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
    if (ImGui::IsKeyPressed(ImGuiKey_F10)) stepInstruction = true;
    //rtps
    ImGui::SameLine();
    ImGui::PushItemWidth(75);
    ImGui::DragInt("##rtps", &rtps, 1, 1, INT_MAX, nullptr, ImGuiSliderFlags_ClampOnInput);
    ImGui::SetItemTooltip("rtps");
    ImGui::PopItemWidth();
    //unlimited rtps
    ImGui::SameLine();
    ImGui::Checkbox("rtps limit", &rtpsLimit);
    ImGui::SameLine();
    ImGui::Checkbox("##Highlight current line(toggle)", &highlightCurrentLine);
    ImGui::SetItemTooltip("Highlight current line(toggle)");
    ImGui::SameLine();
    ImGui::Checkbox("##Follow highlighted line", &followHighlights);
    ImGui::SetItemTooltip("Follow highlighted line");
    ImGui::SameLine();
    if (ImGui::Button("##Highlight current line", ImVec2(ImGui::GetItemRectSize().y,0))) highlight_line();
    ImGui::SetItemTooltip("Highlight current line");

    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
    editor.Render("CodeEditor");
    ImGui::PopFont();

    ImGui::End();
    ImGui::PopStyleVar();
}