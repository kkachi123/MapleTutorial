#include "MapManager.h"
#include "SoundManager.h"

MapleTileMapManager::MapleTileMapManager()
{
	keys[255] = { FALSE }; 
	speed = 10;
	inputMousePos = { -1,-1 };
	mouseLeftDown = false;
	mouseRightDown = false;
	dragMousePos = new POINT;
    ctrlClick = FALSE;
}
MapleTileMapManager::~MapleTileMapManager()
{
    stopBgm(dwID);
	delete dragMousePos;
}
void MapleTileMapManager::SetUp()
{
    playingBgm(SoundType::play);
	camera.SetWidth(1500);
	camera.SetHeight(1000);
    camera.SetPos({ 0,0 });
    mapMove = { 0,0 };
    zoomRatio = 1.0f;
}
void MapleTileMapManager::MapMovingButton(UINT message,BOOL isDown, WPARAM wParam)
{
	switch (message)
	{
		case WM_KEYDOWN:
		{
			switch (wParam)
			{
			case VK_CONTROL:
				ctrlClick = TRUE;
				break;
			case VK_LEFT:
				keys[VK_LEFT] = TRUE;
                keys[VK_RIGHT] = FALSE;
                keys[VK_UP] = FALSE;
                keys[VK_DOWN] = FALSE;
				break;
			case VK_RIGHT:
				keys[VK_RIGHT] = TRUE;
                keys[VK_LEFT] = FALSE;
                keys[VK_UP] = FALSE;
                keys[VK_DOWN] = FALSE;
				break;
			case VK_UP:
				keys[VK_UP] = TRUE;
                keys[VK_LEFT] = FALSE;
                keys[VK_RIGHT] = FALSE;
                keys[VK_DOWN] = FALSE;
				break;
			case VK_DOWN:
				keys[VK_DOWN] = TRUE;
                keys[VK_LEFT] = FALSE;
                keys[VK_RIGHT] = FALSE;
                keys[VK_UP] = FALSE;
				break;
			case 'S':
				if (GetKeyState(VK_CONTROL) & 0x8000)
					SaveTileMap(&tileMap, &camera, &mapMove);
				break;
			case 'O':
				if (GetKeyState(VK_CONTROL) & 0x8000)
					LoadTileMap(&tileMap, &camera, &mapMove);
				break;
			case 'B':
                if (GetKeyState(VK_CONTROL) & 0x8000)
				    openButtonGuide = !openButtonGuide;
				break;
			}
			InvalidateRect(g_hWnd, NULL, false);
		}
			break;
		case WM_KEYUP:
			if(wParam == VK_CONTROL)
				ctrlClick = FALSE;
			else
				keys[wParam] = FALSE;
			break;
	}
	
}
void MapleTileMapManager::MapMoving()
{
	BOOL canMove = true;
	if (keys[VK_LEFT])
	{
		camera.SetPos({ (long)(camera.GetPos().x - speed * zoomRatio), camera.GetPos().y });
		mapMove.x -= speed * zoomRatio;
	}
	if (keys[VK_RIGHT])
	{
		camera.SetPos({ (long)(camera.GetPos().x + speed * zoomRatio), camera.GetPos().y });
		mapMove.x += speed * zoomRatio;
	}
	if (keys[VK_UP])
	{
		camera.SetPos({ camera.GetPos().x, (long)(camera.GetPos().y - speed * zoomRatio) });
		mapMove.y -= speed * zoomRatio;
	}
	if (keys[VK_DOWN])
	{
		camera.SetPos({ camera.GetPos().x, (long)(camera.GetPos().y + speed * zoomRatio) });
		mapMove.y += speed * zoomRatio;
	}

	if (camera.GetPos().x < 0)
	{
		camera.SetPos({ 0, camera.GetPos().y });
		mapMove.x = 0;
	}
	if (camera.GetPos().y < 0)
	{
		camera.SetPos({ camera.GetPos().x, 0 });
		mapMove.y = 0;
	}
	if (camera.GetPos().x + camera.GetWidth() > bitBack.bmWidth * zoomRatio)
	{
		camera.SetPos({ (long)(bitBack.bmWidth * zoomRatio - camera.GetWidth()), camera.GetPos().y });
		mapMove.x = bitBack.bmWidth * zoomRatio - camera.GetWidth();
	}
	if (camera.GetPos().y + camera.GetHeight() > bitBack.bmHeight * zoomRatio)
	{
		camera.SetPos({ camera.GetPos().x, (long)(bitBack.bmHeight * zoomRatio - camera.GetHeight()) });
		mapMove.y = bitBack.bmHeight * zoomRatio - camera.GetHeight();
	}
}

