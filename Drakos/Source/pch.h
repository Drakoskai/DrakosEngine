#pragma once

#include <WinSDKVer.h>
#include <SDKDDKVer.h>

#define NOMINMAX
#define NODRAWTEXT
#define NOBITMAP
#define NOMCX
#define NOSERVICE
#define NOHELP
#define DIRECTINPUT_VERSION 0x0800

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <wrl.h>

#include <d3d11_2.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <DirectXPackedVector.h>
#include <dinput.h>

#include <algorithm>
#include <exception>
#include <memory>
#include <stdexcept>
#include <cstdint>
#include <cstdio>
#include <cstdarg>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <ppltasks.h>
#include <timeapi.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")
