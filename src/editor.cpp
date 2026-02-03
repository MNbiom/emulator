#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include <string>

#include "editor.h"
#include "assembler.h"

std::string editorContent;

void do_editor(){
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1, ImGui::GetStyle().FramePadding.y));
    ImGui::SetNextWindowSize(ImVec2(500, 900-20-1), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(1600-500-1, 20), ImGuiCond_FirstUseEver);

    ImGui::Begin("Code editor");
    if (ImGui::Button("Run")) init_rom();
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.12f, 0.12f, 0.12f, 1.0f));
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
    ImGui::InputTextMultiline(
        "##TextArea",
        &editorContent,
        ImVec2(-FLT_MIN, -FLT_MIN),
        ImGuiInputTextFlags_AllowTabInput
    );
    ImGui::PopFont();
    ImGui::PopStyleColor();


    ImGui::End();
    ImGui::PopStyleVar();
}