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
#ifndef _TIBEE_EXECUTION_STACKSBUILDER_HPP
#define _TIBEE_EXECUTION_STACKSBUILDER_HPP

#include <unordered_map>
#include <vector>

#include "base/BasicTypes.hpp"
#include "db/Database.hpp"
#include "stacks/Identifiers.hpp"

namespace tibee
{
namespace stacks
{

class StacksBuilder
{
public:
    typedef std::function<void (
        StackId stackId, timestamp_t duration)> EnumerateStacksCallback;

    StacksBuilder();
    ~StacksBuilder();

    // Set current timestamp.
    void SetTimestamp(timestamp_t ts) { _ts = ts; }

    // Removes everything that is before the specified timestamp.
    void Cleanup(timestamp_t ts);

    // Set database.
    void SetDatabase(db::Database* db) { _db = db; }

    // Set the current stack for a thread.
    void SetStack(thread_t thread, StackId stackId, bool isSyscall);
    void SetStack(thread_t thread, StackId stackId);
    void SetStack(thread_t thread, const std::vector<std::string>& stack);
    void SetNoStack(thread_t thread);

    // Start a system call on a thread.
    void StartSystemCall(thread_t thread, const std::string& syscall);

    // End a system call on a thread.
    void EndSytemCall(thread_t thread);

    // Set the stack for the last system call of a thread.
    // The stack is usually set for long system calls.
    void SetLastSystemCallStack(thread_t thread, StackId stackId);
    void SetLastSystemCallStack(thread_t thread, const std::vector<std::string>& stack);

    // Enumerate the stacks encountered on a thread in
    // the specified time interval.
    void EnumerateStacks(thread_t thread, timestamp_t start, timestamp_t end,
                         const EnumerateStacksCallback& callback) const;

    // Get the stack at the specified timestamp.
    stacks::StackId GetStack(thread_t thread, timestamp_t ts) const;

    // Set the end timestamp of the last stack of each thread to now.
    void Terminate();

private:
    // Get the identifier for a stack.
    StackId GetStackIdentifier(const std::vector<std::string>& stack);

    // Current timestamp.
    timestamp_t _ts;

    // Pointer to the database.
    db::Database* _db;

    // Stacks per thread.
    struct StackWrapper
    {
        StackId stackId;
        timestamp_t startTs;
        timestamp_t endTs;
        bool isSyscall;
        bool isResolved;
    };
    typedef std::unordered_map<thread_t, std::vector<StackWrapper>>
        StacksPerThread;
    StacksPerThread _stacks;

    class StacksComparator
    {
    public:
        bool operator() (const StackWrapper& stack, timestamp_t ts) const;
        bool operator() (timestamp_t ts, const StackWrapper& stack) const;
    };
};

}  // namespace stacks
}  // namespace tibee

#endif // _TIBEE_EXECUTION_STACKSBUILDER_HPP
