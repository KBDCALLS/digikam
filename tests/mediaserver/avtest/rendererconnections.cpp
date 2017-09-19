/*
 *  Copyright (C) 2011 Tuomo Penttinen, all rights reserved.
 *
 *  Author: Tuomo Penttinen <tp@herqq.org>
 *
 *  This file is part of an application named HUpnpAvSimpleTestApp
 *  used for demonstrating how to use the Herqq UPnP A/V (HUPnPAv) library.
 *
 *  HUpnpAvSimpleTestApp is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  HUpnpAvSimpleTestApp is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with HUpnpAvSimpleTestApp. If not, see <http://www.gnu.org/licenses/>.
 */

#include "rendererconnections.h"

#include "hduration.h"
#include "hseekinfo.h"
#include "hmediainfo.h"
#include "htransportstate.h"

#include <QtCore/QUrl>
#include <QtCore/QTime>
#include <QtCore/QtDebug>
#include <QtCore/QBuffer>

#include <QtGui/QPixmap>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QtGui/QResizeEvent>

#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>

// QtAV includes

#include <QtAV/AudioOutput.h>

using namespace Herqq::Upnp;
using namespace Herqq::Upnp::Av;

/*******************************************************************************
 * CustomRendererConnection
 *******************************************************************************/
CustomRendererConnection::CustomRendererConnection(QObject* parent) :
    HRendererConnection(parent)
{
}

void CustomRendererConnection::resizeEventOccurred(const QResizeEvent&)
{
}

/*******************************************************************************
 * RendererConnectionForImagesAndText
 *******************************************************************************/
RendererConnectionForImagesAndText::RendererConnectionForImagesAndText(
    ContentType ct, QNetworkAccessManager& nam, QWidget* parent) :
        CustomRendererConnection(parent),
            m_contentType(ct), m_nam(nam), m_currentResource(0),
            m_currentData(), m_showWhenReady(false), m_stopped(true)
{
    if (m_contentType == Text)
    {
        m_textEdit = new QTextEdit(parent);
        m_textEdit->setReadOnly(true);
        m_textEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_textEdit->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        parent->layout()->addWidget(m_textEdit);
    }
    else if (m_contentType == Images)
    {
        m_image = new Image();
        m_image->m_label = new QLabel(parent);
        m_image->m_label->setScaledContents(true);
        m_image->m_label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        m_image->m_pixmap = QPixmap(200, 200);
        parent->layout()->addWidget(m_image->m_label);
    }
    show();
}

RendererConnectionForImagesAndText::~RendererConnectionForImagesAndText()
{
    if (m_currentResource)
    {
        m_currentResource->deleteLater();
    }
    if (m_contentType == Images)
    {
        delete m_image;
    }
}

void RendererConnectionForImagesAndText::show()
{
    if (m_contentType == Text)
    {
        if (m_stopped)
        {
            m_textEdit->clear();
            m_textEdit->setEnabled(false);
        }
        else
        {
            m_textEdit->setText(QString::fromUtf8(m_currentData));
            m_textEdit->setEnabled(true);
        }
    }
    else if (m_contentType == Images)
    {
        if (!m_stopped)
        {
            m_image->m_label->setPixmap(
                m_image->m_pixmap.scaled(
                    m_image->m_label->size(), Qt::KeepAspectRatio));
        }
        else
        {
            m_image->m_pixmap.fill(QColor(Qt::black));
            m_image->m_label->setPixmap(
                m_image->m_pixmap.scaled(m_image->m_label->size()));
        }
    }
}

void RendererConnectionForImagesAndText::finished()
{
    m_currentData = m_currentResource->readAll();
    if (m_showWhenReady)
    {
        m_image->m_pixmap.loadFromData(m_currentData);
        show();
    }
    m_currentResource->deleteLater(); m_currentResource = 0;
}

qint32 RendererConnectionForImagesAndText::doPlay(const QString& arg)
{
    Q_UNUSED(arg)
    m_stopped = false;
    if (m_currentResource)
    {
        m_showWhenReady = true;
    }
    else
    {
        if (m_contentType == Images)
        {
            m_image->m_pixmap.loadFromData(m_currentData);
        }
        m_showWhenReady = false;
        show();
    }
    return UpnpSuccess;
}

qint32 RendererConnectionForImagesAndText::doStop()
{
    m_stopped = true;
    if (m_currentResource)
    {
        m_currentResource->deleteLater();
    }
    show();
    return UpnpSuccess;
}

qint32 RendererConnectionForImagesAndText::doSeek(const Herqq::Upnp::Av::HSeekInfo& seekInfo)
{
    Q_UNUSED(seekInfo)
    return UpnpSuccess;
}

qint32 RendererConnectionForImagesAndText::doNext()
{
    return UpnpSuccess;
}

qint32 RendererConnectionForImagesAndText::doPrevious()
{
    return UpnpSuccess;
}

qint32 RendererConnectionForImagesAndText::doSetResource(
    const QUrl& resourceUri, Herqq::Upnp::Av::HObject* cdsObjectData)
{
    Q_UNUSED(resourceUri)
    Q_UNUSED(cdsObjectData)

    QNetworkRequest req(resourceUri);
    m_currentResource = m_nam.get(req);
    bool ok = connect(m_currentResource, SIGNAL(finished()), this, SLOT(finished()));
    Q_ASSERT(ok); Q_UNUSED(ok)

    return UpnpSuccess;
}

qint32 RendererConnectionForImagesAndText::doSelectPreset(const QString&)
{
    return UpnpSuccess;
}

void RendererConnectionForImagesAndText::resizeEventOccurred(const QResizeEvent&)
{
    show();
}

