#include <windows.h>
#include <d3d11.h>

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include "gdmc/InstalledMods.hpp"
#include "gdmc/GeodeIndex.hpp"
#include "gdmc/ModLoader.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_dx11.h"

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

static ID3D11Device* g_Device = nullptr;
static ID3D11DeviceContext* g_DeviceContext = nullptr;
static IDXGISwapChain* g_SwapChain = nullptr;
static ID3D11RenderTargetView* g_MainRenderTargetView = nullptr;

static void CreateRenderTarget()
{
    ID3D11Texture2D* backBuffer = nullptr;

    g_SwapChain->GetBuffer(
        0,
        IID_PPV_ARGS(&backBuffer)
    );

    if (backBuffer)
    {
        g_Device->CreateRenderTargetView(
            backBuffer,
            nullptr,
            &g_MainRenderTargetView
        );

        backBuffer->Release();
    }
}

static void CleanupRenderTarget()
{
    if (g_MainRenderTargetView)
    {
        g_MainRenderTargetView->Release();
        g_MainRenderTargetView = nullptr;
    }
}

static bool CreateDeviceD3D(GLFWwindow* window)
{
    DXGI_SWAP_CHAIN_DESC swapChainDesc{};

    swapChainDesc.BufferCount = 2;
    swapChainDesc.BufferDesc.Width = 0;
    swapChainDesc.BufferDesc.Height = 0;
    swapChainDesc.BufferDesc.Format =
        DXGI_FORMAT_R8G8B8A8_UNORM;

    swapChainDesc.BufferUsage =
        DXGI_USAGE_RENDER_TARGET_OUTPUT;

    swapChainDesc.OutputWindow =
        glfwGetWin32Window(window);

    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.Windowed = TRUE;

    swapChainDesc.SwapEffect =
        DXGI_SWAP_EFFECT_DISCARD;

    D3D_FEATURE_LEVEL featureLevel;

    const D3D_FEATURE_LEVEL levels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_0
    };

    HRESULT result = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
        levels,
        2,
        D3D11_SDK_VERSION,
        &swapChainDesc,
        &g_SwapChain,
        &g_Device,
        &featureLevel,
        &g_DeviceContext
    );

    if (FAILED(result))
        return false;

    CreateRenderTarget();

    return true;
}

static void CleanupDeviceD3D()
{
    CleanupRenderTarget();

    if (g_SwapChain)
    {
        g_SwapChain->Release();
        g_SwapChain = nullptr;
    }

    if (g_DeviceContext)
    {
        g_DeviceContext->Release();
        g_DeviceContext = nullptr;
    }

    if (g_Device)
    {
        g_Device->Release();
        g_Device = nullptr;
    }
}

