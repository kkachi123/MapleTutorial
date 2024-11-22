#pragma once
#include "framework.h"

#define INPUTMGR InputMgr::GetInstance()


class InputMgr
{
	DECLARE_SINGLETON(InputMgr)
public:
	bool	m_bKeyDown[256];	// 키가 눌렸는지 체크할 배열
	bool	m_bKeyUp[256];	// 키가 떼졌는지 체크할 배열

private:
	InputMgr(void);
	~InputMgr(void);

public:
	bool StayKeyDown(int nKey); // 키가 눌리고 있는지 체크
	bool OnceKeyDown(int nKey); // 키가 한번 눌렸는지 체크
	bool OnceKeyUp(int nKey);	// 키가 한번 눌렸다 떼졌는지 체크
	bool IsToggleKey(int nKey);	// 한번 눌릴때마다 on off로 바꾸도록
};

