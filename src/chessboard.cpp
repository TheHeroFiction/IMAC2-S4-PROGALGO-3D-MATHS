#include "chessboard.hpp"
#include <imgui.h>
#include <iostream>
#include <map>
#include <string>

std::map<std::string, ImVec2> casePositions;

void draw_board(float caseSize)
{
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0)); // Pour supprimer le paddding des cases

    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            ImGui::PushID(row * 8 + col);

            if ((row + col) % 2 == 0)
            {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.f, 1.f, 1.f, 1.f));
            }
            else
            {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.2f, 0.2f, 1.f));
            }

            if (ImGui::Button(" ", ImVec2(caseSize, caseSize)))
            {
                // Position
                char file = static_cast<char>('a' + col);

                int rank = 8 - row;

                std::string caseName = std::string(1, file) + std::to_string(rank);
            
                casePositions[caseName] = ImGui::GetCursorPos();

                std::cout << "Case cliquée : " << caseName << "\n";
            }

            ImGui::PopStyleColor();
            ImGui::PopID();

            if (col < 7)
            {
                ImGui::SameLine();
            }
        }
    }

    ImGui::PopStyleVar();
}
