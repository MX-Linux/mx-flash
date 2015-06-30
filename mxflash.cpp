/*****************************************************************************
 * mxflash.cpp
 *****************************************************************************
 * Copyright (C) 2014 MX Authors
 *
 * Authors: Adrian
 *          MX & MEPIS Community <http://forum.mepiscommunity.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MX Flash is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with MX Flash.  If not, see <http://www.gnu.org/licenses/>.
 **********************************************************************/

#include "mxflash.h"
#include "ui_mxflash.h"

#include <QDir>
#include <QTextStream>

mxflash::mxflash(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::mxflash)
{
    ui->setupUi(this);
    refresh();
    proc = new QProcess(this);
    timer = new QTimer(this);
}

mxflash::~mxflash() {
    delete ui;
}


// Util function
QString mxflash::getCmdOut(QString cmd) {
    qApp->processEvents();
    proc = new QProcess(this);
    proc->start("/bin/bash", QStringList() << "-c" << cmd);
    proc->waitForFinished(-1);
    return proc->readAllStandardOutput().trimmed();
}

// Get version of the program
QString mxflash::getVersion(QString name)
{
    QString cmd = QString("dpkg -l %1 | awk 'NR==6 {print $3}'").arg(name);
    return getCmdOut(cmd);
}

bool mxflash::checkOnline()
{
    if(system("wget -q --spider http://www.mepiscommunity.org >/dev/null 2>&1") == 0) {
        return true;
    }
    return false;
}

/////////////////////////////////////////////////////////////////////////
// general

bool mxflash::checkDebInstalled() {
    proc = new QProcess(this);
    proc->start("/bin/bash -c \"dpkg -s flashplugin-nonfree | grep Status | cut -f 3 -d \' \'\"");
    proc->setReadChannel(QProcess::StandardOutput);
    proc->setReadChannelMode(QProcess::MergedChannels);
    proc->waitForFinished();
    QString out = proc->readAllStandardOutput();
    return out.contains("ok");
}

void mxflash::refresh() {
    this->show();
    ui->stackedWidget->setCurrentIndex(0);
    ui->progressBar->hide();
    ui->progressBar->setValue(0);
    ui->removeFlashButton->setAutoExclusive(false);
    ui->removeFlashButton->setChecked(false);
    ui->removeFlashButton->setAutoExclusive(true);
    setCursor(QCursor(Qt::ArrowCursor));
    ui->labelVersion->setText(tr("Please wait, loading..."));

    // checks if PepperFlash is installed
    if (getCmdOut("dpkg -s pepperflashplugin-nonfree| grep Status") != "Status: install ok installed") {
        ui->pepperButton->setText(tr("Install PepperFlash for Chromium"));
        ui->updatePepperButton->hide();
    } else {
        ui->pepperButton->setText(tr("Remove PepperFlash"));
        ui->updatePepperButton->show();
    }

    // checks if Flash present
    QFile file("/usr/lib/flashplugin-nonfree/libflashplayer.so");
    if (file.exists()) {
        ui->updateFlashButton->show();
        ui->removeFlashButton->show();
        ui->installFlashButton->setText(tr("Reinstall Flash"));
    } else {
        ui->updateFlashButton->hide();
        ui->removeFlashButton->hide();
        ui->installFlashButton->setText(tr("Install Flash"));
    }
    detectVersion();
}

