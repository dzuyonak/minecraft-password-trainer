#define UNICODE
#define _UNICODE
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <mmsystem.h>
#include <shlwapi.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

typedef ULONG PROPID;
#include <gdiplus.h>

#include "assets_data.h"

#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "shlwapi.lib")

using namespace Gdiplus;

// Control IDs
#define IDC_EDIT_LOGIN       1001
#define IDC_EDIT_PASSWORD    1002
#define IDC_BTN_LOGIN        1003
#define IDC_BTN_EYE          1004
#define IDC_BTN_AGAIN        1005
#define IDC_BTN_EXIT         1006

// Language selector button IDs
#define IDC_BTN_LANG_RU      1010
#define IDC_BTN_LANG_EN      1011
#define IDC_BTN_LANG_PL      1012

// Supported Languages
enum AppLanguage {
    LANG_RU = 0,
    LANG_EN = 1,
    LANG_PL = 2
};

// Application States
enum AppState {
    STATE_LOGIN = 0,
    STATE_SUCCESS = 1,
    STATE_SUPERPRIZE = 2
};

// String identifiers for localization
enum StringId {
    STR_APP_TITLE,
    STR_HEADER_TITLE,
    STR_HEADER_SUB_FMT,
    STR_LBL_LOGIN,
    STR_LBL_PASS,
    STR_BTN_LOGIN,
    STR_MSG_DEFAULT,
    STR_ERR_EMPTY,
    STR_ERR_BOTH,
    STR_ERR_LOGIN,
    STR_ERR_PASS,
    STR_HINT_TITLE,
    STR_HINT_FMT,
    STR_HINT_CONFIG_ONLY,
    STR_WIN_TITLE,
    STR_WIN_SUB,
    STR_PROGRESS_FMT,
    STR_BTN_AGAIN,
    STR_BTN_EXIT,
    STR_SUPER_TITLE,
    STR_SUPER_SUB,
    STR_DRAGON_TITLE,
    STR_DRAGON_BADGE,
    STR_DRAGON_DESC,
    STR_TROPHY_TEXT,
    STR_BTN_RESET
};

// Global variables
HINSTANCE g_hInstance = NULL;
HWND g_hWndMain = NULL;
HWND g_hEditLogin = NULL;
HWND g_hEditPassword = NULL;
HWND g_hBtnLogin = NULL;
HWND g_hBtnEye = NULL;
HWND g_hBtnAgain = NULL;
HWND g_hBtnExit = NULL;

HWND g_hBtnLangRU = NULL;
HWND g_hBtnLangEN = NULL;
HWND g_hBtnLangPL = NULL;

WNDPROC g_oldEditLoginProc = NULL;
WNDPROC g_oldEditPasswordProc = NULL;

AppLanguage g_currentLanguage = LANG_RU;
AppState g_state = STATE_LOGIN;
bool g_defaultShowPassword = true;
bool g_showPassword = true;
bool g_hasError = false;
StringId g_currentErrorId = STR_ERR_EMPTY;

wchar_t g_expectedLogin[128] = L"steve";
wchar_t g_expectedPassword[128] = L"diamond";
bool g_showHintInUI = true;

int g_currentMobIndex = 0;
bool g_unlockedMobs[32] = { false };
int g_unlockedCount = 0;

HFONT g_hFontTitle = NULL;
HFONT g_hFontSub = NULL;
HFONT g_hFontLabel = NULL;
HFONT g_hFontInput = NULL;
HFONT g_hFontBtn = NULL;
HFONT g_hFontLangBtn = NULL;
HFONT g_hFontHint = NULL;
HFONT g_hFontCardTitle = NULL;
HFONT g_hFontCardDesc = NULL;
HFONT g_hFontEye = NULL;

HBRUSH g_hBrushEditBg = NULL;

Image* g_mobImages[32] = { NULL };
Image* g_superprizeImage = NULL;

// Forward declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK EditSubclassProc(HWND, UINT, WPARAM, LPARAM);
void LoadOrCreateConfig();
void PlaySoundAsset(const unsigned char* data, size_t size);
Image* LoadImageFromMemory(const unsigned char* data, size_t size);
void SwitchState(AppState newState);
void CheckCredentials();
void SetLanguage(AppLanguage lang, bool saveConfig = true);
const wchar_t* GetStr(StringId id);

// Trims leading and trailing whitespace
void TrimString(wchar_t* str) {
    if (!str) return;
    int len = (int)wcslen(str);
    while (len > 0 && (str[len - 1] == L' ' || str[len - 1] == L'\t' || str[len - 1] == L'\r' || str[len - 1] == L'\n')) {
        str[len - 1] = L'\0';
        len--;
    }
    int start = 0;
    while (str[start] == L' ' || str[start] == L'\t') {
        start++;
    }
    if (start > 0) {
        wmemmove(str, str + start, len - start + 1);
    }
}

// Get path to config.ini alongside the exe
void GetConfigPath(wchar_t* outPath, size_t maxLen) {
    GetModuleFileNameW(NULL, outPath, (DWORD)maxLen);
    wchar_t* lastSlash = wcsrchr(outPath, L'\\');
    if (lastSlash) {
        *(lastSlash + 1) = L'\0';
    }
    wcsncat(outPath, L"config.ini", maxLen - wcslen(outPath) - 1);
}

