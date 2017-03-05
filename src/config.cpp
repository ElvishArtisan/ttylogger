// config.cpp
//
// Configuration for ttylogger(8)
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

#include "config.h"
#include "profile.h"

Config::Config(QObject *parent)
  : QObject(parent)
{
  conf_watchdog_timer=new QTimer(this);
  connect(conf_watchdog_timer,SIGNAL(timeout()),
	  this,SLOT(checkWatchdogsData()));
}


QString Config::ttyDevice() const
{
  return conf_tty_device;
}


TTYDevice::Parity Config::ttyParity() const
{
  return conf_tty_parity;
}


int Config::ttySpeed() const
{
  return conf_tty_speed;
}


int Config::ttyWordLength() const
{
  return conf_tty_word_length;
}


int Config::channelQuantity() const
{
  return conf_channel_directories.size();
}


QString Config::channelDirectory(int chan) const
{
  return conf_channel_directories.at(chan);
}


QString Config::channelLogFilename(int chan,QDateTime now)
{
  if(now.isNull()) {
    now=QDateTime::currentDateTime();
  }
  return channelDirectory(chan)+"/"+now.toString("yyyy-MM-dd")+".txt";
}


int Config::patternQuantity(int chan) const
{
  return conf_channel_patterns.at(chan).size();
}


QString Config::pattern(int chan,int pat) const
{
  return conf_channel_patterns.at(chan).at(pat);
}


QString Config::string(int chan,int pat) const
{
  return conf_channel_strings.at(chan).at(pat);
}


QString Config::script(int chan,int pat) const
{
  return conf_channel_scripts.at(chan).at(pat);
}


int Config::watchdogTimeout(int chan,int pat) const
{
  return conf_watchdog_timeouts.at(chan).at(pat);
}


QString Config::watchdogSetString(int chan,int pat) const
{
  return conf_watchdog_set_strings.at(chan).at(pat);
}


QString Config::watchdogResetString(int chan,int pat) const
{
  return conf_watchdog_reset_strings.at(chan).at(pat);
}


QString Config::watchdogScript(int chan,int pat) const
{
  return conf_watchdog_scripts.at(chan).at(pat);
}


QDateTime Config::timestamp(int chan,int pat)
{
  return conf_timestamps[chan][pat];
}


void Config::touchTimestamp(int chan,int pat)
{
  conf_timestamps[chan][pat].setDate(QDate::currentDate());
  conf_timestamps[chan][pat].setTime(QTime::currentTime());
}


void Config::load()
{
  Profile *p=new Profile();
  QString str;
  QString section;
  int channel=0;
  bool ok=false;

  p->setSource(CONFIG_FILE);
  conf_tty_device=p->stringValue("Global","TtyDevice","/dev/ttyS0");
  str=p->stringValue("Global","TtyParity","none").toLower();
  if(str=="even") {
    conf_tty_parity=TTYDevice::Even;
  }
  if(str=="odd") {
    conf_tty_parity=TTYDevice::Odd;
  }
  if(str=="none") {
    conf_tty_parity=TTYDevice::None;
  }
  conf_tty_speed=p->intValue("Global","TtySpeed",9600);
  conf_tty_word_length=p->intValue("Global","TtyWordLength",8);

  section=QString().sprintf("Channel%d",channel+1);
  str=p->stringValue(section,"Directory","",&ok);
  while(ok) {
    conf_channel_directories.push_back(str);
    conf_channel_patterns.push_back(QStringList());
    conf_channel_strings.push_back(QStringList());
    conf_channel_scripts.push_back(QStringList());
    QList<int> timeouts;
    conf_watchdog_timeouts.push_back(timeouts);
    conf_watchdog_set_strings.push_back(QStringList());
    conf_watchdog_reset_strings.push_back(QStringList());
    conf_watchdog_scripts.push_back(QStringList());
    QList<QDateTime> dts;
    conf_timestamps.push_back(dts);
    QList<bool> states;
    conf_watchdog_states.push_back(states);
    int pattern=0;
    str=p->stringValue(section,QString().sprintf("Pattern%d",pattern+1),"",&ok);
    while(ok) {
      conf_channel_patterns.back().push_back(str);
      conf_channel_strings.back().
        push_back(p->stringValue(section,QString().sprintf("String%d",
							   pattern+1),str));
      conf_channel_scripts.back().
        push_back(p->stringValue(section,QString().sprintf("Script%d",
							   pattern+1)));

      conf_watchdog_timeouts.back().
        push_back(p->intValue(section,QString().sprintf("WatchdogTimeout%d",
							pattern+1)));
      conf_watchdog_set_strings.back().
	push_back(p->stringValue(section,QString().
				 sprintf("WatchdogSetString%d",pattern+1)));
      conf_watchdog_reset_strings.back().
        push_back(p->stringValue(section,QString().
				 sprintf("WatchdogResetString%d",pattern+1)));
      conf_watchdog_scripts.back().
        push_back(p->stringValue(section,QString().sprintf("WatchdogScript%d",
							   pattern+1)));
      conf_timestamps.back().push_back(QDateTime());
      conf_timestamps.back().back()=QDateTime::currentDateTime();
      conf_watchdog_states.back().push_back(false);
      pattern++;
      str=p->stringValue(section,QString().sprintf("Pattern%d",pattern+1),"",
			 &ok);
    }
    channel++;
    section=QString().sprintf("Channel%d",channel+1);
    str=p->stringValue(section,"Directory","",&ok);    
  }
  conf_watchdog_timer->start(1000);
}


void Config::checkWatchdogsData()
{
  QDateTime now=QDateTime::currentDateTime();

  for(int i=0;i<conf_channel_patterns.size();i++) {
    for(int j=0;j<conf_channel_patterns.at(i).size();j++) {
      if(conf_watchdog_timeouts.at(i).at(j)>0) {
	if((!conf_watchdog_states.at(i).at(j))&&
	   (conf_timestamps.at(i).at(j).
	    addSecs(conf_watchdog_timeouts.at(i).at(j))<now)) {
	  conf_watchdog_states[i][j]=true;
	  emit watchdogStateChanged(i,j,true);
	}
	if(conf_watchdog_states.at(i).at(j)&&
	   (conf_timestamps.at(i).at(j).
	    addSecs(conf_watchdog_timeouts.at(i).at(j))>now)) {
	  conf_watchdog_states[i][j]=false;
	  emit watchdogStateChanged(i,j,false);
	}
      }
    }
  }
}
