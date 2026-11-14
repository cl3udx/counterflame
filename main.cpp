#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>
#include <cstring>
#include <mach/mach.h>
#include <sys/mman.h>
#include <dlfcn.h>
#include <imgui.h>

// ==========================================
// 1. CONFIG & OFFSETS (Ground Truth)
// ==========================================
namespace config {
    // Build-dependent offsets
    inline constexpr uintptr_t kGWorldOffset       = 0x0;
    inline constexpr uintptr_t kLocalPlayerOffset  = 0x0;
    inline constexpr uintptr_t kPlayerController   = 0x0;
    inline constexpr uintptr_t kAcknowledgedPawn   = 0x0;
    inline constexpr uintptr_t kPlayerCameraManager= 0x0;
    inline constexpr uintptr_t kActorListOffset    = 0x0;
    inline constexpr uintptr_t kRoundManagerOffset = 0x0;

    // Feature Toggles
    inline bool bMenuVisible       = false;
    inline bool bEspBox            = true;
    inline bool bEspSkeleton       = true;
    inline bool bEspName           = true;
    inline bool bEspHealth         = true;
    inline bool bEspDistance       = true;
    inline bool bEspLoot           = false;
    inline bool bRadar             = false;
    inline bool bChams             = false;
    
    inline bool bAimbot            = false;
    inline float fAimbotFov        = 4.5f;
    inline float fAimbotSmooth     = 6.0f;
    inline bool bShowFovCircle     = true;
    
    inline bool bNoRecoil          = true;
    inline bool bInfiniteAmmo      = false;
    inline bool bGodMode           = false;
    inline bool bSpeedhack         = false;
    inline float fSpeedMultiplier  = 1.15f;
    
    // Visuals & Effects
    inline bool bCustomCrosshair   = true;
    inline int  iCrosshairStyle    = 0; // 0 - Dot, 1 - Cross
    inline bool bRainbowAccent     = true;
    inline bool bHitmarker         = true;

    // Match Exploits & Hardcore
    inline bool bAutoWin           = false;
    inline bool bFastPlantDefuse   = false;
    inline bool bServerCrashFlood  = false;
    inline bool bLagSwitch         = false;
    inline bool bInvisibleMode     = false;
    inline bool bSkinChanger       = true;
    inline bool bInfiniteMoney     = true;
}

// ==========================================
// 2. MEMORY & VIRTUAL MACHINE WRAPPER
// ==========================================
namespace memory {
    inline uintptr_t GetModuleBase(const char* moduleName) {
        Dl_info info;
        if (dladdr(reinterpret_cast<void*>(GetModuleBase), &info) && info.dli_fname) {
            return reinterpret_cast<uintptr_t>(info.dli_fbase);
        }
        return 0;
    }

    template <typename T>
    inline T Read(uintptr_t address) {
        if (!address) return T{};
        try {
            return *reinterpret_cast<T*>(address);
        } catch (...) {
            return T{};
        }
    }

    template <typename T>
    inline bool Write(uintptr_t address, const T& val) {
        if (!address) return false;
        mach_port_t task = mach_task_self();
        vm_address_t targetPage = address & ~(vm_address_t)(PAGE_SIZE - 1);
        
        kern_return_t kr = vm_protect(task, targetPage, PAGE_SIZE, FALSE, VM_PROT_READ | VM_PROT_WRITE | VM_PROT_COPY);
        if (kr != KERN_SUCCESS) return false;

        *reinterpret_cast<T*>(address) = val;

        vm_protect(task, targetPage, PAGE_SIZE, FALSE, VM_PROT_READ | VM_PROT_EXECUTE);
        return true;
    }
}

// ==========================================
// 3. UI, FLOATING TOGGLE & IMGUI MENU
// ==========================================
namespace menu {
    inline void RenderFloatingToggle() {
        ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
                                 ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize;

        ImGui::SetNextWindowPos(ImVec2(50, 100), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowBgAlpha(0.65f);

        ImGui::Begin("##FloatingToggleWindow", nullptr, flags);

        ImVec4 neonColor = config::bRainbowAccent ? 
            ImVec4(sinf(static_cast<float>(ImGui::GetTime()) * 2.0f) * 0.3f + 0.7f, 
                   sinf(static_cast<float>(ImGui::GetTime()) * 2.0f + 2.094f) * 0.3f + 0.7f, 
                   sinf(static_cast<float>(ImGui::GetTime()) * 2.0f + 4.188f) * 0.3f + 0.7f, 1.0f) : 
            ImVec4(0.00f, 0.80f, 1.00f, 1.00f);

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.12f, 0.12f, 0.16f, 0.90f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.20f, 0.20f, 0.28f, 1.00f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, neonColor);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 25.0f);

        if (ImGui::Button(" CF ", ImVec2(48, 48))) {
            config::bMenuVisible = !config::bMenuVisible;
        }

