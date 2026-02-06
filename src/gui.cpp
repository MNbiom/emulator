#include "gui.h"

#include "imgui.h"
#include "ImGuiFileDialog.h"

#include "display.h"
#include "editor.h"
#include "monitors.h"

#include <iostream>
#include <fstream>

bool showFileDialog = false;

void instert_file_to_editor(std::string path){
    std::ifstream file(path);
    if (!file.is_open()){
        std::cout << "Failed to open file: " << path << "\n";
        return;
    }
    std::string editorContent;
    std::string line;
    editorContent.clear();
    while (std::getline(file, line)){
    editorContent += line;
    editorContent.push_back('\n');
    }
    editor.SetText(editorContent);
}

void do_file_dialog() { 
    // open Dialog Simple
    if (showFileDialog){
        IGFD::FileDialogConfig config;
        config.path = ".";
        ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".*,.asm,.txt", config);
        showFileDialog = false;
    }
    // display
    ImGui::SetNextWindowSize(ImVec2(800, 450), ImGuiCond_FirstUseEver);
    if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")){
        if (ImGuiFileDialog::Instance()->IsOk()) { // action if OK
            std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
            // action
            std::cout << filePathName << std::endl;
            instert_file_to_editor(filePathName);
        }
        
        // close
        ImGuiFileDialog::Instance()->Close();
    }
}

void do_menu(){
    if (ImGui::BeginMainMenuBar()){
        if (ImGui::BeginMenu("File")){
            if (ImGui::MenuItem("Open...")){
                showFileDialog = true;
            }
        ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View")){
            if (ImGui::BeginMenu("Visibility")){
                bool showAllState = (showIpsWindow && showDisplayWindow && showEditorWindow);
                if (ImGui::MenuItem("All", nullptr, &showAllState)){
                    showIpsWindow = showAllState;
                    showDisplayWindow = showAllState;
                    showEditorWindow = showAllState;
                }
                ImGui::MenuItem("IPS counter", nullptr, &showIpsWindow);
                ImGui::MenuItem("Display", nullptr, &showDisplayWindow);
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Reset position...")){
                if (ImGui::MenuItem("All")){
                    resetDisplayPos = true;
                }
                if (ImGui::MenuItem("Display")) resetDisplayPos = true;
                    if (ImGui::MenuItem("IPS")) resetIpsPos = true;
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Reset size...")){
                if (ImGui::MenuItem("All")){
                    resetDisplaySize = true;
                }
                if (ImGui::MenuItem("Display")) resetDisplaySize = true;
                ImGui::EndMenu();
            }
            if (ImGui::MenuItem("Reset everything")){
                    resetDisplayPos = true;
                    resetDisplaySize = true;
            }

            ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void gui(){
    do_menu();
    do_file_dialog();
    if (showDisplayWindow) do_display();
    if (showEditorWindow) do_editor();
    if (showIpsWindow) do_ips();
    do_regs();
}