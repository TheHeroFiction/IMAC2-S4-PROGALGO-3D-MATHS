#include "wonderland_stats.hpp"
#include <imgui.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <numeric>

namespace {

template<typename T>
float mean_of(const std::vector<T>& values)
{
    float sum = std::accumulate(values.begin(), values.end(), 0.0f);
    return sum / static_cast<float>(values.size());
}

template<size_t N>
float max_of(const std::array<float, N>& values)
{
    return *std::max_element(values.begin(), values.end());
}

template<size_t N>
void plot_hist(const std::array<float, N>& freqs, const ImVec2 size)
{
    ImGui::PlotHistogram("", freqs.data(), static_cast<int>(N), 0, nullptr, 0.0f, max_of(freqs), size);
}

void draw_geometric_cell(const GameState& game_state)
{
    ImVec4 geo_color = ImVec4(0.9f, 0.2f, 0.3f, 1.0f);
    ImGui::TextColored(geo_color, "Red Queen (Geometric Law)");

    if (game_state.geometric_history.empty())
    {
        ImGui::TextDisabled("Waiting for data...");
        return;
    }

    float mean = mean_of(game_state.geometric_history);
    ImGui::Text("N: %zu | Theory: 1.50 | Real: %.2f", game_state.geometric_history.size(), mean);
    ImGui::Spacing();

    constexpr size_t        bins = 6;
    std::array<float, bins> freqs{};
    for (int v : game_state.geometric_history)
    {
        int index = std::clamp(v, 0, static_cast<int>(bins) - 1);
        freqs[static_cast<size_t>(index)] += 1.0f;
    }

    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, geo_color);
    plot_hist(freqs, ImVec2(300, 50));
    ImGui::PopStyleColor();
}

void draw_cauchy_cell(const GameState& game_state)
{
    ImVec4 cauchy_color = ImVec4(0.2f, 0.8f, 0.9f, 1.0f);
    ImGui::TextColored(cauchy_color, "Looking Glass (Cauchy Law)");

    if (game_state.cauchy_history.empty())
    {
        ImGui::TextDisabled("Waiting for data...");
        return;
    }

    float mean = mean_of(game_state.cauchy_history);
    ImGui::Text("N: %zu | Theory: UNDEFINED | Real: %.2f", game_state.cauchy_history.size(), mean);
    ImGui::Spacing();

    ImGui::PushStyleColor(ImGuiCol_PlotLines, cauchy_color);
    ImGui::PlotLines(
        "",
        game_state.cauchy_history.data(),
        static_cast<int>(game_state.cauchy_history.size()),
        0,
        nullptr,
        -5.0f,
        5.0f,
        ImVec2(300, 50)
    );
    ImGui::PopStyleColor();
}

void draw_exponential_tab(const GameState& game_state, ImGuiTabItemFlags flags)
{
    if (!ImGui::BeginTabItem("Exponential", nullptr, flags))
    {
        return;
    }

    ImVec4 c = ImVec4(1.0f, 0.6f, 0.2f, 1.0f);
    ImGui::Spacing();
    ImGui::TextColored(c, "Endless Fall (Exponential Law)");

    if (!game_state.exp_history.empty())
    {
        float mean = mean_of(game_state.exp_history);
        ImGui::Text("N: %zu | Theory: 15.15s | Real: %.2fs", game_state.exp_history.size(), mean);
        ImGui::Spacing();

        ImGui::PushStyleColor(ImGuiCol_PlotLines, c);
        ImGui::PlotLines(
            "",
            game_state.exp_history.data(),
            static_cast<int>(game_state.exp_history.size()),
            0,
            nullptr,
            0.0f,
            60.0f,
            ImVec2(600, 100)
        );
        ImGui::PopStyleColor();
    }
    else
    {
        ImGui::TextDisabled("Waiting for data...");
    }

    ImGui::EndTabItem();
}