// Localized string retrieval
const wchar_t* GetStr(StringId id) {
    switch (id) {
    case STR_APP_TITLE:
        if (g_currentLanguage == LANG_EN) return L"Login & Password Trainer in Minecraft Style";
        if (g_currentLanguage == LANG_PL) return L"Trening loginu i hasła w stylu Minecraft";
        return L"Тренажёр логина и пароля в стиле Minecraft";
    case STR_HEADER_TITLE:
        return L"MINECRAFT";
    case STR_HEADER_SUB_FMT:
        if (g_currentLanguage == LANG_EN) return L"LOGIN & PASSWORD TRAINER (UNLOCKED %d OF %d)";
        if (g_currentLanguage == LANG_PL) return L"TRENING LOGINU I HASŁA (ODBLOKOWANO %d Z %d)";
        return L"ТРЕНАЖЁР ЛОГИНА И ПАРОЛЯ (ОТКРЫТО %d ИЗ %d)";
    case STR_LBL_LOGIN:
        if (g_currentLanguage == LANG_EN) return L"👤  PLAYER LOGIN:";
        if (g_currentLanguage == LANG_PL) return L"👤  LOGIN GRACZA:";
        return L"👤  ЛОГИН ИГРОКА:";
    case STR_LBL_PASS:
        if (g_currentLanguage == LANG_EN) return L"🔑  PASSWORD:";
        if (g_currentLanguage == LANG_PL) return L"🔑  HASŁO:";
        return L"🔑  ПАРОЛЬ:";
    case STR_BTN_LOGIN:
        if (g_currentLanguage == LANG_EN) return L"▶  LOG IN TO GAME";
        if (g_currentLanguage == LANG_PL) return L"▶  ZALOGUJ DO GRY";
        return L"▶  ВОЙТИ В ИГРУ";
    case STR_MSG_DEFAULT:
        if (g_currentLanguage == LANG_EN) return L"Enter your login and password and click «Log In»";
        if (g_currentLanguage == LANG_PL) return L"Wpisz swój login i hasło oraz kliknij «Zaloguj»";
        return L"Введи свой логин и пароль и нажми «Войти»";
    case STR_ERR_EMPTY:
        if (g_currentLanguage == LANG_EN) return L"❌ Enter your login and password!";
        if (g_currentLanguage == LANG_PL) return L"❌ Wpisz swój login i hasło!";
        return L"❌ Введи свой логин и пароль!";
    case STR_ERR_BOTH:
        if (g_currentLanguage == LANG_EN) return L"❌ Incorrect login and password! Try again.";
        if (g_currentLanguage == LANG_PL) return L"❌ Nieprawidłowy login i hasło! Spróbuj ponownie.";
        return L"❌ Неверный логин и пароль! Попробуй ещё раз.";
    case STR_ERR_LOGIN:
        if (g_currentLanguage == LANG_EN) return L"❌ Login does not match! Check each letter.";
        if (g_currentLanguage == LANG_PL) return L"❌ Login się nie zgadza! Sprawdź każdą literę.";
        return L"❌ Логин не совпадает! Проверь каждую букву.";
    case STR_ERR_PASS:
        if (g_currentLanguage == LANG_EN) return L"❌ Incorrect password! Check letter casing.";
        if (g_currentLanguage == LANG_PL) return L"❌ Nieprawidłowe hasło! Sprawdź wielkość liter.";
        return L"❌ Неверный пароль! Проверь регистр букв.";
    case STR_HINT_TITLE:
        if (g_currentLanguage == LANG_EN) return L"💡 Practice hint:";
        if (g_currentLanguage == LANG_PL) return L"💡 Podpowiedź do ćwiczeń:";
        return L"💡 Подсказка для тренировки:";
    case STR_HINT_FMT:
        if (g_currentLanguage == LANG_EN) return L"💡 Practice hint:\nLogin: «%ls»    Password: «%ls»\n(Configured in config.ini)";
        if (g_currentLanguage == LANG_PL) return L"💡 Podpowiedź do ćwiczeń:\nLogin: «%ls»    Hasło: «%ls»\n(Ustawiane w pliku config.ini)";
        return L"💡 Подсказка для тренировки:\nЛогин: «%ls»    Пароль: «%ls»\n(Логин и пароль настраиваются в файле config.ini)";
    case STR_HINT_CONFIG_ONLY:
        if (g_currentLanguage == LANG_EN) return L"⚙️ Practice login and password are configured in config.ini";
        if (g_currentLanguage == LANG_PL) return L"⚙️ Login i hasło do ćwiczeń można ustawić w pliku config.ini";
        return L"⚙️ Логин и пароль для тренировки настраиваются в файле config.ini";
    case STR_WIN_TITLE:
        if (g_currentLanguage == LANG_EN) return L"🎉  PASSWORD IS CORRECT!";
        if (g_currentLanguage == LANG_PL) return L"🎉  HASŁO POPRAWNE!";
        return L"🎉  ПАРОЛЬ ВЕРНЫЙ!";
    case STR_WIN_SUB:
        if (g_currentLanguage == LANG_EN) return L"Great job! New character unlocked!";
        if (g_currentLanguage == LANG_PL) return L"Świetna robota! Nowa postać odblokowana!";
        return L"Отличная работа! Новый персонаж открыт!";
    case STR_PROGRESS_FMT:
        if (g_currentLanguage == LANG_EN) return L"⭐ Characters unlocked: %d of %d  (%d left)";
        if (g_currentLanguage == LANG_PL) return L"⭐ Odblokowane postacie: %d z %d  (Pozostało: %d)";
        return L"⭐ Открыто персонажей: %d из %d  (Осталось: %d)";
    case STR_BTN_AGAIN:
        if (g_currentLanguage == LANG_EN) return L"🔄  TRAIN AGAIN";
        if (g_currentLanguage == LANG_PL) return L"🔄  ĆWICZ DALEJ";
        return L"🔄  ТРЕНИРОВАТЬСЯ ЕЩЁ";
    case STR_BTN_EXIT:
        if (g_currentLanguage == LANG_EN) return L"🚪  EXIT";
        if (g_currentLanguage == LANG_PL) return L"🚪  WYJDŹ";
        return L"🚪  ВЫЙТИ";
    case STR_SUPER_TITLE:
        if (g_currentLanguage == LANG_EN) return L"🏆  GRAND PRIZE!  🏆";
        if (g_currentLanguage == LANG_PL) return L"🏆  SUPER NAGRODA!  🏆";
        return L"🏆  СУПЕР-ПРИЗ!  🏆";
    case STR_SUPER_SUB:
        if (g_currentLanguage == LANG_EN) return L"🌟 ALL 15 CHARACTERS UNLOCKED! 🌟";
        if (g_currentLanguage == LANG_PL) return L"🌟 CAŁA KOLEKCJA 15 POSTACI ZEBRANA! 🌟";
        return L"🌟 ВСЯ КОЛЛЕКЦИЯ ИЗ 15 ПЕРСОНАЖЕЙ СОБРАНА! 🌟";
    case STR_DRAGON_TITLE:
        if (g_currentLanguage == LANG_EN) return L"🐉 ENDER DRAGON DEFEATED!";
        if (g_currentLanguage == LANG_PL) return L"🐉 SMOK KRESU POKONANY!";
        return L"🐉 ДРАКОН КРАЯ ПОБЕЖДЁН!";
    case STR_DRAGON_BADGE:
        if (g_currentLanguage == LANG_EN) return L"[ LEGENDARY MINECRAFT MASTER ]";
        if (g_currentLanguage == LANG_PL) return L"[ LEGENDARNY MISTRZ MINECRAFT ]";
        return L"[ ЛЕГЕНДАРНЫЙ МАСТЕР MINECRAFT ]";
    case STR_DRAGON_DESC:
        if (g_currentLanguage == LANG_EN)
            return L"HOORAY! YOU ARE A TRUE HERO!\n\n"
                   L"You unlocked all 15 characters and mastered your login and password!\n"
                   L"Your account is completely secure, and your fingers remember every key.";
        if (g_currentLanguage == LANG_PL)
            return L"HURA! JESTEŚ PRAWDZIWYM BOHATEREM!\n\n"
                   L"Odblokowałeś wszystkie 15 postaci i bezbłędnie opanowałeś swój login i hasło!\n"
                   L"Twoje konto jest w pełni bezpieczne, a palce pamiętają każdy klawisz.";
        return L"УРА! ТЫ НАСТОЯЩИЙ ГЕРОЙ!\n\n"
               L"Ты открыл всех персонажей и безошибочно выучил свой логин и пароль!\n"
               L"Твоя учётная запись надёжно защищена, а пальцы помнят каждую букву.";
    case STR_TROPHY_TEXT:
        if (g_currentLanguage == LANG_EN) return L"🥇 YOU ARE AWARDED THE GOLDEN MINECRAFT TROPHY! 🥇";
        if (g_currentLanguage == LANG_PL) return L"🥇 OTRZYMUJESZ ZŁOTY PUCHAR ZNAWCY MINECRAFTA! 🥇";
        return L"🥇 ТЕБЕ ВРУЧАЕТСЯ ЗОЛОТОЙ КУБОК ЗНАТОКА MINECRAFT! 🥇";
    case STR_BTN_RESET:
        if (g_currentLanguage == LANG_EN) return L"🏆  PLAY AGAIN (RESET)";
        if (g_currentLanguage == LANG_PL) return L"🏆  ZACZNIJ OD NOWA (RESET)";
        return L"🏆  НАЧАТЬ ЗАНОВО (СБРОС)";
    }
    return L"";
}

