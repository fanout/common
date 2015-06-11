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

#include "zhttprequestpacket.h"

#include "tnetstring.h"

QVariant ZhttpRequestPacket::toVariant() const
{
	QVariantHash obj;

	if(!from.isEmpty())
		obj["from"] = from;

	obj["id"] = id;

	QByteArray typeStr;
	switch(type)
	{
		case Error:          typeStr = "error"; break;
		case Credit:         typeStr = "credit"; break;
		case KeepAlive:      typeStr = "keep-alive"; break;
		case Cancel:         typeStr = "cancel"; break;
		case HandoffStart:   typeStr = "handoff-start"; break;
		case HandoffProceed: typeStr = "handoff-proceed"; break;
		case Close:          typeStr = "close"; break;
		case Ping:           typeStr = "ping"; break;
		case Pong:           typeStr = "pong"; break;
		default: break;
	}

	if(!typeStr.isEmpty())
		obj["type"] = typeStr;

	if(type == Error && !condition.isEmpty())
		obj["condition"] = condition;

	if(seq != -1)
		obj["seq"] = seq;

	if(credits != -1)
		obj["credits"] = credits;

	if(more)
		obj["more"] = true;

	if(stream)
		obj["stream"] = true;

	if(maxSize != -1)
		obj["max-size"] = maxSize;

	if(timeout != -1)
		obj["timeout"] = timeout;

	if(!method.isEmpty())
		obj["method"] = method.toLatin1();

	if(!uri.isEmpty())
		obj["uri"] = uri.toEncoded();

	if(!headers.isEmpty())
	{
		QVariantList vheaders;
		foreach(const HttpHeader &h, headers)
		{
			QVariantList vheader;
			vheader += h.first;
			vheader += h.second;
			vheaders += QVariant(vheader);
		}

		obj["headers"] = vheaders;
	}

	if(!body.isNull())
		obj["body"] = body;

	if(!contentType.isEmpty())
		obj["content-type"] = contentType;

	if(code != -1)
		obj["code"] = code;

	if(userData.isValid())
		obj["user-data"] = userData;

	if(!peerAddress.isNull())
		obj["peer-address"] = peerAddress.toString().toUtf8();

	if(peerPort != -1)
		obj["peer-port"] = QByteArray::number(peerPort);

	if(!connectHost.isEmpty())
		obj["connect-host"] = connectHost.toUtf8();

	if(connectPort != -1)
		obj["connect-port"] = connectPort;

	if(ignorePolicies)
		obj["ignore-policies"] = true;

	if(ignoreTlsErrors)
		obj["ignore-tls-errors"] = true;

	if(followRedirects)
		obj["follow-redirects"] = true;

	return obj;
}

