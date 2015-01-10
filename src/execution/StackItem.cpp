/* Copyright (c) 2014 Francois Doray <francois.pierre-doray@polymtl.ca>
 *
 * This file is part of tibeecompare.
 *
 * tibeecompare is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * tibeecompare is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with tibeecompare.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "execution/StackItem.hpp"

#include <assert.h>

namespace tibee
{
namespace execution
{

StackItem::StackItem()
    : _depth(0), _start(0), _end(0)
{
}

StackItem::~StackItem()
{
}

bool StackItem::GetNumericProperty(quark::Quark key, uint64_t* value) const
{
    assert(value != nullptr);

    auto it = _numericProperties.find(key);
    if (it == _numericProperties.end())
        return false;

    *value = it->second;
    return true;
}

void StackItem::SetNumericProperty(quark::Quark key, uint64_t value)
{
    _numericProperties[key] = value;
}

bool StackItem::GetStringProperty(quark::Quark key, std::string* value) const
{
    assert(value != nullptr);

    auto it = _stringProperties.find(key);
    if (it == _stringProperties.end())
        return false;

    *value = it->second;
    return true;
}

void StackItem::SetStringProperty(quark::Quark key, std::string value)
{
    _stringProperties[key] = value;
}

bool StackItem::operator==(const StackItem& other) const
{
    return _name == other._name &&
        _depth == other._depth &&
        _start == other._start &&
        _end == other._end &&
        _numericProperties == other._numericProperties &&
        _stringProperties == other._stringProperties;
}

}  // namespace execution
}  // namespace tibee