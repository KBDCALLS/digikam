/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 2013-02-25
 * Description : Table view column helpers: Thumbnail column
 *
 * Copyright (C) 2013 by Michael G. Hansen <mike at mghansen dot de>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#include "tableview_column_thumbnail.moc"

// Qt includes

#include <QPainter>
#include <QStyleOptionViewItem>

// KDE includes

#include <klocale.h>

// local includes

#include "imagefiltermodel.h"
#include "tableview.h"
#include "thumbnailloadthread.h"
#include "thumbnailsize.h"

namespace
{
    const int ThumbnailBorder = 2;
}

namespace Digikam
{

namespace TableViewColumns
{

ColumnThumbnail::ColumnThumbnail(
        TableViewShared* const tableViewShared,
        const TableViewColumnConfiguration& pConfiguration,
        QObject* const parent)
  : TableViewColumn(tableViewShared, pConfiguration, parent),
    m_thumbnailSize(s->tableView->getThumbnailSize().size())
{
    connect(s->thumbnailLoadThread, SIGNAL(signalThumbnailLoaded(LoadingDescription,QPixmap)),
            this, SLOT(slotThumbnailLoaded(LoadingDescription,QPixmap)));
}

ColumnThumbnail::~ColumnThumbnail()
{

}

TableViewColumnDescription ColumnThumbnail::getDescription()
{
    return TableViewColumnDescription(QLatin1String("thumbnail"), QLatin1String("Thumbnail"));
}

TableViewColumn::ColumnFlags ColumnThumbnail::getColumnFlags() const
{
    return ColumnCustomPainting;
}

QString ColumnThumbnail::getTitle() const
{
    return i18n("Thumbnail");
}

QVariant ColumnThumbnail::data(const QModelIndex& sourceIndex, const int role) const
{
    Q_UNUSED(sourceIndex)
    Q_UNUSED(role)

    // we do not return any data, but paint(...) something
    return QVariant();
}

bool ColumnThumbnail::paint(QPainter* const painter, const QStyleOptionViewItem& option, const QModelIndex& sourceIndex) const
{
    if (option.state & QStyle::State_Selected)
    {
        painter->fillRect(option.rect, option.palette.highlight());
    }

    const ImageInfo info = getImageInfo(sourceIndex);
    if (!info.isNull())
    {
        const QSize imageSize = info.dimensions();
        const QSize availableSize = option.rect.size() - QSize(ThumbnailBorder, ThumbnailBorder);

        QSize size(m_thumbnailSize, m_thumbnailSize);
        if (imageSize.isValid() && (imageSize.width()>imageSize.height()) )
        {
            // for landscape pictures, try to use all available horizontal space
            qreal scaleFactor = qreal(availableSize.height()) / qreal(imageSize.height());
            if (qreal(imageSize.width())*scaleFactor > availableSize.width())
            {
                scaleFactor = qreal(availableSize.width()) / qreal(imageSize.width());
            }

            size.setWidth(imageSize.width()*scaleFactor);
        }

        const QString path = info.filePath();
        QPixmap thumbnail;
        if (s->thumbnailLoadThread->find(path, thumbnail, qMax(size.width()/* + 2*/, size.height()/* + 2*/)))
        {
            /// @todo Is slotThumbnailLoaded still called when the thumbnail is found right away?
            /// @todo Remove borders - but they actually look nice in the table
            const QSize alignSize = option.rect.size();
//                 thumbnail = thumbnail.copy(1, 1, thumbnail.size().width()-2, thumbnail.size().height()-2)
            const QSize pixmapSize    = thumbnail.size().boundedTo(availableSize);
            QPoint startPoint((alignSize.width() - pixmapSize.width()) / 2,
                              (alignSize.height() - pixmapSize.height()) / 2);
            startPoint += option.rect.topLeft();
            painter->drawPixmap(QRect(startPoint, pixmapSize), thumbnail, QRect(QPoint(0, 0), pixmapSize));

            return true;
        }
    }

    // we did not get to paint a thumbnail...
    return false;
}

QSize ColumnThumbnail::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& sourceIndex) const
{
    Q_UNUSED(option)
    Q_UNUSED(sourceIndex)

    /// @todo On portrait pictures, the borders are too close. There should be a gap. Is this setting okay?
    const int thumbnailSizeWithBorder = m_thumbnailSize+ThumbnailBorder;
    return QSize(thumbnailSizeWithBorder, thumbnailSizeWithBorder);
}

void ColumnThumbnail::slotThumbnailLoaded(const LoadingDescription& loadingDescription, const QPixmap& thumb)
{
    if (thumb.isNull())
    {
        return;
    }

    const QModelIndex sourceIndex = s->imageFilterModel->indexForPath(loadingDescription.filePath);
    if (!sourceIndex.isValid())
    {
        return;
    }

    // The model will convert the sourceIndex to the index it needs
    // and find out the column number and set it.
    emit(signalDataChanged(sourceIndex));
}

void ColumnThumbnail::updateThumbnailSize()
{
    /// @todo Set minimum column width to m_thumbnailSize

    m_thumbnailSize = s->tableView->getThumbnailSize().size();

    emit(signalAllDataChanged());
}

} /* namespace TableViewColumns */

} /* namespace Digikam */