static void RefreshInstalledMods(
    std::vector<std::string>& installedMods)
{
    installedMods =
        gdmc::InstalledMods::scan("mods");
} int main()
{
    if (!glfwInit())
        return 1;

    glfwWindowHint(
        GLFW_CLIENT_API,
        GLFW_NO_API
    );

    GLFWwindow* window =
        glfwCreateWindow(
            1100,
            700,
            "GD Mod Client",
            nullptr,
            nullptr
        );

    if (!window)
    {
        glfwTerminate();
        return 1;
    }

    if (!CreateDeviceD3D(window))
    {
        CleanupDeviceD3D();
        glfwDestroyWindow(window);
        glfwTerminate();

        return 1;
    }

    IMGUI_CHECKVERSION();

    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();

    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOther(
        window,
        true
    );

    ImGui_ImplDX11_Init(
        g_Device,
        g_DeviceContext
    );

    gdmc::GeodeIndex index;
    gdmc::ModLoader loader;

    char searchBuffer[256] = "";

    std::vector<gdmc::ModInfo> results;

    std::vector<std::string> installedMods;

int currentPage = 0;

    std::string status =
        "Ready.";
    RefreshInstalledMods(installedMods);
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(
            ImVec2(0, 0)
        );

        ImGui::SetNextWindowSize(
            io.DisplaySize
        );

        ImGui::Begin(
            "GD Mod Client",
            nullptr,
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoCollapse
        );

        ImGui::Text(
            "Geometry Dash Mod Client"
        );

        ImGui::Separator();

       if (ImGui::Button("Browse"))
{
    currentPage = 0;
}

ImGui::SameLine();

if (ImGui::Button("Installed"))
{
    currentPage = 1;
    RefreshInstalledMods(installedMods);
}

ImGui::Separator();

if (currentPage == 0)
{
    ImGui::Text("Search Geode Index");

    ImGui::SetNextItemWidth(700);

    ImGui::InputText(
        "##search",
        searchBuffer,
        sizeof(searchBuffer)
    );

    ImGui::SameLine();

    if (ImGui::Button("Search"))
    {
        status = "Searching...";

        results =
            index.search(searchBuffer);

        status =
            "Found " +
            std::to_string(results.size()) +
            " mod(s).";
    }

    ImGui::Spacing();

    ImGui::Text("Results");

    ImGui::BeginChild(
        "Results",
        ImVec2(0, 450),
        true
    );

    for (const auto& mod : results)
    {
        ImGui::Separator();

        ImGui::Text(
            "%s",
            mod.name.c_str()
        );

        ImGui::Text(
            "ID: %s",
            mod.id.c_str()
        );

        ImGui::Text(
            "Version: %s",
            mod.version.c_str()
        );

        ImGui::Text(
            "Developer: %s",
            mod.developer.c_str()
        );

        if (!mod.description.empty())
        {
            ImGui::TextWrapped(
                "%s",
                mod.description.c_str()
            );
        }

        if (ImGui::Button(
                ("Install##" + mod.id).c_str()))
        {
            status =
                "Installing " +
                mod.name +
                "...";

            if (loader.install(mod))
            {
                status =
                    "Installed " +
                    mod.name +
                    " successfully.";

                RefreshInstalledMods(
                    installedMods
                );
            }
            else
            {
                status =
                    "Failed to install " +
                    mod.name +
                    ".";
            }
        }
    }

    ImGui::EndChild();
}
else
{
    ImGui::Text("Installed Mods");

    if (ImGui::Button("Refresh"))
    {
        RefreshInstalledMods(
            installedMods
        );

        status =
            "Installed mods refreshed.";
    }

    ImGui::Spacing();

    ImGui::BeginChild(
        "Installed",
        ImVec2(0, 500),
        true
    );

    if (installedMods.empty())
    {
        ImGui::Text(
            "No mods installed."
        );
    }

    for (const auto& path : installedMods)
    {
        std::filesystem::path filePath(path);

        std::string filename =
            filePath.filename().string();

        ImGui::Separator();

        ImGui::Text(
            "%s",
            filename.c_str()
        );

        ImGui::TextWrapped(
            "%s",
            path.c_str()
        );

        ImGui::SameLine();

        if (ImGui::Button(
                ("Uninstall##" + filename).c_str()))
        {
            if (std::filesystem::remove(path))
            {
                status =
                    "Uninstalled " +
                    filename;

                RefreshInstalledMods(
                    installedMods
                );
            }
            else
            {
                status =
                    "Failed to uninstall " +
                    filename;
            }
        }
    }

    ImGui::EndChild();
}

        ImGui::Spacing();

        ImGui::Text("Results");

        ImGui::BeginChild(
            "Results",
            ImVec2(0, 450),
            true
        );

        for (const auto& mod : results)
        {
            ImGui::Separator();

            ImGui::Text(
                "%s",
                mod.name.c_str()
            );

            ImGui::Text(
                "ID: %s",
                mod.id.c_str()
            );

            ImGui::Text(
                "Version: %s",
                mod.version.c_str()
            );

            ImGui::Text(
                "Developer: %s",
                mod.developer.c_str()
            );

            if (!mod.description.empty())
            {
                ImGui::TextWrapped(
                    "%s",
                    mod.description.c_str()
                );
            }

            if (ImGui::Button(
                    ("Install##" + mod.id).c_str()))
            {
                status =
                    "Installing " +
                    mod.name +
                    "...";

                if (loader.install(mod))
                {
                    status =
                        "Installed " +
                        mod.name +
                        " successfully.";
                }
                else
                {
                    status =
                        "Failed to install " +
                        mod.name + ".";
                }
            }
        }

        ImGui::EndChild();

        ImGui::Separator();

        ImGui::Text(
            "Status: %s",
            status.c_str()
        );

        ImGui::End();

        ImGui::Render();

        const float clearColor[] =
        {
            0.06f,
            0.06f,
            0.07f,
            1.0f
        };

        g_DeviceContext->OMSetRenderTargets(
            1,
            &g_MainRenderTargetView,
            nullptr
        );

        g_DeviceContext->ClearRenderTargetView(
            g_MainRenderTargetView,
            clearColor
        );

        ImGui_ImplDX11_RenderDrawData(
            ImGui::GetDrawData()
        );

        g_SwapChain->Present(
            1,
            0
        );
    }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    ImGui::DestroyContext();

    CleanupDeviceD3D();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
