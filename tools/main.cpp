﻿// SPDX-License-Identifier: MIT
/*
 * Copyright (c) 2020 Lukas Sommer sommerluk@gmail.com
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include "PerceptualColor/perceptualcolorglobal.h"
#include "perceptualcolorinternal.h"

#include "PerceptualColor/chromahuediagram.h"
#include "PerceptualColor/colordialog.h"
#include "PerceptualColor/colorpatch.h"
#include "PerceptualColor/colorwheel.h"
#include "PerceptualColor/gradientslider.h"
#include "PerceptualColor/lchdouble.h"
#include "PerceptualColor/multispinbox.h"
#include "PerceptualColor/rgbcolorspacefactory.h"
#include "constpropagatingrawpointer.h"
#include "polarpointf.h"
#include "refreshiconengine.h"
#include "rgbcolorspace.h"
#include "version.h"

#include <lcms2.h>
#include <math.h>
#include <memory>

#include <QAccessible>
#include <QAction>
#include <QApplication>
#include <QColorDialog>
#include <QDateTimeEdit>
#include <QDebug>
#include <QImageReader>
#include <QLabel>
#include <QLineEdit>
#include <QMetaObject>
#include <QPainter>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>
#include <QtMath>

// TODO Test the main function etc from KColorChooser to see if
// PerceptualColor::ColorDialog is really a drop-in replacement
// for QColorDialog.
// TODO Drop this executable in favor of KColorChooser?

// This is just a program for testing purpuses.
int main(int argc, char *argv[])
{
    // Prepare configuratin before instanciating the application object
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    // Instanciate the application object
    QApplication app(argc, argv);
    app.setApplicationName(QObject::tr("Perceptual color picker"));
    // app.setLayoutDirection(Qt::RightToLeft);
    // QLocale::setDefault(QLocale::Bengali);
    // QLocale::setDefault(QLocale::German);

    // Initialize the color dialog
    PerceptualColor::ColorDialog m_colorDialog;
    m_colorDialog.setOption(QColorDialog::ColorDialogOption::ShowAlphaChannel, true);
    QColor myColor = QColor(Qt::yellow);
    myColor.setAlphaF(0.5);
    //     m_colorDialog.setCurrentColor(myColor);
    // m_colorDialog.setOption(QColorDialog::ColorDialogOption::NoButtons);
    m_colorDialog.setLayoutDimensions(PerceptualColor::ColorDialog::DialogLayoutDimensions::expanded);
    m_colorDialog.show();
    // m_colorDialog.setEnabled(false);

    // m_colorDialog.setStyleSheet(
    //     "background: yellow; color: red; border: 15px solid #FF0000;"
    // );

    //     QSlider mySlider;
    //     mySlider.show();
    //     qDebug() << QAccessible::queryAccessibleInterface(new QObject);
    //     qDebug() << QAccessible::queryAccessibleInterface(new QWidget);
    //     qDebug() << QAccessible::queryAccessibleInterface(new PerceptualColor::MultiSpinBox);
    //     qDebug() << QAccessible::queryAccessibleInterface(
    //         // Test with GradientSlider
    //         new PerceptualColor::GradientSlider(
    //             // Create a new color space just to keep this debug code independant:
    //             PerceptualColor::RgbColorSpaceFactory::createSrgb()
    //         )
    //     );
    //     qDebug() << QAccessible::queryAccessibleInterface(&m_colorDialog);

    // Run
    return app.exec();
}
