// ImGui - standalone example application for Glfw + OpenGL 2, using fixed pipeline
// If you are new to ImGui, see examples/README.txt and documentation at the top of imgui.cpp.

#include <imgui.h>
#include "imgui_impl_glfw.h"
#include <stdio.h>
#include <glfw3.h>
#include <windows.h>

int ProxyQuery();
int ProxyOff(bool proxy_on);

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error %d: %s\n", error, description);
}

int main(int, char**)
{
    // Setup window
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        return 1;
    GLFWwindow* window = glfwCreateWindow(180, 40, "Proxy toggle", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(2);

    // Setup ImGui binding
    ImGui_ImplGlfw_Init(window, true);
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = NULL;

    // Load Fonts
    // (there is a default font, this is only if you want to change it. see extra_fonts/README.txt for more details)
    //ImGuiIO& io = ImGui::GetIO();
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../extra_fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../extra_fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyClean.ttf", 13.0f);
    //io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyTiny.ttf", 10.0f);
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());

    ImVec4 clear_color = ImColor(0.0f, 0.0f, 0.0f);
    bool show_main_window = true;
    bool proxy_on = false;
    int proxy = 0;
    int i=4;

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        ImGui_ImplGlfw_NewFrame();

        // 1. Show a simple window
        // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
        {   ImGui::SetNextWindowPos(ImVec2(0,0));
            ImGui::SetNextWindowSize(ImVec2(180,40));
            ImGui::Begin("Proxy toggle", &show_main_window,ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoSavedSettings);
            ImGui::PushID(i);
            ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(i/7.0f, 0.6f, 0.6f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(i/7.0f, 0.7f, 0.7f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(i/7.0f, 0.8f, 0.8f));
            ImGui::Checkbox("Proxy", &proxy_on);ImGui::SameLine();
            if (proxy) ImGui::Text("ON ");
            else ImGui::Text("OFF");
            ImGui::SameLine();
            if (ImGui::Button("  Quit  ")) {ImGui_ImplGlfw_Shutdown();glfwTerminate();return 0;}
            proxy = ProxyQuery();
            ProxyOff(proxy_on);
            ImGui::PopStyleColor(3);
            ImGui::PopID();
            ImGui::End();

        // Rendering
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui::Render();
        glfwSwapBuffers(window);
    }
    }

    // Cleanup
    ImGui_ImplGlfw_Shutdown();
    glfwTerminate();

    return 0;
}


int ProxyQuery () {
HKEY hKey;
DWORD buffer;
LONG result;
unsigned long type=REG_DWORD, size=32;

	result = RegOpenKeyEx(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings",0,KEY_READ,&hKey);
	if(result == ERROR_SUCCESS)
	{
	   RegQueryValueEx(hKey,"ProxyEnable",NULL,&type,(LPBYTE)&buffer,&size);
	   RegCloseKey(hKey);
       if (buffer) return 1;
       else return 0;
     }
}

int ProxyOff (bool proxy_on) {
HKEY hKey;
LONG result;
DWORD dwenable;
if (!proxy_on) dwenable=0;
else dwenable=1;
	result = RegOpenKey(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings",&hKey);
	if(result == ERROR_SUCCESS)
	{
	   if (RegSetValueEx(hKey,"ProxyEnable",0,REG_DWORD,(LPBYTE)&dwenable,sizeof(dwenable)) !=ERROR_SUCCESS);
	   {
	       RegCloseKey(hKey);
	       return 1;
     }}
     else {
	       RegCloseKey(hKey);
	       return 0;
     }
}

