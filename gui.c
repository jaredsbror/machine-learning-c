#include <windows.h>
#include <commdlg.h>
#include <shlobj.h>
#include <stdio.h>
#include <tts.h>

#define ID_INPUT_BUTTON 101
#define ID_OUTPUT_BUTTON 102
#define ID_VOICE_COMBO 103
#define ID_SPEED_EDIT 104
#define ID_PAUSE_EDIT 105
#define ID_GENERATE_BUTTON 106

HWND hwndInputPath, hwndOutputPath, hwndSpeed, hwndPause, hwndVoiceCombo;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void OpenFolderDialog(HWND hwnd, HWND hwndEdit);
BOOL ValidateFloatInput(HWND hwndEdit, float min, float max);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char CLASS_NAME[] = "PiperTTSClass";

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, "Piper TTS Configuration",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 600, 300,
        NULL, NULL, hInstance, NULL
    );

    if (!hwnd) return 0;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

void AddControls(HWND hwnd) {
    CreateWindow("STATIC", "Input Folder:", WS_VISIBLE | WS_CHILD, 10, 10, 100, 20, hwnd, NULL, NULL, NULL);
    hwndInputPath = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 120, 10, 350, 20, hwnd, NULL, NULL, NULL);
    CreateWindow("BUTTON", "Browse...", WS_VISIBLE | WS_CHILD, 480, 10, 80, 20, hwnd, (HMENU)ID_INPUT_BUTTON, NULL, NULL);

    CreateWindow("STATIC", "Output Folder:", WS_VISIBLE | WS_CHILD, 10, 40, 100, 20, hwnd, NULL, NULL, NULL);
    hwndOutputPath = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 120, 40, 350, 20, hwnd, NULL, NULL, NULL);
    CreateWindow("BUTTON", "Browse...", WS_VISIBLE | WS_CHILD, 480, 40, 80, 20, hwnd, (HMENU)ID_OUTPUT_BUTTON, NULL, NULL);

    CreateWindow("STATIC", "Speed Scale (0.001-2.0):", WS_VISIBLE | WS_CHILD, 10, 70, 120, 20, hwnd, NULL, NULL, NULL);
    hwndSpeed = CreateWindow("EDIT", "1.0", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 140, 70, 100, 20, hwnd, (HMENU)ID_SPEED_EDIT, NULL, NULL);

    CreateWindow("STATIC", "Pause (0.001-2.0):", WS_VISIBLE | WS_CHILD, 10, 100, 120, 20, hwnd, NULL, NULL, NULL);
    hwndPause = CreateWindow("EDIT", "0.2", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 140, 100, 100, 20, hwnd, (HMENU)ID_PAUSE_EDIT, NULL, NULL);

    CreateWindow("STATIC", "Voice:", WS_VISIBLE | WS_CHILD, 10, 130, 50, 20, hwnd, NULL, NULL, NULL);
    hwndVoiceCombo = CreateWindow("COMBOBOX", "", WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL, 140, 130, 250, 150, hwnd, (HMENU)ID_VOICE_COMBO, NULL, NULL);
    
    for (int i = 0; i < sizeof(piper_voices)/sizeof(piper_voices[0]); i++) {
        SendMessage(hwndVoiceCombo, CB_ADDSTRING, 0, (LPARAM)piper_voices[i]);
    }
    SendMessage(hwndVoiceCombo, CB_SETCURSEL, 0, 0);

    CreateWindow("BUTTON", "Generate", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 
        250, 170, 100, 30, hwnd, (HMENU)ID_GENERATE_BUTTON, NULL, NULL);
}

void OpenFolderDialog(HWND hwnd, HWND hwndEdit) {
    BROWSEINFO bi = {0};
    bi.hwndOwner = hwnd;
    bi.lpszTitle = "Select Folder";
    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;

    LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
    if (pidl) {
        char path[MAX_PATH];
        if (SHGetPathFromIDList(pidl, path)) {
            SetWindowText(hwndEdit, path);
        }
        CoTaskMemFree(pidl);
    }
}

BOOL ValidateFloatInput(HWND hwndEdit, float min, float max) {
    char text[32];
    GetWindowText(hwndEdit, text, sizeof(text));
    
    char* end;
    float value = strtof(text, &end);
    
    if (*end != '\0' || value < min || value > max) {
        MessageBox(NULL, "Invalid value!\nPlease enter a number between 0.001 and 2.0", 
                 "Validation Error", MB_ICONERROR);
        return FALSE;
    }
    return TRUE;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:
            AddControls(hwnd);
            break;
            
        case WM_COMMAND:
            if (HIWORD(wParam) == BN_CLICKED) {
                switch (LOWORD(wParam)) {
                    case ID_INPUT_BUTTON:
                        OpenFolderDialog(hwnd, hwndInputPath);
                        break;
                        
                    case ID_OUTPUT_BUTTON:
                        OpenFolderDialog(hwnd, hwndOutputPath);
                        break;
                        
                    case ID_GENERATE_BUTTON:
                        if (!ValidateFloatInput(hwndSpeed, 0.001f, 2.0f) ||
                            !ValidateFloatInput(hwndPause, 0.001f, 2.0f)) {
                            break;
                        }
                        
                        // Add your generate function call here
                        MessageBox(hwnd, "Generation started!", "Info", MB_OK);
                        break;
                }
            }
            break;
            
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
            
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}
