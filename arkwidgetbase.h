/*
  Copyright (C)

  2001: Corel Corporation (author: Michael Jarrett <michaelj@corel.com>)

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

*/

#ifndef ARKWIDGETBASE_H
#define ARKWIDGETBASE_H

#include "arch.h"

class QWidget;
class QString;
class QStringList;

class Arch;
class FileListView;
class ArkSettings;

/**
* Common functionality between ArkWidget and ArkWidgetPart, to avoid
* code duplication.
*/
class ArkWidgetBase
{
// Methods
public:
	// Accessors
	QWidget *getArkWidget() const { return m_widget; }

	bool isArchiveOpen() const { return m_bIsArchiveOpen; }
	QString getArchName() const { return m_strArchName; }
	FileListView *fileList() const { return archiveContent; }
	int getNumFilesInArchive() const { return m_nNumFiles; }
	bool dragInProgress() const { return m_bDragInProgress; }
	int getCol(const QString &);
	QString getColData(const QString &_filename, int _col);
	int getSizeColumn();

	// Mutators
	void setHeaders(QStringList *headers, int *_rightAlignCols,
			int _numColsToAlignRight);
	void clearHeaders();
	void listingAdd(QStringList *_entries);
	void setDragInProgress(bool _b) { m_bDragInProgress = _b; }
	void storeDragNames(QStringList &dragFiles) { mDragFiles = dragFiles; }

	// Others
	void viewShellOutput();
	void unarchFile(QStringList * _l) { arch->unarchFile(_l); }
	void prepareViewFiles(QStringList *fileList);

protected:
	ArkWidgetBase(QWidget *widget);
	~ArkWidgetBase();

// Data
protected:
	// We need to see a widget representation of ourselves.
	// Could be any parent, if needs be.
	QWidget *m_widget;

	Arch *arch;
	ArkSettings *m_settings;
	FileListView *archiveContent;
	QString m_strArchName;

	int m_nSizeOfFiles;
	int m_nSizeOfSelectedFiles;
	int m_nNumFiles;
	int m_nNumSelectedFiles;

	int m_currentSizeColumn;

	bool m_bIsArchiveOpen;
	bool m_bIsSimpleCompressedFile;
	bool m_bDragInProgress;
	bool m_bDropSourceIsSelf;

	QStringList mDragFiles;
	QStringList *m_extractList;
};


#endif
