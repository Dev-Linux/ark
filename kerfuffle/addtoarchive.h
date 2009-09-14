/*
 * ark -- archiver for the KDE project
 *
 * Copyright (C) 2008 Harald Hvaal <haraldhv (at@at) stud.ntnu.no>
 * Copyright (C) 2009 Raphael Kubo da Costa <kubito@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#ifndef ADDTOARCHIVE_H
#define ADDTOARCHIVE_H

#include "kerfuffle_export.h"

#include <KJob>
#include <KUrl>

/**
 * Compresses all input files into an archive.
 *
 * This is a job class that creates a compressed archive
 * with all the given input files.
 *
 * It provides the functionality for the --add command-line
 * option, and does not need the GUI to be running.
 *
 * @author Harald Hvaal <haraldhv@stud.ntnu.no>
 */
namespace Kerfuffle
{
class KERFUFFLE_EXPORT AddToArchive : public KJob
{
    Q_OBJECT

public:
    AddToArchive(QObject *parent = 0);
    ~AddToArchive();

    bool showAddDialog();
    void setPreservePaths(bool value);
    void setChangeToFirstPath(bool value);

public slots:
    bool addInput(const KUrl& url);
    void setAutoFilenameSuffix(const QString& suffix);
    void setFilename(const KUrl& path);
    void setMimeType(const QString & mimeType);
    void start();

private slots:
    void slotFinished(KJob*);
    void slotStartJob();

private:
    QString m_filename;
    QString m_strippedPath;
    QString m_autoFilenameSuffix;
    QString m_firstPath;
    QString m_mimeType;
    QStringList m_inputs;
    bool m_changeToFirstPath;
};
}

#endif // ADDTOARCHIVE_H

