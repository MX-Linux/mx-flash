/*****************************************************************************
 * mxflash.cpp
 *****************************************************************************
 * Copyright (C) 2014 MX Authors
 *
 * Authors: Adrian
 *          MEPIS Community <http://forum.mepiscommunity.org>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#include "mxflash.h"
#include "ui_mxflash.h"

#include <QDir>
#include <QTextStream>
#include <QWebView>

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
    proc = new QProcess(this);
    proc->start("/bin/bash", QStringList() << "-c" << cmd);
    proc->setReadChannel(QProcess::StandardOutput);
    proc->setReadChannelMode(QProcess::MergedChannels);
    proc->waitForFinished(-1);
    return proc->readAllStandardOutput().trimmed();
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
    ui->stackedWidget->setCurrentIndex(0);
    ui->progressBar->hide();
    ui->progressBar->setValue(0);
    ui->removeRadioButton->setAutoExclusive(false);
    ui->removeRadioButton->setChecked(false);
    ui->removeRadioButton->setAutoExclusive(true);
    setCursor(QCursor(Qt::ArrowCursor));
    detectVersion();
}

// detect and list the version of Flash and PepperFlash
void mxflash::detectVersion() {
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
    // PepperFlash
    QString versionChrome = getCmdOut("strings /opt/google/chrome/PepperFlash/libpepflashplayer.so | grep LNX | awk '{print $2}' | sed 's/,/./g'");
    QString versionChromium = getCmdOut("strings /usr/lib/pepperflashplugin-nonfree/libpepflashplayer.so | grep LNX | awk '{print $2}' | sed 's/,/./g'");
    if (versionChrome != "") {
        out += "\n" + tr("PepperFlash for Chrome version: ") + versionChrome;
    }
    if (versionChromium != "" ) {
        out += "\n" + tr("PepperFlash for Chromium version: ") + versionChromium;
    }
    ui->labelVersion->setText(out);
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
    QString fname = "/usr/lib/flashplugin-nonfree/libflashplayer.so";
    QFile file(fname);

    // checks if Flash present
    if (!file.exists()) {
        int ans = QMessageBox::critical(0, tr("Error"),
                                        tr("<b>Flash is not installed. Would you like to install it?</b>"),
                                        tr("Yes"), tr("No"));
        if (ans == 0) {
            installFlash();
            return;
        } else {
            refresh();
            return;
        }
    // if file is present but not installed with a deb
    } else if (!checkDebInstalled()) {
        QMessageBox::critical(0, tr("Error"),
                              tr("Flash is installed through other means, cannot update with this program."));
        refresh();
        return;
    }

    setCursor(QCursor(Qt::WaitCursor));
    qApp->processEvents();

    // manual update
    if (ui->manualRadioButton->isChecked()) {
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
        system(cmd.toAscii());

        setCursor(QCursor(Qt::ArrowCursor));
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

    ui->labelInstall->setText("Downloading Flash..");
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
        ui->labelInstall->setText("Running apt-get update...");
        proc->start("apt-get update");
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
        ui->labelInstall->setText("Installing...");
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
        ui->labelInstall->setText("Installing...");
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
    if (ui->removeRadioButton->isChecked()) {
      removeFlash();
    } else if (ui->updateRadioButton->isChecked()) {
      ui->stackedWidget->setCurrentWidget(ui->pageUpdate);
    } else if (ui->installRadioButton->isChecked()) {
      installFlash();
    }
    } else if (ui->stackedWidget->currentWidget() == ui->pageUpdate) {
        updateFlash();
    } else {
        qApp->exit(0);
    }
}


// About button clicked
void mxflash::on_buttonAbout_clicked() {
    QMessageBox msgBox(QMessageBox::NoIcon,
                       tr("About MX Flash Manager"), "<p align=\"center\"><b><h2>" +
                       tr("MX Flash Manager") + "</h2></b></p><p align=\"center\">MX14+git20140625</p><p align=\"center\"><h3>" +
                       tr("Simple Flash manager for antiX MX") + "</h3></p><p align=\"center\"><a href=\"http://www.mepiscommunity.org/mx\">http://www.mepiscommunity.org/mx</a><br /></p><p align=\"center\">" +
                       tr("Copyright (c) antiX") + "<br /><br /></p>", 0, this);
    msgBox.addButton(tr("License"), QMessageBox::AcceptRole);
    msgBox.addButton(tr("Cancel"), QMessageBox::DestructiveRole);
    if (msgBox.exec() == QMessageBox::AcceptRole)
        displaySite("file:///usr/local/share/doc/mx-flash-license.html");
}


// Help button clicked
void mxflash::on_buttonHelp_clicked() {
    displaySite("file:///usr/local/share/doc/mxapps.html#flash");
}

// pop up a window and display website
void mxflash::displaySite(QString site) {
    QWidget *window = new QWidget(this, Qt::Dialog);
    window->setWindowTitle(this->windowTitle());
    window->resize(800, 500);
    QWebView *webview = new QWebView(window);
    webview->load(QUrl(site));
    webview->show();
    window->show();
}
