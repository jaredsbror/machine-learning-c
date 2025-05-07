// gui.c
#include <windows.h>
#include <commdlg.h>
#include <shlobj.h>
#include <stdio.h>
#include "application.h"
#include "voices.h"
#include "data.h"
#include "files.h"

#define ID_INPUT_BUTTON 101
#define ID_OUTPUT_BUTTON 102
#define ID_VOICE_COMBO 103
#define ID_SPEED_EDIT 104
#define ID_PAUSE_EDIT 105
#define ID_GENERATE_BUTTON 106
#define ID_TXT_FILE_BUTTON 107
#define ID_WORDS_EDIT 108
#define ID_SPLIT_BUTTON 109

HWND hwndInputPath, hwndOutputPath, hwndSpeed, hwndPause, hwndVoiceCombo;

void AddControls(HWND hwnd);
void OpenFolderDialog(HWND hwnd, HWND hwndEdit);
void OpenFileDialog(HWND hwnd, HWND hwndEdit);
BOOL ValidateFloatInput(HWND hwndEdit, float min, float max);
BOOL ValidateIntegerInput(HWND hwndEdit, int min, int max);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"PiperTTSClass";

    WNDCLASSW wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClassW(&wc);

    HWND hwnd = CreateWindowExW(
        0, CLASS_NAME, L"Piper TTS Configuration",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 600, 300,
        NULL, NULL, hInstance, NULL
    );

    if (!hwnd) return 0;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessageW(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return (int)msg.wParam;
}


void AddControls(HWND hwnd) {
    CreateWindowW(L"STATIC", L"Input Folder:", WS_VISIBLE | WS_CHILD, 10, 10, 200, 20, hwnd, NULL, NULL, NULL);
    hwndInputPath = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 120, 10, 350, 20, hwnd, NULL, NULL, NULL);
    CreateWindowW(L"BUTTON", L"Browse...", WS_VISIBLE | WS_CHILD, 480, 10, 80, 20, hwnd, (HMENU)ID_INPUT_BUTTON, NULL, NULL);

    CreateWindowW(L"STATIC", L"Output Folder:", WS_VISIBLE | WS_CHILD, 10, 40, 200, 20, hwnd, NULL, NULL, NULL);
    hwndOutputPath = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 120, 40, 350, 20, hwnd, NULL, NULL, NULL);
    CreateWindowW(L"BUTTON", L"Browse...", WS_VISIBLE | WS_CHILD, 480, 40, 80, 20, hwnd, (HMENU)ID_OUTPUT_BUTTON, NULL, NULL);

    CreateWindowW(L"STATIC", L"Speed Scale (0.001-2.0):", WS_VISIBLE | WS_CHILD, 10, 70, 200, 20, hwnd, NULL, NULL, NULL);
    hwndSpeed = CreateWindowW(L"EDIT", L"1.0", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 190, 70, 100, 20, hwnd, (HMENU)ID_SPEED_EDIT, NULL, NULL);

    CreateWindowW(L"STATIC", L"Pause (0.001-2.0):", WS_VISIBLE | WS_CHILD, 10, 100, 200, 20, hwnd, NULL, NULL, NULL);
    hwndPause = CreateWindowW(L"EDIT", L"0.2", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 190, 100, 200, 20, hwnd, (HMENU)ID_PAUSE_EDIT, NULL, NULL);

    CreateWindowW(L"STATIC", L"Voice:", WS_VISIBLE | WS_CHILD, 10, 130, 50, 20, hwnd, NULL, NULL, NULL);
    hwndVoiceCombo = CreateWindowW(L"COMBOBOX", L"", WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL, 190, 130, 250, 150, hwnd, (HMENU)ID_VOICE_COMBO, NULL, NULL);

    // Text file selection
    CreateWindowW(L"STATIC", L"Text File:", WS_VISIBLE | WS_CHILD, 10, 160, 100, 20, hwnd, NULL, NULL, NULL);
    HWND hwndTxtPath = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL, 120, 160, 350, 20, hwnd, NULL, NULL, NULL);
    CreateWindowW(L"BUTTON", L"Browse...", WS_VISIBLE | WS_CHILD, 480, 160, 80, 20, hwnd, (HMENU)ID_TXT_FILE_BUTTON, NULL, NULL);

    // Words per file input
    CreateWindowW(L"STATIC", L"Words/File:", WS_VISIBLE | WS_CHILD, 10, 190, 100, 20, hwnd, NULL, NULL, NULL);
    hwndWords = CreateWindowW(L"EDIT", L"3700", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 120, 190, 100, 20, hwnd, (HMENU)ID_WORDS_EDIT, NULL, NULL);

    // Split button
    CreateWindowW(L"BUTTON", L"Split", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 250, 220, 100, 30, hwnd, (HMENU)ID_SPLIT_BUTTON, NULL, NULL);


    // Populate the combo box with voice names
    for (int i = 0; i < piper_voices_count; i++) {
        wchar_t voiceNameWide[256];
        MultiByteToWideChar(CP_UTF8, 0, piper_voices[i].voice_name, -1, voiceNameWide, 256);
        SendMessageW(hwndVoiceCombo, CB_ADDSTRING, 0, (LPARAM)voiceNameWide);
    }
    SendMessageW(hwndVoiceCombo, CB_SETCURSEL, 0, 0);

    CreateWindowW(L"BUTTON", L"Convert", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 
        250, 170, 100, 30, hwnd, (HMENU)ID_GENERATE_BUTTON, NULL, NULL);
}