/*******************************************************************************
 * DefaultRendererConnection
 *******************************************************************************/

DefaultRendererConnection::DefaultRendererConnection(ContentType ct, QWidget* parent)
    : CustomRendererConnection(parent),
      m_mediaObject(parent),
      m_mediaSource(0),
      m_videoWidget(0)
{
    bool ok = connect(&m_mediaObject, SIGNAL(stateChanged(QtAV::AVPlayer::State)),
                      this, SLOT(stateChanged(QtAV::AVPlayer::State)));

    Q_ASSERT(ok);
    Q_UNUSED(ok)

    ok = connect(&m_mediaObject, SIGNAL(durationChanged(qint64)),
                 this, SLOT(tick(qint64)));

    Q_ASSERT(ok);
    Q_UNUSED(ok)

    ok = connect(&m_mediaObject, SIGNAL(durationChanged(qint64)),
                 this, SLOT(totalTimeChanged(qint64)));

    Q_ASSERT(ok);
    Q_UNUSED(ok)

    if (ct == AudioVideo)
    {
        setupVideo();
    }

    QtAV::AudioOutput* audioOutput = new QtAV::AudioOutput();
    audioOutput->setParent(parent);
}

DefaultRendererConnection::~DefaultRendererConnection()
{
}

void DefaultRendererConnection::setupVideo()
{
    QWidget* parentWidget = static_cast<QWidget*>(parent());
    m_videoWidget = new QtAV::WidgetRenderer(parentWidget);
    m_videoWidget->setMinimumSize(200, 200);
    m_videoWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    parentWidget->layout()->addWidget(m_videoWidget);
}

void DefaultRendererConnection::tick(qint64 time)
{
    QTime tmp;
    tmp = tmp.addMSecs(time);
    HDuration position(tmp);
    writableRendererConnectionInfo()->setRelativeTimePosition(position);
}

void DefaultRendererConnection::totalTimeChanged(qint64 time)
{
    QTime tmp;
    tmp = tmp.addMSecs(time);
    HDuration duration(tmp);
    writableRendererConnectionInfo()->setCurrentTrackDuration(duration);
}

void DefaultRendererConnection::stateChanged(QtAV::AVPlayer::State newstate)
{
    switch(newstate)
    {
        case QtAV::AVPlayer::PlayingState:

            if (m_mediaObject.position() == m_mediaObject.duration())
            {
                if (m_mediaObject.isSeekable())
                {
                    m_mediaObject.setPosition(0);
                }

                m_mediaObject.play();
            }

            writableRendererConnectionInfo()->setTransportState(HTransportState::Playing);
            break;

        case QtAV::AVPlayer::StoppedState:
            if (m_mediaObject.isSeekable())
            {
                m_mediaObject.setPosition(0);
            }
            writableRendererConnectionInfo()->setTransportState(HTransportState::Stopped);
            break;

        case QtAV::AVPlayer::PausedState:
            if (m_mediaObject.position() == m_mediaObject.duration())
            {
                if (m_mediaObject.isSeekable())
                {
                    m_mediaObject.setPosition(0);
                }
            }

            writableRendererConnectionInfo()->setTransportState(HTransportState::PausedPlayback);
            break;

        default:
            m_mediaObject.play();
            break;
    }
}

qint32 DefaultRendererConnection::doPlay(const QString& arg)
{
    Q_UNUSED(arg)

    qint32 retVal = UpnpSuccess;

    switch(writableRendererConnectionInfo()->transportState().type())
    {
        case HTransportState::PausedPlayback:
        case HTransportState::Stopped:
        case HTransportState::Transitioning:

            if (m_mediaObject.position() == m_mediaObject.duration())
            {
                if (m_mediaObject.isSeekable())
                {
                    m_mediaObject.setPosition(0);
                }
            }
            m_mediaObject.play();
            break;

        default:
            retVal = HAvTransportInfo::TransitionNotAvailable;
    }

    return retVal;
}

qint32 DefaultRendererConnection::doStop()
{
    m_mediaObject.stop();
    writableRendererConnectionInfo()->setRelativeTimePosition(HDuration());
    return UpnpSuccess;
}

qint32 DefaultRendererConnection::doPause()
{
    m_mediaObject.pause();
    return UpnpSuccess;
}

qint32 DefaultRendererConnection::doSeek(const Herqq::Upnp::Av::HSeekInfo& seekInfo)
{
    Q_UNUSED(seekInfo)
    return UpnpSuccess;
}

qint32 DefaultRendererConnection::doNext()
{
    return UpnpSuccess;
}

qint32 DefaultRendererConnection::doPrevious()
{
    return UpnpSuccess;
}

qint32 DefaultRendererConnection::doSetResource(const QUrl& resourceUri,
                                                Herqq::Upnp::Av::HObject* cdsObjectData)
{
    Q_UNUSED(resourceUri)
    Q_UNUSED(cdsObjectData)

    if (m_mediaSource)
    {
        m_mediaObject.stop();
/*
        if(m_mediaObject.playlist())
            m_mediaObject.playlist()->clear();*/
    }

    m_mediaSource.reset(QtAV::MediaIO::createForUrl(resourceUri.toString()));
    m_mediaObject.setInput(m_mediaSource.take());

    if (!m_videoWidget)
    {
        setupVideo();
        m_videoWidget->show();
    }

    writableRendererConnectionInfo()->setRelativeTimePosition(HDuration());

    return UpnpSuccess;
}

qint32 DefaultRendererConnection::doSelectPreset(const QString&)
{
    return UpnpSuccess;
}

void DefaultRendererConnection::hasVideoChanged(bool b)
{
    if (!m_videoWidget && b)
    {
        setupVideo();
        m_videoWidget->show();
    }
}
