// SPDX-FileCopyrightText: 2021 - 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef HISTORYEVENTRECEIVER_H
#define HISTORYEVENTRECEIVER_H

#include "dfmplugin_history_global.h"

#include <QObject>
#include <QUrl>

namespace dfmplugin_history {

class HistoryEventReceiver : public QObject
{
    Q_OBJECT

public:
    static HistoryEventReceiver *instance();

public slots:
    void handleWindowUrlChanged(quint64 winId, const QUrl &url);
    void handleOpenFiles(quint64 winId, const QList<QUrl> &urls);

private:
    explicit HistoryEventReceiver(QObject *parent = nullptr);
};

}   // namespace dfmplugin_history

#endif   // HISTORYEVENTRECEIVER_H
