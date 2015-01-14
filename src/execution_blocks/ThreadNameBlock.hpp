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
#ifndef _TIBEE_EXECUTIONBLOCKS_THREADNAMEBLOCK_HPP
#define _TIBEE_EXECUTIONBLOCKS_THREADNAMEBLOCK_HPP

#include "execution_blocks/AbstractExecutionBlock.hpp"
#include "notification/Path.hpp"
#include "value/Value.hpp"

namespace tibee {
namespace execution_blocks {

/**
 * Sets the name of threads.
 *
 * @author Francois Doray
 */
class ThreadNameBlock : public execution_blocks::AbstractExecutionBlock
{
public:
    ThreadNameBlock();

    virtual void AddObservers(notification::NotificationCenter* notificationCenter) override;

private:
    void onExecName(uint32_t tid, const notification::Path& path, const value::Value* value);
};

}  // namespace execution_blocks
}  // namespace tibee

#endif // _TIBEE_EXECUTIONBLOCKS_THREADNAMEBLOCK_HPP