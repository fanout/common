/*
 * Copyright (C) 2014 Fanout, Inc.
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

#ifndef LAYERTRACKER_H
#define LAYERTRACKER_H

#include <QList>

class LayerTracker
{
public:
	LayerTracker();

	void reset();

	void addPlain(int plain);
	void specifyEncoded(int encoded, int plain);
	int finished(int encoded);

private:
	class Item
	{
	public:
		int plain;
		int encoded;
	};

	int plain_;
	QList<Item> items_;
};

#endif
