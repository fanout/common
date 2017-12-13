/*
 * Copyright (C) 2017 Fanout, Inc.
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
 *
 */

#include <QtTest/QtTest>
#include "httpheaders.h"

class HttpHeadersTest : public QObject
{
	Q_OBJECT

private slots:
	void parseParameters()
	{
		HttpHeaders h;
		h += HttpHeader("Fruit", "apple");
		h += HttpHeader("Fruit", "banana");
		h += HttpHeader("Fruit", "cherry");

		QList<HttpHeaderParameters> params = h.getAllAsParameters("Fruit");
		QCOMPARE(params.count(), 3);
		QCOMPARE(params[0][0].first, QByteArray("apple"));
		QCOMPARE(params[1][0].first, QByteArray("banana"));
		QCOMPARE(params[2][0].first, QByteArray("cherry"));

		h.clear();
		h += HttpHeader("Fruit", "apple, banana, cherry");

		params = h.getAllAsParameters("Fruit");
		QCOMPARE(params.count(), 3);
		QCOMPARE(params[0][0].first, QByteArray("apple"));
		QCOMPARE(params[1][0].first, QByteArray("banana"));
		QCOMPARE(params[2][0].first, QByteArray("cherry"));

		h.clear();
		h += HttpHeader("Fruit", "apple; type=\"granny, smith\", banana; type=\"\\\"yellow\\\"\"");

		params = h.getAllAsParameters("Fruit");
		QCOMPARE(params.count(), 2);
		QCOMPARE(params[0][0].first, QByteArray("apple"));
		QCOMPARE(params[0][1].first, QByteArray("type"));
		QCOMPARE(params[0][1].second, QByteArray("granny, smith"));
		QCOMPARE(params[1][0].first, QByteArray("banana"));
		QCOMPARE(params[1][1].first, QByteArray("type"));
		QCOMPARE(params[1][1].second, QByteArray("\"yellow\""));

		h.clear();
		h += HttpHeader("Fruit", "\"apple");

		QList<QByteArray> l = h.getAll("Fruit");
		QCOMPARE(l.count(), 1);
		QCOMPARE(l[0], QByteArray("\"apple"));

		h.clear();
		h += HttpHeader("Fruit", "\"apple\\");

		l = h.getAll("Fruit");
		QCOMPARE(l.count(), 1);
		QCOMPARE(l[0], QByteArray("\"apple\\"));

		h.clear();
		h += HttpHeader("Fruit", "apple; type=gala, banana; type=\"yellow, cherry");

		params = h.getAllAsParameters("Fruit");
		QCOMPARE(params.count(), 1);
		QCOMPARE(params[0][0].first, QByteArray("apple"));
		QCOMPARE(params[0][1].first, QByteArray("type"));
		QCOMPARE(params[0][1].second, QByteArray("gala"));
	}
};

QTEST_MAIN(HttpHeadersTest)
#include "httpheaderstest.moc"
