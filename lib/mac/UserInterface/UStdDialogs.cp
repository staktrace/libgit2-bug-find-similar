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

#include <LPane.h>
#include <LString.h>
#include <LCaption.h>
#include <LIconPane.h>
#include <UTextTraits.h>
#include <LStdControl.h>

#include "UStdDialogs.h"
#include "UProcessUtils.h"
#include "CNotificationAttachment.h"

#include "UTextBox.h"	// for line height calc function
#include "resgui.h"		// for res id's

short	gSaveAsType = 0;	// SaveAsHook stores file type in this variable
short	gUploadAsType = 1;	// UploadAsHook stores file type in this variable

const	PaneIDT	okAlertBtnID = 900;
const	PaneIDT	cancelAlertBtnID = 901;




// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
//	₯	StStdDialogHandler
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ

StStdDialogHandler::StStdDialogHandler(
	ResIDT 			inDialogID,
	LCommander* 	inSuper)
	:	StDialogHandler(inDialogID, inSuper)
{
	mDialogResID = inDialogID;
}

// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
//	₯	WaitUserResponse
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ

MessageT StStdDialogHandler::WaitUserResponse(void)
{
//	SysBeep(1);
	MessageT theMessage = msg_Nothing;
	while(theMessage == msg_Nothing)
		theMessage = DoDialog();
		
	return theMessage;
}

// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
//	₯	
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ

void StStdDialogHandler::SetInitialDialogPosition(LWindow* inParentWindow)
{
	Int16 thePlacement = CalcDialogPlacement();
	if (thePlacement == noAutoCenter)
		return;

	Rect theTargetRect;
	CalcDialogTargetBounds(thePlacement, inParentWindow, theTargetRect);

	// Now that we have the screen we can center the window ...
	LWindow* theDialogWindow = GetDialog();
	SDimension16 theWindowDims;
	theDialogWindow->GetFrameSize(theWindowDims);
	Int16 targetHeight = theTargetRect.bottom - theTargetRect.top;
	Int16 targetWidth = theTargetRect.right - theTargetRect.left;
	
	Point thePosition;
	
	switch (thePlacement)
		{
		case alertPositionMainScreen:
		case alertPositionParentWindow:
		case alertPositionParentWindowScreen:
			// Place the window at the alert position (20% down) ...
			thePosition.v = theTargetRect.top + (targetHeight - theWindowDims.height) / 5;
			thePosition.h = theTargetRect.left + (targetWidth - theWindowDims.width) / 2;
			break;
			
		case centerMainScreen:
		case centerParentWindow:
		case centerParentWindowScreen:
			// Center it vertically
			thePosition.v = theTargetRect.top + (targetHeight - theWindowDims.height) / 2;
			thePosition.h = theTargetRect.left + (targetWidth - theWindowDims.width) / 2;
			break;
			
		case staggerParentWindowScreen:
		case staggerParentWindow:
		case staggerMainScreen:
		default:
			Assert_(false);
			break;
		}
			
	theDialogWindow->DoSetPosition(thePosition);
}

// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
//	₯	
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ

void StStdDialogHandler::CalcDialogTargetBounds(
	Int16 				inPlacement,
	LWindow*			inParentWindow,
	Rect&				outBounds)
{
	Boolean bFindWindowScreen = (inPlacement == centerParentWindowScreen) 			||
								(inPlacement == alertPositionParentWindowScreen)	||
								(inPlacement == staggerParentWindowScreen);

	GDHandle gdh = GetDeviceList();
	if (inParentWindow == NULL)
		{
		GDHandle theGDH = ::GetMainDevice();
		outBounds = (*theGDH)->gdRect;
		outBounds.top += GetMBarHeight();
		}
	else
		{
		Rect theWindowBounds;
		inParentWindow->CalcPortFrameRect(theWindowBounds);
		inParentWindow->PortToGlobalPoint(topLeft(theWindowBounds));
		inParentWindow->PortToGlobalPoint(botRight(theWindowBounds));
		
		if (bFindWindowScreen)
			{
			// Find out on which screen the window exists ...
			Int32 theMaxArea = 0;
			while (gdh != NULL)
				{
				if (::TestDeviceAttribute(gdh, screenDevice) && TestDeviceAttribute(gdh, screenActive))
					{
					Rect theSect;
					if (::SectRect(&theWindowBounds, &(*gdh)->gdRect, &theSect))
						{
						Int32 theArea = (Int32)(theSect.right - theSect.left) * (theSect.bottom - theSect.top);
						if (theArea > theMaxArea)
							{
							theMaxArea = theArea;
							outBounds = (*gdh)->gdRect;
							if (TestDeviceAttribute(gdh, mainScreen))
								outBounds.top += GetMBarHeight();
							}
						}
					}
				gdh = ::GetNextDevice(gdh);
				}
				
			// If we failed then just center the window on the main screen ...
			if (theMaxArea == 0)
				CalcDialogTargetBounds(alertPositionMainScreen, NULL, outBounds);
			}
		else
			{
			// just use the windows global bounds
			outBounds = theWindowBounds;
			}
		}

}