void draw_uniform_tab(const GameState& game_state, ImGuiTabItemFlags flags)
{
    if (!ImGui::BeginTabItem("Uniform (Engine)", nullptr, flags))
    {
        return;
    }

    ImVec4 c = ImVec4(0.9f, 0.9f, 0.2f, 1.0f);
    ImGui::Spacing();
    ImGui::TextColored(c, "Weather Engine (Uniform Law)");

    int total_rolls = std::accumulate(game_state.weather_distribution.begin(), game_state.weather_distribution.end(), 0);
    if (total_rolls > 0)
    {
        ImGui::Text("N: %d | Theory: 20%% each", total_rolls);
        ImGui::Spacing();

        constexpr size_t        bins = 5;
        std::array<float, bins> freqs{};
        for (size_t i = 0; i < bins; ++i)
        {
            freqs[i] = static_cast<float>(game_state.weather_distribution[i]);
        }

        constexpr std::array<const char*, bins> labels{"Beta", "Normal", "Binom", "Poisson", "Clear"};
        if (ImGui::BeginTable("UniformSummary", 5, ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_NoPadOuterX))
        {
            for (size_t i = 0; i < bins; ++i)
            {
                ImGui::TableSetupColumn(labels[i]);
            }
            ImGui::TableNextRow();

            for (size_t i = 0; i < bins; ++i)
            {
                float observed_ratio = freqs[i] / static_cast<float>(total_rolls);
                float observed_pct   = observed_ratio * 100.0f;
                float delta_pts      = (observed_ratio - 0.20f) * 100.0f;

                float  abs_delta   = std::abs(delta_pts);
                ImVec4 delta_color = ImVec4(1.0f, 0.3f, 0.3f, 1.0f);
                if (abs_delta < 2.0f)
                {
                    delta_color = ImVec4(0.2f, 0.9f, 0.4f, 1.0f);
                }
                else if (abs_delta < 5.0f)
                {
                    delta_color = ImVec4(1.0f, 0.7f, 0.2f, 1.0f);
                }

                ImGui::TableSetColumnIndex(static_cast<int>(i));
                float column_width = ImGui::GetContentRegionAvail().x;
                float cell_start_x = ImGui::GetCursorPosX();
                ImGui::SetCursorPosX(cell_start_x + (column_width - ImGui::CalcTextSize(labels[i]).x) * 0.5f);
                ImGui::TextUnformatted(labels[i]);
                ImGui::SetCursorPosX(cell_start_x + (column_width - ImGui::CalcTextSize("100.0%%").x) * 0.5f);
                ImGui::TextDisabled("%.1f%%", observed_pct);
                ImGui::SetCursorPosX(cell_start_x + (column_width - ImGui::CalcTextSize("+00.0 pts").x) * 0.5f);
                ImGui::TextColored(delta_color, "%+.1f pts", delta_pts);
            }

            ImGui::EndTable();
        }

        ImGui::PushStyleColor(ImGuiCol_PlotHistogram, c);
        plot_hist(freqs, ImVec2(600, 100));
        ImGui::PopStyleColor();
    }
    else
    {
        ImGui::TextDisabled("Waiting for turns to end...");
    }

    ImGui::EndTabItem();
}

void draw_poisson_tab(const GameState& game_state, ImGuiTabItemFlags flags)
{
    if (!ImGui::BeginTabItem("Poisson", nullptr, flags))
    {
        return;
    }

    ImVec4 c = ImVec4(0.59f, 0.20f, 0.78f, 1.0f);
    ImGui::Spacing();
    ImGui::TextColored(c, "Cheshire Cat (Poisson Law)");

    if (!game_state.poisson_history.empty())
    {
        float mean = mean_of(game_state.poisson_history);
        ImGui::Text("N: %zu | Theory: 2.00 | Real: %.2f", game_state.poisson_history.size(), mean);
        ImGui::Spacing();

        constexpr size_t        bins = 6;
        std::array<float, bins> freqs{};
        for (int v : game_state.poisson_history)
        {
            int index = std::clamp(v, 0, static_cast<int>(bins) - 1);
            freqs[static_cast<size_t>(index)] += 1.0f;
        }

        ImGui::PushStyleColor(ImGuiCol_PlotHistogram, c);
        plot_hist(freqs, ImVec2(600, 100));
        ImGui::PopStyleColor();
    }
    else
    {
        ImGui::TextDisabled("Waiting for data...");
    }

    ImGui::EndTabItem();
}

