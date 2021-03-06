/*
 * Copyright (c) 2010-2011 Raphael Kubo da Costa <rakuco@FreeBSD.org>
 * Copyright (c) 2016 Elvis Angelaccio <elvis.angelaccio@kde.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES ( INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION ) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * ( INCLUDING NEGLIGENCE OR OTHERWISE ) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "archive_kerfuffle.h"
#include "pluginmanager.h"
#include "jobs.h"
#include "testhelper.h"

#include <QDirIterator>
#include <QStandardPaths>
#include <QTest>

using namespace Kerfuffle;

class ExtractTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testExtraction_data();
    void testExtraction();
};

QTEST_GUILESS_MAIN(ExtractTest)

void ExtractTest::testExtraction_data()
{
    QTest::addColumn<QString>("archivePath");
    QTest::addColumn<QVector<Archive::Entry*>>("entriesToExtract");
    QTest::addColumn<ExtractionOptions>("extractionOptions");
    QTest::addColumn<int>("expectedExtractedEntriesCount");

    ExtractionOptions optionsPreservePaths;

    ExtractionOptions optionsNoPaths;
    optionsNoPaths.setPreservePaths(false);

    ExtractionOptions dragAndDropOptions;
    dragAndDropOptions.setDragAndDropEnabled(true);

    QString archivePath = QFINDTESTDATA("data/simplearchive.tar.gz");
    QTest::newRow("extract the whole simplearchive.tar.gz")
            << archivePath
            << QVector<Archive::Entry*>()
            << optionsPreservePaths
            << 4;

    archivePath = QFINDTESTDATA("data/simplearchive.tar.gz");
    QTest::newRow("extract selected entries from a tar.gz, without paths")
            << archivePath
            << QVector<Archive::Entry*> {
                   new Archive::Entry(this, QStringLiteral("aDir/b.txt"), QStringLiteral("aDir")),
                   new Archive::Entry(this, QStringLiteral("c.txt"), QString())
               }
            << optionsNoPaths
            << 2;

    archivePath = QFINDTESTDATA("data/simplearchive.tar.gz");
    QTest::newRow("extract selected entries from a tar.gz, preserve paths")
            << archivePath
            << QVector<Archive::Entry*> {
                   new Archive::Entry(this, QStringLiteral("aDir/b.txt"), QStringLiteral("aDir")),
                   new Archive::Entry(this, QStringLiteral("c.txt"), QString())
               }
            << optionsPreservePaths
            << 3;

    archivePath = QFINDTESTDATA("data/simplearchive.tar.gz");
    QTest::newRow("extract selected entries from a tar.gz, drag-and-drop")
            << archivePath
            << QVector<Archive::Entry*> {
                   new Archive::Entry(this, QStringLiteral("c.txt"), QString()),
                   new Archive::Entry(this, QStringLiteral("aDir/b.txt"), QStringLiteral("aDir/"))
               }
            << dragAndDropOptions
            << 2;

    archivePath = QFINDTESTDATA("data/one_toplevel_folder.zip");
    QTest::newRow("extract the whole one_toplevel_folder.zip")
            << archivePath
            << QVector<Archive::Entry*>()
            << optionsPreservePaths
            << 9;

    archivePath = QFINDTESTDATA("data/one_toplevel_folder.zip");
    QTest::newRow("extract selected entries from a zip, without paths")
            << archivePath
            << QVector<Archive::Entry*> {
                   new Archive::Entry(this, QStringLiteral("A/test2.txt"), QStringLiteral("A")),
                   new Archive::Entry(this, QStringLiteral("A/B/test1.txt"), QStringLiteral("A/B"))
               }
            << optionsNoPaths
            << 2;

    archivePath = QFINDTESTDATA("data/one_toplevel_folder.zip");
    QTest::newRow("extract selected entries from a zip, preserve paths")
            << archivePath
            << QVector<Archive::Entry*> {
                   new Archive::Entry(this, QStringLiteral("A/test2.txt"), QStringLiteral("A")),
                   new Archive::Entry(this, QStringLiteral("A/B/test1.txt"), QStringLiteral("A/B"))
               }
            << optionsPreservePaths
            << 4;

    archivePath = QFINDTESTDATA("data/one_toplevel_folder.zip");
    QTest::newRow("extract selected entries from a zip, drag-and-drop")
            << archivePath
            << QVector<Archive::Entry*> {
                   new Archive::Entry(this, QStringLiteral("A/test2.txt"), QStringLiteral("A/")),
                   new Archive::Entry(this, QStringLiteral("A/B/C/"), QStringLiteral("A/B/")),
                   new Archive::Entry(this, QStringLiteral("A/B/C/test1.txt"), QStringLiteral("A/B/")),
                   new Archive::Entry(this, QStringLiteral("A/B/C/test2.txt"), QStringLiteral("A/B/"))
               }
            << dragAndDropOptions
            << 4;

    archivePath = QFINDTESTDATA("data/one_toplevel_folder.7z");
    QTest::newRow("extract the whole one_toplevel_folder.7z")
            << archivePath
            << QVector<Archive::Entry*>()
            << optionsPreservePaths
            << 9;

    archivePath = QFINDTESTDATA("data/one_toplevel_folder.7z");
    QTest::newRow("extract selected entries from a 7z, without paths")
            << archivePath
            << QVector<Archive::Entry*> {
                   new Archive::Entry(this, QStringLiteral("A/test2.txt"), QStringLiteral("A")),
                   new Archive::Entry(this, QStringLiteral("A/B/test1.txt"), QStringLiteral("A/B"))
               }
            << optionsNoPaths
            << 2;

    archivePath = QFINDTESTDATA("data/one_toplevel_folder.7z");
    QTest::newRow("extract selected entries from a 7z, preserve paths")
            << archivePath
            << QVector<Archive::Entry*> {
                   new Archive::Entry(this, QStringLiteral("A/test2.txt"), QStringLiteral("A")),
                   new Archive::Entry(this, QStringLiteral("A/B/test1.txt"), QStringLiteral("A/B"))
               }
            << optionsPreservePaths
            << 4;

    archivePath = QFINDTESTDATA("data/one_toplevel_folder.7z");
    QTest::newRow("extract selected entries from a 7z, drag-and-drop")
            << archivePath
            << QVector<Archive::Entry*> {new Archive::Entry(this, QStringLiteral("A/B/test2.txt"), QStringLiteral("A/B/"))}
            << dragAndDropOptions
            << 1;

    archivePath = QFINDTESTDATA("data/empty_folders.zip");
    QTest::newRow("zip with empty folders")
            << archivePath
            << QVector<Archive::Entry*>()
            << optionsPreservePaths
            << 5;

    archivePath = QFINDTESTDATA("data/empty_folders.tar.gz");
    QTest::newRow("tar with empty folders")
            << archivePath
            << QVector<Archive::Entry*>()
            << optionsPreservePaths
            << 5;

    archivePath = QFINDTESTDATA("data/simplearchive.tar.bz2");
    QTest::newRow("extract selected entries from a bzip2-compressed tarball without path")
            << archivePath
            << QVector<Archive::Entry*> {
                   new Archive::Entry(this, QStringLiteral("file3.txt"), QString()),
                   new Archive::Entry(this, QStringLiteral("dir2/file22.txt"), QString())
               }
            << optionsNoPaths
            << 2;

    archivePath = QFINDTESTDATA("data/simplearchive.tar.bz2");
    QTest::newRow("extract all entries from a bzip2-compressed tarball with path")
            << archivePath
            << QVector<Archive::Entry*>()
            << optionsPreservePaths
            << 7;

    archivePath = QFINDTESTDATA("data/simplearchive.tar.xz");
    QTest::newRow("extract selected entries from a xz-compressed tarball without path")
            << archivePath
            << QVector<Archive::Entry*> {
                   new Archive::Entry(this, QStringLiteral("file3.txt"), QString()),
                   new Archive::Entry(this, QStringLiteral("dir2/file22.txt"), QString())
               }
            << optionsNoPaths
            << 2;

    archivePath = QFINDTESTDATA("data/simplearchive.tar.xz");
    QTest::newRow("extract all entries from a xz-compressed tarball with path")
            << archivePath
            << QVector<Archive::Entry*>()
            << optionsPreservePaths
            << 7;

    archivePath = QFINDTESTDATA("data/simplearchive.tar.lzma");
    QTest::newRow("extract selected entries from a lzma-compressed tarball without path")
            << archivePath
            << QVector<Archive::Entry*> {
                   new Archive::Entry(this, QStringLiteral("file3.txt"), QString()),
                   new Archive::Entry(this, QStringLiteral("dir2/file22.txt"), QString())
               }
            << optionsNoPaths
            << 2;

    archivePath = QFINDTESTDATA("data/simplearchive.tar.lzma");
    QTest::newRow("extract all entries from a lzma-compressed tarball with path")
            << archivePath
            << QVector<Archive::Entry*>()
            << optionsPreservePaths
            << 7;

    archivePath = QFINDTESTDATA("data/simplearchive.tar.Z");
    QTest::newRow("extract selected entries from a compress (.Z)-compressed tarball without path")
            << archivePath
            << QVector<Archive::Entry*> {
                   new Archive::Entry(this, QStringLiteral("file3.txt"), QString()),
                   new Archive::Entry(this, QStringLiteral("dir2/file22.txt"), QString())
               }
            << optionsNoPaths
            << 2;

    archivePath = QFINDTESTDATA("data/simplearchive.tar.Z");
    QTest::newRow("extract all entries from a compress (.Z)-compressed tarball with path")
            << archivePath
            << QVector<Archive::Entry*>()
            << optionsPreservePaths
            << 7;

    archivePath = QFINDTESTDATA("data/simplearchive.tar.lz");
    QTest::newRow("extract selected entries from a lzipped tarball without path")
            << archivePath
            << QVector<Archive::Entry*> {
                   new Archive::Entry(this, QStringLiteral("file3.txt"), QString()),
                   new Archive::Entry(this, QStringLiteral("dir2/file22.txt"), QString())
               }
            << optionsNoPaths
            << 2;

    archivePath = QFINDTESTDATA("data/simplearchive.tar.lz");
    QTest::newRow("extract all entries from a lzipped tarball with path")
            << archivePath
            << QVector<Archive::Entry*>()
            << optionsPreservePaths
            << 7;

    // Only run tests if tar.lzo format is available
    if (PluginManager().supportedMimeTypes().contains(QStringLiteral("application/x-tzo"))) {
        archivePath = QFINDTESTDATA("data/simplearchive.tar.lzo");
        QTest::newRow("extract selected entries from a lzop-compressed tarball without path")
                << archivePath
                << QVector<Archive::Entry*> {
                       new Archive::Entry(this, QStringLiteral("file3.txt"), QString()),
                       new Archive::Entry(this, QStringLiteral("dir2/file22.txt"), QString())
                   }
                << optionsNoPaths
                << 2;

        archivePath = QFINDTESTDATA("data/simplearchive.tar.lzo");
        QTest::newRow("extract all entries from a lzop-compressed tarball with path")
                << archivePath
                << QVector<Archive::Entry*>()
                << optionsPreservePaths
                << 7;
    }

    // Only run test for lrzipped tar if lrzip executable is found in path.
    if (!QStandardPaths::findExecutable(QStringLiteral("lrzip")).isEmpty()) {
        archivePath = QFINDTESTDATA("data/simplearchive.tar.lrz");
        QTest::newRow("extract selected entries from a lrzip-compressed tarball without path")
                << archivePath
                << QVector<Archive::Entry*> {
                       new Archive::Entry(this, QStringLiteral("file3.txt"), QString()),
                       new Archive::Entry(this, QStringLiteral("dir2/file22.txt"), QString())
                   }
                << optionsNoPaths
                << 2;

        archivePath = QFINDTESTDATA("data/simplearchive.tar.lrz");
        QTest::newRow("extract all entries from a lrzip-compressed tarball with path")
                << archivePath
                << QVector<Archive::Entry*>()
                << optionsPreservePaths
                << 7;
    } else {
        qDebug() << "lrzip executable not found in path. Skipping lrzip test.";
    }

    // Only run test for lz4-compressed tar if lz4 executable is found in path.
    if (!QStandardPaths::findExecutable(QStringLiteral("lz4")).isEmpty()) {
        archivePath = QFINDTESTDATA("data/simplearchive.tar.lz4");
        QTest::newRow("extract selected entries from a lz4-compressed tarball without path")
                << archivePath
                << QVector<Archive::Entry*> {
                       new Archive::Entry(this, QStringLiteral("file3.txt"), QString()),
                       new Archive::Entry(this, QStringLiteral("dir2/file22.txt"), QString())
                   }
                << optionsNoPaths
                << 2;

        archivePath = QFINDTESTDATA("data/simplearchive.tar.lz4");
        QTest::newRow("extract all entries from a lz4-compressed tarball with path")
                << archivePath
                << QVector<Archive::Entry*>()
                << optionsPreservePaths
                << 7;
    } else {
        qDebug() << "lz4 executable not found in path. Skipping lz4 test.";
    }

    archivePath = QFINDTESTDATA("data/simplearchive.xar");
    QTest::newRow("extract selected entries from a xar archive without path")
            << archivePath
            << QVector<Archive::Entry*> {
                   new Archive::Entry(this, QStringLiteral("dir1/file11.txt"), QString()),
                   new Archive::Entry(this, QStringLiteral("file4.txt"), QString())
               }
            << optionsNoPaths
            << 2;

    archivePath = QFINDTESTDATA("data/simplearchive.xar");
    QTest::newRow("extract all entries from a xar archive with path")
            << archivePath
            << QVector<Archive::Entry*>()
            << optionsPreservePaths
            << 6;

    archivePath = QFINDTESTDATA("data/hello-1.0-x86_64.AppImage");
    QTest::newRow("extract all entries from an AppImage with path")
            << archivePath
            << QVector<Archive::Entry*>()
            << optionsPreservePaths
            << 7;

    archivePath = QFINDTESTDATA("data/hello-1.0-x86_64.AppImage");
    QTest::newRow("extract selected entries from an AppImage with path")
            << archivePath
            << QVector<Archive::Entry*> {
                   new Archive::Entry(this, QStringLiteral("usr/bin/hello"), QString())
               }
            << optionsPreservePaths
            << 3;

    archivePath = QFINDTESTDATA("data/archive-multivolume.7z.001");
    QTest::newRow("extract all entries from a multivolume 7z archive with path")
            << archivePath
            << QVector<Archive::Entry*>()
            << optionsPreservePaths
            << 3;

    archivePath = QFINDTESTDATA("data/archive-multivolume.part1.rar");
    QTest::newRow("extract all entries from a multivolume rar archive with path")
            << archivePath
            << QVector<Archive::Entry*>()
            << optionsPreservePaths
            << 3;

    archivePath = QFINDTESTDATA("data/firmware-pine64-20160329-6.1.aarch64.rpm");
    QTest::newRow("extract selected entries from rpm with path")
            << archivePath
            << QVector<Archive::Entry*> {
                   new Archive::Entry(this, QStringLiteral("boot/sunxi-spl.bin"), QString())
               }
            << optionsPreservePaths
            << 2;

    archivePath = QFINDTESTDATA("data/firmware-pine64-20160329-6.1.aarch64.rpm");
    QTest::newRow("#369535: broken drag-and-drop from rpm")
            << archivePath
            << QVector<Archive::Entry*> {
                   new Archive::Entry(this, QStringLiteral("boot/sunxi-spl.bin"), QStringLiteral("boot/")),
                   new Archive::Entry(this, QStringLiteral("boot/u-boot.img"), QStringLiteral("boot/"))
               }
            << dragAndDropOptions
            << 2;
}

void ExtractTest::testExtraction()
{
    QFETCH(QString, archivePath);
    auto loadJob = Archive::load(archivePath, this);
    QVERIFY(loadJob);
    loadJob->setAutoDelete(false);

    TestHelper::startAndWaitForResult(loadJob);
    auto archive = loadJob->archive();
    QVERIFY(archive);

    if (!archive->isValid()) {
        QSKIP("Could not find a plugin to handle the archive. Skipping test.", SkipSingle);
    }

    QTemporaryDir destDir;
    if (!destDir.isValid()) {
        QSKIP("Could not create a temporary directory for extraction. Skipping test.", SkipSingle);
    }

    QFETCH(QVector<Archive::Entry*>, entriesToExtract);
    QFETCH(ExtractionOptions, extractionOptions);
    auto extractionJob = archive->extractFiles(entriesToExtract, destDir.path(), extractionOptions);
    QVERIFY(extractionJob);
    extractionJob->setAutoDelete(false);

    TestHelper::startAndWaitForResult(extractionJob);

    QFETCH(int, expectedExtractedEntriesCount);
    int extractedEntriesCount = 0;

    QDirIterator dirIt(destDir.path(), QDir::AllEntries | QDir::Hidden | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    while (dirIt.hasNext()) {
        extractedEntriesCount++;
        dirIt.next();
    }

    QCOMPARE(extractedEntriesCount, expectedExtractedEntriesCount);

    loadJob->deleteLater();
    extractionJob->deleteLater();
    archive->deleteLater();
}

#include "extracttest.moc"
