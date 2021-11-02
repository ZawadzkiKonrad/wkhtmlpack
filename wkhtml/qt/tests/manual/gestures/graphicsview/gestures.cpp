/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the test suite of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "gestures.h"

#include <QTouchEvent>

Qt::GestureType ThreeFingerSlideGesture::Type = Qt::CustomGesture;

QGesture *ThreeFingerSlideGestureRecognizer::create(QObject *)
{
    return new ThreeFingerSlideGesture;
}

QGestureRecognizer::Result ThreeFingerSlideGestureRecognizer::recognize(QGesture *state, QObject *, QEvent *event)
{
    ThreeFingerSlideGesture *d = static_cast<ThreeFingerSlideGesture *>(state);
    QGestureRecognizer::Result result;
    switch (event->type()) {
    case QEvent::TouchBegin:
        result = QGestureRecognizer::MayBeGesture;
    case QEvent::TouchEnd:
        if (d->gestureFired)
            result = QGestureRecognizer::FinishGesture;
        else
            result = QGestureRecognizer::CancelGesture;
    case QEvent::TouchUpdate:
        if (d->state() != Qt::NoGesture) {
            QTouchEvent *ev = static_cast<QTouchEvent*>(event);
            if (ev->touchPoints().size() == 3) {
                d->gestureFired = true;
                result = QGestureRecognizer::TriggerGesture;
            } else {
                result = QGestureRecognizer::MayBeGesture;
                for (int i = 0; i < ev->touchPoints().size(); ++i) {
                    const QTouchEvent::TouchPoint &pt = ev->touchPoints().at(i);
                    const int distance = (pt.pos().toPoint() - pt.startPos().toPoint()).manhattanLength();
                    if (distance > 20) {
                        result = QGestureRecognizer::CancelGesture;
                    }
                }
            }
        } else {
            result = QGestureRecognizer::CancelGesture;
        }

        break;
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease:
    case QEvent::MouseMove:
        if (d->state() != Qt::NoGesture)
            result = QGestureRecognizer::Ignore;
        else
            result = QGestureRecognizer::CancelGesture;
        break;
    default:
        result = QGestureRecognizer::Ignore;
        break;
    }
    return result;
}

void ThreeFingerSlideGestureRecognizer::reset(QGesture *state)
{
    static_cast<ThreeFingerSlideGesture *>(state)->gestureFired = false;
    QGestureRecognizer::reset(state);
}


QGesture *RotateGestureRecognizer::create(QObject *)
{
    return new QGesture;
}

QGestureRecognizer::Result RotateGestureRecognizer::recognize(QGesture *, QObject *, QEvent *event)
{
    switch (event->type()) {
    case QEvent::TouchBegin:
    case QEvent::TouchEnd:
    case QEvent::TouchUpdate:
        break;
    default:
        break;
    }
    return QGestureRecognizer::Ignore;
}

void RotateGestureRecognizer::reset(QGesture *state)
{
    QGestureRecognizer::reset(state);
}

#include "moc_gestures.cpp"