// Mob localization helpers
const wchar_t* GetMobName(const MobInfo& mob) {
    if (g_currentLanguage == LANG_EN) return mob.name_en;
    if (g_currentLanguage == LANG_PL) return mob.name_pl;
    return mob.name_ru;
}
const wchar_t* GetMobType(const MobInfo& mob) {
    if (g_currentLanguage == LANG_EN) return mob.type_en;
    if (g_currentLanguage == LANG_PL) return mob.type_pl;
    return mob.type_ru;
}
const wchar_t* GetMobDesc(const MobInfo& mob) {
    if (g_currentLanguage == LANG_EN) return mob.desc_en;
    if (g_currentLanguage == LANG_PL) return mob.desc_pl;
    return mob.desc_ru;
}

void SetLanguage(AppLanguage lang, bool saveConfig) {
    g_currentLanguage = lang;
    if (saveConfig) {
        wchar_t configPath[MAX_PATH];
        GetConfigPath(configPath, MAX_PATH);
        const wchar_t* langCode = (lang == LANG_EN) ? L"en" : ((lang == LANG_PL) ? L"pl" : L"ru");
        WritePrivateProfileStringW(L"Options", L"language", langCode, configPath);
    }

    if (g_hWndMain) {
        SetWindowTextW(g_hWndMain, GetStr(STR_APP_TITLE));
        InvalidateRect(g_hWndMain, NULL, TRUE);
    }
    if (g_hBtnLogin) InvalidateRect(g_hBtnLogin, NULL, TRUE);
    if (g_hBtnAgain) InvalidateRect(g_hBtnAgain, NULL, TRUE);
    if (g_hBtnExit) InvalidateRect(g_hBtnExit, NULL, TRUE);
    if (g_hBtnLangRU) InvalidateRect(g_hBtnLangRU, NULL, TRUE);
    if (g_hBtnLangEN) InvalidateRect(g_hBtnLangEN, NULL, TRUE);
    if (g_hBtnLangPL) InvalidateRect(g_hBtnLangPL, NULL, TRUE);
}

void LoadOrCreateConfig() {
    wchar_t configPath[MAX_PATH];
    GetConfigPath(configPath, MAX_PATH);

    DWORD attr = GetFileAttributesW(configPath);
    if (attr == INVALID_FILE_ATTRIBUTES) {
        // File does not exist, create it
        FILE* f = _wfopen(configPath, L"w, ccs=UTF-8");
        if (f) {
            fwprintf(f, L"[Credentials]\n");
            fwprintf(f, L"; Задайте здесь правильные логин и пароль для тренировки ребенка:\n");
            fwprintf(f, L"login = steve\n");
            fwprintf(f, L"password = diamond\n\n");
            fwprintf(f, L"[Options]\n");
            fwprintf(f, L"; Язык приложения (ru / en / pl):\n");
            fwprintf(f, L"language = ru\n");
            fwprintf(f, L"; 1 = показывать пароль открытым текстом по умолчанию, 0 = скрывать точками\n");
            fwprintf(f, L"show_password = 1\n");
            fwprintf(f, L"; 1 = показывать подсказку внизу окна, 0 = скрыть\n");
            fwprintf(f, L"show_hint = 1\n");
            fclose(f);
        }
        wcscpy(g_expectedLogin, L"steve");
        wcscpy(g_expectedPassword, L"diamond");
        g_showHintInUI = true;
        g_currentLanguage = LANG_RU;
    } else {
        GetPrivateProfileStringW(L"Credentials", L"login", L"steve", g_expectedLogin, 128, configPath);
        GetPrivateProfileStringW(L"Credentials", L"password", L"diamond", g_expectedPassword, 128, configPath);
        TrimString(g_expectedLogin);
        TrimString(g_expectedPassword);

        wchar_t langBuf[32] = L"ru";
        GetPrivateProfileStringW(L"Options", L"language", L"ru", langBuf, 32, configPath);
        TrimString(langBuf);
        if (_wcsicmp(langBuf, L"en") == 0) {
            g_currentLanguage = LANG_EN;
        } else if (_wcsicmp(langBuf, L"pl") == 0) {
            g_currentLanguage = LANG_PL;
        } else {
            g_currentLanguage = LANG_RU;
        }

        int showPassVal = GetPrivateProfileIntW(L"Options", L"show_password", 1, configPath);
        g_defaultShowPassword = (showPassVal != 0);
        g_showPassword = g_defaultShowPassword;

        int hintVal = GetPrivateProfileIntW(L"Options", L"show_hint", 1, configPath);
        g_showHintInUI = (hintVal != 0);
    }
}

void PlaySoundAsset(const unsigned char* data, size_t size) {
    if (data && size > 0) {
        PlaySoundW((LPCWSTR)data, NULL, SND_MEMORY | SND_ASYNC);
    }
}

Image* LoadImageFromMemory(const unsigned char* data, size_t size) {
    if (!data || size == 0) return NULL;
    HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, size);
    if (!hGlobal) return NULL;
    void* pBuf = GlobalLock(hGlobal);
    if (!pBuf) {
        GlobalFree(hGlobal);
        return NULL;
    }
    memcpy(pBuf, data, size);
    GlobalUnlock(hGlobal);

    IStream* pStream = NULL;
    if (CreateStreamOnHGlobal(hGlobal, TRUE, &pStream) == S_OK) {
        Image* img = Image::FromStream(pStream);
        pStream->Release();
        return img;
    }
    GlobalFree(hGlobal);
    return NULL;
}

// Subclass edit controls to handle Enter key navigation
LRESULT CALLBACK EditSubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (uMsg == WM_KEYDOWN && wParam == VK_RETURN) {
        if (hWnd == g_hEditLogin) {
            SetFocus(g_hEditPassword);
            return 0;
        } else if (hWnd == g_hEditPassword) {
            CheckCredentials();
            return 0;
        }
    }
    WNDPROC oldProc = (hWnd == g_hEditLogin) ? g_oldEditLoginProc : g_oldEditPasswordProc;
    return CallWindowProcW(oldProc, hWnd, uMsg, wParam, lParam);
}

