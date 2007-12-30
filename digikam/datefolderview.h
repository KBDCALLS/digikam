/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 * 
 * Date        : 2005-04-27
 * Descritpion : a folder view for date albums.
 *
 * Copyright (C) 2005 by Renchi Raju <renchi@pooh.tam.uiuc.edu>
 * Copyright (C) 2006-2007 by Gilles Caulier <caulier dot gilles at gmail dot com>
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

#ifndef DATEFOLDERVIEW_H
#define DATEFOLDERVIEW_H

// Qt includes.

#include <qvbox.h>

// Local includes.

#include "albummanager.h"
#include "folderitem.h"

class QListViewItem;

namespace Digikam
{

class DateFolderViewPriv;
class DAlbum;

class DateFolderItem : public FolderItem
{

public:

    DateFolderItem(QListView* parent, const QString& name);
    DateFolderItem(QListViewItem* parent, const QString& name, DAlbum* album);
    
    ~DateFolderItem();
    
    void refresh();

    int     compare(QListViewItem *i, int, bool) const;
    QString date() const;
    QString name() const;
    
    DAlbum* album() const;
    
    int count() const;
    void setCount(int v);

private:

    int               m_count;
    
    QString           m_name;

    DAlbum           *m_album;
};

// -----------------------------------------------------------------

class DateFolderView : public QVBox
{
    Q_OBJECT
    
public:

    DateFolderView(QWidget* parent);
    ~DateFolderView();

    void setActive(bool val);

    void setSelected(QListViewItem *item);

    void gotoDate(const QDate& dt);
    void refresh();

private slots:

    void slotAllDAlbumsLoaded();
    void slotAlbumAdded(Album* album);
    void slotAlbumDeleted(Album* album);
    void slotSelectionChanged();
    void slotRefresh(const QMap<YearMonth, int>&);

private:

    /**
     * load the last view state from disk
     */
    void loadViewState();
    
    /**
     * writes the view state to disk
     */
    void saveViewState();

    QListViewItem *findRootItemByYear(const QString& year);
    
    DateFolderViewPriv* d;
};

}  // namespace Digikam

#endif /* DATEFOLDERVIEW_H */
