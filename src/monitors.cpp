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
bool resetRegsSize = true;

bool showFlagsWindow = true;
bool resetFlagsPos = true;
bool resetFlagsSize = true;

bool showRamWindow = true;
bool resetRamPos = true;
bool resetRamSize = true;

bool showDStackWindow = true;
bool resetDStackPos = true;
bool resetDStackSize = true;

bool showCStackWindow = true;
bool resetCStackPos = true;
bool resetCStackSize = true;

bool showOutWindow = true;
bool resetOutPos = true;
bool resetOutSize = true;

void get_change(){
    bool change = false;
    static uint32_t oldReg[REG_AMOUNT] = {0};
    static uint32_t oldAcc = 0;
    static uint32_t oldOut[IO_AMOUNT] = {0};
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
    for (int i = 0; i < IO_AMOUNT; i++){
        if (oldOut[i] != out[i]){
            changedValue = i+(REG_AMOUNT+1);
            oldOut[i] = out[i];
            change = true;
        }
    }
    if (!change) changedValue = INT_MAX;
}

void do_ips(){
    if (resetIpsPos){
        ImGui::SetNextWindowPos(ImVec2(1033, 20));
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
        ImGui::SetNextWindowPos(ImVec2(1 + (2*DISPLAY_MAX_SIZE)+2*ImGui::GetStyle().WindowBorderSize + 1,20));
        resetRegsPos = false;
    }
    if (resetRegsSize){
        ImGui::SetNextWindowSize(ImVec2(0, 0));
        resetRegsSize = false;
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

void do_flags(){
    if (resetFlagsPos){
        ImGui::SetNextWindowPos(ImVec2(1 + (2*DISPLAY_MAX_SIZE)+2*ImGui::GetStyle().WindowBorderSize + 1 + 92, 37));
        resetFlagsPos = false;
    }
    if (resetFlagsSize){
        ImGui::SetNextWindowSize(ImVec2(0, 0));
        resetFlagsSize = false;
    }
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1, 0));
    ImGui::Begin("Flags", &showFlagsWindow);

    if (ImGui::BeginTable("Flags table", 2, ImGuiTableFlags_Borders)){
        ImGui::TableSetupColumn("Flag");
        ImGui::TableSetupColumn("Value");
        ImGui::TableHeadersRow();

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("TRUE");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%i", flag[0]);

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("NEQ, NZERO");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%i", flag[1]);

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("EQ, ZERO");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%i", flag[2]);
        
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("LT, NCARRY");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%i", flag[3]);

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("LEQ");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%i", flag[4]);

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("GT");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%i", flag[5]);

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("GEQ, CARRY");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%i", flag[6]);

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("MSB/LSB");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%i", flag[7]);

        ImGui::EndTable();
    }
    ImGui::End();
    ImGui::PopStyleVar();
}

void do_ram(){
    if (resetRamPos){
        ImGui::SetNextWindowPos(ImVec2(1, 581));
        resetRamPos = false;
    }
    if (resetRamSize){
        ImGui::SetNextWindowSize(ImVec2(0, 308));
        resetRamSize = false;
    }
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1, 0));
    ImGui::Begin("Ram", &showRamWindow);
    if (ImGui::BeginTable("Ram table", 17, ImGuiTableFlags_Borders)){
        ImGui::TableSetupColumn(std::to_string(poi).c_str());
        for (int i = 0; i < 16; i++) ImGui::TableSetupColumn(std::to_string(i).c_str());
        ImGui::TableHeadersRow();
        for (int i = 0; i < (1 << RAM_ADDRESS_SIZE); i += 16){
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, 0xff333030);
            ImGui::Text("%i", i);
            for (int j = 0; j < 16; j++){
                ImGui::TableSetColumnIndex(j+1);
                if (i+j == poi) ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, 0x40ffffff);
                ImGui::Text("%i", ram[i+j]);
            }
        }
        ImGui::EndTable();
    }
    ImGui::End();
    ImGui::PopStyleVar();
}

void do_dstack(){
    if (resetDStackPos){
        ImGui::SetNextWindowPos(ImVec2(516, 210));
        resetDStackPos = false;
    }
    if (resetDStackSize){
        ImGui::SetNextWindowSize(ImVec2(0, 0));
        resetDStackSize = false;
    }
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1, 0));
    ImGui::Begin("dStack", &showDStackWindow);
    if (ImGui::BeginTable("dStack table", 9, ImGuiTableFlags_Borders)){
        ImGui::TableSetupColumn(std::to_string(dSp).c_str());
        for (int i = 0; i < 8; i++) ImGui::TableSetupColumn(std::to_string(i).c_str());
        ImGui::TableHeadersRow();
        for (int i = 0; i < (1 << SP_ADDRESS_SIZE); i += 8){
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, 0xff333030);
            ImGui::Text("%i", i);
            for (int j = 0; j < 8; j++){
                ImGui::TableSetColumnIndex(j+1);
                if (i+j == dSp) ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, 0x40ffffff);
                ImGui::Text("%i", dStack[i+j]);
            }
        }
        ImGui::EndTable();
    }
    ImGui::End();
    ImGui::PopStyleVar();
}

void do_cstack(){
    if (resetCStackPos){
        ImGui::SetNextWindowPos(ImVec2(664, 210));
        resetCStackPos = false;
    }
    if (resetCStackSize){
        ImGui::SetNextWindowSize(ImVec2(0, 0));
        resetCStackSize = false;
    }
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1, 0));
    ImGui::Begin("cStack", &showCStackWindow);
    if (ImGui::BeginTable("cStack table", 9, ImGuiTableFlags_Borders)){
        ImGui::TableSetupColumn(std::to_string(cSp).c_str());
        for (int i = 0; i < 8; i++) ImGui::TableSetupColumn(std::to_string(i).c_str());
        ImGui::TableHeadersRow();
        for (int i = 0; i < (1 << SP_ADDRESS_SIZE); i += 8){
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, 0xff333030);
            ImGui::Text("%i", i);
            for (int j = 0; j < 8; j++){
                ImGui::TableSetColumnIndex(j+1);
                if (i+j == cSp) ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, 0x40ffffff);
                ImGui::Text("%i", cStack[i+j]);
            }
        }
        ImGui::EndTable();
    }
    ImGui::End();
    ImGui::PopStyleVar();
}

void do_out(){
    if (resetOutPos){
        ImGui::SetNextWindowPos(ImVec2(770, 37));
        resetOutPos = false;
    }
    if (resetOutSize){
        ImGui::SetNextWindowSize(ImVec2(0, 0));
        resetOutSize = false;
    }
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1, 0));
    ImGui::Begin("Out", &showOutWindow);

    if (ImGui::BeginTable("Out table", 2, ImGuiTableFlags_Borders)){
        ImGui::TableSetupColumn("Out");
        ImGui::TableSetupColumn("Value");
        ImGui::TableHeadersRow();

        for (int i = 0; i < IO_AMOUNT; i++){
            ImGui::TableNextRow();
            if (changedValue == i+(REG_AMOUNT+1)) ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, 0x40ffffff);
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("Out %i", i);
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%i", out[i]);
        }
        ImGui::EndTable();
    }
    ImGui::End();
    ImGui::PopStyleVar();
}


//ImVec2 posTest = ImGui::GetWindowPos();