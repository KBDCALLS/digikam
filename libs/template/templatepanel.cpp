/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 2009-07-06
 * Description : metadata template settings panel.
 *
 * Copyright (C) 2009 by Gilles Caulier <caulier dot gilles at gmail dot com>
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

#include "templatepanel.h"

// Qt includes

#include <QLabel>
#include <QGridLayout>
#include <QFrame>

// KDE includes

#include <klocale.h>
#include <kdialog.h>
#include <klineedit.h>

// Local includes

#include "altlangstredit.h"
#include "templatelist.h"

namespace Digikam
{

class TemplatePanelPriv
{
public:

    enum TemplateTab
    {
        RIGHTS=0,
        LOCATION,
        CONTACT
    };

public:

    TemplatePanelPriv()
    {
        authorsEdit               = 0;
        authorsPositionEdit       = 0;
        creditEdit                = 0;
        sourceEdit                = 0;
        copyrightEdit             = 0;
        rightUsageEdit            = 0;
        instructionsEdit          = 0;

        locationCountryEdit       = 0;
        locationCountryCodeEdit   = 0;
        locationProvinceStateEdit = 0;
        locationCityEdit          = 0;
        locationEdit              = 0;

        contactCityEdit           = 0;
        contactCountryEdit        = 0;
        contactAddressEdit        = 0;
        contactPostalCodeEdit     = 0;
        contactProvinceStateEdit  = 0;
        contactEmailEdit          = 0;
        contactPhoneEdit          = 0;
        contactWebUrlEdit         = 0;
    }

    // Rights template informations panel.
    KLineEdit      *authorsEdit;
    KLineEdit      *authorsPositionEdit;
    KLineEdit      *creditEdit;
    KLineEdit      *sourceEdit;
    KLineEdit      *instructionsEdit;

    AltLangStrEdit *copyrightEdit;
    AltLangStrEdit *rightUsageEdit;

    // Location template informations panel.
    KLineEdit      *locationCountryEdit;
    KLineEdit      *locationCountryCodeEdit;
    KLineEdit      *locationProvinceStateEdit;
    KLineEdit      *locationCityEdit;
    KLineEdit      *locationEdit;

