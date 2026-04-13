#pragma once
#include <imgui.h>
#include <string>
#include <vector>


struct GameLogger {
    std::vector<std::string> logs;

    void AddLog(const std::string& message)
    {
        logs.push_back(message);
    }

    // --- TEXT ZONE ---
    void Draw(float height = 150.f)
    {
        ImGui::Separator();
        ImGui::Text("Play history :");

        // --- SCROLLABLE REGION ---
        ImGui::BeginChild("LogScroll", ImVec2(0, height), true);

        for (const auto& log : logs)
        {
            ImGui::TextUnformatted(log.c_str());
        }

        // --- AUTO-SCROLL DOWNWARD WHEN ADDING NEW MESSAGES ---
        if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        {
            ImGui::SetScrollHereY(1.0f);
        }

        ImGui::EndChild();
    }

    void Clear()
    {
        logs.clear();
    }
};