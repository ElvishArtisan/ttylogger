// ttylogger.h
//
// ttylogger(8) serial logger
//
//   (C) Copyright 2017 Fred Gleason <fredg@paravelsystems.com>
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as
//   published by the Free Software Foundation; either version 2 of
//   the License, or (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public
//   License along with this program; if not, write to the Free Software
//   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//

#ifndef TTYLOGGER_H
#define TTYLOGGER_H

#include <QList>
#include <QObject>
#include <QTimer>

#include "config.h"
#include "parser.h"
#include "scriptevent.h"
#include "ttydevice.h"

#define TTYLOGGER_USAGE "[options]\n"

class MainObject : public QObject
{
 Q_OBJECT;
 public:
  MainObject(QObject *parent=0);

 private slots:
  void matchFoundData(int chan_id,int pat_id);
  void readyReadData();
  void scriptFinishedData();
  void collectGarbageData();
  void watchdogStateChangedData(int chan_id,int pat_id,bool state);

 private:
  void LogMatch(int chan_id,int pat_id) const;
  void LogWatchdog(int chan_id,int pat_id,bool state) const;
  void Log(int chan_id,int pat_id,const QString &msg) const;
  void RunMatchScript(int chan_id,int pat_id);
  void RunWatchdogScript(int chan_id,int pat_id,bool state);
  QList<Parser *> main_parsers;
  TTYDevice *main_tty_device;
  QList<ScriptEvent *> main_script_list;
  QTimer *main_garbage_timer;
  Config *main_config;
};


#endif  // TTYLOGGER_H
