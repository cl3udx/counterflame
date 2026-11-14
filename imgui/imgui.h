#pragma once
#define IMGUI_VERSION "1.89.9"
#include <float.h>
#include <stddef.h>
#include <stdint.h>

struct ImVec2 {
    float x, y;
    ImVec2() : x(0.0f), y(0.0f) {}
    ImVec2(float _x, float _y) : x(_x), y(_y) {}
};

struct ImVec4 {
    float x, y, z, w;
    ImVec4() : x(0), y(0), z(0), w(0) {}
    ImVec4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
};

enum ImGuiWindowFlags_ {
    ImGuiWindowFlags_None = 0,
    ImGuiWindowFlags_NoTitleBar = 1 << 0,
    ImGuiWindowFlags_NoResize = 1 << 1,
    ImGuiWindowFlags_NoScrollbar = 1 << 3,
    ImGuiWindowFlags_NoCollapse = 1 << 5,
    ImGuiWindowFlags_AlwaysAutoResize = 1 << 6,
};
typedef int ImGuiWindowFlags;

enum ImGuiCol_ {
    ImGuiCol_WindowBg,
    ImGuiCol_ChildBg,
    ImGuiCol_Border,
    ImGuiCol_Button,
    ImGuiCol_ButtonHovered,
    ImGuiCol_ButtonActive,
    ImGuiCol_CheckMark,
    ImGuiCol_SliderGrab,
    ImGuiCol_SliderGrabActive,
    ImGuiCol_COUNT
};
typedef int ImGuiCol;

enum ImGuiStyleVar_ {
    ImGuiStyleVar_WindowRounding,
    ImGuiStyleVar_ChildRounding,
    ImGuiStyleVar_FrameRounding,
    ImGuiStyleVar_COUNT
};
typedef int ImGuiStyleVar;

enum ImGuiCond_ {
    ImGuiCond_Always = 1 << 0,
    ImGuiCond_Once = 1 << 1,
    ImGuiCond_FirstUseEver = 1 << 2,
};
typedef int ImGuiCond;

struct ImGuiStyle {
    float WindowRounding;
    float ChildRounding;
    float FrameRounding;
    ImVec4 Colors[ImGuiCol_COUNT];
};

namespace ImGui {
    inline bool Begin(const char* name, bool* p_open = NULL, ImGuiWindowFlags flags = 0) { return true; }
    inline void End() {}
    inline bool BeginTabBar(const char* str_id, int flags = 0) { return true; }
    inline void EndTabBar() {}
    inline bool BeginTabItem(const char* label, bool* p_open = NULL, int flags = 0) { return true; }
    inline void EndTabItem() {}
    inline bool Checkbox(const char* label, bool* v) { return false; }
    inline bool SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format = "%.3f", int flags = 0) { return false; }
    inline bool Combo(const char* label, int* current_item, const char* items_separated_by_zeros, int popup_max_height_in_items = -1) { return false; }
    inline bool Button(const char* label, const ImVec2& size = ImVec2(0,0)) { return false; }
    inline ImGuiStyle& GetStyle() { static ImGuiStyle s; return s; }
    inline double GetTime() { return 0.0; }
    inline void SetNextWindowPos(const ImVec2& pos, int cond = 0, const ImVec2& pivot = ImVec2(0,0)) {}
    inline void SetNextWindowSize(const ImVec2& size, int cond = 0) {}
    inline void SetNextWindowBgAlpha(float alpha) {}
    inline void PushStyleColor(int idx, ImVec4 col) {}
    inline void PopStyleColor(int count = 1) {}
    inline void PushStyleVar(int idx, float val) {}
    inline void PopStyleVar(int count = 1) {}
}
