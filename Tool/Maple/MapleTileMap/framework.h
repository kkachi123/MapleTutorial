// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
//

#pragma once
#pragma warning(disable : 4996)
#pragma warning(disable : 4244)
#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
#include <commdlg.h>
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <ole2.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "msimg32.lib")


#include <vector>
using namespace std;
extern HWND g_hWnd;

#include <fstream>
#include "utill.h"

#include <atlimage.h>
#include "WinSock2.h"
#pragma comment(lib,"ws2_32.lib")
#define WM_ASYNC WM_USER+2

#include "Define.h"
#include "Object.h"
#include "MapObject.h"

#include "CCharacter.h"
#include "CPlayer.h"
#include "CNPC.h"

#include "CMonster.h"
#include "Skill.h"
#include "Snail1.h"
#include "Snail2.h"
#include "MushMom.h"
#include "levelUp.h"
#include "random"



