/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "CGLicence.h"

#include <QtGui>
CAppLicence::CAppLicence(QWidget *parent)
    : QWizard(parent)
{
    setPage(LicencePage_Conclusion, new ConclusionLicencePage);

    setStartId(LicencePage_Intro);

#ifndef Q_WS_MAC
    setWizardStyle(ModernStyle);
#endif
    setOption(HaveHelpButton, true);
    setPixmap(QWizard::LogoPixmap, QPixmap(":logo.ico"));

    connect(this, SIGNAL(helpRequested()), this, SLOT(showHelp()));

    setWindowTitle(tr("License Wizard"));
}

void CAppLicence::showHelp()
{
    static QString lastHelpMessage;

    QString message;

    switch (currentId()) {
    case LicencePage_Intro:
        message = tr("The decision you make here will affect which LicencePage you "
                     "get to see next.");
        break;
    case LicencePage_Processing:
        message = tr("Make sure to provide a valid email address, such as "
                     "toni.buddenbrook@example.de.");
        break;
    case LicencePage_Conclusion:
        message = tr("You must accept the terms and conditions of the "
                     "license to proceed.");
        break;
    default:
        message = tr("This help is likely not to be of any help.");
    }

    if (lastHelpMessage == message)
        message = tr("Sorry, I already gave what help I could. "
                     "Maybe you should try asking a human?");

    QMessageBox::information(this, tr("License Wizard Help"), message);

    lastHelpMessage = message;
}

IntroLicencePage::IntroLicencePage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Introduction"));
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/watermark.png"));

    topLabel = new QLabel(tr("This wizard will help you register your copy of "
                             "<i>Super Product One</i>&trade; or start "
                             "evaluating the product."));
    topLabel->setWordWrap(true);

    registerRadioButton = new QRadioButton(tr("&Register your copy"));
    evaluateRadioButton = new QRadioButton(tr("&Evaluate the product for 30 "
                                              "days"));
    registerRadioButton->setChecked(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(topLabel);
    layout->addWidget(registerRadioButton);
    layout->addWidget(evaluateRadioButton);
    setLayout(layout);
}

int IntroLicencePage::nextId() const
{
        return CAppLicence::LicencePage_Processing;
}

ProcessingLicencePage::ProcessingLicencePage(QWidget *parent)
     : QWizardPage(parent)
 {
     setTitle(tr("Fill In Your Details"));
     setSubTitle(tr("Please fill all three fields. Make sure to provide a valid "
                    "email address (e.g., tanaka.aya@example.co.jp)."));

     companyLabel = new QLabel(tr("&Company name:"));
     companyLineEdit = new QLineEdit;
     companyLabel->setBuddy(companyLineEdit);

     emailLabel = new QLabel(tr("&Email address:"));
     emailLineEdit = new QLineEdit;
     emailLineEdit->setValidator(new QRegExpValidator(QRegExp(".*@.*"), this));
     emailLabel->setBuddy(emailLineEdit);

     postalLabel = new QLabel(tr("&Postal address:"));
     postalLineEdit = new QLineEdit;
     postalLabel->setBuddy(postalLineEdit);

     registerField("details.company*", companyLineEdit);
     registerField("details.email*", emailLineEdit);
     registerField("details.postal*", postalLineEdit);

     QGridLayout *layout = new QGridLayout;
     layout->addWidget(companyLabel, 0, 0);
     layout->addWidget(companyLineEdit, 0, 1);
     layout->addWidget(emailLabel, 1, 0);
     layout->addWidget(emailLineEdit, 1, 1);
     layout->addWidget(postalLabel, 2, 0);
     layout->addWidget(postalLineEdit, 2, 1);
     setLayout(layout);
 }

 int ProcessingLicencePage::nextId() const
 {
     return CAppLicence::LicencePage_Conclusion;
 }


ConclusionLicencePage::ConclusionLicencePage(QWidget *parent)
     : QWizardPage(parent)
 {
     setTitle(tr("License agreement"));
     setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/watermark.png"));

     bottomLabel = new QLabel;
     bottomLabel->setWordWrap(true);
     QTextEdit* text = new QTextEdit();

     QFile f("../licence.txt");
     bool y = f.open(QIODevice::ReadOnly);
     if(!y){
//         //qDebug << "[WARN] Can't open " << "licence.txt";
     }

     QByteArray b = f.readAll();

     QString s (b);

     text->setText(s);
     text->setReadOnly(true);
     agreeCheckBox = new QCheckBox(tr("I agree to the terms of the license"));

     registerField("conclusion.agree*", agreeCheckBox);

     QVBoxLayout *layout = new QVBoxLayout;
     layout->addWidget(bottomLabel);
     layout->addWidget(text);
     layout->addWidget(agreeCheckBox);
     setLayout(layout);
 }

 int ConclusionLicencePage::nextId() const
 {
     return -1;
 }

 void ConclusionLicencePage::initializeLicencePage()
 {
     QString licenseText;


     if (wizard()->hasVisitedPage(CAppLicence::LicencePage_Processing)) {
         licenseText = tr("<u>First-Time License Agreement:</u> "
                          "You can use this software subject to the license "
                          "you will receive by email.");
     } else {
         licenseText = tr("<u>Upgrade License Agreement:</u> "
                          "This software is licensed under the terms of your "
                          "current license.");
     }
     bottomLabel->setText(licenseText);
 }

 void ConclusionLicencePage::setVisible(bool visible)
 {
     QWizardPage::setVisible(visible);

     if (visible) {
         wizard()->setButtonText(QWizard::CustomButton1, tr("&Print"));
         wizard()->setOption(QWizard::HaveCustomButton1, true);
         connect(wizard(), SIGNAL(customButtonClicked(int)),
                 this, SLOT(printButtonClicked()));
     } else {
         wizard()->setOption(QWizard::HaveCustomButton1, false);
         disconnect(wizard(), SIGNAL(customButtonClicked(int)),
                    this, SLOT(printButtonClicked()));
     }
 }

 void ConclusionLicencePage::printButtonClicked()
 {
     QPrinter printer;
     QPrintDialog dialog(&printer, this);
     if (dialog.exec())
         QMessageBox::warning(this, tr("Print License"),
                              tr("As an environmentally friendly measure, the "
                                 "license text will not actually be printed."));
 }
