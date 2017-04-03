// Test.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Test.h"

#define MAX_LOADSTRING 100

// ȫ�ֱ���: 
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

// �˴���ģ���а����ĺ�����ǰ������: 
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

	// ��ʼ��ȫ���ַ���
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_TEST, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��: 
	if (!InitInstance(hInstance, nCmdShow)) {
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TEST));

	RECT rcView;
	GetClientRect(hWnd, &rcView);

	// ��ʼ������
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

	// ����Ϣѭ��: 
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0)) {
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	// ��������
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
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
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
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����
	hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, 0, 0, 500, 500, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd) {
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  ����: WndProc(HWND, uint32_t, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND  - ����Ӧ�ó���˵�
//  WM_PAINT    - ����������
//  WM_DESTROY  - �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, uint32_t message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			// �����˵�ѡ��: 
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
			// TODO: �ڴ˴����ʹ�� hdc ���κλ�ͼ����...
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

// �����ڡ������Ϣ�������
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
		layout(location = 0) in vec3 inPosition;\n\
		layout(location = 1) in vec3 inNormal;\n\
		layout(location = 2) in vec3 inColor;\n\
		layout(location = 3) in vec2 inTexcoord0;\n\
		void main() {}";

		pShader->Create(source.c_str(), source.size(), shaderc_glsl_vertex_shader);
	}
	pDevice->GetShaderManager()->Free(pShader);
}