void SwitchState(AppState newState) {
    g_state = newState;
    if (g_state == STATE_LOGIN) {
        ShowWindow(g_hEditLogin, SW_SHOW);
        ShowWindow(g_hEditPassword, SW_SHOW);
        ShowWindow(g_hBtnLogin, SW_SHOW);
        ShowWindow(g_hBtnEye, SW_SHOW);

        ShowWindow(g_hBtnAgain, SW_HIDE);
        ShowWindow(g_hBtnExit, SW_HIDE);

        // Reset password visibility to default setting
        g_showPassword = g_defaultShowPassword;
        SendMessageW(g_hEditPassword, EM_SETPASSWORDCHAR, g_showPassword ? 0 : 0x25CF, 0);
        InvalidateRect(g_hBtnEye, NULL, TRUE);

        // Clear both login and password for a fresh practice session
        SetWindowTextW(g_hEditLogin, L"");
        SetWindowTextW(g_hEditPassword, L"");
        SetFocus(g_hEditLogin);
    } else {
        ShowWindow(g_hEditLogin, SW_HIDE);
        ShowWindow(g_hEditPassword, SW_HIDE);
        ShowWindow(g_hBtnLogin, SW_HIDE);
        ShowWindow(g_hBtnEye, SW_HIDE);

        ShowWindow(g_hBtnAgain, SW_SHOW);
        ShowWindow(g_hBtnExit, SW_SHOW);

        SetFocus(g_hBtnAgain);
    }
    InvalidateRect(g_hWndMain, NULL, TRUE);
}

void CheckCredentials() {
    wchar_t enteredLogin[128] = L"";
    wchar_t enteredPassword[128] = L"";
    GetWindowTextW(g_hEditLogin, enteredLogin, 128);
    GetWindowTextW(g_hEditPassword, enteredPassword, 128);

    TrimString(enteredLogin);
    TrimString(enteredPassword);

    // Reload config dynamically in case parent edited it in notepad while app was running
    LoadOrCreateConfig();

    bool loginMatch = (_wcsicmp(enteredLogin, g_expectedLogin) == 0);
    bool passMatch = (wcscmp(enteredPassword, g_expectedPassword) == 0);

    if (loginMatch && passMatch) {
        // Collect list of remaining locked mobs to guarantee progress!
        int lockedIndices[32];
        int lockedCount = 0;
        for (int i = 0; i < g_mobs_count; i++) {
            if (!g_unlockedMobs[i]) {
                lockedIndices[lockedCount++] = i;
            }
        }

        g_hasError = false;

        if (lockedCount > 0) {
            // Pick a random locked mob
            int pick = lockedIndices[rand() % lockedCount];
            g_unlockedMobs[pick] = true;
            g_unlockedCount++;
            g_currentMobIndex = pick;

            if (g_unlockedCount == g_mobs_count) {
                // ALL 15 MOBS UNLOCKED! TRIGGER GRAND SUPERPRIZE!
                PlaySoundAsset(g_sound_fanfare_data, g_sound_fanfare_size);
                SwitchState(STATE_SUPERPRIZE);
            } else {
                // Normal mob unlocked
                PlaySoundAsset(g_sound_success_data, g_sound_success_size);
                SwitchState(STATE_SUCCESS);
            }
        } else {
            // All already unlocked, show superprize celebration!
            PlaySoundAsset(g_sound_fanfare_data, g_sound_fanfare_size);
            SwitchState(STATE_SUPERPRIZE);
        }
    } else {
        // Error
        g_hasError = true;
        if (wcslen(enteredLogin) == 0 && wcslen(enteredPassword) == 0) {
            g_currentErrorId = STR_ERR_EMPTY;
            SetFocus(g_hEditLogin);
        } else if (!loginMatch && !passMatch) {
            g_currentErrorId = STR_ERR_BOTH;
            SetFocus(g_hEditPassword);
        } else if (!loginMatch) {
            g_currentErrorId = STR_ERR_LOGIN;
            SetFocus(g_hEditLogin);
        } else {
            g_currentErrorId = STR_ERR_PASS;
            SetFocus(g_hEditPassword);
        }

        PlaySoundAsset(g_sound_error_data, g_sound_error_size);
        SendMessageW(g_hEditPassword, EM_SETSEL, 0, -1);
        InvalidateRect(g_hWndMain, NULL, TRUE);
    }
}

