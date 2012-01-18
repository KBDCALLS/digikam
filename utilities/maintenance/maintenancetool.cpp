/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 2012-01-16
 * Description : Maintenance tool class
 *
 * Copyright (C) 2012 by Gilles Caulier <caulier dot gilles at gmail dot com>
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

#include "maintenancetool.moc"

// Qt includes

#include <QTimer>
#include <QDir>
#include <QFileInfo>
#include <QDateTime>
#include <QPixmap>

// KDE includes

#include <kapplication.h>
#include <kcodecs.h>
#include <klocale.h>
#include <kapplication.h>

// Local includes

#include "album.h"
#include "albumdb.h"
#include "albuminfo.h"
#include "albummanager.h"
#include "imageinfo.h"
#include "databaseaccess.h"
#include "knotificationwrapper.h"

namespace Digikam
{

class MaintenanceTool::MaintenanceToolPriv
{
public:

    MaintenanceToolPriv() :
        cancel(false),
        mode(MaintenanceTool::AllItems),
        albumId(-1)
    {
        duration.start();
    }

    bool                  cancel;
    QTime                 duration;

    // Managed by thumbLoadThread or previewLoadThread as items processors.
    QStringList           allPicturesPath;

    MaintenanceTool::Mode mode;
    int                   albumId;
};

MaintenanceTool::MaintenanceTool(const QString& id, Mode mode, int albumId)
    : ProgressItem(0,
                   id,
                   QString(),
                   QString(),
                   true,
                   true),
      d(new MaintenanceToolPriv)
{
    d->mode              = mode;
    d->albumId           = albumId;

    connect(this, SIGNAL(progressItemCanceled(ProgressItem*)),
            this, SLOT(slotCancel()));

    QTimer::singleShot(500, this, SLOT(slotRun()));
}

MaintenanceTool::~MaintenanceTool()
{
    delete d;
}

void MaintenanceTool::setTitle(const QString& title)
{
    QString label = QString("%1: ").arg(title);

    switch(d->mode)
    {
        case AllItems:
            label.append(i18n("process all items"));
            break;
        case MissingItems:
            label.append(i18n("process missing items"));
            break;
        case AlbumItems:
            label.append(i18n("process album items"));
            break;
    }

    setLabel(label);
}

bool MaintenanceTool::cancel() const
{
    return d->cancel;
}

QStringList& MaintenanceTool::allPicturesPath()
{
    return d->allPicturesPath;
}

MaintenanceTool::Mode MaintenanceTool::mode() const
{
    return d->mode;
}

void MaintenanceTool::slotRun()
{
    if (ProgressManager::addProgressItem(this))
        populateAllPicturesPath();
}

void MaintenanceTool::populateAllPicturesPath()
{
    // Get all digiKam albums collection pictures path.
    AlbumList palbumList;

    if (d->mode != AlbumItems)
    {
        palbumList  = AlbumManager::instance()->allPAlbums();
    }
    else
    {
        palbumList.append(AlbumManager::instance()->findPAlbum(d->albumId));
    }

    for (AlbumList::const_iterator it = palbumList.constBegin();
         !d->cancel && (it != palbumList.constEnd()); ++it )
    {
        if (!(*it))
        {
            continue;
        }

        d->allPicturesPath += DatabaseAccess().db()->getItemURLsInAlbum((*it)->id());
    }

    listItemstoProcess();

    if (d->allPicturesPath.isEmpty())
    {
        slotCancel();
        return;
    }

    setTotalItems(d->allPicturesPath.count());
    processOne();
}

bool MaintenanceTool::checkToContinue() const
{
    if (d->cancel || d->allPicturesPath.isEmpty())
    {
        return false;
    }

    return true;
}

void MaintenanceTool::complete()
{
    setComplete();
    QTime now, t = now.addMSecs(d->duration.elapsed());
    // Pop-up a message to bring user when all is done.
    KNotificationWrapper(id(),
                         i18n("Process is done.\nDuration: %1", t.toString()),
                         kapp->activeWindow(), label());
    emit signalProcessDone();
}

void MaintenanceTool::slotCancel()
{
    d->cancel = true;
    emit signalProcessDone();
    setComplete();
}

}  // namespace Digikam
