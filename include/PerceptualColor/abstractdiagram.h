// SPDX-License-Identifier: MIT
/*
 * Copyright (c) 2020 Lukas Sommer somerluk@gmail.com
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

#ifndef ABSTRACTDIAGRAM_H
#define ABSTRACTDIAGRAM_H

#include <QFrame>

#include "PerceptualColor/constpropagatinguniquepointer.h"

namespace PerceptualColor {
    
/** @brief Base class for LCh diagrams.
 * 
 * Provides some elements that are common for all LCh diagrams in this
 * library.
 * 
 * @note Qt provides some possibilities to declare that a certain widget
 * has a fixed ration between width and height. You can reimplement
 * <tt>QWidget::hasHeightForWidth()</tt> (indicates that the widget's preferred
 * height depends on its width) and <tt>QWidget::heightForWidth()</tt>
 * (returns the preferred height for this widget, given the width <tt>w</tt>).
 * However, Qt’s layout management makes only very limited use of this
 * information. It is ignored, when the sourrounding window is resized by
 * grapping the window border with the mouse. It is however considered when
 * the sourrounding window is resized by grapping a <tt>QSizeGrip</tt>
 * widget. This behavior is inconsistend and would be suprising for the
 * user. Furthermore, if the widget is yet touching the border of the
 * screen, then the policy cannot be honnored anyway; but it causes
 * flickering. Another possibility is QSizePolicy::setHeightForWidth or
 * QSizePolicy::setWidthForHeight wich seem both to be “only supported for
 * QGraphicsLayout’s subclasses”. Therefore, it’s better not to use at all
 * these features; that’s the only way to provide a consistent and good
 * user experience.
 * 
 * @todo Circular diagrams should be right-aligned on RTL layouts.
 * 
 * @todo Touchscreen support: Magnify the handle circle, when diagram is
 * used on a touch device?  */
class AbstractDiagram : public QFrame
{
    Q_OBJECT

public:

    Q_INVOKABLE AbstractDiagram(QWidget *parent = nullptr);
    /** @brief Default destructor */
    virtual ~AbstractDiagram() noexcept override;

protected:

    // constexpr

    /** @brief The radius of a circular handle.
     * 
     * Measured in Device Independent Pixels: Pixels used by application (user
     * space), subject to scaling by the operating system or Qt. */
    static constexpr int handleRadius = 4;
    /** @brief The outline thickness of a (either circular or linear) handle.
     * 
     * Measured in Device Independent Pixels: Pixels used by application (user
     * space), subject to scaling by the operating system or Qt. */
    static constexpr int handleOutlineThickness = 2;
    /** @brief Amount of single step for chroma.
     * 
     * Measured in LCh chroma units.
     * 
     * The smaller of two natural steps that this widget provides and
     * typically corresponds to the user pressing a key or using the mouse
     * wheel: The value will be incremented/decremented by the amount of this
     * value.
     *
     * @sa @ref pageStepChroma */
    static constexpr int singleStepChroma = 1;
    /** @brief Amount of single step for hue.
     * 
     * Measured in degree.
     * 
     * The smaller of two natural steps that this widget provides and
     * typically corresponds to the user pressing a key or using the mouse
     * wheel: The value will be incremented/decremented by the amount of this
     * value.
     * 
     * @sa @ref pageStepHue
     * 
     * @todo What would be a good value for this? Its effect depends on
     * chroma: On higher chroma, the same step in hue means a bigger visual
     * color difference. We could even calculate that, but it does not seem to
     * be very intuitive if the reaction on mouse wheel events are different
     * depending on chroma - that would not be easy to understand for the
     * user. And it might be better that the user this way also notices
     * intuitively that hue changes are not linear across chroma. Anyway:
     * What would be a sensible default step? */
    static constexpr int singleStepHue = 360 / 100;
    /** @brief Amount of page step for chroma.
     * 
     * Measured in LCh chroma units.
     * 
     * The larger of two natural steps that this widget provides and
     * typically corresponds to the user pressing a key or using the mouse
     * wheel: The value will be incremented/decremented by the amount of this
     * value.
     * 
     * The value is 10 times @ref singleStepChroma. This behavior
     * corresponds to QAbstractSlider, who’s page step is also 10  bigger than
     * its single step. */
    static constexpr int pageStepChroma = 10 * singleStepChroma;
    /** @brief Amount of page step for hue.
     * 
     * Measured in degree.
     * 
     * The larger of two natural steps that this widget provides and
     * typically corresponds to the user pressing a key or using the mouse
     * wheel: The value will be incremented/decremented by the amount of this
     * value.
     * 
     * The value is 10 times @ref singleStepHue. This behavior
     * corresponds to QAbstractSlider, who’s page step is also 10  bigger than
     * its single step. */
    static constexpr int pageStepHue = 10 * singleStepHue;

    // methods
    QColor focusIndicatorColor() const;
    QSize physicalPixelSize() const;
    QImage transparencyBackground() const;

private:
    Q_DISABLE_COPY(AbstractDiagram)

    class AbstractDiagramPrivate;
    /** @brief Declare the private implementation as friend class.
     * 
     * This allows the private class to access the protected members and
     * functions of instances of <em>this</em> class. */
    friend class AbstractDiagramPrivate;
    /** @brief Pointer to implementation (pimpl) */
    ConstPropagatingUniquePointer<AbstractDiagramPrivate> d_pointer;

    /** @brief Only for unit tests. */
    friend class TestAbstractDiagram;

};

}

#endif // ABSTRACTDIAGRAM_H
