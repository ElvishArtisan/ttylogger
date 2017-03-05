// parser.h
//
// Find a matching string in a stream of characters
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

#ifndef PARSER_H
#define PARSER_H

#include <QByteArray>
#include <QObject>
#include <QString>

class Parser : public QObject
{
  Q_OBJECT;
 public:
  Parser(int chan_id,int pat_id);
  void setPattern(const QString &str);
  void process(const QByteArray &data);

 signals:
  void matchFound(int chan_id,int pat_id);

 private:
  int parser_channel_id;
  int parser_pattern_id;
  QString parser_pattern;
  int parser_istate;
};


#endif  // PARSER_H
