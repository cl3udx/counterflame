#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <string>
#include <dlfcn.h>
#include <objc/runtime.h>
#include <objc/message.h>

// Подключаем ImGui безопасно
#include <imgui.h>

namespace CounterFlame {
    bool menuOpen = true;
    
    // 1. Visuals
    bool espBox = true;
    bool espSkeleton = true;
    bool espName = true;
    bool espDistance = true;
    bool espHealth = true;
    bool chams = false;
    bool customCrosshair = true;

    // 2. Aimbot & Weapon
    bool aimbot = false;
    int aimbone = 0;
    float aimbotFov = 5.0f;
    bool showFovCircle = true;
    float aimbotSmooth = 5.0f;
    bool noRecoil = true;
    bool infiniteAmmo = true;
    bool rapidFire = false;

    // 3. Misc & Exploits
    bool godMode = false;
    float speedMultiplier = 1.0f;
    bool fastPlant = false;
    bool fastDefuse = false;
    bool infiniteMoney = true;
    float fovChanger = 90.0f;

    // 4. Skins
    int knifeIndex = 0;
    int gloveIndex = 0;
    int weaponSkinIndex = 0;
}

// Отрисовка ImGui меню со всеми вкладками
void DrawImGuiMenu() {
    if (!CounterFlame::menuOpen) return;

    ImGui::SetNextWindowSize(ImVec2(450, 380), ImGuiCond_FirstUseEver);
    ImGui::Begin("CounterFlame // Ultimate v1.4.1", &CounterFlame::menuOpen, 0);
    
    if (ImGui::BeginTabBar("MainTabs")) {
        
        // --- ВИЗУАЛЫ ---
        if (ImGui::BeginTabItem("Visuals")) {
            ImGui::Checkbox("Box ESP", &CounterFlame::espBox);
            ImGui::Checkbox("Skeleton ESP", &CounterFlame::espSkeleton);
            ImGui::Checkbox("Name Tags", &CounterFlame::espName);
            ImGui::Checkbox("Distance ESP", &CounterFlame::espDistance);
            ImGui::Checkbox("Health Bar", &CounterFlame::espHealth);
            ImGui::Separator();
            ImGui::Checkbox("Chams (Wallhack)", &CounterFlame::chams);
            ImGui::Checkbox("Custom Crosshair", &CounterFlame::customCrosshair);
            ImGui::EndTabItem();
        }
        
        // --- АИМБОТ ---
        if (ImGui::BeginTabItem("Aimbot")) {
            ImGui::Checkbox("Enable Aimbot", &CounterFlame::aimbot);
            
            const char* bones[] = { "Head", "Neck", "Chest" };
            ImGui::Combo("Target Bone", &CounterFlame::aimbone, bones, IM_ARRAYSIZE(bones));
            
            ImGui::SliderFloat("FOV Radius", &CounterFlame::aimbotFov, 1.0f, 30.0f);
            ImGui::Checkbox("Show FOV Circle", &CounterFlame::showFovCircle);
            ImGui::SliderFloat("Smoothing", &CounterFlame::aimbotSmooth, 1.0f, 20.0f);
            
            ImGui::Separator();
            ImGui::Checkbox("No Recoil", &CounterFlame::noRecoil);
            ImGui::Checkbox("Infinite Ammo", &CounterFlame::infiniteAmmo);
            ImGui::Checkbox("Rapid Fire", &CounterFlame::rapidFire);
            ImGui::EndTabItem();
        }
        
        // --- МИСК ---
        if (ImGui::BeginTabItem("Misc")) {
            ImGui::Checkbox("God Mode", &CounterFlame::godMode);
            ImGui::SliderFloat("Speed Multiplier", &CounterFlame::speedMultiplier, 1.0f, 3.0f);
            
            ImGui::Separator();
            ImGui::Checkbox("Fast Plant", &CounterFlame::fastPlant);
            ImGui::Checkbox("Fast Defuse", &CounterFlame::fastDefuse);
            ImGui::Checkbox("Infinite Money", &CounterFlame::infiniteMoney);
            
            ImGui::Separator();
            ImGui::SliderFloat("Camera FOV", &CounterFlame::fovChanger, 60.0f, 120.0f);
            ImGui::EndTabItem();
        }

        // --- СКИНЫ ---
        if (ImGui::BeginTabItem("Skins")) {
            ImGui::Text("Skin Changer");
            
            const char* knives[] = { "Default", "Karambit", "M9 Bayonet", "Butterfly" };
            ImGui::Combo("Knife", &CounterFlame::knifeIndex, knives, IM_ARRAYSIZE(knives));
            
            const char* gloves[] = { "Default", "Vice", "Fade", "Imperial Plaid" };
            ImGui::Combo("Gloves", &CounterFlame::gloveIndex, gloves, IM_ARRAYSIZE(gloves));
            
            const char* skins[] = { "Default", "Dragon Lore", "Neo-Noir", "Howl" };
            ImGui::Combo("Weapon Skin", &CounterFlame::weaponSkinIndex, skins, IM_ARRAYSIZE(skins));
            
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }
    ImGui::End();
}

// Указатель на оригинальный метод отрисовки
static void (*original_drawRect)(id, SEL, void*) = nullptr;

// Хук кадра игры для вызова ImGui
void hooked_drawRect(id self, SEL _cmd, void* rect) {
    if (original_drawRect) {
        original_drawRect(self, _cmd, rect);
    }

    static bool imgui_initialized = false;
    if (!imgui_initialized) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); 
        (void)io;
        ImGui::StyleColorsDark();
        imgui_initialized = true;
    }

    ImGui::NewFrame();
    DrawImGuiMenu();
    ImGui::Render();
}

// Инициализация безопасного хука через рантайм
__attribute__((constructor)) void InitCounterFlameCheat() {
    std::thread([]() {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        
        Class viewClass = objc_getClass("MTKView");
        if (viewClass) {
            SEL selector = sel_registerName("drawRect:");
            Method method = class_getInstanceMethod(viewClass, selector);
            if (method) {
                original_drawRect = (void(*)(id, SEL, void*))method_setImplementation(viewClass, (IMP)hooked_drawRect);
            }
        }
    }).detach();
}