// detect and list the version of Flash and PepperFlash
void mxflash::detectVersion() {
    ui->whatGroupBox->hide();
    QString out;
    // Current version on Adobe site
    QString bit = getCmdOut("dpkg --print-architecture | sed 's/amd64/64/;s/i386/32/'");
    QString cmd = QString("wget -qO- http://get.adobe.com/flashplayer/download/%3Finstaller=Flash_Player_11.2_for_other_Linux_%28.tar.gz%29_%1-bit%26standalone=1 | grep location.href | cut -f2 -d\\' | cut -f7 -d/").arg(bit);
    QString currentversion = getCmdOut(cmd);
    out = tr("Adobe Flash most recent version: ") + currentversion + "\n";
    // Adobe Flash version
    QString version = getCmdOut("strings /usr/lib/flashplugin-nonfree/libflashplayer.so | grep LNX | awk '{print $2}' | sed 's/,/./g'");
    if (version != "") {
        out += tr("Adobe Flash installed version: ") + version;
    } else {
        out += tr("Adobe Flash is not installed.");
    }
    //// PepperFlash
    // Current version
    cmd = "wget -qO- https://www.adobe.com/software/flash/about/ 2>/dev/null | grep Linux -A10 | grep Chrome -A1 -m1 | tail -n1 | cut -f2 -d\\> | cut -f1 -d\\<";
    QString peppercurrent = getCmdOut(cmd);
    out += "\n\n" + tr("PepperFlash most recent version: ") + peppercurrent;

    QString versionChrome = getCmdOut("strings /opt/google/chrome/PepperFlash/libpepflashplayer.so | grep LNX | awk '{print $2}' | sed 's/,/./g'");
    QString versionChromium = getCmdOut("strings /usr/lib/pepperflashplugin-nonfree/libpepflashplayer.so | grep LNX | awk '{print $2}' | sed 's/,/./g'");
    if (versionChrome != "") {
        out += "\n" + tr("PepperFlash for Chrome installed version: ") + versionChrome;
    }
    if (versionChromium != "" ) {
        out += "\n" + tr("PepperFlash for Chromium installed version: ") + versionChromium;
    } else {
        out += "\n" + tr("PepperFlash for Chromium is not installed.") + versionChromium;
    }
    ui->labelVersion->setText(out);
    ui->whatGroupBox->show();
}


void mxflash::removeFlash() {
    ui->stackedWidget->setCurrentWidget(ui->pageRemove);
    setCursor(QCursor(Qt::WaitCursor));

    QString fname = "/usr/lib/flashplugin-nonfree/libflashplayer.so";
    QFile file(fname);
    // checks if Flash is present
    if (file.exists()) {
        // if deb installed
        if (checkDebInstalled()) {
            QString cmd = QString("dpkg -P flashplugin-nonfree");
            setConnections(timer, proc);
            proc->start(cmd);
        } else {
            if (QFile::remove(fname)) {
                setConnections(timer, proc);
                proc->start("true");
            } else {
                QMessageBox::critical(0, tr("Error"),
                                      tr("Could not remove ") + fname);
                refresh();
            }
        }
    // Flash is not installed
    } else {
        QMessageBox::critical(0, tr("Error"),
                              tr("Flash is not installed"));
        refresh();
    }
}

void mxflash::updateFlash() {
    QFile file("/usr/lib/flashplugin-nonfree/libflashplayer.so");
    // checks if Flash present
    if (!file.exists()) {
        this->hide();
        int ans = QMessageBox::critical(0, tr("Error"),
                                        tr("<b>Flash is not installed. Would you like to install it?</b>"),
                                        tr("Yes"), tr("No"));
        this->show();
        if (ans == 0) {
            installFlash();
            return;
        } else {
            refresh();
            return;
        }
    // if file is present but not installed with a deb
    } else if (!checkDebInstalled()) {
        this->hide();
        QMessageBox::critical(0, tr("Error"),
                              tr("Flash is installed through other means, cannot update with this program."));
        refresh();
        return;
    }

    setCursor(QCursor(Qt::WaitCursor));
    qApp->processEvents();

    // manual update
    if (ui->manualFlashButton->isChecked()) {
        setConnections(timer, proc);
        proc->start("update-flashplugin-nonfree -i");

    //automatic update
    } else {
        // write a flashupdate file in cron.daily
        QFile file("/etc/cron.daily/flashupdate");
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&file);
        out << "#!/bin/sh\n\ngrep -q \"^flags.*\\<sse2\\>\" /proc/cpuinfo || exit 0\n\ntest -x /usr/sbin/update-flashplugin-nonfree && /usr/sbin/update-flashplugin-nonfree --install --quiet";
        file.close();

        // set file executable
        QString cmd = QString("chmod +x %1").arg(file.fileName());
        system(cmd.toUtf8());

        setCursor(QCursor(Qt::ArrowCursor));
        this->hide();
        if (QMessageBox::information(0, tr("Success"),
                                     tr("An automatic daily update was scheduled. You can close the program now.<p><b>Do you want to exit MX Flash Manager?</b>"),
                                     tr("Yes"), tr("No")) == 0){
            qApp->exit(0);
        } else {
            refresh();
        }
    }
}