// Handy macros
LPane* StStdDialogHandler::GetPane(PaneIDT id)
{
	LPane *pane = mDialog->FindPaneByID(id); 
	SignalIf_(!pane); 
	return pane;
}

void StStdDialogHandler::HidePane(PaneIDT id)
{
	GetPane(id)->Hide();
}

void StStdDialogHandler::EnablePane(PaneIDT id)
{
	GetPane(id)->Enable();
}

void StStdDialogHandler::DisablePane(PaneIDT id)
{
	GetPane(id)->Disable();
}

void StStdDialogHandler::ShowPane(PaneIDT id)
{
	GetPane(id)->Show();
}

void StStdDialogHandler::SetBoolean(PaneIDT id, Boolean value)
{
	GetControl(id)->SetValue(value);
}

Boolean StStdDialogHandler::GetBoolean(PaneIDT id)
{
	return GetControl(id)->GetValue();
}

void StStdDialogHandler::SetValue(PaneIDT id, Int32 value)
{
	GetControl(id)->SetValue(value);
}

Int32 StStdDialogHandler::GetValue(PaneIDT id)
{
	return GetControl(id)->GetValue();
}

void StStdDialogHandler::SetText(PaneIDT id, ConstStr255Param value)
{
	GetPane(id)->SetDescriptor(value);
}

void StStdDialogHandler::SetNumberText(PaneIDT id, SInt32 value)
{
	Str255 s;
	::NumToString(value, s);
	GetPane(id)->SetDescriptor(s);
}

void StStdDialogHandler::GetText(PaneIDT id, Str255 value)
{
	GetPane(id)->GetDescriptor(value);
}

SInt32 StStdDialogHandler::GetNumberText(PaneIDT id)
{
	Str255 s;
	SInt32 value;
	GetPane(id)->GetDescriptor(s);
	::StringToNum(s, &value);
	return value;
}

// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
//	₯	
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
						
Int16 StStdDialogHandler::CalcDialogPlacement(void)
{
	Int16 thePlacement = noAutoCenter;
	StResource theWind('WIND', mDialogResID);
	
	Int32 theResSize = ::GetHandleSize(theWind);
	thePlacement = *(Int16*)(((Ptr)(*theWind.mResourceH)) + (theResSize - sizeof(thePlacement)));
	return thePlacement;
}

// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
//	₯	SetCaption
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ

void StStdDialogHandler::SetCaption(ConstStringPtr inCaption)
{
	LCaption* theCaption = (LCaption*)(GetDialog()->FindPaneByID(PaneID_AlertCaption));
	Assert_(theCaption != NULL);
	theCaption->SetDescriptor(inCaption);
}

// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
//	
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ

StAlertHandler::StAlertHandler(
	ConstStringPtr		 	inAlertText,
	EAlertType				inAlertType,
	LCommander*				inSuper)
	:	StStdDialogHandler(WIND_StandardAlert, inSuper)
{
	SetCaption(inAlertText);

	TString<Str255> theAlertTitle(16000, inAlertType);
	GetDialog()->SetDescriptor(theAlertTitle);
	
	SetAlertIcon(inAlertType);
}

StAlertHandler::StAlertHandler(
	ResIDT					inStrListID,
	Int16 					inIndex,
	EAlertType				inAlertType,
	LCommander*				inSuper)
	:	StStdDialogHandler(WIND_StandardAlert, inSuper)
{
	TString<Str255> theCaptionString(inStrListID, inIndex);
	SetCaption(theCaptionString);
	
	TString<Str255> theAlertTitle(16000, inAlertType);
	GetDialog()->SetDescriptor(theAlertTitle);
	
	SetAlertIcon(inAlertType);
}
			


