/* Copyright (c) 2015 Francois Doray <francois.pierre-doray@polymtl.ca>
 *
 * This file is part of tigerbeetle.
 *
 * tigerbeetle is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * tigerbeetle is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with tigerbeetle.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "stacks_blocks/ProfilerBlock.hpp"

#include "base/BindObject.hpp"
#include "value/Value.hpp"

namespace tibee {
namespace stacks_blocks {

using notification::Token;

ProfilerBlock::ProfilerBlock()
{
}

ProfilerBlock::~ProfilerBlock()
{
}

void ProfilerBlock::AddObservers(
    notification::NotificationCenter* notificationCenter)
{
    AddUstObserver(notificationCenter, 
                   Token("ust_baddr_statedump:soinfo"),
                   base::BindObject(&ProfilerBlock::OnBaddr, this));
    AddUstObserver(notificationCenter,
                   Token("lttng_profile:sample"),
                   base::BindObject(&ProfilerBlock::OnSample, this));
}

void ProfilerBlock::OnBaddr(const trace::EventValue& event)
{
    process_t pid = ProcessForEvent(event);
    uint64_t baddr = event.getEventField("baddr")->AsULong();
    std::string sopath = event.getEventField("sopath")->AsString();
    uint64_t size = event.getEventField("size")->AsULong();

    if (baddr == 0x400000)
    {
        baddr = 0;
        size += 0x400000;
    }

    symbols::Image image;
    image.set_path(sopath);
    image.set_base_address(baddr);
    image.set_size(size);

    _images[pid].push_back(image);
}

void ProfilerBlock::OnSample(const trace::EventValue& event)
{
    thread_t tid = ThreadForEvent(event);
    process_t pid = ProcessForEvent(event);

    std::vector<std::string> symbolizedStack;
    const auto* stack = value::ArrayValue::Cast(event.getEventField("stack"));

    for (const auto& addressValue : *stack)
    {
        uint64_t address = addressValue.AsULong();

        symbols::Symbol symbol;
        uint64_t offset = 0;
        if (!_symbols.LookupSymbol(address, _images[pid], &symbol, &offset))
            symbol.set_name("unknown");

        symbolizedStack.push_back(symbol.name());
    }

    Stacks()->SetStack(tid, symbolizedStack);
}

}  // namespace stacks_blocks
}  // namespace tibee