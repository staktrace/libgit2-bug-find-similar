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
/* */
/*
 *----------------------------------------------------------------------------
 *
 *  EditorToolbar.h --- Toolbar for Editor and HTML Mail Compose.
 *
 *  Created: David Williams <djw@netscape.com>, Feb-7-1997
 *  RCSID: "$Id: EditorToolbar.h,v 3.1 1998/03/28 03:26:25 ltabb Exp $"
 *
 *----------------------------------------------------------------------------
 */


#ifndef _xfe_editor_toolbar_h_
#define _xfe_editor_toolbar_h_

#include "ToolboxItem.h"
#include "Command.h"
#include "Frame.h"

class XFE_ComponentList;

class XFE_EditorToolbar : public XFE_ToolboxItem
{
public:
	XFE_EditorToolbar(XFE_Component* parent_frame,
					  XFE_Toolbox *	parent_toolbox,
					  char*        name,
					  ToolbarSpec* spec,
					  Boolean      show_frame);

	~XFE_EditorToolbar();

	// contract:
	void   update();
	Widget findButton(const char*, EChromeTag);
	void   updateCommand(CommandType);

	// local:
	XFE_Frame* getParentFrame() { return (XFE_Frame*)m_toplevel; }
	Widget     getChildrenManager() { return m_rowcol; };
	void       show();

private:
	XFE_ComponentList* m_update_list;
	Widget             m_rowcol;
};

class XFE_AbstractMenuItem : public XFE_Component
{
public:
	// methods
	virtual void update() { };
	virtual CommandType getCmdId() = 0;
	virtual XP_Bool showsUpdate() { return TRUE; };
	
};

class XFE_MenuItem : public XFE_AbstractMenuItem
{
public:
	XFE_MenuItem(XFE_Component* parent) {
		m_toplevel = parent->getToplevel();
		m_parent = parent;
	}
	XFE_Frame* getParentFrame() {
		return (XFE_Frame*)getToplevel();
	}
	XFE_Component* getParent() {
		return m_parent;
	}
protected:
	// data
	XFE_Component* m_parent;
};

class XFE_ActionMenuItem : public XFE_MenuItem
{
public:
	XFE_ActionMenuItem(XFE_Component* parent, CommandType id)
		: XFE_MenuItem(parent) {
		m_cmd_id = id;
		m_cmd_handler = NULL;
	}
	CommandType getCmdId() {
		return m_cmd_id;
	}
	void doCommand(XFE_CommandInfo* info) {
		if (m_cmd_handler != NULL)
			m_cmd_handler->doCommand(getParentFrame(), info);
		else
			getParentFrame()->doCommand(m_cmd_id, NULL, info);
	}
	XP_Bool showsUpdate();
protected:
	// data
	CommandType    m_cmd_id;
	XFE_Command*   m_cmd_handler;
};

class XFE_EditorToolbarItem : public XFE_ActionMenuItem
{
public:
	XFE_EditorToolbarItem(XFE_Component* tb, ToolbarSpec* spec) 
	: XFE_ActionMenuItem(tb, 0) {
		m_spec = spec;
		if (spec != NULL)
			m_cmd_id = (CommandType)m_spec->toolbarButtonName;
		else
			m_cmd_id = 0; // FIXME
	}
protected:
	// data
	ToolbarSpec*   m_spec;
};

class XFE_MenuSpecItem : public XFE_ActionMenuItem
{
public:
	XFE_MenuSpecItem(XFE_Component* tb, MenuSpec* spec) 
	: XFE_ActionMenuItem(tb, 0) {
		m_spec = spec;
		if (spec != NULL)
			m_cmd_id = m_spec->menuItemName;
		else
			m_cmd_id = 0; // FIXME
	}
protected:
	// data
	MenuSpec* m_spec;
};

class XFE_EditorToolbarPushButton : public XFE_EditorToolbarItem
{
public:
	XFE_EditorToolbarPushButton(Widget         parent,
								ToolbarSpec*   spec,
								XFE_Component* tb);
	void update();
};

class XFE_EditorToolbarToggleButton : public XFE_EditorToolbarItem
{
public:
	XFE_EditorToolbarToggleButton(Widget parent,
								  ToolbarSpec* spec,
								  XFE_Component* tb);
								  
	void update();
};

class XFE_EditorToolbarRadioButton : public XFE_EditorToolbarToggleButton
{
public:
	XFE_EditorToolbarRadioButton(Widget parent,
								 ToolbarSpec* spec,
								 XFE_Component* tb);
};

class XFE_EditorToolbarSpacer : public XFE_EditorToolbarItem
{
public:
	XFE_EditorToolbarSpacer(Widget parent, XFE_Component* tb);
	XP_Bool showsUpdate() { return FALSE; };
};

#endif /*_xfe_editor_toolbar_h_*/



