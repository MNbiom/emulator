#include "display.h"

#include "raylib.h"
#include "imgui.h"

#include "imgui_helpers.h"

#include <cstdint>

bool showDisplayWindow = true;
bool resetDisplayPos = true;
bool resetDisplaySize = false;
const uint16_t DISPLAY_MAX_SIZE = 256;
int screenSizePower = 6;

bool showDisplaySettings = false;
uint8_t OutX = 5;
uint8_t OutY = 6;
uint8_t OutColor = 7;
uint8_t OutControl = 4;

uint32_t vram[DISPLAY_MAX_SIZE * DISPLAY_MAX_SIZE];
static Texture2D displayTexture;

void init_display(){
    Image img = {};
    img.data = vram;
    img.width = DISPLAY_MAX_SIZE;
    img.height = DISPLAY_MAX_SIZE;
    img.mipmaps = 1;
    img.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
    
    displayTexture = LoadTextureFromImage(img);
}

void clear_display(uint32_t color){
    for (int i = 0; i < DISPLAY_MAX_SIZE * DISPLAY_MAX_SIZE; i++)
    vram[i] = color;
}

void update_display(){
    UpdateTexture(displayTexture, vram);
}

void shutdown_display(){
    UnloadTexture(displayTexture);
}

void draw_pixel(uint8_t x, uint8_t y, uint32_t color){
    if (x >= DISPLAY_MAX_SIZE || y >= DISPLAY_MAX_SIZE )
    return;
    
    vram[y * DISPLAY_MAX_SIZE + x] = color;
}

void do_display_settings(){
    ImGui::SetNextWindowSize(ImVec2(0, 0));
    ImGui::Begin(
        "Display settings",
        &showDisplaySettings
    );

    const char* outputs[] = {"0", "1", "2", "3", "4", "5", "6", "7"};
    if (ImGui::BeginCombo("Output X", outputs[OutX], ImGuiComboFlags_WidthFitPreview)){
        for (int i = 0; i < IM_ARRAYSIZE(outputs); i++){
            bool selected = (OutX == i);
            if (ImGui::Selectable(outputs[i], selected))
                OutX = i;
            if (selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    if (ImGui::BeginCombo("Output Y", outputs[OutY], ImGuiComboFlags_WidthFitPreview)){
        for (int i = 0; i < IM_ARRAYSIZE(outputs); i++){
            bool selected = (OutY == i);
            if (ImGui::Selectable(outputs[i], selected))
            OutY = i;
            if (selected)
            ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    ImGui::SameLine();
    HelpMarker("Pixel will draw when you update Y");

    if (ImGui::BeginCombo("Output Color", outputs[OutColor], ImGuiComboFlags_WidthFitPreview)){
        for (int i = 0; i < IM_ARRAYSIZE(outputs); i++){
            bool selected = (OutColor == i);
            if (ImGui::Selectable(outputs[i], selected))
            OutColor = i;
            if (selected)
            ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    ImGui::SameLine();
    HelpMarker("You pick one of 16 colors in the palette");

    if (ImGui::BeginCombo("Output Control", outputs[OutControl], ImGuiComboFlags_WidthFitPreview)){
        for (int i = 0; i < IM_ARRAYSIZE(outputs); i++){
            bool selected = (OutControl == i);
            if (ImGui::Selectable(outputs[i], selected))
            OutControl = i;
            if (selected)
            ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    ImGui::SameLine();
    HelpMarker("bit 7 - nothing\nbit 6 - nothing\nbit 5 - nothing\nbit 4 - nothing\nbit 3 - nothing\nbit 2 - nothing\nbit 1 - clear screen\nbit 0 - buffer");

    ImGui::End();
}

void do_display(){
    static float display_window_size = (2*DISPLAY_MAX_SIZE)+2*ImGui::GetStyle().WindowBorderSize;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1, ImGui::GetStyle().FramePadding.y));
    if (resetDisplaySize){
        display_window_size = (2*DISPLAY_MAX_SIZE)+2*ImGui::GetStyle().WindowBorderSize;
        resetDisplaySize = false;
    }
    ImGui::SetNextWindowSize(ImVec2(display_window_size, 48+display_window_size-2*ImGui::GetStyle().WindowBorderSize));

    if (resetDisplayPos){
        ImGui::SetNextWindowPos(ImVec2(1,20));
        resetDisplayPos = false;
    }
    
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.12f, 0.12f, 0.12f, 1.0f));
    ImGui::Begin(
        "Display",
        &showDisplayWindow,
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoScrollWithMouse
    );
    
    ImGui::AlignTextToFramePadding();
    ImGui::Indent(ImGui::GetStyle().FramePadding.y);
    ImGui::Text("Change screen size:");
    ImGui::Unindent(ImGui::GetStyle().FramePadding.y);
    ImGui::SameLine();
    float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
    ImGui::PushItemFlag(ImGuiItemFlags_ButtonRepeat, true);
    if (ImGui::ArrowButton("##left", ImGuiDir_Left) && screenSizePower>4) { screenSizePower--; }
    ImGui::SameLine(0.0f, spacing);
    if (ImGui::ArrowButton("##right", ImGuiDir_Right) && screenSizePower<8) { screenSizePower++; }
    ImGui::SameLine();
    ImGui::PopItemFlag();
    ImGui::Text("%ix%i px", 1<<screenSizePower, 1<<screenSizePower);

    ImGui::Image(
        (ImTextureID)(uintptr_t)displayTexture.id,
        ImVec2{float((display_window_size-2*ImGui::GetStyle().WindowBorderSize)*((2*DISPLAY_MAX_SIZE)/(2<<screenSizePower))), float(display_window_size*((2*DISPLAY_MAX_SIZE)/(2<<screenSizePower)))}
    );
    
    display_window_size = ImGui::GetWindowWidth();
    
    ImGui::End();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();
}