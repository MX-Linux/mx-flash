/*****************************************************************************
 * mxflash.cpp
 *****************************************************************************
 * Copyright (C) 2014 MX Authors with the exeption of getCmdOut function
 *  getCmdOut function copyright (C) 2003-2014 Warren Woodford
 *   released under the Apache License version 2.0
 *
//  * Authors: Adrian
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

#include <stdio.h>

#include <QDesktopServices>
#include <QUrl>
#include <QFile>
#include <QTextStream>


mxflash::mxflash(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::mxflash)
{
  ui->setupUi(this);
  ui->stackedWidget->setCurrentIndex(0);
  ui->progressBar->hide();
  proc = new QProcess(this);
  timer = new QTimer(this);
}

mxflash::~mxflash()
{
  delete ui;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Util function taken from minstall, part of MEPIS, Copyright (C) 2003-2010 by Warren Woodford
// Licensed under the Apache License, Version 2.0

QString mxflash::getCmdOut(QString cmd) {
  char line[260];
  const char* ret = "";
  FILE* fp = popen(cmd.toAscii(), "r");
  if (fp == NULL) {
    return QString (ret);
  }
  int i;
  if (fgets(line, sizeof line, fp) != NULL) {
    i = strlen(line);
    line[--i] = '\0';
    ret = line;
  }
  pclose(fp);
  return QString (ret);
}


/////////////////////////////////////////////////////////////////////////
// general

void mxflash::removeFlash() {
  setConnections(timer, proc);
  QString cmd = QString("dpkg -P flashplugin-nonfree");
  proc->start(cmd);
}

void mxflash::updateFlash() {
  //check if installed
  QString cmd = "dpkg -s flashplugin-nonfree | grep Status | cut -f 3 -d \" \"";
  QString out = getCmdOut(cmd);
  if (out != "ok") {
    QMessageBox::critical(0, tr("Error"),
                          tr("Flash is not installed"));
    ui->stackedWidget->setCurrentIndex(0);
    return;
  }

  setCursor(QCursor(Qt::WaitCursor));
  qApp->processEvents();

  // manual update
  if (ui->manualRadioButton->isChecked()) {
    setConnections(timer, proc);
    cmd = QString("update-flashplugin-nonfree -i");
    proc->start(cmd);

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
    QMessageBox::information(0, tr("Finished"),
                             tr("Flash update will run daily."));
    qApp->exit(0);
  }
}

void mxflash::installFlash() {
  setConnections(timer, proc);
  QString cmd = QString("apt-get install flashplugin-nonfree");
  proc->start(cmd);
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

void mxflash::procDone(int exitCode, QProcess::ExitStatus exitStatus) {
  timer->stop();
  ui->progressBar->setValue(100);
  setCursor(QCursor(Qt::ArrowCursor));
  if (exitStatus == QProcess::NormalExit) {
    QMessageBox::information(0, tr("Finished"),
                             tr("Process finished. Success."));
    qApp->exit(0);
  } else {
    QMessageBox::critical(0, tr("Error"),
                          tr("Process finished. Errors have occurred."));
    qApp->exit(1);

  }
}

void mxflash::setConnections(QTimer* timer, QProcess* proc) {
  ui->progressBar->show();
  disconnect(timer, SIGNAL(timeout()), 0, 0);
  connect(timer, SIGNAL(timeout()), this, SLOT(procTime()));
  disconnect(proc, SIGNAL(started()), 0, 0);
  connect(proc, SIGNAL(started()), this, SLOT(procStart()));
  disconnect(proc, SIGNAL(finished(int, QProcess::ExitStatus)), 0, 0);
  connect(proc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(procDone(int, QProcess::ExitStatus)));
}

/////////////////////////////////////////////////////////////////////////
// slots

// OK button clicked
void mxflash::on_buttonOk_clicked() {
  if (ui->stackedWidget->currentIndex() == 0) {
    if (ui->removeRadioButton->isChecked()) {
      setCursor(QCursor(Qt::WaitCursor));
      ui->stackedWidget->setCurrentWidget(ui->pageRemove);
      qApp->processEvents();
      removeFlash();
    } else if (ui->updateRadioButton->isChecked()) {
      ui->stackedWidget->setCurrentWidget(ui->pageUpdate);
    } else if (ui->installRadioButton->isChecked()) {
      ui->stackedWidget->setCurrentWidget(ui->pageInstall);
      setCursor(QCursor(Qt::WaitCursor));
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
                     tr("MX Flash Manager") + "</h2></b></p><p align=\"center\">MX14+git20140301</p><p align=\"center\"><h3>" +
                     tr("Simple Flash manager for antiX MX") + "</h3></p><p align=\"center\"><a href=\"http://www.mepiscommunity.org/mx\">http://www.mepiscommunity.org/mx</a><br /></p><p align=\"center\">" +
                     tr("Copyright (c) antiX") + "<br /><br /></p>", 0, this);
      msgBox.addButton(tr("License"), QMessageBox::AcceptRole);
                     msgBox.addButton(QMessageBox::Cancel);
      if (msgBox.exec() == QMessageBox::AcceptRole)
      QDesktopServices::openUrl(QUrl("file:///usr/local/share/doc/mx-flash-license.html"));
}


// Help button clicked
void mxflash::on_buttonHelp_clicked() {
  QDesktopServices::openUrl(QUrl("file:///usr/local/share/doc/mxapps.html#flash"));
}

