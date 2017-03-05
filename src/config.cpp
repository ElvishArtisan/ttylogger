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

Config::Config()
{
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
    int pattern=0;
    str=p->stringValue(section,QString().sprintf("Pattern%d",pattern+1),"",&ok);
    while(ok) {
      conf_channel_patterns.back().push_back(str);
      conf_channel_strings.back().
        push_back(p->stringValue(section,QString().sprintf("String%d",pattern+1),str));
      pattern++;
      str=p->stringValue(section,QString().sprintf("Pattern%d",pattern+1),"",
			 &ok);
    }
    channel++;
    section=QString().sprintf("Channel%d",channel+1);
    str=p->stringValue(section,"Directory","",&ok);    
  }
}