// Draw a Minecraft styled 3D beveled button
void DrawMinecraftButton(HDC hdc, RECT rect, const wchar_t* text, bool isHover, bool isPressed, bool isGreen, bool isGold = false, bool isSmall = false) {
    COLORREF colBg;
    COLORREF colBorderLight;
    COLORREF colBorderDark;

    if (isGold) {
        colBg = isPressed ? RGB(180, 130, 20) : (isHover ? RGB(230, 180, 30) : RGB(212, 160, 23));
        colBorderLight = RGB(255, 230, 100);
        colBorderDark  = RGB(120, 85, 10);
    } else if (isGreen) {
        colBg = isPressed ? RGB(34, 110, 39) : (isHover ? RGB(56, 170, 64) : RGB(46, 139, 52));
        colBorderLight = RGB(90, 210, 100);
        colBorderDark  = RGB(18, 65, 22);
    } else {
        colBg = isPressed ? RGB(75, 78, 85)  : (isHover ? RGB(115, 118, 128) : RGB(92, 95, 105));
        colBorderLight = RGB(160, 165, 175);
        colBorderDark  = RGB(35, 36, 40);
    }

    HBRUSH hBr = CreateSolidBrush(colBg);
    FillRect(hdc, &rect, hBr);
    DeleteObject(hBr);

    int borderW = isSmall ? 2 : 3;
    HPEN hPenLight = CreatePen(PS_SOLID, borderW, isPressed ? colBorderDark : colBorderLight);
    HPEN hPenDark  = CreatePen(PS_SOLID, borderW, isPressed ? colBorderLight : colBorderDark);

    HPEN hOld = (HPEN)SelectObject(hdc, hPenLight);
    MoveToEx(hdc, rect.left, rect.bottom - borderW, NULL);
    LineTo(hdc, rect.left, rect.top);
    LineTo(hdc, rect.right - borderW, rect.top);

    SelectObject(hdc, hPenDark);
    MoveToEx(hdc, rect.right - borderW, rect.top, NULL);
    LineTo(hdc, rect.right - borderW, rect.bottom - borderW);
    LineTo(hdc, rect.left, rect.bottom - borderW);

    SelectObject(hdc, hOld);
    DeleteObject(hPenLight);
    DeleteObject(hPenDark);

    // Text with subtle 3D shadow
    SetBkMode(hdc, TRANSPARENT);
    RECT textRect = rect;
    if (isPressed) {
        textRect.top += 1;
        textRect.left += 1;
    }
    RECT shadowRect = textRect;
    OffsetRect(&shadowRect, 1, 1);
    SetTextColor(hdc, RGB(20, 20, 20));
    DrawTextW(hdc, text, -1, &shadowRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    SetTextColor(hdc, isGold ? RGB(20, 20, 20) : RGB(255, 255, 255));
    DrawTextW(hdc, text, -1, &textRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

// Window procedure
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE: {
        g_hWndMain = hWnd;
        srand((unsigned int)time(NULL));

        // Create enlarged, prominent fonts
        g_hFontTitle = CreateFontW(44, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
        g_hFontSub = CreateFontW(21, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
        g_hFontLabel = CreateFontW(22, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
        g_hFontInput = CreateFontW(26, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
        g_hFontBtn = CreateFontW(25, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
        g_hFontLangBtn = CreateFontW(14, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
        g_hFontHint = CreateFontW(17, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
        g_hFontCardTitle = CreateFontW(34, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
        g_hFontCardDesc = CreateFontW(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
        g_hFontEye = CreateFontW(26, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI Symbol");

        g_hBrushEditBg = CreateSolidBrush(RGB(24, 26, 30));

        // Language switcher buttons in top-right
        int langBtnW = 46;
        int langBtnH = 28;
        int langY = 12;
        int langStartX = 660 - (langBtnW * 3 + 12) - 20;

        g_hBtnLangRU = CreateWindowExW(0, L"BUTTON", L"RU",
            WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_OWNERDRAW,
            langStartX, langY, langBtnW, langBtnH, hWnd, (HMENU)IDC_BTN_LANG_RU, g_hInstance, NULL);
        SendMessageW(g_hBtnLangRU, WM_SETFONT, (WPARAM)g_hFontLangBtn, TRUE);

        g_hBtnLangEN = CreateWindowExW(0, L"BUTTON", L"EN",
            WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_OWNERDRAW,
            langStartX + langBtnW + 4, langY, langBtnW, langBtnH, hWnd, (HMENU)IDC_BTN_LANG_EN, g_hInstance, NULL);
        SendMessageW(g_hBtnLangEN, WM_SETFONT, (WPARAM)g_hFontLangBtn, TRUE);

        g_hBtnLangPL = CreateWindowExW(0, L"BUTTON", L"PL",
            WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_OWNERDRAW,
            langStartX + (langBtnW + 4) * 2, langY, langBtnW, langBtnH, hWnd, (HMENU)IDC_BTN_LANG_PL, g_hInstance, NULL);
        SendMessageW(g_hBtnLangPL, WM_SETFONT, (WPARAM)g_hFontLangBtn, TRUE);

        int startX = 60;
        int fieldW = 540;

        // Login input
        g_hEditLogin = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"",
            WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL,
            startX, 185, fieldW, 50, hWnd, (HMENU)IDC_EDIT_LOGIN, g_hInstance, NULL);
        SendMessageW(g_hEditLogin, WM_SETFONT, (WPARAM)g_hFontInput, TRUE);
        SendMessageW(g_hEditLogin, EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN, MAKELPARAM(10, 10));

        // Password input + Eye button
        int eyeBtnW = 60;
        int passW = fieldW - eyeBtnW - 8;
        g_hEditPassword = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"",
            WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_PASSWORD | ES_AUTOHSCROLL,
            startX, 285, passW, 50, hWnd, (HMENU)IDC_EDIT_PASSWORD, g_hInstance, NULL);
        SendMessageW(g_hEditPassword, WM_SETFONT, (WPARAM)g_hFontInput, TRUE);
        SendMessageW(g_hEditPassword, EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN, MAKELPARAM(10, 10));

        g_hBtnEye = CreateWindowExW(0, L"BUTTON", L"👁",
            WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_OWNERDRAW,
            startX + passW + 8, 285, eyeBtnW, 50, hWnd, (HMENU)IDC_BTN_EYE, g_hInstance, NULL);
        SendMessageW(g_hBtnEye, WM_SETFONT, (WPARAM)g_hFontEye, TRUE);

        // "Войти" button
        g_hBtnLogin = CreateWindowExW(0, L"BUTTON", L"▶ ВОЙТИ",
            WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_OWNERDRAW,
            startX, 410, fieldW, 64, hWnd, (HMENU)IDC_BTN_LOGIN, g_hInstance, NULL);
        SendMessageW(g_hBtnLogin, WM_SETFONT, (WPARAM)g_hFontBtn, TRUE);

        // Action buttons
        g_hBtnAgain = CreateWindowExW(0, L"BUTTON", L"🔄 ТРЕНИРОВАТЬСЯ ЕЩЁ",
            WS_CHILD | WS_TABSTOP | BS_OWNERDRAW,
            40, 660, 420, 64, hWnd, (HMENU)IDC_BTN_AGAIN, g_hInstance, NULL);
        SendMessageW(g_hBtnAgain, WM_SETFONT, (WPARAM)g_hFontBtn, TRUE);

        g_hBtnExit = CreateWindowExW(0, L"BUTTON", L"🚪 ВЫЙТИ",
            WS_CHILD | WS_TABSTOP | BS_OWNERDRAW,
            475, 660, 145, 64, hWnd, (HMENU)IDC_BTN_EXIT, g_hInstance, NULL);
        SendMessageW(g_hBtnExit, WM_SETFONT, (WPARAM)g_hFontBtn, TRUE);

        // Subclass edits
        g_oldEditLoginProc = (WNDPROC)SetWindowLongPtrW(g_hEditLogin, GWLP_WNDPROC, (LONG_PTR)EditSubclassProc);
        g_oldEditPasswordProc = (WNDPROC)SetWindowLongPtrW(g_hEditPassword, GWLP_WNDPROC, (LONG_PTR)EditSubclassProc);

        LoadOrCreateConfig();
        SendMessageW(g_hEditPassword, EM_SETPASSWORDCHAR, g_showPassword ? 0 : 0x25CF, 0);
        SetWindowTextW(g_hEditLogin, L"");
        SetWindowTextW(g_hEditPassword, L"");
        SetFocus(g_hEditLogin);

        // Load mob images
        for (int i = 0; i < g_mobs_count; i++) {
            g_mobImages[i] = LoadImageFromMemory(g_mobs[i].png_data, g_mobs[i].png_size);
        }
        g_superprizeImage = LoadImageFromMemory(g_superprize_png, g_superprize_size);

        return 0;
    }

    case WM_CTLCOLOREDIT: {
        HDC hdcEdit = (HDC)wParam;
        SetTextColor(hdcEdit, RGB(250, 250, 250));
        SetBkColor(hdcEdit, RGB(24, 26, 30));
        return (INT_PTR)g_hBrushEditBg;
    }

    case WM_DRAWITEM: {
        LPDRAWITEMSTRUCT pDIS = (LPDRAWITEMSTRUCT)lParam;
        bool isPressed = (pDIS->itemState & ODS_SELECTED);
        bool isFocus = (pDIS->itemState & ODS_FOCUS);

        if (pDIS->CtlID == IDC_BTN_LOGIN) {
            SelectObject(pDIS->hDC, g_hFontBtn);
            DrawMinecraftButton(pDIS->hDC, pDIS->rcItem, GetStr(STR_BTN_LOGIN), isFocus, isPressed, true);
            return TRUE;
        } else if (pDIS->CtlID == IDC_BTN_EYE) {
            SelectObject(pDIS->hDC, g_hFontEye);
            DrawMinecraftButton(pDIS->hDC, pDIS->rcItem, g_showPassword ? L"🔒" : L"👁", isFocus, isPressed, false);
            return TRUE;
        } else if (pDIS->CtlID == IDC_BTN_AGAIN) {
            SelectObject(pDIS->hDC, g_hFontBtn);
            if (g_state == STATE_SUPERPRIZE) {
                DrawMinecraftButton(pDIS->hDC, pDIS->rcItem, GetStr(STR_BTN_RESET), isFocus, isPressed, false, true);
            } else {
                DrawMinecraftButton(pDIS->hDC, pDIS->rcItem, GetStr(STR_BTN_AGAIN), isFocus, isPressed, true);
            }
            return TRUE;
        } else if (pDIS->CtlID == IDC_BTN_EXIT) {
            SelectObject(pDIS->hDC, g_hFontBtn);
            DrawMinecraftButton(pDIS->hDC, pDIS->rcItem, GetStr(STR_BTN_EXIT), isFocus, isPressed, false);
            return TRUE;
        } else if (pDIS->CtlID == IDC_BTN_LANG_RU) {
            SelectObject(pDIS->hDC, g_hFontLangBtn);
            bool isActive = (g_currentLanguage == LANG_RU);
            DrawMinecraftButton(pDIS->hDC, pDIS->rcItem, L"RU", isFocus, isPressed, isActive, false, true);
            return TRUE;
        } else if (pDIS->CtlID == IDC_BTN_LANG_EN) {
            SelectObject(pDIS->hDC, g_hFontLangBtn);
            bool isActive = (g_currentLanguage == LANG_EN);
            DrawMinecraftButton(pDIS->hDC, pDIS->rcItem, L"EN", isFocus, isPressed, isActive, false, true);
            return TRUE;
        } else if (pDIS->CtlID == IDC_BTN_LANG_PL) {
            SelectObject(pDIS->hDC, g_hFontLangBtn);
            bool isActive = (g_currentLanguage == LANG_PL);
            DrawMinecraftButton(pDIS->hDC, pDIS->rcItem, L"PL", isFocus, isPressed, isActive, false, true);
            return TRUE;
        }
        break;
    }

    case WM_COMMAND: {
        int wmId = LOWORD(wParam);
        if (wmId == IDC_BTN_LOGIN) {
            CheckCredentials();
        } else if (wmId == IDC_BTN_EYE) {
            g_showPassword = !g_showPassword;
            SendMessageW(g_hEditPassword, EM_SETPASSWORDCHAR, g_showPassword ? 0 : 0x25CF, 0);
            InvalidateRect(g_hEditPassword, NULL, TRUE);
            InvalidateRect(g_hBtnEye, NULL, TRUE);
        } else if (wmId == IDC_BTN_AGAIN) {
            if (g_state == STATE_SUPERPRIZE) {
                // Reset collection
                for (int i = 0; i < 32; i++) g_unlockedMobs[i] = false;
                g_unlockedCount = 0;
            }
            SwitchState(STATE_LOGIN);
        } else if (wmId == IDC_BTN_EXIT) {
            PostQuitMessage(0);
        } else if (wmId == IDC_BTN_LANG_RU) {
            SetLanguage(LANG_RU);
        } else if (wmId == IDC_BTN_LANG_EN) {
            SetLanguage(LANG_EN);
        } else if (wmId == IDC_BTN_LANG_PL) {
            SetLanguage(LANG_PL);
        }
        return 0;
    }

    case WM_KEYDOWN: {
        if (wParam == VK_ESCAPE) {
            if (g_state != STATE_LOGIN) {
                SwitchState(STATE_LOGIN);
                return 0;
            }
        } else if (wParam == VK_RETURN || wParam == VK_SPACE) {
            if (g_state != STATE_LOGIN) {
                if (g_state == STATE_SUPERPRIZE) {
                    for (int i = 0; i < 32; i++) g_unlockedMobs[i] = false;
                    g_unlockedCount = 0;
                }
                SwitchState(STATE_LOGIN);
                return 0;
            }
        }
        break;
    }

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        RECT clientRect;
        GetClientRect(hWnd, &clientRect);
        int width = clientRect.right - clientRect.left;
        int height = clientRect.bottom - clientRect.top;

        HDC memDC = CreateCompatibleDC(hdc);
        HBITMAP memBitmap = CreateCompatibleBitmap(hdc, width, height);
        HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);

        Graphics g(memDC);
        g.SetSmoothingMode(SmoothingModeAntiAlias);
        g.SetInterpolationMode(InterpolationModeHighQualityBicubic);
        g.SetTextRenderingHint(TextRenderingHintClearTypeGridFit);

        SolidBrush bgBrush(Color(255, 20, 21, 24));
        g.FillRectangle(&bgBrush, 0, 0, width, height);

        LinearGradientBrush headerGrad(
            Point(0, 0), Point(0, 120),
            (g_state == STATE_SUPERPRIZE) ? Color(255, 60, 45, 15) : Color(255, 33, 43, 35),
            Color(255, 20, 21, 24)
        );
        g.FillRectangle(&headerGrad, 0, 0, width, 120);

        Pen borderPen(Color(255, 45, 48, 55), 2.0f);
        g.DrawRectangle(&borderPen, 1, 1, width - 2, height - 2);

        SetBkMode(memDC, TRANSPARENT);

        if (g_state == STATE_LOGIN) {
            // === STATE LOGIN ===
            SelectObject(memDC, g_hFontTitle);
            RECT titleRect = { 0, 16, width, 68 };
            RECT titleShadow = { 3, 19, width + 3, 71 };
            SetTextColor(memDC, RGB(20, 30, 20));
            DrawTextW(memDC, GetStr(STR_HEADER_TITLE), -1, &titleShadow, DT_CENTER | DT_TOP | DT_SINGLELINE);
            SetTextColor(memDC, RGB(85, 255, 85));
            DrawTextW(memDC, GetStr(STR_HEADER_TITLE), -1, &titleRect, DT_CENTER | DT_TOP | DT_SINGLELINE);

            SelectObject(memDC, g_hFontSub);
            RECT subRect = { 0, 68, width, 100 };
            SetTextColor(memDC, RGB(100, 220, 255));
            wchar_t subBuf[128];
            swprintf(subBuf, 128, GetStr(STR_HEADER_SUB_FMT), g_unlockedCount, g_mobs_count);
            DrawTextW(memDC, subBuf, -1, &subRect, DT_CENTER | DT_TOP | DT_SINGLELINE);

            SolidBrush cardBg(Color(255, 27, 29, 34));
            Pen cardBorder(Color(255, 52, 56, 66), 2.0f);
            g.FillRectangle(&cardBg, 40, 115, width - 80, 500);
            g.DrawRectangle(&cardBorder, 40, 115, width - 80, 500);

            SelectObject(memDC, g_hFontLabel);
            SetTextColor(memDC, RGB(235, 240, 245));

            RECT lblLogin = { 60, 150, width - 60, 180 };
            DrawTextW(memDC, GetStr(STR_LBL_LOGIN), -1, &lblLogin, DT_LEFT | DT_TOP | DT_SINGLELINE);

            RECT lblPass = { 60, 250, width - 60, 280 };
            DrawTextW(memDC, GetStr(STR_LBL_PASS), -1, &lblPass, DT_LEFT | DT_TOP | DT_SINGLELINE);

            SelectObject(memDC, g_hFontSub);
            RECT msgRect = { 50, 350, width - 50, 398 };
            if (g_hasError) {
                SetTextColor(memDC, RGB(255, 85, 85));
                DrawTextW(memDC, GetStr(g_currentErrorId), -1, &msgRect, DT_CENTER | DT_TOP | DT_WORDBREAK);
            } else {
                SetTextColor(memDC, RGB(170, 180, 195));
                DrawTextW(memDC, GetStr(STR_MSG_DEFAULT), -1, &msgRect, DT_CENTER | DT_TOP | DT_SINGLELINE);
            }

            if (g_showHintInUI) {
                SelectObject(memDC, g_hFontHint);
                RECT hintRect = { 50, 495, width - 50, 600 };
                SetTextColor(memDC, RGB(140, 150, 168));
                wchar_t hintBuf[256];
                swprintf(hintBuf, 256, GetStr(STR_HINT_FMT), g_expectedLogin, g_expectedPassword);
                DrawTextW(memDC, hintBuf, -1, &hintRect, DT_CENTER | DT_TOP | DT_WORDBREAK);
            } else {
                SelectObject(memDC, g_hFontHint);
                RECT hintRect = { 50, 530, width - 50, 590 };
                SetTextColor(memDC, RGB(120, 130, 145));
                DrawTextW(memDC, GetStr(STR_HINT_CONFIG_ONLY), -1, &hintRect, DT_CENTER | DT_TOP | DT_SINGLELINE);
            }
        } else if (g_state == STATE_SUCCESS) {
            // === STATE SUCCESS (MOB UNLOCKED) ===
            const MobInfo& mob = g_mobs[g_currentMobIndex];

            SelectObject(memDC, g_hFontTitle);
            RECT vicRect = { 0, 12, width, 58 };
            RECT vicShadow = { 3, 15, width + 3, 61 };
            SetTextColor(memDC, RGB(30, 40, 20));
            DrawTextW(memDC, GetStr(STR_WIN_TITLE), -1, &vicShadow, DT_CENTER | DT_TOP | DT_SINGLELINE);
            SetTextColor(memDC, RGB(255, 215, 0));
            DrawTextW(memDC, GetStr(STR_WIN_TITLE), -1, &vicRect, DT_CENTER | DT_TOP | DT_SINGLELINE);

            SelectObject(memDC, g_hFontSub);
            RECT vicSub = { 0, 58, width, 88 };
            SetTextColor(memDC, RGB(85, 255, 85));
            DrawTextW(memDC, GetStr(STR_WIN_SUB), -1, &vicSub, DT_CENTER | DT_TOP | DT_SINGLELINE);

            SolidBrush mobCardBg(Color(255, 27, 29, 34));
            Pen mobCardBorder(Color(255, 60, 65, 78), 2.0f);
            int cardX = 40, cardY = 95, cardW = width - 80, cardH = 545;
            g.FillRectangle(&mobCardBg, cardX, cardY, cardW, cardH);
            g.DrawRectangle(&mobCardBorder, cardX, cardY, cardW, cardH);

            Image* pImg = g_mobImages[g_currentMobIndex];
            if (pImg) {
                int imgW = (int)pImg->GetWidth();
                int imgH = (int)pImg->GetHeight();
                int maxBox = 230;
                int maxDim = (imgW > imgH) ? imgW : imgH;
                float scale = 1.0f;
                if (maxDim > maxBox) {
                    scale = (float)maxBox / (float)maxDim;
                }
                int drawW = (int)(imgW * scale);
                int drawH = (int)(imgH * scale);
                int drawX = cardX + (cardW - drawW) / 2;
                int drawY = cardY + 12 + (maxBox - drawH) / 2;

                SolidBrush shadowBr(Color(70, 0, 0, 0));
                g.FillEllipse(&shadowBr, drawX + 15, drawY + drawH - 12, drawW - 30, 24);

                g.DrawImage(pImg, drawX, drawY, drawW, drawH);
            }

            SelectObject(memDC, g_hFontCardTitle);
            RECT nameRect = { cardX + 15, cardY + 255, cardX + cardW - 15, cardY + 295 };
            SetTextColor(memDC, RGB(255, 255, 255));
            wchar_t fullName[128];
            if (g_currentLanguage == LANG_EN) {
                swprintf(fullName, 128, L"%ls", mob.name_en);
            } else if (g_currentLanguage == LANG_PL) {
                swprintf(fullName, 128, L"%ls (%ls)", mob.name_pl, mob.name_en);
            } else {
                swprintf(fullName, 128, L"%ls (%ls)", mob.name_ru, mob.name_en);
            }
            DrawTextW(memDC, fullName, -1, &nameRect, DT_CENTER | DT_TOP | DT_SINGLELINE);

            SelectObject(memDC, g_hFontSub);
            RECT typeRect = { cardX + 15, cardY + 298, cardX + cardW - 15, cardY + 328 };
            SetTextColor(memDC, RGB(100, 220, 255));
            wchar_t typeBadge[128];
            swprintf(typeBadge, 128, L"[ %ls ]", GetMobType(mob));
            DrawTextW(memDC, typeBadge, -1, &typeRect, DT_CENTER | DT_TOP | DT_SINGLELINE);

            SelectObject(memDC, g_hFontCardDesc);
            RECT descRect = { cardX + 25, cardY + 338, cardX + cardW - 25, cardY + 475 };
            SetTextColor(memDC, RGB(220, 225, 235));
            DrawTextW(memDC, GetMobDesc(mob), -1, &descRect, DT_CENTER | DT_TOP | DT_WORDBREAK);

            SelectObject(memDC, g_hFontHint);
            RECT progRect = { cardX + 15, cardY + 495, cardX + cardW - 15, cardY + 530 };
            SetTextColor(memDC, RGB(255, 215, 0));
            wchar_t progBuf[128];
            swprintf(progBuf, 128, GetStr(STR_PROGRESS_FMT), g_unlockedCount, g_mobs_count, g_mobs_count - g_unlockedCount);
            DrawTextW(memDC, progBuf, -1, &progRect, DT_CENTER | DT_TOP | DT_SINGLELINE);
        } else if (g_state == STATE_SUPERPRIZE) {
            // === STATE SUPERPRIZE (ALL 15 UNLOCKED!) ===
            SelectObject(memDC, g_hFontTitle);
            RECT vicRect = { 0, 10, width, 58 };
            RECT vicShadow = { 3, 13, width + 3, 61 };
            SetTextColor(memDC, RGB(50, 35, 10));
            DrawTextW(memDC, GetStr(STR_SUPER_TITLE), -1, &vicShadow, DT_CENTER | DT_TOP | DT_SINGLELINE);
            SetTextColor(memDC, RGB(255, 215, 0)); // Pure gold
            DrawTextW(memDC, GetStr(STR_SUPER_TITLE), -1, &vicRect, DT_CENTER | DT_TOP | DT_SINGLELINE);

            SelectObject(memDC, g_hFontSub);
            RECT vicSub = { 0, 58, width, 88 };
            SetTextColor(memDC, RGB(85, 255, 85));
            DrawTextW(memDC, GetStr(STR_SUPER_SUB), -1, &vicSub, DT_CENTER | DT_TOP | DT_SINGLELINE);

            // Card container for Superprize
            SolidBrush superCardBg(Color(255, 30, 28, 38));
            Pen superCardBorder(Color(255, 212, 160, 23), 2.5f); // Golden border
            int cardX = 40, cardY = 95, cardW = width - 80, cardH = 545;
            g.FillRectangle(&superCardBg, cardX, cardY, cardW, cardH);
            g.DrawRectangle(&superCardBorder, cardX, cardY, cardW, cardH);

            // Draw Ender Dragon Image
            if (g_superprizeImage) {
                int imgW = (int)g_superprizeImage->GetWidth();
                int imgH = (int)g_superprizeImage->GetHeight();
                int maxBoxW = 420;
                int maxBoxH = 210;
                float scaleX = (float)maxBoxW / (float)imgW;
                float scaleY = (float)maxBoxH / (float)imgH;
                float scale = (scaleX < scaleY) ? scaleX : scaleY;
                int drawW = (int)(imgW * scale);
                int drawH = (int)(imgH * scale);
                int drawX = cardX + (cardW - drawW) / 2;
                int drawY = cardY + 15 + (maxBoxH - drawH) / 2;

                // Purple Ender glow shadow
                SolidBrush shadowBr(Color(90, 70, 0, 90));
                g.FillEllipse(&shadowBr, drawX + 20, drawY + drawH - 10, drawW - 40, 26);

                g.DrawImage(g_superprizeImage, drawX, drawY, drawW, drawH);
            }

            // Dragon Title
            SelectObject(memDC, g_hFontCardTitle);
            RECT nameRect = { cardX + 15, cardY + 235, cardX + cardW - 15, cardY + 275 };
            SetTextColor(memDC, RGB(255, 128, 255)); // Ender magenta
            DrawTextW(memDC, GetStr(STR_DRAGON_TITLE), -1, &nameRect, DT_CENTER | DT_TOP | DT_SINGLELINE);

            // Achievement badge
            SelectObject(memDC, g_hFontSub);
            RECT typeRect = { cardX + 15, cardY + 278, cardX + cardW - 15, cardY + 308 };
            SetTextColor(memDC, RGB(255, 215, 0)); // Gold
            DrawTextW(memDC, GetStr(STR_DRAGON_BADGE), -1, &typeRect, DT_CENTER | DT_TOP | DT_SINGLELINE);

            // Congratulation lore
            SelectObject(memDC, g_hFontCardDesc);
            RECT descRect = { cardX + 25, cardY + 318, cardX + cardW - 25, cardY + 445 };
            SetTextColor(memDC, RGB(235, 240, 250));
            DrawTextW(memDC, GetStr(STR_DRAGON_DESC), -1, &descRect, DT_CENTER | DT_TOP | DT_WORDBREAK);

            // Trophy banner
            SelectObject(memDC, g_hFontSub);
            RECT progRect = { cardX + 15, cardY + 480, cardX + cardW - 15, cardY + 520 };
            SetTextColor(memDC, RGB(85, 255, 85));
            DrawTextW(memDC, GetStr(STR_TROPHY_TEXT), -1, &progRect, DT_CENTER | DT_TOP | DT_SINGLELINE);
        }

        BitBlt(hdc, 0, 0, width, height, memDC, 0, 0, SRCCOPY);

        SelectObject(memDC, oldBitmap);
        DeleteObject(memBitmap);
        DeleteDC(memDC);

        EndPaint(hWnd, &ps);
        return 0;
    }

    case WM_DESTROY: {
        for (int i = 0; i < g_mobs_count; i++) {
            if (g_mobImages[i]) {
                delete g_mobImages[i];
                g_mobImages[i] = NULL;
            }
        }
        if (g_superprizeImage) {
            delete g_superprizeImage;
            g_superprizeImage = NULL;
        }
        DeleteObject(g_hFontTitle);
        DeleteObject(g_hFontSub);
        DeleteObject(g_hFontLabel);
        DeleteObject(g_hFontInput);
        DeleteObject(g_hFontBtn);
        DeleteObject(g_hFontLangBtn);
        DeleteObject(g_hFontHint);
        DeleteObject(g_hFontCardTitle);
        DeleteObject(g_hFontCardDesc);
        DeleteObject(g_hFontEye);
        DeleteObject(g_hBrushEditBg);

        PostQuitMessage(0);
        return 0;
    }
    }
    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    g_hInstance = hInstance;

    HMODULE hUser32 = GetModuleHandleW(L"user32.dll");
    if (hUser32) {
        typedef BOOL (WINAPI *SetProcessDPIAwareFunc)();
        SetProcessDPIAwareFunc pSetDPI = (SetProcessDPIAwareFunc)GetProcAddress(hUser32, "SetProcessDPIAware");
        if (pSetDPI) pSetDPI();
    }

    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_STANDARD_CLASSES | ICC_WIN95_CLASSES;
    InitCommonControlsEx(&icex);

    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    LoadOrCreateConfig();

    const wchar_t CLASS_NAME[] = L"MinecraftPasswordTrainerWindow";
    WNDCLASSEXW wc = { 0 };
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    HICON hAppIcon = CreateIconFromResourceEx((PBYTE)g_app_icon_data, (DWORD)g_app_icon_size, TRUE, 0x00030000, 32, 32, LR_DEFAULTCOLOR);
    if (hAppIcon) {
        wc.hIcon = hAppIcon;
        wc.hIconSm = hAppIcon;
    }

    RegisterClassExW(&wc);

    int clientWidth = 660;
    int clientHeight = 750;
    RECT wr = { 0, 0, clientWidth, clientHeight };
    AdjustWindowRectEx(&wr, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, FALSE, 0);

    int winWidth = wr.right - wr.left;
    int winHeight = wr.bottom - wr.top;

    int screenW = GetSystemMetrics(SM_CXSCREEN);
    int screenH = GetSystemMetrics(SM_CYSCREEN);
    int posX = (screenW - winWidth) / 2;
    int posY = (screenH - winHeight) / 2;

    HWND hWnd = CreateWindowExW(
        0,
        CLASS_NAME,
        GetStr(STR_APP_TITLE),
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        posX, posY, winWidth, winHeight,
        NULL, NULL, hInstance, NULL
    );

    if (!hWnd) {
        GdiplusShutdown(gdiplusToken);
        return 0;
    }

    if (hAppIcon) {
        SendMessageW(hWnd, WM_SETICON, ICON_BIG, (LPARAM)hAppIcon);
        SendMessageW(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hAppIcon);
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessageW(&msg, NULL, 0, 0)) {
        if (!IsDialogMessageW(hWnd, &msg)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

    GdiplusShutdown(gdiplusToken);
    return (int)msg.wParam;
}
