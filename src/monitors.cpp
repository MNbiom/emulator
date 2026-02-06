#include "monitors.h"

#include "imgui.h"

#include "display.h"
#include "isa.h"

int ips = 0;
int changedValue = INT_MAX;

bool showIpsWindow = true;
bool resetIpsPos = true;

bool showRegsWindow = true;
bool resetRegsPos = true;

void get_change(){
    bool change = false;
    static uint32_t oldReg[REG_AMOUNT] = {0};
    static uint32_t oldAcc = 0;
    for (int i = 0; i < REG_AMOUNT; i++){
        if (oldReg[i] != reg[i]){
            changedValue = i;
            oldReg[i] = reg[i];
            change = true;
        }
    }
    if (oldAcc != acc){
        changedValue = REG_AMOUNT;
        oldAcc = acc;
        change = true;
    }
    if (!change) changedValue = INT_MAX;
}

void do_ips(){
    if (resetIpsPos){
        ImGui::SetNextWindowPos(ImVec2(1 + (2*DISPLAY_MAX_SIZE)+2*ImGui::GetStyle().WindowBorderSize + 1,20));
        resetIpsPos = false;
    }

    ImGui::SetNextWindowBgAlpha(0.35f);
    ImGui::Begin("IPS", &showIpsWindow, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);
    ImGui::Text("IPS = %i", ips);
    if (ImGui::BeginPopupContextWindow()){
        if (ImGui::MenuItem("Close")) showIpsWindow = false;
        ImGui::EndPopup();
    }
    ImGui::End();
}

void do_regs(){
    if (resetRegsPos){
        //ImGui::SetNextWindowPos(ImVec2(1 + (2*DISPLAY_MAX_SIZE)+2*ImGui::GetStyle().WindowBorderSize + 1,20));
        resetRegsPos = false;
    }
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1, 0));
    ImGui::Begin("Registers", &showRegsWindow);

    if (ImGui::BeginTable("Regs table", 2, ImGuiTableFlags_Borders)){
        ImGui::TableSetupColumn("Reg");
        ImGui::TableSetupColumn("Value");
        ImGui::TableHeadersRow();

        ImGui::TableNextRow();
        if (changedValue == REG_AMOUNT) ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, 0x40ffffff);
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("acc");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%i", acc);
        for (int i = 0; i < REG_AMOUNT; i++){
            ImGui::TableNextRow();
            if (changedValue == i) ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, 0x40ffffff);
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("R%i", i);
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%i", reg[i]);
        }
        ImGui::EndTable();
    }

    ImGui::End();
    ImGui::PopStyleVar();
}