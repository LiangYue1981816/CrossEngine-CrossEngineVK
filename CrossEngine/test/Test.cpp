// Test.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Test.h"

#define MAX_LOADSTRING 100

// 全局变量: 
HINSTANCE hInst;
HWND hWnd;
wchar_t szTitle[MAX_LOADSTRING];
wchar_t szWindowClass[MAX_LOADSTRING];

const uint32_t indexAttachmentPresent = 0;
const uint32_t indexAttachmentDepth = 1;
const uint32_t indexAttachmentGBuffer = 2;

const uint32_t indexSubpassDepth = 0;
const uint32_t indexSubpassGBuffer = 1;
const uint32_t indexSubpassLighting = 2;

CrossEngine::CRenderer *pRenderer = NULL;
CrossEngine::CRendererRenderPass *pRenderPass = NULL;
CrossEngine::CRendererFrameBuffer *pFrameBuffer[3] = { NULL };
CrossEngine::CRendererRenderTexture *pTextureDepth = NULL;
CrossEngine::CRendererRenderTexture *pTextureGBuffer = NULL;

// 此代码模块中包含的函数的前向声明: 
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, uint32_t, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, uint32_t, WPARAM, LPARAM);

BOOL CreateRenderPassDefferShading(CrossEngine::CRendererDevice *pDevice);
void DestroyRenderPassDefferShading(CrossEngine::CRendererDevice *pDevice);

BOOL CreateFrameBufferDefferShading(CrossEngine::CRendererDevice *pDevice);
void DestroyFrameBufferDefferShading(CrossEngine::CRendererDevice *pDevice);

void Test(CrossEngine::CRendererDevice *pDevice);
void TestHostMemAllocator();
void TestLoadTexture(CrossEngine::CRendererDevice *pDevice);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// 初始化全局字符串
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_TEST, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化: 
	if (!InitInstance(hInstance, nCmdShow)) {
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TEST));

	RECT rcView;
	GetClientRect(hWnd, &rcView);

	// 初始化引擎
	CrossEngine::EnableLog(TRUE, "CrossEngineVK.log");
	pRenderer = SAFE_NEW CrossEngine::CRenderer;

	if (pRenderer->Create(hInstance, hWnd)) {
		pRenderer->CreateSwapchain(rcView.right - rcView.left, rcView.bottom - rcView.top, VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR);
		CreateRenderPassDefferShading(pRenderer->GetGraphicsDevice());
		CreateFrameBufferDefferShading(pRenderer->GetGraphicsDevice());
		pRenderer->GetGraphicsDevice()->DumpLog();
	}

	Test(pRenderer->GetGraphicsDevice());

	//TestHostMemAllocator();
	//TestDeviceMemAllocator(pRenderer->GetGraphicsDevice());

	// 主消息循环: 
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0)) {
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	// 销毁引擎
	if (pRenderer) {
		DestroyRenderPassDefferShading(pRenderer->GetGraphicsDevice());
		DestroyFrameBufferDefferShading(pRenderer->GetGraphicsDevice());

		pRenderer->DestroySwapchain();
		pRenderer->Destroy();

		SAFE_DELETE(pRenderer);
	}

	return (int)msg.wParam;
}

//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TEST));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_TEST);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 将实例句柄存储在全局变量中
	hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, 0, 0, 500, 500, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd) {
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  函数: WndProc(HWND, uint32_t, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, uint32_t message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			// 分析菜单选择: 
			switch (wmId)
			{
			case IDM_ABOUT:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				break;
			case IDM_EXIT:
				DestroyWindow(hWnd);
				break;
			case ID_TEST_HOST_MEMORY_ALLOCATOR:
				TestHostMemAllocator();
				break;
			case ID_TEST_LOAD_TEXTURE:
				TestLoadTexture(pRenderer->GetGraphicsDevice());
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		}
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			// TODO: 在此处添加使用 hdc 的任何绘图代码...
			EndPaint(hWnd, &ps);
			break;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}	
		default:
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, uint32_t message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
		case WM_INITDIALOG:
		{
			return (INT_PTR)TRUE;
		}

		case WM_COMMAND:
		{
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
			break;
		}	
	}
	return (INT_PTR)FALSE;
}

