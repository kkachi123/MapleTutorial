#pragma once
#include "Camera.h"

extern int TILE_SIZE;
class TileRect
{
private:
	int length;
	POINT pos;
	MapType type;
public:
	TileRect(int length = 20 , POINT pos = { 0, 0 } , MapType type = MapType::Platform) : length(length), pos(pos), type(type)
	{}
	int GetLength() { return length; }
	POINT GetPos() { return pos; }
	MapType GetType() { return type; }
    void SetType(MapType type) { this->type = type; }
	void SetPos(POINT pos) { this->pos = pos; }
	void Draw(HDC hdc, int x, int y, int length);
	
};

void InputTileMap(POINT mousePos, MapType mapType, vector<TileRect*>* tileMap);
void deleteTileMap(POINT mousePos, vector<TileRect*>* tileMap);

void SaveTileMap(vector<TileRect*>* tileMap , Camera* camera , POINT* mapMove);
void LoadTileMap(vector<TileRect*>* tileMap, Camera* camera, POINT* mapMove);
void LoadTileData(vector<TileRect*>* tileMap, int fileType);
void SetTileSize(int size);

extern vector<TileRect*> tileMap;
extern POINT mapMove;
extern RECT rectView;
extern POINT mousePos;
extern BOOL openButtonGuide;
extern MapType mapType;
extern BOOL verticalDraw;
extern Camera camera;
extern TCHAR hBackImageFilename[100];

void DrawBitmap(HDC hdc, Camera* camera);
void DrawBitmapDoubleBuffering(HDC hdc, Camera* camera);
void DestroyBitmap();

void DrawGuide(HDC hdc, Camera camera);

