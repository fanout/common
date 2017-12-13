/*
 * Copyright (C) 2012-2013 Fanout, Inc.
 * 
 * $FANOUT_BEGIN_LICENSE:GPL$
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * Alternatively, this program may be used under the terms of a commercial
 * license, where the commercial license agreement is provided with the
 * software or contained in a written agreement between you and Fanout. For
 * further information use the contact form at <https://fanout.io/enterprise/>.
 *
 * $FANOUT_END_LICENSE$
 */

#ifndef HTTPHEADERS_H
#define HTTPHEADERS_H

#include <QByteArray>
#include <QPair>
#include <QList>

typedef QPair<QByteArray, QByteArray> HttpHeaderParameter;

class HttpHeaderParameters : public QList<HttpHeaderParameter>
{
public:
	bool contains(const QByteArray &key) const;
	QByteArray get(const QByteArray &key) const;
};

typedef QPair<QByteArray, QByteArray> HttpHeader;

class HttpHeaders : public QList<HttpHeader>
{
public:
	enum ParseMode
	{
		NoParseFirstParameter,
		ParseAllParameters
	};

	bool contains(const QByteArray &key) const;
	QByteArray get(const QByteArray &key) const;
	HttpHeaderParameters getAsParameters(const QByteArray &key, ParseMode mode = NoParseFirstParameter) const;
	QByteArray getAsFirstParameter(const QByteArray &key) const;
	QList<QByteArray> getAll(const QByteArray &key, bool split = true) const;
	QList<HttpHeaderParameters> getAllAsParameters(const QByteArray &key, ParseMode mode = NoParseFirstParameter, bool split = true) const;
	QList<QByteArray> takeAll(const QByteArray &key, bool split = true);
	void removeAll(const QByteArray &key);

	static QByteArray join(const QList<QByteArray> &values);
	static HttpHeaderParameters parseParameters(const QByteArray &in, ParseMode mode = NoParseFirstParameter, bool *ok = 0);
};

#endif