        ImGui::PopStyleVar();
        ImGui::PopStyleColor(3);
        ImGui::End();
    }

    inline void RenderImGuiMenu() {
        RenderFloatingToggle();
        if (!config::bMenuVisible) return;

        ImVec4 accentColor = config::bRainbowAccent ? 
            ImVec4(sinf(static_cast<float>(ImGui::GetTime()) * 1.5f) * 0.3f + 0.7f, 
                   sinf(static_cast<float>(ImGui::GetTime()) * 1.5f + 2.094f) * 0.3f + 0.7f, 
                   sinf(static_cast<float>(ImGui::GetTime()) * 1.5f + 4.188f) * 0.3f + 0.7f, 1.0f) : 
            ImVec4(0.00f, 0.80f, 1.00f, 1.00f);

        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowRounding = 10.0f;
        style.ChildRounding = 6.0f;
        style.FrameRounding = 5.0f;
        style.Colors[ImGuiCol_WindowBg]         = ImVec4(0.07f, 0.07f, 0.09f, 0.96f);
        style.Colors[ImGuiCol_ChildBg]          = ImVec4(0.10f, 0.10f, 0.13f, 1.00f);
        style.Colors[ImGuiCol_Border]           = ImVec4(0.20f, 0.20f, 0.26f, 0.80f);
        style.Colors[ImGuiCol_CheckMark]        = accentColor;
        style.Colors[ImGuiCol_SliderGrab]       = accentColor;
        style.Colors[ImGuiCol_SliderGrabActive] = accentColor;

        ImGui::SetNextWindowSize(ImVec2(620, 480), ImGuiCond_FirstUseEver);
        ImGui::Begin("CounterFlame // Elite Mod Menu [v3.0]", &config::bMenuVisible, 
                     ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

        if (ImGui::BeginTabBar("MenuTabs")) {
            // 1. VISUALS
            if (ImGui::BeginTabItem("Visuals")) {
                ImGui::Checkbox("Box ESP", &config::bEspBox);
                ImGui::Checkbox("Skeleton ESP", &config::bEspSkeleton);
                ImGui::Checkbox("Name & Health", &config::bEspName);
                ImGui::Checkbox("Distance Tag", &config::bEspDistance);
                ImGui::Checkbox("Mini-Radar", &config::bRadar);
                ImGui::Checkbox("Chams (Wallhack)", &config::bChams);
                ImGui::Checkbox("Custom Crosshair", &config::bCustomCrosshair);
                if (config::bCustomCrosshair) {
                    ImGui::Combo("Style", &config::iCrosshairStyle, "Dot\0Cross\0");
                }
                ImGui::EndTabItem();
            }

            // 2. AIMBOT
            if (ImGui::BeginTabItem("Aimbot")) {
                ImGui::Checkbox("Enable Aimbot", &config::bAimbot);
                ImGui::SliderFloat("FOV Radius", &config::fAimbotFov, 1.0f, 30.0f, "%.1f deg");
                ImGui::Checkbox("Show FOV Circle", &config::bShowFovCircle);
                ImGui::SliderFloat("Smoothing", &config::fAimbotSmooth, 1.0f, 20.0f, "%.1f");
                ImGui::Checkbox("No Recoil", &config::bNoRecoil);
                ImGui::EndTabItem();
            }

            // 3. MISC
            if (ImGui::BeginTabItem("Misc")) {
                ImGui::Checkbox("God Mode", &config::bGodMode);
                ImGui::Checkbox("Infinite Ammo", &config::bInfiniteAmmo);
                ImGui::Checkbox("Speedhack", &config::bSpeedhack);
                if (config::bSpeedhack) {
                    ImGui::SliderFloat("Multiplier", &config::fSpeedMultiplier, 1.0f, 3.0f, "%.2fx");
                }
                ImGui::EndTabItem();
            }

            // 4. HARDCORE
            if (ImGui::BeginTabItem("Hardcore")) {
                ImGui::Checkbox("Auto-Win Round", &config::bAutoWin);
                ImGui::Checkbox("Fast Plant & Defuse", &config::bFastPlantDefuse);
                ImGui::Checkbox("Server Crash Flood", &config::bServerCrashFlood);
                ImGui::Checkbox("Lag Switch", &config::bLagSwitch);
                ImGui::Checkbox("Infinite Money", &config::bInfiniteMoney);
                ImGui::Checkbox("Skin Changer", &config::bSkinChanger);
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        ImGui::End();
    }
}

// ==========================================
// 4. MAIN THREAD & ENTRY POINT
// ==========================================
void MainThread() {
    uintptr_t baseAddress = 0;
    while (baseAddress == 0) {
        baseAddress = memory::GetModuleBase("CounterFlame");
        if (baseAddress == 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }

    while (true) {
        if (config::kGWorldOffset != 0) {
            uintptr_t gWorld = memory::Read<uintptr_t>(baseAddress + config::kGWorldOffset);
            if (gWorld && config::bAutoWin && config::kRoundManagerOffset != 0) {
                uintptr_t roundMgr = memory::Read<uintptr_t>(baseAddress + config::kRoundManagerOffset);
                if (roundMgr) {
                    memory::Write<int32_t>(roundMgr + 0x40, 1);
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

__attribute__((constructor)) void DylibMain() {
    std::thread(MainThread).detach();
}
