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

#ifndef COLORWHEEL_H
#define COLORWHEEL_H

#include "PerceptualColor/perceptualcolorglobal.h"

#include <QWidget>

#include "PerceptualColor/abstractdiagram.h"
#include "PerceptualColor/constpropagatinguniquepointer.h"

namespace PerceptualColor
{
class RgbColorSpace;

/** @brief A color wheel widget.
 *
 * This widget allows the user to choose the hue (as defined in the LCh
 * color space).
 *
 * @image html ColorWheel.png "ColorWheel"
 *
 * @note This widget <em>always</em> accepts focus by a mouse click within
 * the circle. This happens regardless of the <tt>QWidget::focusPolicy</tt>
 * property:
 * - If you set the <tt>QWidget::focusPolicy</tt> property to a
 *   value that does not accept focus by mouse click, the focus
 *   will nevertheless be accepted for clicks within the actual circle.
 *   (This is the default behavior.)
 * - If you set the <tt>QWidget::focusPolicy</tt> property to a
 *   value that accepts focus by mouse click, the focus will not only be
 *   accepted for clicks within the actual circle, but also for clicks
 *   anyware within the (rectangular) widget. */
class PERCEPTUALCOLOR_IMPORTEXPORT ColorWheel : public AbstractDiagram
{
    Q_OBJECT

    /** @brief The currently selected hue.
     *
     * The hue property is the hue angle, as defined in the LCH color model.
     *
     * Measured in degree. Valid range: [0°, 360°[
     *
     * @sa READ @ref hue() const
     * @sa WRITE @ref setHue()
     * @sa NOTIFY @ref hueChanged()
     *
     * @internal
     *
     * The value gets normalized according
     * to @ref PolarPointF::normalizedAngleDegree() */
    Q_PROPERTY(qreal hue READ hue WRITE setHue NOTIFY hueChanged USER true)

public:
    Q_INVOKABLE explicit ColorWheel(const QSharedPointer<PerceptualColor::RgbColorSpace> &colorSpace, QWidget *parent = nullptr);
    virtual ~ColorWheel() noexcept override;
    /** @brief Getter for property @ref hue
     *  @returns the property @ref hue */
    qreal hue() const;
    virtual QSize minimumSizeHint() const override;
    virtual QSize sizeHint() const override;

Q_SIGNALS:
    /** @brief Notify signal for property @ref hue.
     * @param newHue the new hue */
    void hueChanged(const qreal newHue);

public Q_SLOTS:
    void setHue(const qreal newHue);

protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;

private:
    Q_DISABLE_COPY(ColorWheel)

    class ColorWheelPrivate;
    /** @internal
     *
     * @brief Declare the private implementation as friend class.
     *
     * This allows the private class to access the protected members and
     * functions of instances of <em>this</em> class. */
    friend class ColorWheelPrivate;
    /** @brief Pointer to implementation (pimpl) */
    ConstPropagatingUniquePointer<ColorWheelPrivate> d_pointer;

    /** @internal @brief Only for unit tests. */
    friend class TestColorDialog;

    /** @internal
     * @brief Internal friend declaration.
     *
     * This class is used as child class in @ref WheelColorPicker.
     * There is a tight collaboration. */
    friend class WheelColorPicker;
};

} // namespace PerceptualColor

#endif // COLORWHEEL_H