void StAlertHandler::SetAlertIcon(EAlertType inAlertType)
{
	ResIDT theAlertIconID = inAlertType + 10000 - 1;
	if (inAlertType == eAlertTypeError)
		theAlertIconID--;
		
	LWindow* theDialog = GetDialog();
	LIconPane* theIconPane = (LIconPane*)theDialog->FindPaneByID(PaneID_AlertIcon);
	Assert_(theIconPane != NULL);
	theIconPane->SetIconID(theAlertIconID);
}

// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
//	₯ StAutoSizingDialog
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ

StAutoSizingDialog::StAutoSizingDialog(
	ResIDT					inDialogID,
	const CString&			inAlertText,
	const CString&			inDefaultEditText,
	LCommander*				inSuper)
	:	StStdDialogHandler(inDialogID, inSuper)
{
	SetCaption(inAlertText);
	
	LEditField *theEditField = (LEditField *)(GetDialog()->FindPaneByID(PaneID_AlertEditField));
	Assert_(theEditField != NULL);
	theEditField->SetDescriptor(inDefaultEditText);
}

// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
//	₯ SetInitialDialogSize
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ

void StAutoSizingDialog::SetInitialDialogSize(void)
{
	LWindow* theDialog = GetDialog();
	LCaption *theCaption = (LCaption *)theDialog->FindPaneByID(PaneID_AlertCaption);
	Assert_(theCaption != NULL);
	
	CStr255 theCaptionString;
	theCaption->GetDescriptor(theCaptionString);
		
		// fill boundsRect with dimensions of caption pane
	Rect theCaptionFrame;
	theCaption->CalcPortFrameRect(theCaptionFrame);
	
		// set text traits, just in case...
	UTextTraits::SetPortTextTraits(theCaption->GetTextTraitsID());

		// now call line height calc function (this routine destroys the original string)
	Int16 theHeight = UTextBox::TextBoxDialogHeight((Ptr)&theCaptionString[1], theCaptionString.Length(), 
										&theCaptionFrame, teForceLeft, 0);
					
	// this was in macdlg.cp, is it really necessary?
//	if( height < 16 )
//		height = 16;
		
	Int16 theHeightDifference = theHeight - (theCaptionFrame.bottom - theCaptionFrame.top);
	theCaption->ResizeFrameBy(0, theHeightDifference, false);

	// set position of edit field
	LEditField *theEditField = (LEditField *)theDialog->FindPaneByID(PaneID_AlertEditField);
	Assert_(theEditField != NULL);
	SPoint32 theEditLocation;
	theEditField->GetFrameLocation(theEditLocation);
	theEditField->PlaceInSuperFrameAt(theEditLocation.h, theEditLocation.v + theHeightDifference, false);

	// Now we resize the dialog by the amount that the caption text
	// changed.  Note that the Ok and Cacel buttons will automatically
	// adjust because we have their bottom and right bindings set.
	Rect theDialogBounds;
	theDialog->CalcPortFrameRect(theDialogBounds);
	theDialogBounds.bottom += theHeightDifference;
	
	theDialog->PortToGlobalPoint(topLeft(theDialogBounds));
	theDialog->PortToGlobalPoint(botRight(theDialogBounds));
	theDialog->DoSetBounds(theDialogBounds);
}

// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
//	₯	Alert
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ

void UStdDialogs::Alert(
	ConstStringPtr 		inAlertText,
	EAlertType			inType,
	LWindow*			inOverWindow,
	LCommander*			inSuper,
	LStr255*			inWindowTitle
	)
{
	if (inSuper == NULL)
		inSuper = LCommander::GetTopCommander();
		
	StAlertHandler theHandler(inAlertText, inType, inSuper);
	theHandler.SetInitialDialogPosition(inOverWindow);
	LWindow* theDialog = theHandler.GetDialog();
	
	if (inWindowTitle)
		theDialog->SetDescriptor(*inWindowTitle);
		
	if (UStdDialogs::TryToInteract())
		{
			theDialog->Show();
			MessageT theMessage = theHandler.WaitUserResponse();
		}
}

