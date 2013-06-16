/*
 * Copyright (C) 2012-2013 Fanout, Inc.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ZHTTPREQUESTPACKET_H
#define ZHTTPREQUESTPACKET_H

#include <QUrl>
#include <QVariant>
#include <QHostAddress>
#include "httpheaders.h"

class ZhttpRequestPacket
{
public:
	enum Type
	{
		Data,
		Error,
		Credit,
		Cancel,
		HandoffStart,
		HandoffProceed
	};

	QByteArray from;
	QByteArray id;

	Type type;
	QByteArray condition;
	int seq;

	int credits;
	bool more;
	bool stream;
	int maxSize;

	QString method;
	QUrl uri;
	HttpHeaders headers;
	QByteArray body;

	QVariant userData;

	QHostAddress peerAddress;
	int peerPort;

	QString connectHost;
	int connectPort;
	bool ignorePolicies;
	bool ignoreTlsErrors;

	ZhttpRequestPacket() :
		type((Type)-1),
		seq(-1),
		credits(-1),
		more(false),
		stream(false),
		maxSize(-1),
		peerPort(-1),
		connectPort(-1),
		ignorePolicies(false),
		ignoreTlsErrors(false)
	{
	}

	QVariant toVariant() const;
	bool fromVariant(const QVariant &in);
};

#endif