/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     rekols <rekols@foxmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "eventmonitor.h"

#include <X11/Xlibint.h>

EventMonitor::EventMonitor(QObject *parent)
    : QThread(parent)
{

}

EventMonitor::~EventMonitor()
{
    requestInterruption();
    quit();
    wait();
}

void EventMonitor::run()
{
    if (isInterruptionRequested())
        return;

    auto *display = QX11Info::display();

    // unable to open display.
    if (display == nullptr) {
        return;
    }

    XRecordClientSpec clients = XRecordAllClients;
    XRecordRange *range = XRecordAllocRange();
    // unable to allocate XRecordRange
    if (range == nullptr) {
        return;
    }

    memset(range, 0, sizeof(XRecordRange));
    range->device_events.first = KeyPress;
    range->device_events.last = MotionNotify;

    XRecordContext context = XRecordCreateContext(display, 0, &clients, 1, &range, 1);
    if (context == 0) {
        return;
    }
    XFree(range);

    XSync(display, True);

    Display *display_datalink = XOpenDisplay(nullptr);
    if (display_datalink == nullptr) {
        return;
    }

    if (!XRecordEnableContext(display_datalink, context, callback, reinterpret_cast<XPointer>(this))) {
        return;
    }
}

void EventMonitor::callback(XPointer ptr, XRecordInterceptData* data)
{
    reinterpret_cast<EventMonitor *>(ptr)->handleEvent(data);
}

void EventMonitor::handleEvent(XRecordInterceptData* data)
{
    if (data->category == XRecordFromServer) {
        xEvent *event = reinterpret_cast<xEvent *>(data->data);

        switch (event->u.u.type) {
        case ButtonPress:
            Q_EMIT buttonPress(event->u.keyButtonPointer.rootX,
                               event->u.keyButtonPointer.rootY);

            // 鼠标左键点击
            if (event->u.u.detail == 1) {

            }

            break;

        case ButtonRelease:
            Q_EMIT buttonRelease(event->u.keyButtonPointer.rootX,
                                 event->u.keyButtonPointer.rootY);

            // 鼠标左键释放
            if (event->u.u.detail == 1) {

            }
            break;

        case MotionNotify:
            // 鼠标移动
            break;
        }
    }

    fflush(stdout);
    XRecordFreeData(data);
}