// update PepperFlash
void mxflash::updatePepper() {
    setCursor(QCursor(Qt::WaitCursor));
    qApp->processEvents();

    // manual update
    if (ui->manualPepperButton->isChecked()) {
        setConnections(timer, proc);
        proc->start("update-pepperflashplugin-nonfree -i");

        //automatic update
    } else {
        // write a pepperflashupdate file in cron.daily
        QFile file("/etc/cron.daily/pepperflashupdate");
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&file);
        out << "#!/bin/sh\n\ntest -x /usr/sbin/update-pepperflashplugin-nonfree && /usr/sbin/update-pepperflashplugin-nonfree --install --quiet";
        file.close();

        // set file executable
        QString cmd = QString("chmod +x %1").arg(file.fileName());
        system(cmd.toUtf8());

        setCursor(QCursor(Qt::ArrowCursor));
        this->hide();
        if (QMessageBox::information(0, tr("Success"),
                                     tr("An automatic daily update was scheduled. You can close the program now.<p><b>Do you want to exit MX Flash Manager?</b>"),
                                     tr("Yes"), tr("No")) == 0){
            qApp->exit(0);
        } else {
            refresh();
        }
    }
}

// downloads Flash for non-sse2 CPUs
void mxflash::downloadFlash() {
    setCursor(QCursor(Qt::WaitCursor));
    QString path = "/usr/lib/flashplugin-nonfree";
    QDir dir(path);
    dir.mkdir(path);
    dir.setCurrent(path);

    ui->progressBar->show();
    disconnect(timer, SIGNAL(timeout()), 0, 0);
    connect(timer, SIGNAL(timeout()), this, SLOT(procTime()));
    disconnect(proc, SIGNAL(started()), 0, 0);
    connect(proc, SIGNAL(started()), this, SLOT(procStart()));
    disconnect(proc, SIGNAL(finished(int)), 0, 0);
    connect(proc, SIGNAL(finished(int)), this, SLOT(procDownloadDone(int)));

    ui->labelInstall->setText(tr("Downloading Flash.."));
    QString cmd = QString("wget -q -nc http://sourceforge.net/projects/antix-linux/files/Testing/MX-14/tarballs/flashplayer_SSE_11.2.202.235.so.tar.gz");
    proc->start(cmd);
}

void mxflash::installFlash() {
    ui->stackedWidget->setCurrentWidget(ui->pageInstall);
    setCursor(QCursor(Qt::WaitCursor));

    // if non-sse2 CPU
    if (system("grep -q \"^flags.*\\<sse2\\>\" /proc/cpuinfo") != 0) {
        downloadFlash();
    } else {
        ui->progressBar->show();
        disconnect(timer, SIGNAL(timeout()), 0, 0);
        connect(timer, SIGNAL(timeout()), this, SLOT(procTime()));
        disconnect(proc, SIGNAL(started()), 0, 0);
        connect(proc, SIGNAL(started()), this, SLOT(procStart()));
        disconnect(proc, SIGNAL(finished(int)), 0, 0);
        connect(proc, SIGNAL(finished(int)), this, SLOT(procUpdateDone(int)));
        ui->labelInstall->setText(tr("Running apt-get update..."));
        proc->start("apt-get update");
    }
}

