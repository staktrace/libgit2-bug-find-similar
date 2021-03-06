/*
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


#ifndef MISC_I
#define MISC_I

#include "xpresdef.h"

BEGIN_STR (MISC_strings)

ResDef(XP_BKMKS_HOURS_AGO,                      XP_MSG_BASE+282,
 "%ld hours ago")

ResDef(XP_BKMKS_DAYS_AGO,                       XP_MSG_BASE+283,
 "%ld days ago")


ResDef(XP_BKMKS_COUNTALIASES_MANY,              XP_MSG_BASE+284,
 "There are %ld aliases to this %s")

ResDef(XP_BKMKS_COUNTALIASES_ONE,               XP_MSG_BASE+285,
 "There is 1 alias to this %s")

ResDef(XP_BKMKS_COUNTALIASES_NONE,              XP_MSG_BASE+286,
 "There are no aliases to this %s")

ResDef(XP_BKMKS_INVALID_NICKNAME,               XP_MSG_BASE+287,
 "Nicknames may only have letters and numbers\n\
in them.  The nickname has not been changed.")

ResDef(XP_BKMKS_NICKNAME_ALREADY_EXISTS,        XP_MSG_BASE+288,
 "An entry with this nickname already exists.\n\
The nickname has not been changed.")

ResDef(XP_BKMKS_REMOVE_THIS_ITEMS_ALIASES,        XP_MSG_BASE+289,
 "This item has %d alias(es). These aliases \n\
will be removed, as well.")				

ResDef(XP_BKMKS_REMOVE_SOME_ITEMS_ALIASES,        XP_MSG_BASE+290,
 "Some of the items you are about to remove \n\
have one or more aliases. The aliases will \n\
be removed, as well.")

ResDef(XP_BKMKS_AUTOGENERATED_FILE,             XP_MSG_BASE+291,
 "<!-- This is an automatically generated file.")

ResDef(XP_BKMKS_READ_AND_OVERWRITE,             XP_MSG_BASE+292,
 "It will be read and overwritten.")

ResDef(XP_BKMKS_DO_NOT_EDIT,                    XP_MSG_BASE+293,
 "Do Not Edit! -->")

ResDef(XP_BKMKS_NEW_HEADER,                     XP_MSG_BASE+294,
 "New Folder")

ResDef(XP_BKMKS_NEW_BOOKMARK,                   XP_MSG_BASE+295,
 "New Bookmark")

ResDef(XP_BKMKS_NOT_FOUND,                      XP_MSG_BASE+296,
 "Not Found")

ResDef(XP_BKMKS_OPEN_BKMKS_FILE,                XP_MSG_BASE+297,
 "Open bookmarks file")

ResDef(XP_BKMKS_IMPORT_BKMKS_FILE,              XP_MSG_BASE+298,
"Import bookmarks file")

ResDef(XP_BKMKS_SAVE_BKMKS_FILE,                XP_MSG_BASE+299,
 "Save bookmarks file")

ResDef(XP_BKMKS_LESS_THAN_ONE_HOUR_AGO,         XP_MSG_BASE+300,
 "Less than one hour ago")

 ResDef(XP_GLHIST_DATABASE_CLOSED,               XP_MSG_BASE+301,
 "The global history database is currently closed")

ResDef(XP_GLHIST_UNKNOWN,                       XP_MSG_BASE+302,
 "Unknown")

ResDef(XP_GLHIST_DATABASE_EMPTY,                XP_MSG_BASE+303,
"The global history database is currently empty")

ResDef(XP_GLHIST_HTML_DATE,                     XP_MSG_BASE+304,
 "<BR>\n<TT>Date:</TT> %s<P>")

ResDef(XP_GLHIST_HTML_TOTAL_ENTRIES,            XP_MSG_BASE+305,
 "\n<HR>\n<TT>Total entries:</TT> %ld<P>")

 /* this isn't used, I think.  Shall we remove it? */
ResDef(XP_HOTLIST_DEF_NAME,                     XP_MSG_BASE+306,
 "Personal Bookmarks") 

ResDef(XP_HOTLIST_AUTOGENERATED_FILE,           XP_MSG_BASE+307,
 "<!-- This is an automatically generated file.\n\
It will be read and overwritten.\n\
Do Not Edit! -->\n")

ResDef(XP_GLHIST_INFO_HTML,                     XP_MSG_BASE + 551,
 "<TITLE>Information about the Netscape global history</TITLE>\n\
<h2>Global history entries</h2>\n\
<HR>")

ResDef(XP_BKMKS_SOMEONE_S_BOOKMARKS,	XP_MSG_BASE + 591,
       "%s%s's Bookmarks%s")

ResDef(XP_BKMKS_PERSONAL_BOOKMARKS,	XP_MSG_BASE + 592,
       "%sPersonal Bookmarks%s")

ResDef(XP_BKMKS_SOMEONE_S_ADDRESSBOOK,	XP_MSG_BASE + 593,
       "%s%s's Address book%s")

ResDef(XP_BKMKS_PERSONAL_ADDRESSBOOK,	XP_MSG_BASE + 594,
       "%sPersonal Address book%s")

ResDef(XP_BKMKS_IMPORT_ADDRBOOK, XP_MSG_BASE + 636,
"Import address book file")

ResDef(XP_BKMKS_SAVE_ADDRBOOK, XP_MSG_BASE + 637,
"Save address book file")

ResDef(XP_BKMKS_BOOKMARK,   XP_MSG_BASE + 638,
"bookmark" )

ResDef(XP_BKMKS_ENTRY,   XP_MSG_BASE + 639,
"entry" )

ResDef(XP_BKMKS_SECONDS,   XP_MSG_BASE + 640,
"%ld seconds" )

ResDef(XP_BKMKS_MINUTES,   XP_MSG_BASE + 641,
"%ld minutes" )

ResDef(XP_BKMKS_HOURS_MINUTES,   XP_MSG_BASE + 642,
"%ld hours %ld minutes")

ResDef(XP_BKMKS_HEADER,   XP_MSG_BASE + 643,
"Main Bookmarks" )

ResDef(XP_ADDRBOOK_HEADER,   XP_MSG_BASE + 644,
"Address Book" )

ResDef(MK_MSG_WRAP_LONG_LINES, XP_MSG_BASE + 645,
"Wrap long lines" )


END_STR (MISC_strings)

#endif
