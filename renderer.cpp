﻿/*==============================================================================

   レンダリング管理 [renderer.cpp]
														 Author :
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#include "renderer.h"
//#include "MathUtility.h"



//*********************************************************
// 構造体
//*********************************************************


//*****************************************************************************
// グローバル変数:
//*****************************************************************************
D3D_FEATURE_LEVEL       g_FeatureLevel = D3D_FEATURE_LEVEL_11_0;

ID3D11Device*           g_D3DDevice = nullptr;
ID3D11DeviceContext*    g_ImmediateContext = nullptr;
IDXGISwapChain*         g_SwapChain = nullptr;
ID3D11RenderTargetView* g_RenderTargetView = nullptr;
ID3D11DepthStencilView* g_DepthStencilView = nullptr;



ID3D11VertexShader*     g_VertexShader = nullptr;
ID3D11PixelShader*      g_PixelShader = nullptr;
ID3D11InputLayout*      g_VertexLayout = nullptr;
ID3D11Buffer*			g_ConstantBuffer = nullptr;
ID3D11Buffer*			g_MaterialBuffer = nullptr;

D3DXMATRIX				g_WorldMatrix;
D3DXMATRIX				g_ViewMatrix;
D3DXMATRIX				g_ProjectionMatrix;

static ID3D11DepthStencilState*		g_DepthStateEnable;
static ID3D11DepthStencilState*		g_DepthStateDisable;
//=================self
static ID3D11DepthStencilState* g_DepthStateDisableWrite;
//=================
static ID3D11BlendState*		g_BlendStateNone;
static ID3D11BlendState*		g_BlendStateAlphaBlend;
static ID3D11BlendState*		g_BlendStateAdd;
static ID3D11BlendState*		g_BlendStateSubtract;
static BLEND_MODE				g_BlendStateParam;

static ID3D11RasterizerState*	g_RasterStateCullOff;
static ID3D11RasterizerState*	g_RasterStateCullCW;
static ID3D11RasterizerState*	g_RasterStateCullCCW;

static D3DXCOLOR				g_BorderColor;


ID3D11Device* GetDevice( void )
{
	return g_D3DDevice;
}


ID3D11DeviceContext* GetDeviceContext( void )
{
	return g_ImmediateContext;
}


void SetDepthEnable( bool Enable )
{
	if( Enable )
		g_ImmediateContext->OMSetDepthStencilState( g_DepthStateEnable, NULL );
	else
		g_ImmediateContext->OMSetDepthStencilState( g_DepthStateDisable, NULL );

}

void SetDepthState()
{
	g_ImmediateContext->OMSetDepthStencilState(g_DepthStateDisableWrite, NULL);
}

void SetBlendState(BLEND_MODE bm)
{
	g_BlendStateParam = bm;

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	switch (g_BlendStateParam)
	{
	case BLEND_MODE_NONE:
		g_ImmediateContext->OMSetBlendState(g_BlendStateNone, blendFactor, 0xffffffff);
		break;
	case BLEND_MODE_ALPHABLEND:
		g_ImmediateContext->OMSetBlendState(g_BlendStateAlphaBlend, blendFactor, 0xffffffff);
		break;
	case BLEND_MODE_ADD:
		g_ImmediateContext->OMSetBlendState(g_BlendStateAdd, blendFactor, 0xffffffff);
		break;
	case BLEND_MODE_SUBTRACT:
		g_ImmediateContext->OMSetBlendState(g_BlendStateSubtract, blendFactor, 0xffffffff);
		break;
	}
}

void SetCullingMode(CULL_MODE cm)
{
	switch (cm)
	{
	case CULL_MODE_NONE:
		g_ImmediateContext->RSSetState(g_RasterStateCullOff);
		break;
	case CULL_MODE_FRONT:
		g_ImmediateContext->RSSetState(g_RasterStateCullCW);
		break;
	case CULL_MODE_BACK:
		g_ImmediateContext->RSSetState(g_RasterStateCullCCW);
		break;
	}
}

void SetSamplerState(FILTER_MODE fm, ADDRESS_MODE am)
{
	// サンプラーステート設定
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));

	switch (fm)
	{
	case FILTER_MODE_POINT:
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		break;
	case FILTER_MODE_LINEAR:
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		break;
	case FILTER_MODE_ANISOTROPIC:
		samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		break;
	}

	//テクスチャのアドレッシングモードの設定		
	switch (am)
	{
	case ADDRESS_MODE_WRAP:
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		break;
	case ADDRESS_MODE_MIRROR:
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
		break;
	case ADDRESS_MODE_CLAMP:
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		break;
	case ADDRESS_MODE_BORDER:
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
		break;
	}
													   													   
	samplerDesc.BorderColor[0] = g_BorderColor.r;
	samplerDesc.BorderColor[1] = g_BorderColor.g;
	samplerDesc.BorderColor[2] = g_BorderColor.b;
	samplerDesc.BorderColor[3] = g_BorderColor.a;

	samplerDesc.MipLODBias = 0;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	ID3D11SamplerState* samplerState = nullptr;
	g_D3DDevice->CreateSamplerState(&samplerDesc, &samplerState);

	g_ImmediateContext->PSSetSamplers(0, 1, &samplerState);

	samplerState->Release();
	samplerState = nullptr;
}

void SetSamplerBorderColor(D3DXCOLOR col)
{
	g_BorderColor = col;
}

void SetWorldViewProjection2D( void )
{
	D3DXMATRIX worldViewProjection;

	D3DXMatrixOrthoOffCenterLH(&worldViewProjection, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, -100.0f, 100.0f);
	D3DXMatrixTranspose(&worldViewProjection, &worldViewProjection);

	GetDeviceContext()->UpdateSubresource( g_ConstantBuffer, 0, nullptr, &worldViewProjection, 0, 0 );
}


void SetWorldMatrix( D3DXMATRIX *WorldMatrix )
{
	g_WorldMatrix = *WorldMatrix;

	D3DXMATRIX worldViewProjection = g_WorldMatrix * g_ViewMatrix * g_ProjectionMatrix;
	D3DXMatrixTranspose(&worldViewProjection, &worldViewProjection);

	GetDeviceContext()->UpdateSubresource( g_ConstantBuffer, 0, nullptr, &worldViewProjection, 0, 0 );
}

void SetViewMatrix( D3DXMATRIX *ViewMatrix )
{
	g_ViewMatrix = *ViewMatrix;

	D3DXMATRIX worldViewProjection = g_WorldMatrix * g_ViewMatrix * g_ProjectionMatrix;
	D3DXMatrixTranspose(&worldViewProjection, &worldViewProjection);

	GetDeviceContext()->UpdateSubresource(g_ConstantBuffer, 0, nullptr, &worldViewProjection, 0, 0);
}

void SetProjectionMatrix( D3DXMATRIX *ProjectionMatrix )
{
	g_ProjectionMatrix = *ProjectionMatrix;

	D3DXMATRIX worldViewProjection = g_WorldMatrix * g_ViewMatrix * g_ProjectionMatrix;
	D3DXMatrixTranspose(&worldViewProjection, &worldViewProjection);

	GetDeviceContext()->UpdateSubresource(g_ConstantBuffer, 0, nullptr, &worldViewProjection, 0, 0);
}



void SetMaterial( MATERIAL Material )
{

	GetDeviceContext()->UpdateSubresource( g_MaterialBuffer, 0, nullptr, &Material, 0, 0 );

}



//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitRenderer(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	HRESULT hr = S_OK;

	// デバイス、スワップチェーン、コンテキスト生成
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory( &sd, sizeof( sd ) );
	sd.BufferCount = 1;
	sd.BufferDesc.Width = SCREEN_WIDTH;
	sd.BufferDesc.Height = SCREEN_HEIGHT;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	hr = D3D11CreateDeviceAndSwapChain( nullptr,
										D3D_DRIVER_TYPE_HARDWARE,
										nullptr,
										0,
										nullptr,
										0,
										D3D11_SDK_VERSION,
										&sd,
										&g_SwapChain,
										&g_D3DDevice,
										&g_FeatureLevel,
										&g_ImmediateContext );
	if( FAILED( hr ) )
		return hr;


	// レンダーターゲットビュー生成、設定
	ID3D11Texture2D* pBackBuffer = nullptr;
	g_SwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
	g_D3DDevice->CreateRenderTargetView( pBackBuffer, nullptr, &g_RenderTargetView );
	pBackBuffer->Release();



	//ステンシル用テクスチャー作成
	ID3D11Texture2D* depthTexture = nullptr;
	D3D11_TEXTURE2D_DESC td;
	ZeroMemory( &td, sizeof(td) );
	td.Width			= sd.BufferDesc.Width;
	td.Height			= sd.BufferDesc.Height;
	td.MipLevels		= 1;
	td.ArraySize		= 1;
	td.Format			= DXGI_FORMAT_D24_UNORM_S8_UINT;
	td.SampleDesc		= sd.SampleDesc;
	td.Usage			= D3D11_USAGE_DEFAULT;
	td.BindFlags		= D3D11_BIND_DEPTH_STENCIL;
    td.CPUAccessFlags	= 0;
    td.MiscFlags		= 0;
	g_D3DDevice->CreateTexture2D( &td, nullptr, &depthTexture );

	//ステンシルターゲット作成
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory( &dsvd, sizeof(dsvd) );
	dsvd.Format			= td.Format;
	dsvd.ViewDimension	= D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Flags			= 0;
	g_D3DDevice->CreateDepthStencilView( depthTexture, &dsvd, &g_DepthStencilView );

	g_ImmediateContext->OMSetRenderTargets( 1, &g_RenderTargetView, g_DepthStencilView );


	// ビューポート設定
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)SCREEN_WIDTH;
	vp.Height = (FLOAT)SCREEN_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	g_ImmediateContext->RSSetViewports( 1, &vp );



	// ラスタライザステート設定
	D3D11_RASTERIZER_DESC rd; 
	ZeroMemory( &rd, sizeof( rd ) );
	rd.FillMode = D3D11_FILL_SOLID; 
	rd.CullMode = D3D11_CULL_NONE;
	rd.DepthClipEnable = TRUE; 
	rd.MultisampleEnable = FALSE; 
	g_D3DDevice->CreateRasterizerState(&rd, &g_RasterStateCullOff);

	rd.CullMode = D3D11_CULL_FRONT;//(CW)
	g_D3DDevice->CreateRasterizerState(&rd, &g_RasterStateCullCW);

	rd.CullMode = D3D11_CULL_BACK;//(CCW)
	g_D3DDevice->CreateRasterizerState(&rd, &g_RasterStateCullCCW);

	// カリングモード設定（CCW）
	SetCullingMode(CULL_MODE_NONE);



	// ブレンドステート設定
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory( &blendDesc, sizeof( blendDesc ) );
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	g_D3DDevice->CreateBlendState(&blendDesc, &g_BlendStateAlphaBlend);

	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	g_D3DDevice->CreateBlendState(&blendDesc, &g_BlendStateNone);

	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	g_D3DDevice->CreateBlendState(&blendDesc, &g_BlendStateAdd);

	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	g_D3DDevice->CreateBlendState(&blendDesc, &g_BlendStateSubtract);

	// アルファブレンド設定
	SetBlendState(BLEND_MODE_ALPHABLEND);



	// 深度ステンシルステート設定
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory( &depthStencilDesc, sizeof( depthStencilDesc ) );
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask	= D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = FALSE;

	g_D3DDevice->CreateDepthStencilState( &depthStencilDesc, &g_DepthStateEnable );//深度有効ステート

	//depthStencilDesc.DepthEnable = FALSE;
	depthStencilDesc.DepthWriteMask	= D3D11_DEPTH_WRITE_MASK_ZERO;
	g_D3DDevice->CreateDepthStencilState( &depthStencilDesc, &g_DepthStateDisable );//深度無効ステート

	g_ImmediateContext->OMSetDepthStencilState( g_DepthStateEnable, NULL );

	//=================self
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	depthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	g_D3DDevice->CreateDepthStencilState(&depthStencilDesc, &g_DepthStateDisableWrite);
	//=================

	// サンプラーステート設定
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory( &samplerDesc, sizeof( samplerDesc ) );	 //POINT:一番近いテクセルの色をサンプリングする
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;//LINEAR:近傍平均を取ったテクセルの色をサンプリングする
	//テクスチャのアドレッシングモードの設定			 //ANISOTROPIC:異方性フィルタ　3Dに対応したサンプリング
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;//WRAP：繰り返し
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;//MIRROR:鏡面繰り返し
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;//CLAMP:最後の色を繰り返す
													   //BORDER:指定色を繰り返す
													   //MIRROR_ONCE:鏡面一回
	g_BorderColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	samplerDesc.BorderColor[0] = g_BorderColor.r;//BORDERの色を指定
	samplerDesc.BorderColor[1] = g_BorderColor.g;
	samplerDesc.BorderColor[2] = g_BorderColor.b;
	samplerDesc.BorderColor[3] = g_BorderColor.a;

	samplerDesc.MipLODBias = 0;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	ID3D11SamplerState* samplerState = nullptr;
	g_D3DDevice->CreateSamplerState( &samplerDesc, &samplerState );

	g_ImmediateContext->PSSetSamplers( 0, 1, &samplerState );
	samplerState->Release();


	// 頂点シェーダコンパイル・生成
	ID3DBlob* pErrorBlob;
	ID3DBlob* pVSBlob = nullptr;
	hr = D3DX11CompileFromFile( "shader.hlsl", nullptr, nullptr, "VertexShaderPolygon", "vs_4_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, nullptr, &pVSBlob, &pErrorBlob, nullptr );
	if( FAILED(hr) )
	{
		MessageBox( nullptr , (char*)pErrorBlob->GetBufferPointer(), "VS", MB_OK | MB_ICONERROR );
	}

	g_D3DDevice->CreateVertexShader( pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &g_VertexShader );

	// 入力レイアウト生成
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,			0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE( layout );

	g_D3DDevice->CreateInputLayout( layout,
		numElements,
		pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(),
		&g_VertexLayout );

	pVSBlob->Release();


	// ピクセルシェーダコンパイル・生成
	ID3DBlob* pPSBlob = nullptr;
	hr = D3DX11CompileFromFile( "shader.hlsl", nullptr, nullptr, "PixelShaderPolygon", "ps_4_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, nullptr, &pPSBlob, &pErrorBlob, nullptr );
	if( FAILED(hr) )
	{
		MessageBox( nullptr , (char*)pErrorBlob->GetBufferPointer(), "PS", MB_OK | MB_ICONERROR );
	}

	g_D3DDevice->CreatePixelShader( pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &g_PixelShader );
	
	pPSBlob->Release();


	// 定数バッファ生成
	D3D11_BUFFER_DESC hBufferDesc;
	hBufferDesc.ByteWidth = sizeof(D3DXMATRIX);
	hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hBufferDesc.CPUAccessFlags = 0;
	hBufferDesc.MiscFlags = 0;
	hBufferDesc.StructureByteStride = sizeof(float);

	g_D3DDevice->CreateBuffer( &hBufferDesc, nullptr, &g_ConstantBuffer );
	g_ImmediateContext->VSSetConstantBuffers( 0, 1, &g_ConstantBuffer );


	hBufferDesc.ByteWidth = sizeof(MATERIAL);
	hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hBufferDesc.CPUAccessFlags = 0;
	hBufferDesc.MiscFlags = 0;
	hBufferDesc.StructureByteStride = sizeof(float);

	g_D3DDevice->CreateBuffer( &hBufferDesc, nullptr, &g_MaterialBuffer );
	g_ImmediateContext->VSSetConstantBuffers( 1, 1, &g_MaterialBuffer );

	// 入力レイアウト設定
	g_ImmediateContext->IASetInputLayout( g_VertexLayout );

	// シェーダ設定
	g_ImmediateContext->VSSetShader( g_VertexShader, nullptr, 0 );
	g_ImmediateContext->PSSetShader( g_PixelShader, nullptr, 0 );

	return S_OK;
}


//=============================================================================
// 終了処理
//=============================================================================
void UninitRenderer(void)
{
	// オブジェクト解放
	if (g_DepthStateEnable)		g_DepthStateEnable->Release();
	if (g_DepthStateDisable)	g_DepthStateDisable->Release();

	if (g_BlendStateNone)		g_BlendStateNone->Release();
	if (g_BlendStateAlphaBlend) g_BlendStateAlphaBlend->Release();
	if (g_BlendStateAdd)		g_BlendStateAdd->Release();
	if (g_BlendStateSubtract)	g_BlendStateSubtract->Release();

	if (g_RasterStateCullOff)	g_RasterStateCullOff->Release();
	if (g_RasterStateCullCW)	g_RasterStateCullCW->Release();
	if (g_RasterStateCullCCW)	g_RasterStateCullCCW->Release();

	if( g_ConstantBuffer )		g_ConstantBuffer->Release();
	if( g_MaterialBuffer )		g_MaterialBuffer->Release();
	if( g_VertexLayout )		g_VertexLayout->Release();
	if( g_VertexShader )		g_VertexShader->Release();
	if( g_PixelShader )			g_PixelShader->Release();

	if( g_ImmediateContext )	g_ImmediateContext->ClearState();
	if( g_RenderTargetView )	g_RenderTargetView->Release();
	if( g_SwapChain )			g_SwapChain->Release();
	if( g_ImmediateContext )	g_ImmediateContext->Release();
	if( g_D3DDevice )			g_D3DDevice->Release();

	if (g_DepthStateDisableWrite) g_DepthStateDisableWrite->Release();
}


//=============================================================================
// バックバッファクリア
//=============================================================================
void Clear(void)
{
	// バックバッファクリア
	float ClearColor[4] = { 0.098f, 0.298f, 0.2f, 1.0f };
	g_ImmediateContext->ClearRenderTargetView( g_RenderTargetView, ClearColor );
	g_ImmediateContext->ClearDepthStencilView( g_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}


//=============================================================================
// プレゼント
//=============================================================================
void Present(void)
{
	g_SwapChain->Present( 0, 0 );
}

DrawData::DrawData() :drawVertexIndex(-1), shaderParam(nullptr),
size(vector2s(0.0f, 0.0f)), isUseEbo(true), useDefaultVertices(true),
useBatching(false), batchingType(E_BatchingType::BatchingType_None),color(1.0f,1.0f,1.0f,1.0f)
{
	texIndex = vector2s(0, 0);
	//tex_offset = vector2s(0, 0);
	tex_texel_size = vector2s(0, 0);
	tex_w_count = 1;
	usePreComputeUV = false;
	preComputeUV = vector2s(0, 0);
	preComputeUVSize = vector2s(1, 1);
}

DrawData::DrawData(const DrawData& src)
{
	this->drawVertexIndex = src.drawVertexIndex;

	this->size = src.size;

	this->isUseEbo = src.isUseEbo;
	this->useDefaultVertices = src.useDefaultVertices;

	this->useBatching = src.useBatching;
	this->batchingType = src.batchingType;
	this->color = src.color;

	if (tileData != nullptr) {
		this->tileData = new TileData(*src.tileData);
	}
	else {
		this->tileData = nullptr;
	}

	//new logic
	this->textureNo = src.textureNo;
	this->texIndex = src.texIndex;
	//this->tex_offset = src.tex_offset;
	this->tex_texel_size = src.tex_texel_size;
	this->tex_w_count = src.tex_w_count;
	this->usePreComputeUV = src.usePreComputeUV;
	this->preComputeUV = src.preComputeUV;
	this->preComputeUVSize = src.preComputeUVSize;

	this->SetShaderParam(src.shaderParam);
}

DrawData::~DrawData()
{
	if (shaderParam != nullptr) {
		delete shaderParam;
		shaderParam = nullptr;
	}

	if (tileData != nullptr) {
		delete tileData;
		tileData = nullptr;
	}

	if (this->useBatching == true) {
		RenderMgr::ReleaseDrawDataVertex(drawVertexIndex, this->batchingType);
	}
	else {
		RenderMgr::ReleaseDrawDataVertex(drawVertexIndex, E_BatchingType::BatchingType_None);
	}
}

void DrawData::SetDrawVertexIndex(int index)
{
	drawVertexIndex = index;
}

int DrawData::GetDrawVertexIndex()
{
	return drawVertexIndex;
}

void DrawData::SetShaderParam(BaseShaderParam* param)
{

	if (param == nullptr)return;

	if (shaderParam != nullptr) {
		delete shaderParam;
		shaderParam = nullptr;
	}

	BaseShaderParam* temp = nullptr;
	switch (param->shaderParamType) {
	case E_ShaderParam::ShaderParamType_Normal:
		temp = new ShaderParam_Normal();
		//temp = (ShaderParam_Normal*)param;
		((ShaderParam_Normal*)temp)->Copy(*(ShaderParam_Normal*)param);
		shaderParam = temp;

		break;
	case E_ShaderParam::ShaderParamType_Mask:
		temp = new ShaderParam_Mask();
		//temp = (ShaderParam_Mask*)param;
		((ShaderParam_Mask*)temp)->Copy(*(ShaderParam_Mask*)param);
		shaderParam = temp;

		break;
	case E_ShaderParam::ShaderParamType_WhiteLightObj:
		temp = new ShaderParam_WhiteLightObj();
		//temp = (ShaderParam_WhiteLightObj*)param;
		((ShaderParam_WhiteLightObj*)temp)->Copy(*(ShaderParam_WhiteLightObj*)param);
		shaderParam = temp;

		break;
	case E_ShaderParam::ShaderParamType_SceneObj:
		temp = new ShaderParam_WhiteLightObj();
		//temp = (ShaderParam_WhiteLightObj*)param;
		((ShaderParam_WhiteLightObj*)temp)->Copy(*(ShaderParam_WhiteLightObj*)param);
		shaderParam = temp;

		break;
	case E_ShaderParam::ShaderParamType_None:
	default:
		break;
	}
}

void DrawData::SetDrawDataType(E_DrawDataType dataType)
{
	drawDataType = dataType;
}

E_DrawDataType DrawData::GetDrawDataType()
{
	return drawDataType;
}

void DrawData::SetTileData(TileData* data)
{
	if (data == nullptr) {
		return;
	}

	if (tileData != nullptr) {
		delete tileData;
		tileData = nullptr;
	}

	tileData = data;
}

TileData* DrawData::GetTileData()
{
	return tileData;
}