void mxflash::installRemovePepper() {
    setCursor(QCursor(Qt::WaitCursor));
    ui->stackedWidget->setCurrentWidget(ui->pageInstall);
    if (getCmdOut("dpkg -s pepperflashplugin-nonfree| grep Status") != "Status: install ok installed") {
        if (system("grep -q \"^flags.*\\<sse2\\>\" /proc/cpuinfo") != 0) {
            QMessageBox::critical(0, tr("Error"),tr("PepperFlash cannot be installed because your CPU does not support SSE2."));;
            return;
        }
        if (getCmdOut("dpkg -s chromium| grep Status") != "Status: install ok installed") {
            this->hide();
            int ans = QMessageBox::question(0, tr("MX Flash Manager"),
                                  tr("Chromium is not installed. Do you want to install PepperFlash anyway?"), tr("Yes"), tr("No"));
            this->show();
            if (ans == 1) {
                setCursor(QCursor(Qt::ArrowCursor));
                ui->stackedWidget->setCurrentWidget(ui->pageAction);
                return;
            }
        }
        // run updates
        ui->progressBar->show();
        QEventLoop loop;
        disconnect(timer, SIGNAL(timeout()), 0, 0);
        connect(timer, SIGNAL(timeout()), this, SLOT(procTime()));
        disconnect(proc, SIGNAL(started()), 0, 0);
        connect(proc, SIGNAL(started()), this, SLOT(procStart()));
        disconnect(proc, SIGNAL(finished(int)), 0, 0);
        connect(proc, SIGNAL(finished(int)), &loop, SLOT(quit()));
        ui->labelInstall->setText(tr("Running apt-get update..."));
        proc->start("apt-get update");
        loop.exec();

        // run install
        ui->progressBar->show();
        ui->labelInstall->setText(tr("Installing PepperFlash..."));
        connect(proc, SIGNAL(finished(int)), this, SLOT(procDone(int)));
        proc->start("apt-get install pepperflashplugin-nonfree");

    } else { //remove pepperflash
        setConnections(timer, proc);
        ui->labelInstall->setText(tr("Removing PepperFlash..."));
        proc->start("dpkg -P pepperflashplugin-nonfree");
    }
}

/////////////////////////////////////////////////////////////////////////
// sync process events

void mxflash::procStart() {
    timer->start(100);
}

void mxflash::procTime() {
    int i = ui->progressBar->value() + 1;
    if (i > 100) {
        i = 0;
    }
    ui->progressBar->setValue(i);
}

void mxflash::procDone(int exitCode) {
    timer->stop();
    ui->progressBar->setValue(100);
    setCursor(QCursor(Qt::ArrowCursor));
    if (exitCode == 0) {
        this->hide();
        if (QMessageBox::information(0, tr("Success"),
                                     tr("Process finished with success.<p><b>Do you want to exit MX Flash Manager?</b>"),
                                     tr("Yes"), tr("No")) == 0){
            qApp->exit(0);
        } else {
            refresh();
        }
    } else {
        QMessageBox::critical(0, tr("Error"),
                              tr("Process finished. Errors have occurred."));
        qApp->exit(1);
    }
}

// install flashplugin-nonfree when done with apt-get update
void mxflash::procUpdateDone(int exitCode) {
    timer->stop();
    if (exitCode == 0) {
        setConnections(timer, proc);
        ui->labelInstall->setText(tr("Installing..."));
        proc->start("apt-get install flashplugin-nonfree");
    } else {
        QMessageBox::critical(0, tr("Error"),
                              tr("\"apt-get update\" command did not run successfully."));
        qApp->exit(1);
    }
}