void MapleTileMapManager::MapButtonChar(UINT message ,WPARAM wParam)
{
	switch (message)
	{
	case WM_CHAR:
        switch (wParam)
        {
        case '1':
            mapType = MapType::Platform;
            break;
        case '2':
            mapType = MapType::Wall;
            break;
        case '3':
            mapType = MapType::Rope;
            break;
        case '4':
            mapType = MapType::Ladder;
            break;
        case 'R':
        case 'r':
            tileMap.clear();
            break;
        case 'Q':
        case 'q':
            SetTileSize(10);
            break;
        case 'W':
        case 'w':
            SetTileSize(20);
            break;
        case 'E':
        case 'e':
            SetTileSize(40);
            break;
        case 'Z':
        case 'z':
            verticalDraw = !verticalDraw;
            break;
        }
		break;
	}
}
void MapleTileMapManager::MapMouseControl(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
		GetCursorPos(dragMousePos);
		ScreenToClient(g_hWnd, dragMousePos);
		if (mouseRightDown)
			break;
		if (verticalDraw)
			mousePos.y = (dragMousePos->y + mapMove.y) / zoomRatio / TILE_SIZE;
		else
			mousePos.x = (dragMousePos->x + mapMove.x) / zoomRatio / TILE_SIZE;
		mouseLeftDown = true;
		mouseRightDown = false;
		break;
	case WM_LBUTTONUP:
		mouseLeftDown = false;
		break;
	case WM_RBUTTONDOWN:
		if (mouseLeftDown)
			break;
		if (verticalDraw)
			mousePos.y = (dragMousePos->y + mapMove.y) / zoomRatio / TILE_SIZE;
		else
			mousePos.x = (dragMousePos->x + mapMove.x) / zoomRatio / TILE_SIZE;
		mouseLeftDown = false;
		mouseRightDown = true;
		break;
	case WM_RBUTTONUP:
		mouseRightDown = false;
		break;
	case WM_MOUSEMOVE:
		GetCursorPos(dragMousePos);
		ScreenToClient(g_hWnd, dragMousePos);
		if (verticalDraw)
		{
			mousePos.x = (dragMousePos->x + mapMove.x) / zoomRatio / TILE_SIZE;
		}
		else
		{
			mousePos.y = (dragMousePos->y + mapMove.y) / zoomRatio / TILE_SIZE;
		}
		inputMousePos = { mousePos.x * TILE_SIZE, mousePos.y * TILE_SIZE };
		if (mouseLeftDown)
		{
			InputTileMap(inputMousePos, mapType, &tileMap);
		}
		if (mouseRightDown)
		{
			deleteTileMap(inputMousePos, &tileMap);
		}
		break;
	}

}

void MapleTileMapManager::Render(HDC hdc)
{
    DrawBitmapDoubleBuffering(hdc, &camera);
    TCHAR str[256];
    wsprintf(str, L"dragMousePos : x:%d y:%d", dragMousePos->x, dragMousePos->y);
    TextOut(hdc, 0, 0, str, wcslen(str));
}

LRESULT MapleTileMapManager::WndProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYDOWN:
	case WM_KEYUP:
		MapMovingButton(message, message == WM_KEYDOWN, wParam);
		break;
	case WM_CHAR:
		MapButtonChar(message, wParam);
		break;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
		MapMouseControl(message, wParam, lParam);
		break;
	case WM_MOUSEWHEEL:
	{
	    if(ctrlClick)
		{
			short zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
			if (zDelta > 0)
			{
				zoomRatio += 0.1f;
				if (zoomRatio > 2.0f)
					zoomRatio = 2.0f;
			}
			else
			{
				zoomRatio -= 0.1f;
				if (zoomRatio < 1.0f)
					zoomRatio = 1.0f;
			}
		}
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(g_hWnd, message, wParam, lParam);
}
