#pragma once

#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <vector>
#include <string>
#include "DirectXTex.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

#define DIRECTINPUT_VERSION 0x0800	// DirectInputのバージョン指定
#include <dinput.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

// スケーリング倍率
XMFLOAT3 scale;
// 回転角
XMFLOAT3 rotation;
// 座標
XMFLOAT3 position;