Boolean
UStdDialogs::AskWithCustomButtons(
	ConstStringPtr 		inQuestion,
	ConstStringPtr		inDefaultTextBtn,
	ConstStringPtr		inCancelTextBtn,
	LWindow*			inOverWindow,
	LCommander*			inSuper,
	LStr255*			inWindowTitle
	)
{
	if (inSuper == NULL)
		inSuper = LCommander::GetTopCommander();

	StStdDialogHandler theHandler(WIND_OkCancelAlert, inSuper);
	theHandler.SetCaption(inQuestion);
	theHandler.SetInitialDialogPosition(inOverWindow);
	LWindow* theDialog = theHandler.GetDialog();

	if (inWindowTitle)
		theDialog->SetDescriptor(*inWindowTitle);

	if ( inDefaultTextBtn )
	{
		LStdButton* okBtnPtr = dynamic_cast<LStdButton*>( theDialog->FindPaneByID( okAlertBtnID ) );
		if( okBtnPtr )
			okBtnPtr->SetDescriptor( inDefaultTextBtn );
	}
		
	if ( inCancelTextBtn )
	{
		LStdButton* cancelBtnPtr = dynamic_cast<LStdButton*>( theDialog->FindPaneByID( cancelAlertBtnID ) );
		if( cancelBtnPtr )
			cancelBtnPtr->SetDescriptor( inCancelTextBtn );
	}

	MessageT theMessage = msg_Cancel;
	if (UStdDialogs::TryToInteract())
		{
		theDialog->Show();
		theMessage = theHandler.WaitUserResponse();
		}
	
	return (theMessage == msg_OK);
}

// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
//	₯	AskOkCancel
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ

Boolean UStdDialogs::AskOkCancel(
	ConstStringPtr		inQuestion,
	LWindow*			inOverWindow,
	LCommander*			inSuper,
	LStr255*			inWindowTitle)
{
	return AskWithCustomButtons(
		inQuestion,
		nil, // default "Quit"
		nil, // default "Cancel"
		inOverWindow,
		inSuper,
		inWindowTitle
		);
}

// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
//	₯	AskStandardTextPrompt
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ

Boolean UStdDialogs::AskStandardTextPrompt(
	const CStr255&			inTitleText,
	const CStr255&			inPromptText,
	CString&				ioString,
	LWindow*				inOverWindow,
	LCommander*				inSuper,
	Int32					maxIOStringSize)
{
	if (inSuper == NULL)
		inSuper = LCommander::GetTopCommander();

	StAutoSizingDialog theHandler(WIND_Resizeable, inPromptText, ioString, inSuper);
	theHandler.SetInitialDialogSize();
	theHandler.SetInitialDialogPosition(inOverWindow);
	LWindow* theDialog = theHandler.GetDialog();

	MessageT theMessage = msg_Cancel;
	if (UStdDialogs::TryToInteract())
		{
		LEditField* theResponseEdit = (LEditField*)theDialog->FindPaneByID(PaneID_AlertEditField);
		Assert_(theResponseEdit != NULL);
		theResponseEdit->SetDescriptor(ioString);
		theResponseEdit->SelectAll();
		theResponseEdit->SetMaxChars(maxIOStringSize);
		theHandler.SetLatentSub(theResponseEdit);

		theDialog->SetDescriptor(inTitleText);
		theDialog->Show();
		theMessage = theHandler.WaitUserResponse();

		if (theMessage == msg_OK)
			theResponseEdit->GetDescriptor(ioString);
		}
	
	return (theMessage == msg_OK);
}

// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
//	₯	AskForPassword
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ

Boolean UStdDialogs::AskForPassword(
	const CString&			inPromptText,
	CString&				ioString,
	LWindow*				inOverWindow,
	LCommander*				inSuper)
{
	if (inSuper == NULL)
		inSuper = LCommander::GetTopCommander();

	StAutoSizingDialog theHandler(WIND_Resizeable, inPromptText, ioString, inSuper);
	theHandler.SetInitialDialogSize();
	theHandler.SetInitialDialogPosition(inOverWindow);
	LWindow* theDialog = theHandler.GetDialog();

	MessageT theMessage = msg_Cancel;
	if (UStdDialogs::TryToInteract())
		{
		LEditField* theResponseEdit = (LEditField*)theDialog->FindPaneByID(PaneID_AlertEditField);
		Assert_(theResponseEdit != NULL);
		theResponseEdit->SetTextTraitsID(cPasswordTextTraitsID);
		theResponseEdit->SetDescriptor(ioString);
		theResponseEdit->SelectAll();
		theDialog->SetLatentSub(theResponseEdit);
		
		theDialog->Show();
		theMessage = theHandler.WaitUserResponse();

		if (theMessage == msg_OK)
			theResponseEdit->GetDescriptor(ioString);
		}
	
	return (theMessage == msg_OK);
}

// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
//	₯	AskForNameAndPassword
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ

Boolean UStdDialogs::AskForNameAndPassword(
	const CString&		inPromptText,
	CString&			ioNameString,
	CString&			ioPassString,
	LWindow*					inOverWindow,
	LCommander*					inSuper)
{
	if (inSuper == NULL)
		inSuper = LCommander::GetTopCommander();

	StStdDialogHandler theHandler(WIND_NameAndPassword, inSuper);
	theHandler.SetInitialDialogPosition(inOverWindow);
	LWindow* theDialog = theHandler.GetDialog();

	MessageT theMessage = msg_Cancel;
	if (UStdDialogs::TryToInteract())
		{
		LCaption* theMessageCaption = (LCaption*)theDialog->FindPaneByID(PaneID_MessageCaption);
		Assert_(theMessageCaption != NULL);
		theMessageCaption->SetDescriptor(inPromptText);
		
		LEditField* theNameField = (LEditField*)theDialog->FindPaneByID(PaneID_PasswordEditName);
		Assert_(theNameField != NULL);
		theNameField->SetDescriptor(ioNameString);
		
		LEditField* thePassField = (LEditField*)theDialog->FindPaneByID(PaneID_PasswordEditPass);
		Assert_(thePassField != NULL);
		thePassField->SetDescriptor(ioPassString);
		
		if (ioNameString.Length() == 0)
			{
			theDialog->SetLatentSub(theNameField);
			theNameField->SelectAll();
			}
		else
			{
			theDialog->SetLatentSub(thePassField);
			thePassField->SelectAll();
			}

		theHandler.SetLatentSub(theNameField);
		
		theDialog->Show();
		theMessage = theHandler.WaitUserResponse();

		if (theMessage == msg_OK)
			{
			theNameField->GetDescriptor(ioNameString);
			thePassField->GetDescriptor(ioPassString);
			}
		}

	return (theMessage == msg_OK);
}

// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
//	₯	TryToInteract
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ

Boolean UStdDialogs::TryToInteract(Int32 inForSeconds)
{
	ProcessSerialNumber theCurrentProcess;
	::GetCurrentProcess(&theCurrentProcess);
		
	Boolean isInFront = UProcessUtils::IsFrontProcess(theCurrentProcess);
	if (!isInFront)
		{
		LEventDispatcher* theDispatcher = LEventDispatcher::GetCurrentEventDispatcher();

		// NOTE:  this attachment is being attached to the current event
		// dispatcher.  Therefore notification will be killed when the app
		// resumes or the dispatcher goes out of scope.  In the case of the
		// UStdDialog alerts, the notification will go away when this routine
		// times out.

		CNotificationAttachment* theNote = new CNotificationAttachment;
		theDispatcher->AddAttachment(theNote);

		Uint32 theStartTime, theCurrentTime;
		::GetDateTime(&theStartTime);
		theCurrentTime = theStartTime;
		while (((theCurrentTime - theStartTime) < inForSeconds) && !isInFront)
			{
			EventRecord theEvent;
			::WaitNextEvent(everyEvent, &theEvent, 10, NULL);
			theDispatcher->DispatchEvent(theEvent);
			::GetDateTime(&theCurrentTime);
			
			isInFront = UProcessUtils::IsFrontProcess(theCurrentProcess);
			}
		}
		
	return isInFront;
}

// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
//	₯ DoSaveAsTextOrSource
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ

// NOTE: I don't call ErrorManager::PrepareToInteract which makes sure
// Netscape is frontmost app and uses Notification Mangler if we aren't.

OSErr UStdDialogs::AskSaveAsTextOrSource(StandardFileReply& reply,
											const CString& initialFilename,
											short& format)
{
	Point dlgTopLeft;
	dlgTopLeft.h = dlgTopLeft.v = -1;
	
	if (gSaveAsType != 1 && gSaveAsType != 2)
		gSaveAsType = 1;
	
	DlgHookYDUPP savehook = NewDlgHookYDProc(SaveAsHook);

	LStr255 saveAsPromptStr(MACDLG_SAVE_AS, 0);	
	
	UDesktop::Deactivate();
	::CustomPutFile(saveAsPromptStr,
					initialFilename,
					&reply,
					DLOG_SAVEAS,
					dlgTopLeft,
					savehook,
					NULL,
					NULL,
					NULL,
					NULL);
	UDesktop::Activate();

#if GENERATINGCFM
	::DisposeRoutineDescriptor(savehook);
#endif // GENERATINGCFM
	
	format = gSaveAsType;
	return noErr;	// uhh, why bother returning a value?
}


// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
//	₯ SaveAsHook
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ

pascal short UStdDialogs::SaveAsHook( short item, DialogPtr dialog, void* /*dataPtr*/ )
{
	short 				myType;				//	menu item selected
	Handle				myHandle;			//	needed for GetDialogItem
	Rect				myRect;				//	needed for GetDialogItem

#define kFileTypePopup	13

	if ( GetWRefCon( (WindowPtr)dialog ) != sfMainDialogRefCon )
		return item;					// this function is only for main dialog box

	if ( item == sfHookFirstCall)
	{
		GetDialogItem( dialog, kFileTypePopup, &myType, &myHandle, &myRect );
		
		if ( myHandle )
			SetControlValue( (ControlHandle)myHandle,  gSaveAsType);
	}
	else
	if ( item == kFileTypePopup )
	{
		GetDialogItem( dialog, kFileTypePopup, &myType, &myHandle, &myRect );
		
		if ( myHandle )
			gSaveAsType = GetControlValue( (ControlHandle)myHandle );
	}
	
	return item;
}

// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
//	₯ AskSaveAsSource
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ

// NOTE: I don't call ErrorManager::PrepareToInteract which makes sure
// Netscape is frontmost app and uses Notification Mangler if we aren't.

OSErr UStdDialogs::AskSaveAsSource(StandardFileReply& reply,
											const CString& initialFilename,
											short& format)
{
	Point dlgTopLeft;
	dlgTopLeft.h = dlgTopLeft.v = -1;
	
	gSaveAsType = 2;		// Set save as type to source
	
	LStr255 saveAsPromptStr(MACDLG_SAVE_AS, 0);	
	
	UDesktop::Deactivate();
	::StandardPutFile(saveAsPromptStr,
				initialFilename,
				&reply);
	UDesktop::Activate();

	format = gSaveAsType;
	return noErr;	// uhh, why bother returning a value?
}


// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
//	₯ AskUploadAsDataOrMacBin
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ

// NOTE: I don't call ErrorManager::PrepareToInteract which makes sure
// Netscape is frontmost app and uses Notification Mangler if we aren't.

OSErr UStdDialogs::AskUploadAsDataOrMacBin(StandardFileReply& reply,
											short& format)
{
	SFTypeList	typeList;
	Point		dlgTopLeft;
	dlgTopLeft.h = dlgTopLeft.v = -1;
	
	DlgHookYDUPP uploadhook = NewDlgHookYDProc(UploadAsHook);

	UDesktop::Deactivate();
	::CustomGetFile(
					NULL,
					-1,
					typeList,
					&reply,
					DLOG_UPLOADAS,
					dlgTopLeft,
					uploadhook,
					NULL,
					NULL,
					NULL,
					NULL);
	UDesktop::Activate();

#if GENERATINGCFM
	::DisposeRoutineDescriptor(uploadhook);
#endif // GENERATINGCFM
	
	format = gUploadAsType;
	return noErr;	// uhh, why bother returning a value?
}

// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ
//	₯ UploadAsHook
// ΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡΡ

pascal short UStdDialogs::UploadAsHook( short item, DialogPtr dialog, void* /*dataPtr*/ )
{
	short 				myType;				//	menu item selected
	Handle				myHandle;			//	needed for GetDialogItem
	Rect				myRect;				//	needed for GetDialogItem

#define kUploadTypePopup	10	// WARNING!!!  Due to an embedded help item if you use Resorcerer's
								//				option to show dialog item #s this item appears as #9
								//				and baaaad things happen if you don't account for the
								//				off by 1 numbering.

	if ( GetWRefCon( (WindowPtr)dialog ) != sfMainDialogRefCon )
		return item;					// this function is only for main dialog box

	if ( item == sfHookFirstCall)
	{
		GetDialogItem( dialog, kUploadTypePopup, &myType, &myHandle, &myRect );
		
		if ( myHandle )
			SetControlValue( (ControlHandle)myHandle,  gUploadAsType);
	}
	else
	if ( item == kUploadTypePopup )
	{
		GetDialogItem( dialog, kUploadTypePopup, &myType, &myHandle, &myRect );
		
		if ( myHandle )
			gUploadAsType = GetControlValue( (ControlHandle)myHandle );
	}
	
	return item;
}

