/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 *
 * The contents of this file are subject to the Netscape Public License
 * Version 1.0 (the "NPL"); you may not use this file except in
 * compliance with the NPL.  You may obtain a copy of the NPL at
 * http://www.mozilla.org/NPL/
 *
 * Software distributed under the NPL is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the NPL
 * for the specific language governing rights and limitations under the
 * NPL.
 *
 * The Initial Developer of this code under the NPL is Netscape
 * Communications Corporation.  Portions created by Netscape are
 * Copyright (C) 1998 Netscape Communications Corporation.  All Rights
 * Reserved.
 */

#ifdef PowerPlant_PCH
#include PowerPlant_PCH
#endif

#include <UGWorld.h>
#include <UDrawingState.h>
#include <UDrawingUtils.h>
#include <UMemoryMgr.h>
#include <UTextTraits.h>
#include <LStream.h>

#if defined(QAP_BUILD)
#include <QAP_Assist.h>
#endif

#include "UGraphicGizmos.h"
#include "CButton.h"
#include "StSetBroadcasting.h"
#include "CTargetedUpdateMenuRegistry.h"

#ifndef __ICONS__
#include <Icons.h>
#endif

#ifndef __PALETTES__
#include <Palettes.h>
#endif

#ifndef __TOOLUTILS__
#include <ToolUtils.h>
#endif

// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
//	₯₯₯
//	₯	Class CButton
//	₯₯₯	
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
/*
#if defined(QAP_BUILD)
void
CButton::QapGetContents (PWCINFO pwc, short *pCount, short max)
{
	QapAddViewItem (pwc, pCount, WT_ASSIST_ITEM, WC_PUSH_BUTTON);
}

void
CButton::QapGetCDescriptorMax (char * cp_buf, short s_max)
{
	Str255	controlTitle;
	short	sMinLen;
	
	GetDescriptor(controlTitle);
	
	memset (cp_buf, 0, s_max);
	sMinLen = ((short)controlTitle[0] < s_max ? (short)controlTitle[0] : s_max);
	strncpy (cp_buf, (const char *)&controlTitle[1], sMinLen);
}
#endif
*/
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
//	₯	CButton
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ

CButton::CButton(LStream *inStream)
	:	LControl(inStream)
{
	mButtonMask = NULL;
	mGraphicHandle = NULL;
	mIconTransform = kTransformNone;
	SetTrackInside(false);
	
	*inStream >> mTrackBehaviour;
	*inStream >> mOvalWidth;
	*inStream >> mOvalHeight;
		
	inStream->ReadPString(mTitle);
	*inStream >> mTitleTraitsID;	
	*inStream >> mTitleAlignment;
	*inStream >> mTitlePadPixels;
		
	*inStream >> mGraphicType;
	*inStream >> mGraphicID;
	*inStream >> mGraphicAlignment;
	*inStream >> mGraphicPadPixels;

	if ((mGraphicID != 0) && (mGraphicType == 'cicn'))
		{
		mGraphicHandle = ::GetCIcon(mGraphicID);
		ThrowIfNULL_(mGraphicHandle);
		}
}

// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
//	₯	~CButton
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ

CButton::~CButton()
{
	if (mButtonMask != NULL)
		::DisposeRgn(mButtonMask);
		
	if (mGraphicHandle != NULL)
		::DisposeCIcon(mGraphicHandle);
}

// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
//	₯	InitBevelButton
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ

void CButton::FinishCreateSelf(void)
{
	LControl::FinishCreateSelf();
	
	Rect theButtonRect;
	CalcLocalFrameRect(theButtonRect);
	
	mButtonMask = ::NewRgn();
	ThrowIfNULL_(mButtonMask);
	
	::OpenRgn();
	::FrameRoundRect(&theButtonRect, mOvalWidth, mOvalHeight);
	::CloseRgn(mButtonMask);
}

// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
//	₯	Draw
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ

void CButton::Draw(RgnHandle inSuperDrawRgnH)
{
	Rect theFrame;
	if ((mVisible == triState_On) && CalcPortFrameRect(theFrame) &&
			((inSuperDrawRgnH == nil) || RectInRgn(&theFrame, inSuperDrawRgnH)) && FocusDraw())
		{
		PortToLocalPoint(topLeft(theFrame));	// Get Frame in Local coords
		PortToLocalPoint(botRight(theFrame));

		if (ExecuteAttachments(msg_DrawOrPrint, &theFrame))
			{
			Boolean bDidDraw = false;

			StColorPenState thePenSaver;
			StColorPenState::Normalize();
			
			// Fail safe offscreen drawing
			StValueChanger<EDebugAction> okayToFail(gDebugThrow, debugAction_Nothing);
			try
				{			
				LGWorld theOffWorld(theFrame, 0, useTempMem);

				if (!theOffWorld.BeginDrawing())
					throw memFullErr;
					
				DrawSelf();
					
				theOffWorld.EndDrawing();
				theOffWorld.CopyImage(GetMacPort(), theFrame, srcCopy, mButtonMask);
				bDidDraw = true;
				}
			catch (...)
				{
				// 	& draw onscreen
				}

			if (!bDidDraw)
				{
// FIX ME!!! the mask is not calculated yet!!!!!
				::SetClip(mButtonMask);
				DrawSelf();
				}
			}
		}
}

// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
//	₯	PrepareDrawButton
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ

void CButton::PrepareDrawButton(void)
{
	CalcLocalFrameRect(mCachedButtonFrame);

	// Calculate the drawing mask region.
	::OpenRgn();
	::FrameRoundRect(&mCachedButtonFrame, mOvalWidth, mOvalHeight);
	::CloseRgn(mButtonMask);
	
	CalcTitleFrame();
	if ( mGraphicID != 0 )
		CalcGraphicFrame();
}

// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
//	₯	CalcTitleFrame
//
//	This calculates the bounding box of the title (if any).  This is useful
//	for both the string placement, as well as position the button graphic
//	(again, if any).
//
//	Note that this routine sets the text traits for the ensuing draw.  If
//	you override this method, make sure that you're doing the same.
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ

void CButton::CalcTitleFrame(void)
{
	if (mTitle.Length() == 0)
		return;

	UTextTraits::SetPortTextTraits(mTitleTraitsID);

	FontInfo theInfo;
	::GetFontInfo(&theInfo);
	mCachedTitleFrame.top = mCachedButtonFrame.top;
	mCachedTitleFrame.left = mCachedButtonFrame.left;		
	mCachedTitleFrame.right = mCachedTitleFrame.left + ::StringWidth(mTitle);;
	mCachedTitleFrame.bottom = mCachedTitleFrame.top + theInfo.ascent + theInfo.descent + theInfo.leading;;

	UGraphicGizmos::AlignRectOnRect(mCachedTitleFrame, mCachedButtonFrame, mTitleAlignment);
	UGraphicGizmos::PadAlignedRect(mCachedTitleFrame, mTitlePadPixels, mTitleAlignment);
}

// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
//	₯	CalcGraphicFrame
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ

void CButton::CalcGraphicFrame(void)
{
	if ( mGraphicID == 0 )
		return;
	
	// The container for the graphic starts out as the whole
	// button area.
	Rect theContainerFrame = mCachedButtonFrame;

	Rect theImageFrame;
	if (mGraphicType == 'cicn' && mGraphicHandle != NULL)
		theImageFrame = (**mGraphicHandle).iconPMap.bounds;
	else if (mGraphicType == 'ICN#')
		{
		CIconFamily theFamily(GetGraphicID());
		theFamily.CalcBestSize(theContainerFrame);
		theImageFrame = theFamily;
		}
	else
		theImageFrame = theContainerFrame;
	
	UGraphicGizmos::AlignRectOnRect(theImageFrame, theContainerFrame, mGraphicAlignment);
	UGraphicGizmos::PadAlignedRect(theImageFrame, mGraphicPadPixels, mGraphicAlignment);
	mCachedGraphicFrame = theImageFrame;
}

// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
//	₯	FinalizeDrawButton
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ

void CButton::FinalizeDrawButton(void)
{
}

// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
//	₯	DrawSelf
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ

void CButton::DrawSelf(void)
{
	PrepareDrawButton();

	DrawButtonContent();

	if (mTitle.Length() > 0)
		DrawButtonTitle();

	if (GetGraphicID() != 0)
		DrawButtonGraphic();
			
	if (!IsEnabled() || !IsActive())
		DrawSelfDisabled();
			
	FinalizeDrawButton();
}

// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
//	₯	DrawSelfDisabled
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ

void CButton::DrawSelfDisabled(void)
{
}

// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
//	₯	DrawButtonContent
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ

void CButton::DrawButtonContent(void)
{
}

// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
//	₯	
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ

void CButton::DrawButtonTitle(void)
{
	StColorPenState::Normalize();
	
	if (IsTrackInside() || GetValue() == Button_On)
		::OffsetRect(&mCachedTitleFrame, 1, 1);

	UGraphicGizmos::PlaceStringInRect(mTitle, mCachedTitleFrame, teCenter, teCenter);
}

// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
//	₯	
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ

void CButton::DrawButtonGraphic(void)
{
	if (IsTrackInside() || (GetValue() == Button_On))
		::OffsetRect(&mCachedGraphicFrame, 1, 1);

	if (mGraphicType == 'cicn' && mGraphicHandle != NULL)
		{
		::PlotCIcon(&mCachedGraphicFrame, mGraphicHandle);
		}
	else if (mGraphicType == 'ICN#')
		{
		CIconFamily theFamily(GetGraphicID());
		theFamily.Plot(mCachedGraphicFrame, mGraphicAlignment, mIconTransform);
		}
	else
		{
		::FillRect(&mCachedGraphicFrame, &UQDGlobals::GetQDGlobals()->ltGray);
		::FrameRect(&mCachedGraphicFrame);
		}
}


// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
//	₯
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ

void CButton::ActivateSelf(void)
{
	if (FocusExposed())
		{
		FocusDraw();
		Draw(NULL);
		
		Rect theFrame;
		CalcLocalFrameRect(theFrame);
		::ValidRgn(mButtonMask);
		}
}

// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
//	₯
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ

void CButton::DeactivateSelf(void)
{
	if (FocusExposed())
		{
		FocusDraw();
		Draw(NULL);
		
		Rect theFrame;
		CalcLocalFrameRect(theFrame);
		::ValidRgn(mButtonMask);
		}
}

// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
//	₯
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ

void CButton::EnableSelf(void)
{
	if (FocusExposed())
		{
		FocusDraw();
		Draw(NULL);
		}
}

// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
//	₯
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ

void CButton::DisableSelf(void)
{
	if (FocusExposed())
		{
		FocusDraw();
		Draw(NULL);
		}
}

// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
//	₯	HotSpotAction
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ

void CButton::HotSpotAction(short /* inHotSpot */, Boolean inCurrInside, Boolean inPrevInside)
{
	if (inCurrInside != inPrevInside)
		{
		SetTrackInside(inCurrInside);
		Draw(NULL);
		}
}

// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
//	₯	HotSpotResult
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ

void CButton::HotSpotResult(Int16 inHotSpot)
{
	switch (GetBehaviour())
		{
		case eBehaviour_Button:
			{
			// Undo Button hilighting
			HotSpotAction(inHotSpot, false, true);
			// Although value doesn't change, send message to inform Listeners
			// that button was clicked
			BroadcastValueMessage();
			}
			break;
			
		case eBehaviour_Radio:
			{
			// Leave the button down, and force the value on
			SetTrackInside(false);
			SetValue(Button_On);
			}
			break;
				
		case eBehaviour_Toggle:
			{
			// Leave the button down, and toggle the value
			SetTrackInside(false);
			SetValue(Button_On - mValue);
			}
			break;
			
		default:
			Assert_(false);
			break;
		}	
}

// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
//	₯	SetValue
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ

void CButton::SetValue(Int32 inValue)
{
	if (inValue < mMinValue)		// Enforce min/max range
		inValue = mMinValue;
	else if (inValue > mMaxValue)
		inValue = mMaxValue;

	if (mValue != inValue)
		{							//	If value is not the current value
		mValue = inValue;			//  Store new value
		FocusDraw();
		Draw(NULL);
		BroadcastValueMessage();	//  Inform Listeners of value change
		
		if (IsBehaviourRadio())
			BroadcastMessage(msg_ControlClicked, (void*)this);
		}

}

// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
//	₯	GetDescriptor
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ

StringPtr CButton::GetDescriptor(Str255 outDescriptor) const
{
	::BlockMoveData(&mTitle[0], &outDescriptor[0], mTitle.Length() + 1);
	return (StringPtr)&mTitle[0];
}

// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
//	₯	SetDescriptor
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ

void CButton::SetDescriptor(ConstStr255Param inDescriptor)
{
	mTitle = inDescriptor;
}

// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
//	₯	SetGraphicID
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ

void CButton::SetGraphicID(ResIDT inResID)
{
	// This no longer throws an exception if a requested cicn does not
	// exist, to support CPatternButtons without multiple graphics.
	if (inResID != mGraphicID)
		{
		CIconHandle graphicHandle = NULL;
		
		if (mGraphicType == 'cicn')
			{		
			graphicHandle = ::GetCIcon(inResID);
			if (graphicHandle == NULL)
				return;
			}
		
		if (mGraphicHandle != NULL)
			::DisposeCIcon(mGraphicHandle);
		
		mGraphicHandle = graphicHandle;
		
		}

	mGraphicID = inResID;
}

// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
//	₯	GetGraphicID
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ

ResIDT CButton::GetGraphicID(void) const
{
	return mGraphicID;
}

#pragma mark -

// ---------------------------------------------------------------------------
//		₯ HandleEnablingPolicy
// ---------------------------------------------------------------------------

void
CButton::HandleEnablingPolicy()
{
	LCommander* theTarget		= LCommander::GetTarget();
	MessageT	command			= GetValueMessage();
	Boolean 	enabled			= false;
	Boolean		usesMark		= false;
	Str255		outName;
	Char16		outMark;
		
	if (!CTargetedUpdateMenuRegistry::UseRegistryToUpdateMenus() ||
			CTargetedUpdateMenuRegistry::CommandInRegistry(command))
	{
		if (!IsActive() || !IsVisible())
			return;
			
		if (!theTarget)
			return;
		
		theTarget->ProcessCommandStatus(command, enabled, usesMark, outMark, outName);
		
		if (enabled)
		{
			Enable();
		}
		else
		{
			Disable();
		}
		
		// This code not only enables and disables toolbar buttons (above)
		// but also (if a toggle button) to set the correct state (pushed or not).
		// The code below was added for use in the editor; other areas may
		// want this functionality.  This could also be done in a subclass... 
		
		if (usesMark)
		{
			if (IsBehaviourToggle())
			{
				StSetBroadcasting setBroadcasting(this, false);	// Stop broadcasting while we change the value

				// Why the cast here - why? If we really want to execute LControl's SetValue
				// and not a derived class, then explain it in the code!
				
				((LControl *) this)->SetValue (outMark == checkMark ? Button_On : Button_Off);	
										// hilite == true if there's a Γ
			}
		}
	}
}