    // Contact template informations panel.
    KLineEdit      *contactCityEdit;
    KLineEdit      *contactCountryEdit;
    KLineEdit      *contactAddressEdit;
    KLineEdit      *contactPostalCodeEdit;
    KLineEdit      *contactProvinceStateEdit;
    KLineEdit      *contactEmailEdit;
    KLineEdit      *contactPhoneEdit;
    KLineEdit      *contactWebUrlEdit;
};

TemplatePanel::TemplatePanel(QWidget* parent)
             : KTabWidget(parent), d(new TemplatePanelPriv)
{

    // -- Rights Template informations panel -------------------------------------------------------------

    QWidget *page1     = new QWidget(this);
    QGridLayout* grid1 = new QGridLayout(page1);

    QLabel *label1 = new QLabel(i18n("Author Names:"), page1);
    d->authorsEdit = new KLineEdit(page1);
    d->authorsEdit->setClearButtonShown(true);
    d->authorsEdit->setClickMessage(i18n("Enter here all creator name. Use semi-colons as separator."));
    label1->setBuddy(d->authorsEdit);
    d->authorsEdit->setWhatsThis(i18n("<p>This field should contain names of the persons who created the photograph. "
                                      "If it is not appropriate to add the name of the photographer (for example, if the identify of "
                                      "the photographer needs to be protected) the name of a company or organization can also be used. "
                                      "Once saved, this field should not be changed by anyone. "
                                      "<p>To enter more than one name, use <b>semi-colons as separators</b>.</p>"
                                      "<p>With IPTC, this field is limited to 32 ASCII characters.</p>"));

    // --------------------------------------------------------

    QLabel *label2         = new QLabel(i18n("Authors Position:"), page1);
    d->authorsPositionEdit = new KLineEdit(page1);
    d->authorsPositionEdit->setClearButtonShown(true);
    d->authorsPositionEdit->setClickMessage(i18n("Enter here the job title of authors."));
    label2->setBuddy(d->authorsPositionEdit);
    d->authorsPositionEdit->setWhatsThis(i18n("<p>This field should contain the job title of authors. Examples might include "
                                              "titles such as: Staff Photographer, Freelance Photographer, or Independent Commercial "
                                              "Photographer. Since this is a qualifier for the Author field, the Author field must also "
                                              "be filled out.</p>"
                                              "<p>With IPTC, this field is limited to 32 ASCII characters.</p>"));

    // --------------------------------------------------------

    QLabel *label3 = new QLabel(i18n("Credit:"), page1);
    d->creditEdit  = new KLineEdit(page1);
    d->creditEdit->setClearButtonShown(true);
    d->creditEdit->setClickMessage(i18n("Enter here the photograph credit."));
    label3->setBuddy(d->creditEdit);
    d->creditEdit->setWhatsThis(i18n("<p>(synonymous to Provider): Use the Provider field to identify who is providing the photograph. "
                                     "This does not necessarily have to be the author. If a photographer is working for a news agency "
                                     "such as Reuters or the Associated Press, these organizations could be listed here as they are "
                                     "\"providing\" the image for use by others. If the image is a stock photograph, then the group "
                                     "(agency) involved in supplying the image should be listed here.</p>"
                                     "<p>With IPTC, this field is limited to 32 ASCII characters.</p>"));

    // --------------------------------------------------------

    d->copyrightEdit = new AltLangStrEdit(page1);
    d->copyrightEdit->setTitle(i18n("Copyright:"));
    d->copyrightEdit->setFixedHeight(75);
    d->copyrightEdit->setWhatsThis(i18n("<p>The Copyright Notice should contain any necessary copyright notice for claiming the intellectual "
                                        "property, and should identify the current owner(s) of the copyright for the photograph. Usually, "
                                        "this would be the photographer, but if the image was done by an employee or as work-for-hire, "
                                        "then the agency or company should be listed. Use the form appropriate to your country. USA: "
                                        "&copy; {date of first publication} name of copyright owner, as in \"&copy;2005 John Doe.\" "
                                        "Note, the word \"copyright\" or the abbreviation \"copr\" may be used in place of the &copy; symbol. "
                                        "In some foreign countries only the copyright symbol is recognized and the abbreviation does not work. "
                                        "Furthermore the copyright symbol must be a full circle with a \"c\" inside; using something like (c) "
                                        "where the parentheses form a partial circle is not sufficient. For additional protection worldwide, "
                                        "use of the phrase, \"all rights reserved\" following the notice above is encouraged. \nIn Europe "
                                        "you would use: Copyright {Year} {Copyright owner}, all rights reserved. \nIn Japan, for maximum "
                                        "protection, the following three items should appear in the copyright field of the IPTC Core: "
                                        "(a) the word, Copyright; (b) year of the first publication; and (c) name of the author. "
                                        "You may also wish to include the phrase \"all rights reserved\".</p>"
                                        "<p>With XMP, you can include more than one copyright string using different languages.</p>"
                                        "<p>With IPTC, this field is limited to 128 ASCII characters.</p>"));

    // --------------------------------------------------------

    d->rightUsageEdit = new AltLangStrEdit(page1);
    d->rightUsageEdit->setTitle(i18n("Right Usage Terms:"));
    d->rightUsageEdit->setFixedHeight(75);
    d->rightUsageEdit->setWhatsThis(i18n("<p>The Right Usage Terms field should be used to list instructions on how "
                                         "a resource can be legally used."
                                         "<p>With XMP, you can include more than one right usage terms string using "
                                         "different languages.</p>"
                                         "<p>This field do not exist with IPTC.</p>"));

    // --------------------------------------------------------

    QLabel *label6 = new QLabel(i18n("Source:"), page1);
    d->sourceEdit  = new KLineEdit(page1);
    d->sourceEdit->setClearButtonShown(true);
    d->sourceEdit->setClickMessage(i18n("Enter here original owner of the photograph."));
    label6->setBuddy(d->sourceEdit);
    d->sourceEdit->setWhatsThis(i18n("<p>The Source field should be used to identify the original owner or copyright holder of the "
                                     "photograph. The value of this field should never be changed after the information is entered "
                                     "following the image's creation. While not yet enforced by the custom panels, you should consider "
                                     "this to be a \"write-once\" field. The source could be an individual, an agency, or a "
                                     "member of an agency. To aid in later searches, it is suggested to separate any slashes "
                                     "\"/\" with a blank space. Use the form \"photographer / agency\" rather than "
                                     "\"photographer/agency.\" Source may also be different from Creator and from the names "
                                     "listed in the Copyright Notice.</p>"
                                     "<p>With IPTC, this field is limited to 32 ASCII characters.</p>"));

    // --------------------------------------------------------

    QLabel *label7      = new QLabel(i18n("Instructions:"), page1);
    d->instructionsEdit = new KLineEdit(page1);
    d->instructionsEdit->setClearButtonShown(true);
    d->instructionsEdit->setClickMessage(i18n("Enter here the editorial notice."));
    label7->setBuddy(d->instructionsEdit);
    d->instructionsEdit->setWhatsThis(i18n("<p>The Instructions field should be used to list editorial "
                                           "instructions concerning the use of photograph.</p>"
                                           "<p>With IPTC, this field is limited to 256 ASCII characters.</p>"));

    // --------------------------------------------------------

    grid1->setMargin(KDialog::spacingHint());
    grid1->setSpacing(KDialog::spacingHint());
    grid1->setAlignment(Qt::AlignTop);
    grid1->setColumnStretch(1, 10);
    grid1->addWidget(label1,                 0, 0, 1, 1);
    grid1->addWidget(d->authorsEdit,         0, 1, 1, 2);
    grid1->addWidget(label2,                 1, 0, 1, 1);
    grid1->addWidget(d->authorsPositionEdit, 1, 1, 1, 2);
    grid1->addWidget(label3,                 2, 0, 1, 1);
    grid1->addWidget(d->creditEdit,          2, 1, 1, 2);
    grid1->addWidget(d->copyrightEdit,       3, 0, 1, 2);
    grid1->addWidget(d->rightUsageEdit,      4, 0, 1, 2);
    grid1->addWidget(label6,                 5, 0, 1, 1);
    grid1->addWidget(d->sourceEdit,          5, 1, 1, 2);
    grid1->addWidget(label7,                 6, 0, 1, 1);
    grid1->addWidget(d->instructionsEdit,    6, 1, 1, 2);

    insertTab(TemplatePanelPriv::RIGHTS, page1, i18n("Rights"));

    // -- Location Template informations panel -------------------------------------------------------------

    QWidget *page2     = new QWidget(this);
    QGridLayout* grid2 = new QGridLayout(page2);

    QLabel *label8         = new QLabel(i18n("Country Name:"), page2);
    d->locationCountryEdit = new KLineEdit(page2);
    d->locationCountryEdit->setClearButtonShown(true);
    d->locationCountryEdit->setClickMessage(i18n("Enter here country name of contents."));
    label8->setBuddy(d->locationCountryEdit);
    d->locationCountryEdit->setWhatsThis(i18n("<p>This field should contain country name "
                                              "where have been taken the photograph.</p>"));

    // --------------------------------------------------------

    QLabel *label9             = new QLabel(i18n("Country Code:"), page2);
    d->locationCountryCodeEdit = new KLineEdit(page2);
    d->locationCountryCodeEdit->setClearButtonShown(true);
    d->locationCountryCodeEdit->setClickMessage(i18n("Enter here country code of contents."));
    label9->setBuddy(d->locationCountryCodeEdit);
    d->locationCountryCodeEdit->setWhatsThis(i18n("<p>This field should contain country code "
                                                  "where have been taken the photograph.</p>"));

    // --------------------------------------------------------

    QLabel *label10            = new QLabel(i18n("Province State:"), page2);
    d->locationCountryCodeEdit = new KLineEdit(page2);
    d->locationCountryCodeEdit->setClearButtonShown(true);
    d->locationCountryCodeEdit->setClickMessage(i18n("Enter here province state of contents."));
    label10->setBuddy(d->locationCountryCodeEdit);
    d->locationCountryCodeEdit->setWhatsThis(i18n("<p>This field should contain province state "
                                                  "where have been taken the photograph.</p>"));

    // --------------------------------------------------------

    QLabel *label11 = new QLabel(i18n("City:"), page2);
    d->locationCityEdit = new KLineEdit(page2);
    d->locationCityEdit->setClearButtonShown(true);
    d->locationCityEdit->setClickMessage(i18n("Enter here city of contents."));
    label11->setBuddy(d->locationCityEdit);
    d->locationCityEdit->setWhatsThis(i18n("<p>This field should contain city name "
                                           "where have been taken the photograph.</p>"));

    // --------------------------------------------------------

    QLabel *label12 = new QLabel(i18n("Location:"), page2);
    d->locationEdit = new KLineEdit(page2);
    d->locationEdit->setClearButtonShown(true);
    d->locationEdit->setClickMessage(i18n("Enter here location place of contents."));
    label12->setBuddy(d->locationEdit);
    d->locationEdit->setWhatsThis(i18n("<p>This field should contain location place "
                                       "where have been taken the photograph.</p>"));

    // --------------------------------------------------------

    grid2->setMargin(KDialog::spacingHint());
    grid2->setSpacing(KDialog::spacingHint());
    grid2->setAlignment(Qt::AlignTop);
    grid2->setColumnStretch(1, 10);
    grid2->setRowStretch(5, 10);
    grid2->addWidget(label8,                       0, 0, 1, 1);
    grid2->addWidget(d->locationCountryEdit,       0, 1, 1, 2);
    grid2->addWidget(label9,                       1, 0, 1, 1);
    grid2->addWidget(d->locationCountryCodeEdit,   1, 1, 1, 2);
    grid2->addWidget(label10,                      2, 0, 1, 1);
    grid2->addWidget(d->locationProvinceStateEdit, 2, 1, 1, 2);
    grid2->addWidget(label11,                      3, 0, 1, 1);
    grid2->addWidget(d->locationCityEdit,          3, 1, 1, 2);
    grid2->addWidget(label12,                      4, 0, 1, 1);
    grid2->addWidget(d->locationEdit,              4, 1, 1, 2);

    insertTab(TemplatePanelPriv::LOCATION, page2, i18n("Location"));
}

TemplatePanel::~TemplatePanel()
{
    delete d;
}

void TemplatePanel::setTemplate(const Template& t)
{
    d->authorsEdit->setText(t.authors().join(";"));
    d->authorsPositionEdit->setText(t.authorsPosition());
    d->creditEdit->setText(t.credit());
    d->copyrightEdit->setValues(t.copyright());
    d->rightUsageEdit->setValues(t.rightUsageTerms());
    d->sourceEdit->setText(t.source());
    d->instructionsEdit->setText(t.instructions());
}

Template TemplatePanel::getTemplate() const
{
    Template t;
    t.setAuthors(d->authorsEdit->text().split(";", QString::SkipEmptyParts));
    t.setAuthorsPosition(d->authorsPositionEdit->text());
    t.setCredit(d->creditEdit->text());
    t.setCopyright(d->copyrightEdit->values());
    t.setRightUsageTerms(d->rightUsageEdit->values());
    t.setSource(d->sourceEdit->text());
    t.setInstructions(d->instructionsEdit->text());
    return t;
}

void TemplatePanel::apply()
{
    d->copyrightEdit->apply();
    d->rightUsageEdit->apply();
}

}  // namespace Digikam
