// parser.cpp
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

#include "parser.h"

Parser::Parser(int chan_id,int pat_id)
{
  parser_channel_id=chan_id;
  parser_pattern_id=pat_id;
}


void Parser::setPattern(const QString &str)
{
  parser_pattern=str;
  parser_istate=0;
}


void Parser::process(const QByteArray &data)
{
  for(int i=0;i<data.size();i++) {
    if(data.at(i)==parser_pattern[parser_istate]) {
      if(++parser_istate==parser_pattern.length()) {
	emit matchFound(parser_channel_id,parser_pattern_id);
	parser_istate=0;
      }
    }
    else {
      parser_istate=0;
    }
  }
}