void OpenFolderDialog(HWND hwnd, HWND hwndEdit) {
    BROWSEINFOW bi = {0};
    bi.hwndOwner = hwnd;
    bi.lpszTitle = L"Select Folder";
    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;

    PIDLIST_ABSOLUTE pidl = SHBrowseForFolderW(&bi);
    if (pidl) {
        wchar_t path[MAX_PATH];
        if (SHGetPathFromIDListW(pidl, path)) {
            SetWindowTextW(hwndEdit, path);
        }
        CoTaskMemFree(pidl);
    }
}

void OpenFileDialog(HWND hwnd, HWND hwndEdit) {
    OPENFILENAMEW ofn = {0};
    wchar_t fileName[MAX_PATH] = {0};
    
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
    
    if(GetOpenFileNameW(&ofn))
        SetWindowTextW(hwndEdit, fileName);
}


BOOL ValidateFloatInput(HWND hwndEdit, float min, float max) {
    wchar_t text[32];
    GetWindowTextW(hwndEdit, text, sizeof(text)/sizeof(wchar_t));
    
    wchar_t* end;
    float value = wcstof(text, &end);
    
    if (*end != L'\0' || value < min || value > max) {
        MessageBoxW(NULL, L"Invalid value!\nPlease enter a number between 0.001 and 2.0", 
                 L"Validation Error", MB_ICONERROR);
        return FALSE;
    }
    return TRUE;
}

BOOL ValidateIntegerInput(HWND hwndEdit, int min, int max) {
    wchar_t text[32];
    GetWindowTextW(hwndEdit, text, 32);
    
    int value = _wtoi(text);
    if(value < min || value > max) {
        MessageBoxW(NULL, L"Invalid value! Enter between 1-100000", L"Error", MB_ICONERROR);
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
                    
                        // Get input/output folder paths
                        wchar_t inputPathW[MAX_PATH], outputPathW[MAX_PATH];
                        GetWindowTextW(hwndInputPath, inputPathW, MAX_PATH);
                        GetWindowTextW(hwndOutputPath, outputPathW, MAX_PATH);
                    
                        char inputPath[MAX_PATH * 3], outputPath[MAX_PATH * 3];
                        WideCharToMultiByte(CP_UTF8, 0, inputPathW, -1, inputPath, sizeof(inputPath), NULL, NULL);
                        WideCharToMultiByte(CP_UTF8, 0, outputPathW, -1, outputPath, sizeof(outputPath), NULL, NULL);
                    
                        // Get selected voice
                        int sel = SendMessageW(hwndVoiceCombo, CB_GETCURSEL, 0, 0);
                        if (sel == CB_ERR) {
                            MessageBoxW(hwnd, L"Please select a voice.", L"Error", MB_ICONERROR);
                            break;
                        }
                        const char *model_path = piper_voices[sel].filepath;
                    
                        // Get speed and pause values
                        wchar_t speedText[32], pauseText[32];
                        GetWindowTextW(hwndSpeed, speedText, 32);
                        GetWindowTextW(hwndPause, pauseText, 32);
                    
                        double length_scale = wcstod(speedText, NULL);
                        double sentence_silence = wcstod(pauseText, NULL);
                    
                        // Call your function
                        convertCustom(model_path, inputPath, outputPath, length_scale, sentence_silence);
                    
                        MessageBoxW(hwnd, L"Generation started!", L"Info", MB_OK);
                        break;

                    case ID_TXT_FILE_BUTTON:
                        OpenFileDialog(hwnd, hwndTxtPath);
                        break;
                        
                    case ID_SPLIT_BUTTON: {
                        if(!ValidateIntegerInput(hwndWords, 1, 100000)) break;
                        
                        wchar_t txtPathW[MAX_PATH], wordsText[32];
                        GetWindowTextW(hwndTxtPath, txtPathW, MAX_PATH);
                        GetWindowTextW(hwndWords, wordsText, 32);
                        
                        char txtPath[MAX_PATH];
                        WideCharToMultiByte(CP_UTF8, 0, txtPathW, -1, txtPath, MAX_PATH, NULL, NULL);
                        
                        int wordsPer = _wtoi(wordsText);
                        // Call split function with parameters
                        split_txt_file_gui(txtPath, wordsPer);
                        MessageBoxW(hwnd, L"File split completed!", L"Info", MB_OK);
                        break;
                }
            }
            break;
            
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
            
        default:
            return DefWindowProcW(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}