bool ZhttpRequestPacket::fromVariant(const QVariant &in)
{
	if(in.type() != QVariant::Hash)
		return false;

	QVariantHash obj = in.toHash();

	from.clear();
	if(obj.contains("from"))
	{
		if(obj["from"].type() != QVariant::ByteArray)
			return false;

		from = obj["from"].toByteArray();
	}

	if(!obj.contains("id") || obj["id"].type() != QVariant::ByteArray)
		return false;
	id = obj["id"].toByteArray();

	type = Data;
	if(obj.contains("type"))
	{
		if(obj["type"].type() != QVariant::ByteArray)
			return false;

		QByteArray typeStr = obj["type"].toByteArray();

		if(typeStr == "error")
			type = Error;
		else if(typeStr == "credit")
			type = Credit;
		else if(typeStr == "keep-alive")
			type = KeepAlive;
		else if(typeStr == "cancel")
			type = Cancel;
		else if(typeStr == "handoff-start")
			type = HandoffStart;
		else if(typeStr == "handoff-proceed")
			type = HandoffProceed;
		else if(typeStr == "close")
			type = Close;
		else if(typeStr == "ping")
			type = Ping;
		else if(typeStr == "pong")
			type = Pong;
		else
			return false;
	}

	if(type == Error)
	{
		condition.clear();
		if(obj.contains("condition"))
		{
			if(obj["condition"].type() != QVariant::ByteArray)
				return false;

			condition = obj["condition"].toByteArray();
		}
	}

	seq = -1;
	if(obj.contains("seq"))
	{
		if(obj["seq"].type() != QVariant::Int)
			return false;

		seq = obj["seq"].toInt();
	}

	credits = -1;
	if(obj.contains("credits"))
	{
		if(obj["credits"].type() != QVariant::Int)
			return false;

		credits = obj["credits"].toInt();
	}

	more = false;
	if(obj.contains("more"))
	{
		if(obj["more"].type() != QVariant::Bool)
			return false;

		more = obj["more"].toBool();
	}

	stream = false;
	if(obj.contains("stream"))
	{
		if(obj["stream"].type() != QVariant::Bool)
			return false;

		stream = obj["stream"].toBool();
	}

	maxSize = -1;
	if(obj.contains("max-size"))
	{
		if(obj["max-size"].type() != QVariant::Int)
			return false;

		maxSize = obj["max-size"].toInt();
	}

	timeout = -1;
	if(obj.contains("timeout"))
	{
		if(obj["timeout"].type() != QVariant::Int)
			return false;

		timeout = obj["timeout"].toInt();
	}

	method.clear();
	if(obj.contains("method"))
	{
		if(obj["method"].type() != QVariant::ByteArray)
			return false;

		method = QString::fromLatin1(obj["method"].toByteArray());
	}

	uri.clear();
	if(obj.contains("uri"))
	{
		if(obj["uri"].type() != QVariant::ByteArray)
			return false;

		uri = QUrl::fromEncoded(obj["uri"].toByteArray(), QUrl::StrictMode);
	}

	headers.clear();
	if(obj.contains("headers"))
	{
		if(obj["headers"].type() != QVariant::List)
			return false;

		foreach(const QVariant &i, obj["headers"].toList())
		{
			QVariantList list = i.toList();
			if(list.count() != 2)
				return false;

			if(list[0].type() != QVariant::ByteArray || list[1].type() != QVariant::ByteArray)
				return false;

			headers += QPair<QByteArray, QByteArray>(list[0].toByteArray(), list[1].toByteArray());
		}
	}

	body.clear();
	if(obj.contains("body"))
	{
		if(obj["body"].type() != QVariant::ByteArray)
			return false;

		body = obj["body"].toByteArray();
	}

	contentType.clear();
	if(obj.contains("content-type"))
	{
		if(obj["content-type"].type() != QVariant::ByteArray)
			return false;

		contentType = obj["content-type"].toByteArray();
	}

	code = -1;
	if(obj.contains("code"))
	{
		if(obj["code"].type() != QVariant::Int)
			return false;

		code = obj["code"].toInt();
	}

	userData = obj["user-data"];

	peerAddress = QHostAddress();
	if(obj.contains("peer-address"))
	{
		if(obj["peer-address"].type() != QVariant::ByteArray)
			return false;

		peerAddress = QHostAddress(QString::fromUtf8(obj["peer-address"].toByteArray()));
	}

	peerPort = -1;
	if(obj.contains("peer-port"))
	{
		if(obj["peer-port"].type() != QVariant::Int)
			return false;

		peerPort = obj["peer-port"].toInt();
	}

	connectHost.clear();
	if(obj.contains("connect-host"))
	{
		if(obj["connect-host"].type() != QVariant::ByteArray)
			return false;

		connectHost = QString::fromUtf8(obj["connect-host"].toByteArray());
	}

	connectPort = -1;
	if(obj.contains("connect-port"))
	{
		if(obj["connect-port"].type() != QVariant::Int)
			return false;

		connectPort = obj["connect-port"].toInt();
	}

	ignorePolicies = false;
	if(obj.contains("ignore-policies"))
	{
		if(obj["ignore-policies"].type() != QVariant::Bool)
			return false;

		ignorePolicies = obj["ignore-policies"].toBool();
	}

	ignoreTlsErrors = false;
	if(obj.contains("ignore-tls-errors"))
	{
		if(obj["ignore-tls-errors"].type() != QVariant::Bool)
			return false;

		ignoreTlsErrors = obj["ignore-tls-errors"].toBool();
	}

	followRedirects = false;
	if(obj.contains("follow-redirects"))
	{
		if(obj["follow-redirects"].type() != QVariant::Bool)
			return false;

		followRedirects = obj["follow-redirects"].toBool();
	}

	return true;
}