void draw_binomial_tab(const GameState& game_state, ImGuiTabItemFlags flags)
{
    if (!ImGui::BeginTabItem("Binomial", nullptr, flags))
    {
        return;
    }

    ImVec4 c = ImVec4(0.8f, 0.1f, 0.1f, 1.0f);
    ImGui::Spacing();
    ImGui::TextColored(c, "Card Gardeners (Binomial Law)");

    if (!game_state.binomial_history.empty())
    {
        float mean = mean_of(game_state.binomial_history);
        ImGui::Text("N: %zu | Theory changes with piece count (p=0.3) | Real: %.2f", game_state.binomial_history.size(), mean);
        ImGui::Spacing();

        constexpr size_t        bins = 8;
        std::array<float, bins> freqs{};
        for (int v : game_state.binomial_history)
        {
            int index = std::clamp(v, 0, static_cast<int>(bins) - 1);
            freqs[static_cast<size_t>(index)] += 1.0f;
        }

        ImGui::PushStyleColor(ImGuiCol_PlotHistogram, c);
        plot_hist(freqs, ImVec2(600, 100));
        ImGui::PopStyleColor();
    }
    else
    {
        ImGui::TextDisabled("Waiting for data...");
    }

    ImGui::EndTabItem();
}

void draw_normal_tab(const GameState& game_state, ImGuiTabItemFlags flags)
{
    if (!ImGui::BeginTabItem("Normal", nullptr, flags))
    {
        return;
    }

    ImVec4 c = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
    ImGui::Spacing();
    ImGui::TextColored(c, "White Rabbit (Normal Law)");

    if (!game_state.normal_history.empty())
    {
        float mean = mean_of(game_state.normal_history);
        ImGui::Text("N: %zu | Theory: 0.00 | Real: %.2f", game_state.normal_history.size(), mean);
        ImGui::Spacing();

        constexpr size_t        bins = 11;
        std::array<float, bins> freqs{};
        for (float v : game_state.normal_history)
        {
            int index = static_cast<int>(std::round(v)) + 5;
            index     = std::clamp(index, 0, static_cast<int>(bins) - 1);
            freqs[static_cast<size_t>(index)] += 1.0f;
        }

        ImGui::PushStyleColor(ImGuiCol_PlotHistogram, c);
        plot_hist(freqs, ImVec2(600, 100));
        ImGui::PopStyleColor();
    }
    else
    {
        ImGui::TextDisabled("Waiting for data...");
    }

    ImGui::EndTabItem();
}

void draw_beta_tab(const GameState& game_state, ImGuiTabItemFlags flags)
{
    if (!ImGui::BeginTabItem("Beta", nullptr, flags))
    {
        return;
    }

    ImVec4 c = ImVec4(0.2f, 0.9f, 0.4f, 1.0f);
    ImGui::Spacing();
    ImGui::TextColored(c, "Magic Mushroom (Beta Law)");

    if (!game_state.beta_history.empty())
    {
        float mean = mean_of(game_state.beta_history);
        ImGui::Text("N: %zu | Theory: 0.50 | Real: %.2f", game_state.beta_history.size(), mean);
        ImGui::Spacing();

        constexpr size_t        bins = 10;
        std::array<float, bins> freqs{};
        for (float v : game_state.beta_history)
        {
            int index = static_cast<int>(v * static_cast<float>(bins));
            index     = std::clamp(index, 0, static_cast<int>(bins) - 1);
            freqs[static_cast<size_t>(index)] += 1.0f;
        }

        ImGui::PushStyleColor(ImGuiCol_PlotHistogram, c);
        plot_hist(freqs, ImVec2(600, 100));
        ImGui::PopStyleColor();
    }
    else
    {
        ImGui::TextDisabled("Waiting for data...");
    }

    ImGui::EndTabItem();
}

} // namespace

