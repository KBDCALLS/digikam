/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
 *
 * Date        : 20013-08-22
 * Description : Reimplemented QListView for Tags Manager, with support for
 *               drag-n-drop
 *
 * Copyright (C) 2013 by Veaceslav Munteanu <veaceslav dot munteanu90 at gmail dot com>
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

#ifndef DIGIKAM_TAG_MNGR_LIST_VIEW_H
#define DIGIKAM_TAG_MNGR_LIST_VIEW_H

// Qt includes

#include <QTreeView>

class QPoint;

namespace Digikam
{

class TagMngrListView : public QTreeView
{
    Q_OBJECT

public:

    explicit TagMngrListView(QWidget* const parent = nullptr);

    QModelIndexList mySelectedIndexes();

protected:

    /**
     * Reimplemented methods to enable custom drag-n-drop in QListView
     */
    void startDrag(Qt::DropActions supportedActions) override;
    void dropEvent(QDropEvent *e) override;

    QModelIndex indexVisuallyAt(const QPoint& p);

    /**
     * @brief contextMenuEvent - reimplemented method from QListView
     *                           to handle custom context menu
     */
    void contextMenuEvent(QContextMenuEvent* event) override;


public Q_SLOTS:

    /**
     * @brief slotDeleteSelected - delete selected item from Quick Access List
     */
    void slotDeleteSelected();
};

} // namespace Digikam

#endif // DIGIKAM_TAG_MNGR_LIST_VIEW_H
