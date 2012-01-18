/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 2012-01-16
 * Description : Maintenance tool using thumbnails load thread as items processor.
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

#ifndef MAINTENANCETHUMBTOOL_H
#define MAINTENANCETHUMBTOOL_H

// Qt includes

#include <QPixmap>

// Local includes

#include "maintenancetool.h"

namespace Digikam
{

class LoadingDescription;
class ThumbnailLoadThread;

class MaintenanceThumbTool : public MaintenanceTool
{
    Q_OBJECT

public:

    MaintenanceThumbTool(const QString& id, Mode mode=AllItems, int albumId=-1);
    virtual ~MaintenanceThumbTool();

protected:

    /** Return thumbs loader instance
     */
    ThumbnailLoadThread* thumbsLoadThread()  const;

    /** Re-implement this if you want to use thumb loader as items processor
     */
    virtual void gotNewThumbnail(const LoadingDescription&, const QPixmap&) {};

private Q_SLOTS:

    /** Called by thumbnail thread. This slot call gotNewThumbnail()
     */
    void slotGotThumbnail(const LoadingDescription&, const QPixmap&);

private:

    class MaintenanceThumbToolPriv;
    MaintenanceThumbToolPriv* const d;
};

}  // namespace Digikam

#endif /* MAINTENANCETHUMBTOOL_H */