BOOL CreateRenderPassDefferShading(CrossEngine::CRendererDevice *pDevice)
{
	pRenderPass = pDevice->GetRenderPassManager()->AllocRenderPass();

	// Attachments
	pRenderPass->SetPresentAttachment(indexAttachmentPresent, VK_FORMAT_B8G8R8A8_UNORM, VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_STORE); // Present
	pRenderPass->SetDepthStencilAttachment(indexAttachmentDepth, VK_FORMAT_D32_SFLOAT, VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_DONT_CARE); // Depth
	pRenderPass->SetColorAttachment(indexAttachmentGBuffer, VK_FORMAT_R32G32B32A32_UINT, VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_DONT_CARE); // GBuffer

	// Depth subpass
	pRenderPass->SetSubpassOutputDepthStencilReference(indexSubpassDepth, indexAttachmentDepth);

	// GBuffer subpass
	pRenderPass->SetSubpassOutputColorReference(indexSubpassGBuffer, indexAttachmentGBuffer);
	pRenderPass->SetSubpassOutputDepthStencilReference(indexSubpassGBuffer, indexAttachmentDepth);

	// Lighting subpass
	pRenderPass->SetSubpassInputColorReference(indexSubpassLighting, indexAttachmentGBuffer);
	pRenderPass->SetSubpassInputDepthStencilReference(indexSubpassLighting, indexAttachmentDepth);
	pRenderPass->SetSubpassOutputColorReference(indexSubpassLighting, indexAttachmentPresent);

	// Dependency
	pRenderPass->SetSubpassDependency(0, indexSubpassDepth, indexSubpassGBuffer);
	pRenderPass->SetSubpassDependency(1, indexSubpassGBuffer, indexSubpassLighting);

	return pRenderPass->Create();
}

void DestroyRenderPassDefferShading(CrossEngine::CRendererDevice *pDevice)
{
	pDevice->GetRenderPassManager()->Free(pRenderPass);
}

BOOL CreateFrameBufferDefferShading(CrossEngine::CRendererDevice *pDevice)
{
	RECT rcView;
	GetClientRect(hWnd, &rcView);

	pTextureDepth = pDevice->GetTextureManager()->AllocRenderTexture();
	pTextureDepth->CreateDepthStencilTarget(VK_FORMAT_D32_SFLOAT, rcView.right - rcView.left, rcView.bottom - rcView.top);

	pTextureGBuffer = pDevice->GetTextureManager()->AllocRenderTexture();
	pTextureGBuffer->CreateColorTarget(VK_FORMAT_R32G32B32A32_UINT, rcView.right - rcView.left, rcView.bottom - rcView.top);

	for (uint32_t index = 0; index < 3; index++) {
		pFrameBuffer[index] = pDevice->GetFrameBufferManager()->AllocFrameBuffer();
		pFrameBuffer[index]->SetAttachment(indexAttachmentPresent, pRenderer->GetSwapchain()->GetWidth(), pRenderer->GetSwapchain()->GetHeight(), pRenderer->GetSwapchain()->GetImageView(index));
		pFrameBuffer[index]->SetAttachment(indexAttachmentDepth, pTextureDepth->GetWidth(), pTextureDepth->GetHeight(), pTextureDepth->GetImageView());
		pFrameBuffer[index]->SetAttachment(indexAttachmentGBuffer, pTextureGBuffer->GetWidth(), pTextureGBuffer->GetHeight(), pTextureGBuffer->GetImageView());
		pFrameBuffer[index]->Create(pRenderPass->GetRenderPass());
	}

	return TRUE;
}

void DestroyFrameBufferDefferShading(CrossEngine::CRendererDevice *pDevice)
{
	pDevice->GetFrameBufferManager()->Free(pFrameBuffer[0]);
	pDevice->GetFrameBufferManager()->Free(pFrameBuffer[1]);
	pDevice->GetFrameBufferManager()->Free(pFrameBuffer[2]);
}

void Test(CrossEngine::CRendererDevice *pDevice)
{
	CrossEngine::CRendererShader *pShader = pDevice->GetShaderManager()->AllocShader();
	{
		const std::string source = "\
		#version 310 es\n\
		layout(location = 1) in vec3 vertex;    \n\
		layout(location = 2) in mat3 model_view;\n\
		layout(location = 5) in mat3 projection;\n\
		void main() {}";

		pShader->Create(source.c_str(), source.size() - 1, shaderc_glsl_vertex_shader);
	}
	pDevice->GetShaderManager()->Free(pShader);
}
