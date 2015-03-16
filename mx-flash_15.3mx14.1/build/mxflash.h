/*****************************************************************************
 * mxflash.h
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

#ifndef MXFLASH_H
#define MXFLASH_H

#include <QMessageBox>
#include <QProcess>
#include <QTimer>
#include <QComboBox>


namespace Ui {
class mxflash;
}

class mxflash : public QDialog
{
    Q_OBJECT

protected:
    QProcess *proc;
    QTimer *timer;

public:
    explicit mxflash(QWidget *parent = 0);
    ~mxflash();
    // helpers
    QString getCmdOut(QString cmd);
    QString getVersion(QString name);
    bool checkOnline();
    void refresh();
    void detectVersion();

    void removeFlash();
    void updateFlash();
    void updatePepper();
    void installFlash();
    void downloadFlash();
    bool checkDebInstalled();
    void installRemovePepper();

public slots:
    void procStart();
    void procTime();
    void procDone(int exitCode);
    void procUpdateDone(int exitCode);
    void procDownloadDone(int exitCode);
    void setConnections(QTimer* timer, QProcess* proc);

    virtual void on_buttonOk_clicked();
    virtual void on_buttonAbout_clicked();
    virtual void on_buttonHelp_clicked();

private:
    Ui::mxflash *ui;
};

#endif // MXFLASH_H
