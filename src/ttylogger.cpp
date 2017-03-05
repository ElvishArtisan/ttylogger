// ttylogger.cpp
//
// ttylogger(8) Serial logger
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

#include <stdio.h>
#include <syslog.h>

#include <QByteArray>
#include <QCoreApplication>
#include <QDateTime>

#include "cmdswitch.h"
#include "ttylogger.h"

MainObject::MainObject(QObject *parent)
  : QObject(parent)
{
  new CmdSwitch(qApp->argc(),qApp->argv(),"ttylogger",VERSION,"ttylogger");

  //
  // Configuration
  //
  main_config=new Config();
  main_config->load();

  //
  // Parsers
  //
  for(int i=0;i<main_config->channelQuantity();i++) {
    for(int j=0;j<main_config->patternQuantity(i);j++) {
      main_parsers.push_back(new Parser(i,j));
      main_parsers.back()->setPattern(main_config->pattern(i,j));
      connect(main_parsers.back(),SIGNAL(matchFound(int,int)),
	      this,SLOT(matchFoundData(int,int)));
    }
  }

  //
  // Serial Device
  //
  main_tty_device=new TTYDevice(this);
  main_tty_device->setDeviceName(main_config->ttyDevice());
  main_tty_device->setSpeed(main_config->ttySpeed());
  main_tty_device->setParity(main_config->ttyParity());
  main_tty_device->setWordLength(main_config->ttyWordLength());
  connect(main_tty_device,SIGNAL(readyRead()),this,SLOT(readyReadData()));
  if(!main_tty_device->open(QIODevice::ReadOnly)) {
    fprintf(stderr,"ttylogger: unable to open serial device \"%s\"",
	    (const char *)main_tty_device->deviceName().toUtf8());
    exit(256);
  }

  main_garbage_timer=new QTimer(this);
  main_garbage_timer->setSingleShot(true);
  connect(main_garbage_timer,SIGNAL(timeout()),this,SLOT(collectGarbageData()));
}


void MainObject::matchFoundData(int chan_id,int pat_id)
{
  if(!main_config->string(chan_id,pat_id).isEmpty()) {
    LogMatch(chan_id,pat_id);
  }
  if(!main_config->script(chan_id,pat_id).isEmpty()) {
    RunMatchScript(chan_id,pat_id);
  }
}


void MainObject::readyReadData()
{
  QByteArray data=main_tty_device->readAll();
  for(int i=0;i<main_parsers.size();i++) {
    main_parsers.at(i)->process(data);
  }
}


void MainObject::scriptFinishedData()
{
  main_garbage_timer->start(1);
}


void MainObject::collectGarbageData()
{
  for(int i=main_script_list.size()-1;i>=0;i--) {
    if(main_script_list.at(i)->processState()==QProcess::NotRunning) {
      delete main_script_list.at(i);
      main_script_list.erase(main_script_list.begin()+i);
    }
  }
}


void MainObject::LogMatch(int chan_id,int pat_id) const
{
  FILE *f=NULL;
  QDateTime now=QDateTime::currentDateTime();
  int tenth=lround((double)now.time().msec()/100.0);
  if(tenth==10) {
    now.setTime(now.time().addSecs(1));
    tenth=0;
  }
  QString filename=main_config->channelLogFilename(chan_id,now);
  
  if((f=fopen(filename.toUtf8(),"a"))==NULL) {
    syslog(LOG_WARNING,"unable to open log file \"%s\"",
	   (const char *)filename.toUtf8());
    return;
  }
  fprintf(f,"%s: %s\n",(const char *)(now.toString("hh:mm:ss")+
				      QString().sprintf(".%d",tenth)).toUtf8(),
	  (const char *)main_config->string(chan_id,pat_id).toUtf8());
  fclose(f);
}


void MainObject::RunMatchScript(int chan_id,int pat_id)
{
  QStringList args;
  args.push_back(main_config->pattern(chan_id,pat_id));

  main_script_list.push_back(new ScriptEvent(this));
  connect(main_script_list.back(),SIGNAL(finished()),
	  this,SLOT(scriptFinishedData()));
  main_script_list.back()->start(main_config->script(chan_id,pat_id),args,
				 main_config->channelLogFilename(chan_id));
}


int main(int argc,char *argv[])
{
  QCoreApplication a(argc,argv);
  new MainObject();

  return a.exec();
}
