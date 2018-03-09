// Permet de désactiver ou réactiver le proxy dans les paramètres système d'une machine Windows.
// Vérifie l'activation en temps réel pour le désactiver immédiatement en cas de réactivation non voulue par la politique système.
// Un bouton permet de choisir si le proxy doit être actif ou non.

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
    GLFWwindow* window = glfwCreateWindow(260, 40, "Proxy toggle", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(6);

    // Setup ImGui binding
    ImGui_ImplGlfw_Init(window, true);
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = NULL;

    // Setup interface
    ImVec4 clear_color = ImColor(0.0f, 0.0f, 0.0f);
    bool show_main_window = true;
    bool proxy_on = false;
    int proxy = 0;
    int i=4;    //used to push and pop UI styles

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        ImGui_ImplGlfw_NewFrame();

        // interface drawing
        {   ImGui::SetNextWindowPos(ImVec2(0,0));
            ImGui::SetNextWindowSize(ImVec2(260,40));
            ImGui::Begin("Proxy toggle", &show_main_window,ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoSavedSettings);
            ImGui::PushID(i);
            ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(i/7.0f, 0.6f, 0.6f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(i/7.0f, 0.7f, 0.7f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(i/7.0f, 0.8f, 0.8f));
            ImGui::Text("  ");ImGui::SameLine();
            ImGui::Checkbox("Proxy", &proxy_on);ImGui::SameLine();
            if (proxy) ImGui::Text("ON ");
            else ImGui::Text("OFF");
            ImGui::SameLine();ImGui::Text("     ");ImGui::SameLine();
            if (ImGui::Button("  Quit  ")) {ImGui_ImplGlfw_Shutdown();glfwTerminate();return 0;}
            ImGui::SameLine();ImGui::Text("  ");
            // query actual proxy status
            proxy = ProxyQuery();
            // sets proxy on or off according to the UI button
            ProxyOff(proxy_on);

            ImGui::PopStyleColor(3);
            ImGui::PopID();
            ImGui::End();

        // Window rendering
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

// registry query
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
     else return 0;
}

// setting proxy state according to user setting
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

