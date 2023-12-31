// Copyright(c) 2017-2019 Alejandro Sirgo Rica & Contributors
//
// This file is part of Flameshot.
//
//     Flameshot is free software: you can redistribute it and/or modify
//     it under the terms of the GNU General Public License as published by
//     the Free Software Foundation, either version 3 of the License, or
//     (at your option) any later version.
//
//     Flameshot is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//     GNU General Public License for more details.
//
//     You should have received a copy of the GNU General Public License
//     along with Flameshot.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include "src/core/capturerequest.h"
#include <QObject>
#include <QPointer>
#include <QPixmap>
#include <QMap>
#include <QTimer>
#include <functional>
#include <QMenu>
#include <QSystemTrayIcon>

class CaptureWidget;
class ConfigWindow;
class InfoWindow;
class QSystemTrayIcon;
class CaptureLauncher;

using lambda = std::function<void(void)>;

class Controller : public QObject {
    Q_OBJECT

public:
    static Controller* getInstance();

    Controller(const Controller&) = delete;
    void operator =(const Controller&) = delete;

    void enableExports();

signals:
    void captureTaken(uint id, QPixmap p);
    void captureFailed(uint id);

public slots:
    void requestCapture(const CaptureRequest &request);

    void openConfigWindow();
    void openInfoWindow();
    void openLauncherWindow();
    void enableTrayIcon();
    void disableTrayIcon();
    void sendTrayNotification(const QString &text,
                              const QString &title = QStringLiteral("Flameshot Info"),
                              const int timeout = 5000);

    void updateConfigComponents();
    //void trayIconActivated(QSystemTrayIcon::ActivationReason r);

private slots:
    void startFullscreenCapture(const uint id = 0);
    void startVisualCapture(const uint id = 0,
                             const QString &forcedSavePath = QString());
    void startScreenGrab(const uint id = 0, const int screenNumber = -1);

    void handleCaptureTaken(uint id, QPixmap p);
    void handleCaptureFailed(uint id);
    void trayIconActivated(QSystemTrayIcon::ActivationReason r);

private:
    Controller();

    // replace QTimer::singleShot introduced in Qt 5.4
    // the actual target Qt version is 5.3
    void doLater(int msec, QObject *receiver, lambda func);

    QMap<uint, CaptureRequest> m_requestMap;
    QPointer<CaptureWidget> m_captureWindow;
    QPointer<CaptureWidget> m_fullcaptureWindow;
    QPointer<InfoWindow> m_infoWindow;
    QPointer<ConfigWindow> m_configWindow;
    QPointer<QSystemTrayIcon> m_trayIcon;

    QPointer<CaptureLauncher> m_captureLauncher;
};