// --- [BEGIN AI-GENERATED] WONDERLAND STATISTICS IMPLEMENTATION ---
void draw_wonderland_stats_window(const GameState& game_state)
{
    if (!game_state.is_wonderland_mode)
    {
        return;
    }

    ImGuiIO& io = ImGui::GetIO();

    ImVec2 top_right_pos = ImVec2(io.DisplaySize.x - 28.0f, 330.0f);
    ImGui::SetNextWindowPos(top_right_pos, ImGuiCond_FirstUseEver, ImVec2(1.0f, 0.0f));

    if (ImGui::Begin("Wonderland Live Statistics", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        // =========================================================
        // --- PART 1: PASSIVE EFFECTS (ALWAYS VISIBLE GRID) ---
        // =========================================================
        ImGui::TextDisabled("ACTION PASSIVES (Click-triggered)");
        ImGui::Spacing();

        if (ImGui::BeginTable("PassivesGrid", 2, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_BordersInnerH))
        {
            ImGui::TableNextRow();

            // ---------------------------------------------------------
            // --- GEOMETRIC (RED QUEEN) ---
            // ---------------------------------------------------------
            ImGui::TableSetColumnIndex(0);
            draw_geometric_cell(game_state);

            // ---------------------------------------------------------
            // --- CAUCHY (LOOKING GLASS) ---
            // ---------------------------------------------------------
            ImGui::TableSetColumnIndex(1);
            draw_cauchy_cell(game_state);

            ImGui::EndTable();
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // =========================================================
        // --- PART 2: WEATHER EFFECTS (DYNAMIC TABS) ---
        // =========================================================
        ImGui::TextDisabled("TURN & WEATHER EFFECTS (Auto-switching)");
        ImGui::Spacing();

        if (ImGui::BeginTabBar("WeatherTabs", ImGuiTabBarFlags_None))
        {
            // --- PRIORITY SYSTEM TO PREVENT TAB FLICKERING ---
            ImGuiTabItemFlags exp_flag = 0;
            ImGuiTabItemFlags bet_flag = 0;
            ImGuiTabItemFlags nor_flag = 0;
            ImGuiTabItemFlags bin_flag = 0;
            ImGuiTabItemFlags poi_flag = 0;
            ImGuiTabItemFlags uni_flag = 0;

            // --- ENDLESS FALL HAS MAXIMUM PRIORITY OVER REGULAR WEATHER ---
            if (game_state.current_echo == WonderlandLore::Event::ENDLESS_FALL)
            {
                exp_flag = ImGuiTabItemFlags_SetSelected;
            }
            else if (game_state.current_weather == WonderlandLore::Event::MAGIC_MUSHROOM)
            {
                bet_flag = ImGuiTabItemFlags_SetSelected;
            }
            else if (game_state.current_weather == WonderlandLore::Event::WHITE_RABBIT)
            {
                nor_flag = ImGuiTabItemFlags_SetSelected;
            }
            else if (game_state.current_weather == WonderlandLore::Event::CARD_GARDENERS)
            {
                bin_flag = ImGuiTabItemFlags_SetSelected;
            }
            else if (game_state.current_weather == WonderlandLore::Event::CHESHIRE_CAT)
            {
                poi_flag = ImGuiTabItemFlags_SetSelected;
            }
            else if (game_state.current_weather == WonderlandLore::Event::NONE)
            {
                uni_flag = ImGuiTabItemFlags_SetSelected;
            }

            // ---------------------------------------------------------
            // --- EXPONENTIAL (ENDLESS FALL) ---
            // ---------------------------------------------------------
            draw_exponential_tab(game_state, exp_flag);

            // ---------------------------------------------------------
            // --- BETA (MAGIC MUSHROOM) ---
            // ---------------------------------------------------------
            draw_beta_tab(game_state, bet_flag);

            // ---------------------------------------------------------
            // --- NORMAL (WHITE RABBIT) ---
            // ---------------------------------------------------------
            draw_normal_tab(game_state, nor_flag);

            // ---------------------------------------------------------
            // --- BINOMIAL (CARD GARDENERS) ---
            // ---------------------------------------------------------
            draw_binomial_tab(game_state, bin_flag);

            // ---------------------------------------------------------
            // --- POISSON (CHESHIRE CAT) ---
            // ---------------------------------------------------------
            draw_poisson_tab(game_state, poi_flag);

            // ---------------------------------------------------------
            // --- UNIFORM (WEATHER ENGINE) ---
            // ---------------------------------------------------------
            draw_uniform_tab(game_state, uni_flag);

            ImGui::EndTabBar();
        }
    }
    ImGui::End();
}
// --- [END AI-GENERATED] ---