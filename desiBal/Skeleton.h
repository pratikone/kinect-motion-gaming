//------------------------------------------------------------------------------
// <copyright file="SkeletonBasics.h" company="Microsoft">
//     Copyright (c) Microsoft Corporation.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

#pragma once

#include "stdafx.h"

class CSkeletonBasics
{
	static const int        cScreenWidth = 320;
	static const int        cScreenHeight = 240;

	static const int        cStatusMessageMaxLen = 260 * 2;



public:
	/// <summary>
	/// Constructor
	/// </summary>
	CSkeletonBasics();

	/// <summary>
	/// Destructor
	/// </summary>
	~CSkeletonBasics();

	/// <summary>
	/// Handles window messages, passes most to the class instance to handle
	/// </summary>
	/// <param name="hWnd">window message is for</param>
	/// <param name="uMsg">message</param>
	/// <param name="wParam">message data</param>
	/// <param name="lParam">additional message data</param>
	/// <returns>result of message processing</returns>

	static LRESULT CALLBACK MessageRouter(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	/// <summary>
	/// Handle windows messages for a class instance
	/// </summary>
	/// <param name="hWnd">window message is for</param>
	/// <param name="uMsg">message</param>
	/// <param name="wParam">message data</param>
	/// <param name="lParam">additional message data</param>
	/// <returns>result of message processing</returns>
	LRESULT  CALLBACK     DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	

	HANDLE                  m_hNextSkeletonEvent;
	void                    Update(Graphics *graphics);
	HWND                    m_hWnd;

//private:

	bool                    m_bSeatedMode;

	// Current Kinect
	INuiSensor*             m_pNuiSensor;

	// Skeletal drawing
	ID2D1HwndRenderTarget*   m_pRenderTarget;
	ID2D1SolidColorBrush*    m_pBrushJointTracked;
	ID2D1SolidColorBrush*    m_pBrushJointInferred;
	ID2D1SolidColorBrush*    m_pBrushBoneTracked;
	ID2D1SolidColorBrush*    m_pBrushBoneInferred;
	D2D1_POINT_2F            m_Points[NUI_SKELETON_POSITION_COUNT];

	// Direct2D
	ID2D1Factory*           m_pD2DFactory;
	IDWriteFactory*			m_pDWriteFactory;
	IDWriteTextFormat*		m_pTextFormat;

	HANDLE                  m_pSkeletonStreamHandle;

	/// <summary>
	/// Main processing function
	/// </summary>

	/// <summary>
	/// Create the first connected Kinect found 
	/// </summary>
	/// <returns>S_OK on success, otherwise failure code</returns>
	HRESULT                 CreateFirstConnected();

	/// <summary>
	/// Handle new skeleton data
	/// </summary>
	void                    ProcessSkeleton();

	/// <summary>
	/// Ensure necessary Direct2d resources are created
	/// </summary>
	/// <returns>S_OK if successful, otherwise an error code</returns>
	HRESULT                 EnsureDirect2DResources();

	/// <summary>
	/// Dispose Direct2d resources 
	/// </summary>
	void                    DiscardDirect2DResources();

	/// <summary>
	/// Draws a bone line between two joints
	/// </summary>
	/// <param name="skel">skeleton to draw bones from</param>
	/// <param name="joint0">joint to start drawing from</param>
	/// <param name="joint1">joint to end drawing at</param>
	void                    DrawBone(const NUI_SKELETON_DATA & skel, NUI_SKELETON_POSITION_INDEX bone0, NUI_SKELETON_POSITION_INDEX bone1);

	/// <summary>
	/// Draws a skeleton
	/// </summary>
	/// <param name="skel">skeleton to draw</param>
	/// <param name="windowWidth">width (in pixels) of output buffer</param>
	/// <param name="windowHeight">height (in pixels) of output buffer</param>
	void                    DrawSkeleton(const NUI_SKELETON_DATA & skel, int windowWidth, int windowHeight, bool inverse, int skeletonNumber);

	/// <summary>
	/// Converts a skeleton point to screen space
	/// </summary>
	/// <param name="skeletonPoint">skeleton point to tranform</param>
	/// <param name="width">width (in pixels) of output buffer</param>
	/// <param name="height">height (in pixels) of output buffer</param>
	/// <returns>point in screen-space</returns>
	D2D1_POINT_2F           SkeletonToScreen(Vector4 skeletonPoint, int width, int height, bool inverse);


	/// <summary>
	/// Set the status bar message
	/// </summary>
	/// <param name="szMessage">message to display</param>
	void                    SetStatusMessage(WCHAR* szMessage);

	HRESULT CreateDeviceIndependentResources();
};
