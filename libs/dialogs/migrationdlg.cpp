/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 2009-11-14
 * Description : database migration dialog
 *
 * Copyright (C) 2009 by Holger Foerster <Hamsi2k at freenet dot de>
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

#include "migrationdlg.h"

// QT includes
#include <qgridlayout.h>
#include <qpushbutton.h>
#include <qprogressbar.h>
#include <qwidget.h>
#include <qlist.h>
#include <qsqlquery.h>
#include <qmap.h>
#include <qsqlerror.h>
#include <qlabel.h>

// KDE includes
#include <klocale.h>
#include <kdebug.h>
#include <kmessagebox.h>

// Local includes
#include "albumsettings.h"
#include "databaseaccess.h"
#include "databasewidget.h"
#include "databasebackend.h"
#include "databaseparameters.h"
#include "schemaupdater.h"
#include "databasecopymanager.h"


namespace Digikam
{
    DatabaseCopyThread::DatabaseCopyThread(QWidget *parent) : QThread(parent)
    {
    }

    void DatabaseCopyThread::run()
    {
        copyManager.copyDatabases(fromDatabaseParameters, toDatabaseParameters);
    }

    void DatabaseCopyThread::init(DatabaseParameters fromDatabaseParameters, DatabaseParameters toDatabaseParameters)
    {
        this->fromDatabaseParameters=fromDatabaseParameters;
        this->toDatabaseParameters=toDatabaseParameters;
    }

    MigrationDlg::MigrationDlg(QWidget* parent): KDialog(parent)
    {
        setupMainArea();
    }

    MigrationDlg::~MigrationDlg()
    {

    }

    void MigrationDlg::setupMainArea()
    {
        thread                          = new DatabaseCopyThread(this);
        fromDatabaseWidget              = new DatabaseWidget(this);
        toDatabaseWidget                = new DatabaseWidget(this);
        migrateButton                   = new QPushButton(i18n("Migrate ->"), this);
        cancelButton                    = new QPushButton(i18n("Cancel"), this);
        cancelButton->setEnabled(false);

        QGroupBox *progressBox = new QGroupBox(i18n("Progress Information"), this);
        QVBoxLayout *vlay         = new QVBoxLayout(progressBox);

        progressBar                     = new QProgressBar(progressBox);
        progressBar->setTextVisible(true);
        progressBar->setRange(0,13);
        progressBarSmallStep            = new QProgressBar(progressBox);
        progressBarSmallStep->setTextVisible(true);

        vlay->addWidget(new QLabel(i18n("Overall Progress"), progressBox));
        vlay->addWidget(progressBar);
        vlay->addWidget(new QLabel(i18n("Step Progress"), progressBox));
        vlay->addWidget(progressBarSmallStep);

        QWidget *mainWidget     = new QWidget;
        QGridLayout *layout     = new QGridLayout;
        mainWidget->setLayout(layout);

        layout->addWidget(fromDatabaseWidget, 0,0, 4,1);
        layout->addWidget(migrateButton,1,1);
        layout->addWidget(cancelButton,2,1);
        layout->addWidget(toDatabaseWidget, 0,2, 4,1);
        layout->addWidget(progressBox, 4,0, 1,3);
//        layout->addWidget(progressBar, 4,0, 1,3);
//        layout->addWidget(progressBarSmallStep, 5,0, 1,3);

        setMainWidget(mainWidget);
        dataInit();

        // setup dialog
        setButtons(Close);

        connect(migrateButton, SIGNAL(clicked()), this, SLOT(performCopy()));


        // connect signal handlers for copy thread
        this->connect(&(thread->copyManager), SIGNAL(finished(int, QString)), SLOT(handleFinish(int, QString)));

        this->connect(&(thread->copyManager), SIGNAL(stepStarted(QString)), SLOT(handleStepStarted(QString)));
        this->connect(&(thread->copyManager), SIGNAL(smallStepStarted(int, int)), SLOT(handleSmallStepStarted(int, int)));

        connect(cancelButton, SIGNAL(clicked()), &(thread->copyManager), SLOT(stopThread()));

        this->connect(cancelButton, SIGNAL(closeClicked()), &(thread->copyManager), SLOT(stopThread()));
    }

    void MigrationDlg::performCopy()
    {
        DatabaseParameters toDBParameters = toDatabaseWidget->getDatabaseParameters();
        DatabaseParameters fromDBParameters = fromDatabaseWidget->getDatabaseParameters();
        thread->init(fromDBParameters, toDBParameters);

        lockInputFields();
        thread->start();
    }

    void MigrationDlg::dataInit()
    {
        fromDatabaseWidget->setParametersFromSettings(AlbumSettings::instance());
        toDatabaseWidget->setParametersFromSettings(AlbumSettings::instance());
    }

    void MigrationDlg::unlockInputFields()
    {
        fromDatabaseWidget->setEnabled(true);
        toDatabaseWidget->setEnabled(true);
        migrateButton->setEnabled(true);
        progressBar->setValue(0);
        progressBarSmallStep->setValue(0);

        cancelButton->setEnabled(false);
    }

    void MigrationDlg::lockInputFields()
    {
         fromDatabaseWidget->setEnabled(false);
         toDatabaseWidget->setEnabled(false);
         migrateButton->setEnabled(false);

         cancelButton->setEnabled(true);
    }

    void MigrationDlg::handleFinish(int finishState, QString errorMsg)
    {
        switch (finishState)
        {
            case DatabaseCopyManager::failed:   KMessageBox::error(this, errorMsg );
                                                unlockInputFields();
                                                break;
            case DatabaseCopyManager::success:  KMessageBox::information(this, i18n("Database copied successfully.") );
                                                unlockInputFields();
                                                break;
            case DatabaseCopyManager::canceled: KMessageBox::information(this, i18n("Database conversion canceled.") );
                                                unlockInputFields();
                                                break;
        }
    }

    void MigrationDlg::handleStepStarted(QString stepName)
    {
        int progressBarValue = progressBar->value();
        progressBar->setValue(++progressBarValue);
    }

    void MigrationDlg::handleSmallStepStarted(int currentValue, int maxValue)
    {
        progressBarSmallStep->setMaximum(maxValue);
        progressBarSmallStep->setValue(currentValue);
    }
}
