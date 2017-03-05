// config.h
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

#ifndef CONFIG_H
#define CONFIG_H

#include <QList>
#include <QString>
#include <QStringList>

#include "ttydevice.h"

#define CONFIG_FILE "/etc/ttylogger.conf"

class Config
{
 public:
  Config();
  QString ttyDevice() const;
  TTYDevice::Parity ttyParity() const;
  int ttySpeed() const;
  int ttyWordLength() const;
  int channelQuantity() const;
  QString channelDirectory(int chan) const;
  int patternQuantity(int chan) const;
  QString pattern(int chan,int pat) const;  
  QString string(int chan,int pat) const;
  void load();

 private:
  QString conf_tty_device;
  TTYDevice::Parity conf_tty_parity;
  int conf_tty_speed;
  int conf_tty_word_length;
  QStringList conf_channel_directories;
  QList<QStringList> conf_channel_patterns;
  QList<QStringList> conf_channel_strings;
};


#endif  // CONFIG_H
