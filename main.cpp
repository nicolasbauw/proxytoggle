// A quick and dirty utility to toggle proxy ON or OFF. Checks status every 1/6th frame to fight against eventual proxy set by system policy.

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl2.h>
#include <stdio.h>
#include <GLFW/glfw3.h>
#include <windows.h>
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

int ProxyQuery();
int ProxyOff(bool proxy_on);
static int display_w, display_h;

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

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
        io.IniFilename = NULL;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL2_Init();

    // Setup interface
    bool show_main_window = true;
    bool proxy_on = false;
    int proxy = 0;

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // interface drawing
        ImGui::SetNextWindowPos(ImVec2(0,0));
        ImGui::SetNextWindowSize(ImVec2(260,40));
        ImGui::Begin("Proxy toggle", &show_main_window,ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoSavedSettings);
        ImGui::Text("  ");ImGui::SameLine();
        ImGui::Checkbox("Proxy is", &proxy_on);ImGui::SameLine();
        if (proxy) ImGui::Text("ON ");
        else ImGui::Text("OFF");
        ImGui::SameLine();ImGui::Text("   ");ImGui::SameLine();
        if (ImGui::Button("  Quit  ")) {ImGui_ImplGlfw_Shutdown();glfwTerminate();return 0;}
        ImGui::SameLine();ImGui::Text("  ");
        // query actual proxy status
        proxy = ProxyQuery();
        // sets proxy on or off according to the UI button
        ProxyOff(proxy_on);
        ImGui::End();

        // Window rendering
        ImGui::Render();
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window);
    
    }

    // Cleanup
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
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
	   if (RegSetValueEx(hKey,"ProxyEnable",0,REG_DWORD,(LPBYTE)&dwenable,sizeof(dwenable)) !=ERROR_SUCCESS)
	   {
	       RegCloseKey(hKey);
	       return 1;
     }}
     else RegCloseKey(hKey);
     return 0;
}

