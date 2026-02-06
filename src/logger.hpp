#pragma once
#include <vector>
#include <string>
#include <imgui.h>

struct GameLogger {
    std::vector<std::string> logs;

    // Ajoute un message à l'historique
    void AddLog(const std::string& message) 
    {
        logs.push_back(message);
    }

    // Affiche la zone de logs (fenêtre défilante)
    void Draw(float height = 150.f) 
    {
        ImGui::Separator();
        ImGui::Text("Historique de la partie :");
        
        // Sous-région scrollable
        ImGui::BeginChild("LogScroll", ImVec2(0, height), true);

        for (const auto& log : logs) {
            ImGui::TextUnformatted(log.c_str());
        }

        // Auto-scroll vers le bas si on ajoute un nouveau message
        if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        {
            ImGui::SetScrollHereY(1.0f);
        }

        ImGui::EndChild();
    }
    
    // Pour vider les logs si besoin (nouvelle partie)
    void Clear() 
    {
        logs.clear();
    }
};