/*
 * Copyright (C) 2015 Niek Linnenbank
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

#include <Log.h>
#include "Kernel.h"
#include "Scheduler.h"

Scheduler::Scheduler()
{
    DEBUG("");
}

Size Scheduler::count() const
{
    return p1_queue.count() + p2_queue.count() + p3_queue.count() + p4_queue.count() + p5_queue.count();
}

Scheduler::Result Scheduler::enqueue(Process *proc, bool ignoreState)
{
    if (proc->getState() != Process::Ready && !ignoreState)
    {
        ERROR("process ID " << proc->getID() << " not in Ready state");
        return InvalidArgument;
    }

    u8 procPriority = proc->getPriority();
    switch (procPriority)
    {
    case 5:
        p5_queue.push(proc);
        break;
    case 4:
        p4_queue.push(proc);
        break;
    case 3:
        p3_queue.push(proc);
        break;
    case 2:
        p2_queue.push(proc);
        break;
    case 1:
        p1_queue.push(proc);
        break;
    }
    return Success;
}

Scheduler::Result Scheduler::dequeue(Process *proc, bool ignoreState)
{
    if (proc->getState() == Process::Ready && !ignoreState)
    {
        ERROR("process ID " << proc->getID() << " is in Ready state");
        return InvalidArgument;
    }

    for (Size i = 0; i < p5_queue.count(); i++)
    {
        Process *p = p5_queue.pop();
        u8 pPriority = p->getPriority();

        if (pPriority == 4)
        {
            p4_queue.push(p);
            continue;
        }
        else if (pPriority == 3)
        {
            p3_queue.push(p);
            continue;
        }
        else if (pPriority == 2)
        {
            p2_queue.push(p);
            continue;
        }
        else if (pPriority == 1)
        {
            p1_queue.push(p);
            continue;
        }

        if (p == proc)
            return Success;
        else
            p5_queue.push(p);
    }
    for (Size i = 0; i < p4_queue.count(); i++)
    {
        Process *p = p4_queue.pop();
        u8 pPriority = p->getPriority();

        if (pPriority == 5)
        {
            p5_queue.push(p);
            continue;
        }
        else if (pPriority == 3)
        {
            p3_queue.push(p);
            continue;
        }
        else if (pPriority == 2)
        {
            p2_queue.push(p);
            continue;
        }
        else if (pPriority == 1)
        {
            p1_queue.push(p);
            continue;
        }

        if (p == proc)
            return Success;
        else
            p4_queue.push(p);
    }
    for (Size i = 0; i < p3_queue.count(); i++)
    {
        Process *p = p3_queue.pop();
        u8 pPriority = p->getPriority();

        if (pPriority == 5)
        {
            p5_queue.push(p);
            continue;
        }
        else if (pPriority == 4)
        {
            p4_queue.push(p);
            continue;
        }
        else if (pPriority == 2)
        {
            p2_queue.push(p);
            continue;
        }
        else if (pPriority == 1)
        {
            p1_queue.push(p);
            continue;
        }

        if (p == proc)
            return Success;
        else
            p3_queue.push(p);
    }
    for (Size i = 0; i < p2_queue.count(); i++)
    {
        Process *p = p2_queue.pop();
        u8 pPriority = p->getPriority();

        if (pPriority == 5)
        {
            p5_queue.push(p);
            continue;
        }
        else if (pPriority == 4)
        {
            p4_queue.push(p);
            continue;
        }
        else if (pPriority == 3)
        {
            p3_queue.push(p);
            continue;
        }
        else if (pPriority == 1)
        {
            p1_queue.push(p);
            continue;
        }

        if (p == proc)
            return Success;
        else
            p2_queue.push(p);
    }
    for (Size i = 0; i < p1_queue.count(); i++)
    {
        Process *p = p1_queue.pop();
        u8 pPriority = p->getPriority();

        if (pPriority == 5)
        {
            p5_queue.push(p);
            continue;
        }
        else if (pPriority == 4)
        {
            p4_queue.push(p);
            continue;
        }
        else if (pPriority == 3)
        {
            p3_queue.push(p);
            continue;
        }
        else if (pPriority == 2)
        {
            p2_queue.push(p);
            continue;
        }

        if (p == proc)
            return Success;
        else
            p1_queue.push(p);
    }

    FATAL("process ID " << proc->getID() << " is not in the schedule");
    return InvalidArgument;
}

Process *Scheduler::select()
{

    if (p5_queue.count() > 0)
    {
        Process *p = p5_queue.pop();
        p5_queue.push(p);

        return p;
    }
    else if (p4_queue.count() > 0)
    {
        Process *p = p4_queue.pop();
        p4_queue.push(p);

        return p;
    }
    else if (p3_queue.count() > 0)
    {
        Process *p = p3_queue.pop();
        p3_queue.push(p);

        return p;
    }
    else if (p2_queue.count() > 0)
    {
        Process *p = p2_queue.pop();
        p2_queue.push(p);

        return p;
    }
    else if (p1_queue.count() > 0)
    {
        Process *p = p1_queue.pop();
        p1_queue.push(p);

        return p;
    }

    return (Process *)NULL;
}
