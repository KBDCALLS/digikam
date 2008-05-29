/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 2008-05-19
 * Description : Fuzzy search sidebar tab contents.
 *
 * Copyright (C) 2008 by Gilles Caulier <caulier dot gilles at gmail dot com>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#ifndef FUZZYSEARCHVIEW_H
#define FUZZYSEARCHVIEW_H

// Qt includes.

#include <QWidget>

namespace Digikam
{

class SAlbum;
class SearchTextBar;
class FuzzySearchFolderView;
class FuzzySearchViewPriv;

class FuzzySearchView : public QWidget
{
    Q_OBJECT

public:

    FuzzySearchView(QWidget *parent=0);
    ~FuzzySearchView();

    FuzzySearchFolderView* folderView() const;
    SearchTextBar* searchBar() const;

    void setActive(bool val);

private: 

    void readConfig();
    void writeConfig();

    void createNewFuzzySearchAlbumFromSketch(const QString& name);
    void createNewFuzzySearchAlbumFromImage(const QString& name);

    bool checkName(QString& name);
    bool checkAlbum(const QString& name) const;

private slots:

    void slotHSChanged(int h, int s);
    void slotVChanged();
    void slotClearSketch();
    void slotDirtySketch();
    void slotSaveSketch();
    void slotAlbumSelected(SAlbum*);
    void slotCheckNameEditConditions();
    void slotRenameAlbum(SAlbum*);

private:

    FuzzySearchViewPriv *d;
};

}  // NameSpace Digikam

#endif /* FUZZYSEARCHVIEW_H */