void mxflash::procDownloadDone(int exitCode) {
    timer->stop();
    ui->progressBar->setValue(100);
    if (exitCode == 0) {
        QString path = "/usr/lib/flashplugin-nonfree";
        ui->progressBar->show();
        disconnect(timer, SIGNAL(timeout()), 0, 0);
        connect(timer, SIGNAL(timeout()), this, SLOT(procTime()));
        disconnect(proc, SIGNAL(started()), 0, 0);
        connect(proc, SIGNAL(started()), this, SLOT(procStart()));
        disconnect(proc, SIGNAL(finished(int)), 0, 0);
        connect(proc, SIGNAL(finished(int)), this, SLOT(procDone(int)));
        QString cmd = QString("tar --directory=/usr/lib/flashplugin-nonfree --extract --file=%1/flashplayer_SSE_11.2.202.235.so.tar.gz libflashplayer.so").arg(path);
        ui->labelInstall->setText(tr("Installing..."));
        proc->start(cmd);
    } else {
        QMessageBox::critical(0, tr("Error"),
                              tr("Could not download Flash for non-SSE2 CPUs."));
        qApp->exit(1);
    }
}

void mxflash::setConnections(QTimer* timer, QProcess* proc) {
    ui->progressBar->show();
    disconnect(timer, SIGNAL(timeout()), 0, 0);
    connect(timer, SIGNAL(timeout()), this, SLOT(procTime()));
    disconnect(proc, SIGNAL(started()), 0, 0);
    connect(proc, SIGNAL(started()), this, SLOT(procStart()));
    disconnect(proc, SIGNAL(finished(int)), 0, 0);
    connect(proc, SIGNAL(finished(int)), this, SLOT(procDone(int)));
}

/////////////////////////////////////////////////////////////////////////
// slots

// OK button clicked
void mxflash::on_buttonOk_clicked() {
    if (ui->stackedWidget->currentIndex() == 0) {
        if (ui->removeFlashButton->isChecked()) {
            removeFlash();
        } else if (ui->updateFlashButton->isChecked()) {
            ui->stackedWidget->setCurrentWidget(ui->pageFlashUpdate);
        } else if (ui->installFlashButton->isChecked()) {
            installFlash();
        } else if (ui->pepperButton->isChecked()) {
            installRemovePepper();
        } else if (ui->updatePepperButton->isChecked()) {
            ui->stackedWidget->setCurrentWidget(ui->pagePepperUpdate);
        }
    } else if (ui->stackedWidget->currentWidget() == ui->pageFlashUpdate) {
        updateFlash();
    } else if (ui->stackedWidget->currentWidget() == ui->pagePepperUpdate) {
        updatePepper();
    } else {
        qApp->exit(0);
    }
}


// About button clicked
void mxflash::on_buttonAbout_clicked() {
    QMessageBox msgBox(QMessageBox::NoIcon,
                       tr("About MX Flash Manager"), "<p align=\"center\"><b><h2>" +
                       tr("MX Flash Manager") + "</h2></b></p><p align=\"center\">" + tr("Version: ") +
                       getVersion("mx-flash") + "</p><p align=\"center\"><h3>" +
                       tr("Simple Flash manager for antiX MX Linux") + "</h3></p><p align=\"center\"><a href=\"http://www.mepiscommunity.org/mx\">http://www.mepiscommunity.org/mx</a><br /></p><p align=\"center\">" +
                       tr("Copyright (c) MX Linux") + "<br /><br /></p>", 0, this);
    msgBox.addButton(tr("Cancel"), QMessageBox::AcceptRole); // because we want to display the buttons in reverse order we use counter-intuitive roles.
    msgBox.addButton(tr("License"), QMessageBox::RejectRole);
    if (msgBox.exec() == QMessageBox::RejectRole) {
        if (checkOnline()) {
            system("mx-viewer http://www.mepiscommunity.org/doc_mx/mx-flash-license.html " + tr("'MX Flash License'").toUtf8());
        } else {
            system("mx-viewer file:///usr/local/share/doc/mx-flash-license.html " + tr("'MX Flash License'").toUtf8());
        }
    }
}


// Help button clicked
void mxflash::on_buttonHelp_clicked() {
    if (checkOnline()) {
        system("mx-viewer http://mepiscommunity.org/wiki/help-files/help-mx-flash-manager " + tr("'MX Flash Help'").toUtf8());
    } else {
        system("mx-viewer file:///usr/local/share/doc/mxum.html#flash " + tr("'MX Flash Help'").toUtf8());
    }
}
